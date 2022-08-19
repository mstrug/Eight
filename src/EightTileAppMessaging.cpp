/*
 ============================================================================
 Name		: EightTileAppMessaging.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileAppMessaging implementation
 ============================================================================
 */

#include "EightTileAppMessaging.h"
#include "EightSettings.h"
#include "EightTileUiStatic.h"
//#include "logger.h"
#include <aknappui.h> 
#include <APGCLI.H> 
#include <APACMDLN.H> 
#include <APGWGNAM.H> 
#include <MSVIDS.H> 
#include <smut.h> 
#include <mmsconst.h>


CEightTileAppMessaging::CEightTileAppMessaging( TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi )
    : CEightTile( aId, aTileSize, aUi, EFalse )
    {
    }

CEightTileAppMessaging::~CEightTileAppMessaging()
    {
    delete iTimer;
    }

CEightTileAppMessaging* CEightTileAppMessaging::NewL( TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi )
    {
    CEightTileAppMessaging* self = new ( ELeave ) CEightTileAppMessaging( aId, aTileSize, aUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self;
    return self;
    }

void CEightTileAppMessaging::ConstructL()
    {
    BaseConstructL();
    
    iTimer = CSGEUtilsGPTimer::NewL( *this );
    
    if ( iUi )
        {
        CEightMessagingInterface::GetInstance()->RegisterClientL( this );
        
        iSmsIdx = ((CEightTileUiStatic*)iUi)->AddTextL( TVgRect( TVgPoint( 22, 3 ), TVgSize() ), KFontId_SegoeWP_largeNum, _L(""), ETrue ); //, ESGEFontAlignHorizontalLeft, ESGEFontAlignVerticalBottom );
        }
    }

void CEightTileAppMessaging::SetRect( const TVgRect& aRect )
    {
    CEightTile::SetRect( aRect );
    
    CEightTileUiStatic* ui = ((CEightTileUiStatic*)iUi);
    if ( ui )
        {
        if ( iTileSize != ETileSize_3x1 )
            {
            ui->UpdateRect( 1, TVgRect( TVgPoint( 6, 70 ), TVgPoint( iRect.Width() - 6, 6 ) ) );
            UpdateRectsL();
            }
        ui->SetItemHide( 1, ( iTileSize == ETileSize_1x1 ) );
        }
    }

TEightTileType CEightTileAppMessaging::Type() const
    {
    return ETileTypeAppShortcut;
    }

TBool CEightTileAppMessaging::RunInternal()
    {
    if ( !iTimer->IsTimerStarted() )
        {
        iTimer->Start( 50000 );
        }
    return ETrue;
    }

void CEightTileAppMessaging::UpdateRectsL()
    {
    if ( iSmsCount == 0 )
        {
        ((CEightTileUiStatic*)iUi)->UpdateText( iSmsIdx, _L("") );
        ((CEightTileUiStatic*)iUi)->UpdateRect( 0, TVgRect() ); // icon
        }
    else
        {
        if ( iSmsCount > 9 && iTileSize == ETileSize_1x1 )
            {
            ((CEightTileUiStatic*)iUi)->UpdateText( iSmsIdx, _L("9") );
            ((CEightTileUiStatic*)iUi)->UpdateRect( 0, TVgRect( TVgPoint( -10, 0 ), TVgSize() ) ); // icon
            }
        else
            {
            TBuf<10> num;
            num.Num( iSmsCount );
            ((CEightTileUiStatic*)iUi)->UpdateText( iSmsIdx, num );
            if ( iSmsCount > 9 )
                {
                ((CEightTileUiStatic*)iUi)->UpdateRect( 0, TVgRect( TVgPoint( -20, 0 ), TVgSize() ) ); // icon
                }
            else
                {
                ((CEightTileUiStatic*)iUi)->UpdateRect( 0, TVgRect( TVgPoint( -10, 0 ), TVgSize() ) ); // icon
                }
            }
        }
    }

void CEightTileAppMessaging::GPTimerEvent( CSGEUtilsGPTimer* /*aTimer*/ )
    {
    CApaCommandLine* cl = CApaCommandLine::NewLC();
    cl->SetExecutableNameL( _L("mce.exe") );

    RApaLsSession ls;
    ls.Connect();
    
    TInt err = ls.StartApp( *cl );
    
    if ( err == KErrNone )
        {
        ls.GetAllApps();
        TApaAppInfo info;
        TUid uid = { 0 };
        while ( ls.GetNextApp( info ) == KErrNone )
            {
            if ( info.iFullName.FindF( _L("mce.exe") ) != KErrNotFound ) //smsviewer.exe mce.exe
                {
                uid = info.iUid;
                break;
                }
            }
        
        if ( uid.iUid )
            {
            RWsSession& ws( CCoeEnv::Static()->WsSession() );
            TInt id = 0;
            CApaWindowGroupName::FindByAppUid( uid, ws, id );
            if ( id > 0 )
                {
                TApaTask task( ws );
                task.SetWgId( id );
                task.BringToForeground();

                TWsEvent ev;
                ev.SetTimeNow();
                ev.SetType( EEventFocusGained );
                ws.SendEventToWindowGroup( id, ev );
                }
            }
        }
    
    ls.Close();
    CleanupStack::PopAndDestroy();
    }
    
void CEightTileAppMessaging::MessagingNotifySmsMms( TInt aSmsCount, TInt aMmsCount )
    {
    iSmsCount = aSmsCount + aMmsCount;
    UpdateRectsL();
    }

void CEightTileAppMessaging::MessagingNotifyEmail( TInt /*aEmailCount*/ )
    {
    // not used
    }


