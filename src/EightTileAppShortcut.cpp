/*
 ============================================================================
 Name		: EightTileAppShortcut.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileAppShortcut implementation
 ============================================================================
 */

#include "EightTileAppShortcut.h"
#include "EightTileUiStatic.h"
#include "EightTileUiDynFlip.h"
#include "EightTileUiDynCycle.h"
#include "EightTileUiDynMosaic.h"
#include <APGCLI.H> 
#include <APACMDLN.H> 
#include <APGWGNAM.H> 
#include <aknappui.h> 
//#include "logger.h"
#include <Etel3rdParty.h> 


CEightTileAppShortcut::CEightTileAppShortcut( TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi )
    : CEightTile( aId, aTileSize, aUi, EFalse )
    {
    }

CEightTileAppShortcut::~CEightTileAppShortcut()
    {
    iExeName.Close();
    iExeName2.Close();
    delete iTimer;
    }

CEightTileAppShortcut* CEightTileAppShortcut::NewL( const TDesC& aExeName, TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi )
    {
    CEightTileAppShortcut* self = new ( ELeave ) CEightTileAppShortcut( aId, aTileSize, aUi );
    CleanupStack::PushL( self );
    self->ConstructL( aExeName );
    CleanupStack::Pop(); // self;
    return self;
    }

void CEightTileAppShortcut::ConstructL( const TDesC& aExeName )
    {
    BaseConstructL();
    iExeName.CreateL( aExeName );

    iTimer = CSGEUtilsGPTimer::NewL( *this );
    }

void CEightTileAppShortcut::SetExeName2L( const TDesC& aExeName )
    {
    iExeName2.CreateL( aExeName );
    }

void CEightTileAppShortcut::EnableRunTwoTimes()
    {
    iRunTwoTimes = ETrue;
    }

void CEightTileAppShortcut::SetCanBeRemoved( TBool aValue )
    {
    iCanBeRemovedSet = ETrue;
    iCanBeRemoved = aValue;
    }

TBool CEightTileAppShortcut::CanBeRemoved() const
    {
    if ( iCanBeRemovedSet )
        {
        return iCanBeRemoved;
        }
    return CEightTile::CanBeRemoved();
    }

TEightTileType CEightTileAppShortcut::Type() const
    {
    return ETileTypeAppShortcut;
    }

void CEightTileAppShortcut::SetRect( const TVgRect& aRect )
    {
    CEightTile::SetRect( aRect );
    
    if ( iUi->Type() == CEightTileUi::EStatic )
        {
        CEightTileUiStatic* ui = (CEightTileUiStatic*)iUi;
        if ( ui )
            {
            // update name rect
            if ( iTileSize != ETileSize_3x1 ) ui->UpdateRect( 1, TVgRect( TVgPoint( 6, 70 ), TVgPoint( iRect.Width() - 6, 6 ) ) );
            // hide name
            ui->SetItemHide( 1, ( iTileSize == ETileSize_1x1 ) );
            }
        }
    else if ( iUi->Type() == CEightTileUi::EDynFilp )
        {
        CEightTileUiDynFlip* ui = (CEightTileUiDynFlip*)iUi;
        if ( ui && ui->TileUi( 0 ) && ui->TileUi( 0 )->Type() == CEightTileUi::EStatic )
            {
            CEightTileUiStatic* uis = (CEightTileUiStatic*)ui->TileUi( 0 );
            // update name rect
            if ( iTileSize != ETileSize_3x1 ) uis->UpdateRect( 1, TVgRect( TVgPoint( 6, 70 ), TVgPoint( iRect.Width() - 6, 6 ) ) );
            // hide name
            uis->SetItemHide( 1, ( iTileSize == ETileSize_1x1 ) );
            }
        }
    else if ( iUi->Type() == CEightTileUi::EDynCycle )
        {
        CEightTileUiDynCycle* ui = (CEightTileUiDynCycle*)iUi;
        if ( ui && ui->StaticUi() )
            {
            // update name rect
            ui->StaticUi()->UpdateRect( 0, TVgRect( TVgPoint( 6, 70 ), TVgPoint( iRect.Width() - 6, 6 ) ) );
            }
        }
    else if ( iUi->Type() == CEightTileUi::EDynMosaic )
        {
        CEightTileUiDynMosaic* ui = (CEightTileUiDynMosaic*)iUi;
        if ( ui && ui->StaticUi() )
            {
            // update name rect
            ui->StaticUi()->UpdateRect( 0, TVgRect( TVgPoint( 6, 70 ), TVgPoint( iRect.Width() - 6, 6 ) ) );
            }
        }
    }

TBool CEightTileAppShortcut::RunInternal()
    {
    if ( !iTimer->IsTimerStarted() )
        {
        iSecondRun = EFalse;
        iTimer->Start( 50000 );
        }
    return ETrue;
    }

void CEightTileAppShortcut::GPTimerEvent( CSGEUtilsGPTimer* /*aTimer*/ )
    {
    CApaCommandLine* cl = CApaCommandLine::NewLC();
    cl->SetExecutableNameL( iExeName );
    
//    CCoeEnv::Static()->AppUi()->DeactivateActiveViewL();
    
//    RWsSession ws( CCoeEnv::Static()->WsSession() );
//    TApaTask task( ws );
//    TInt id = 0;
//    CApaWindowGroupName::FindByAppUid( TUid::Uid( 0x2006AF94 ), ws, id );
//    task.SetWgId( id );
//    task.SendToBackground();

    RApaLsSession ls;
    ls.Connect();
    
    TInt err = ls.StartApp( *cl );
    
    //Logger::LogStrNum( _L("c:\\data\\e.txt"), iExeName, err );
    if ( err == KErrNone )
        {
        ls.GetAllApps();
        TApaAppInfo info;
        TUid uid = { 0 };
        while ( ls.GetNextApp( info ) == KErrNone )
            {
            if ( info.iFullName.FindF( iExeName ) != KErrNotFound )
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
    else if ( err != KErrNone && iExeName2.Length() > 0 )
        {
        CleanupStack::PopAndDestroy();
        cl = CApaCommandLine::NewLC();
        cl->SetExecutableNameL( iExeName2 );

        err = ls.StartApp( *cl );
        //Logger::LogStrNum( _L("c:\\data\\e.txt"), iExeName2, err );
        if ( err == KErrNone )
            {
            ls.GetAllApps();
            TApaAppInfo info;
            TUid uid = { 0 };
            while ( ls.GetNextApp( info ) == KErrNone )
                {
                if ( info.iFullName.FindF( iExeName2 ) != KErrNotFound )
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
        }
    
    
    ls.Close();
    CleanupStack::PopAndDestroy();
    
    
    if ( iRunTwoTimes && !iSecondRun )
        {
        iSecondRun = ETrue;
        iTimer->Start( 1000 );
        }
    }


