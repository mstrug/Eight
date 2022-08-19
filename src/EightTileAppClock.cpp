/*
 ============================================================================
 Name		: EightTileAppClock.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileAppClock implementation
 ============================================================================
 */

#include "EightTileAppClock.h"
#include "EightSettings.h"
#include "EightTileUiStatic.h"
//#include "logger.h"
#include <aknappui.h> 
#include <ASShdAlarm.h> 
#include <APGCLI.H> 
#include <APACMDLN.H> 
#include <APGWGNAM.H> 
#include <MSVIDS.H> 

const TInt KIconIdx = 0;
const TInt KTitleIdx = 1;


CEightTileAppClock::CEightTileAppClock( TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi )
	    : CEightTile( aId, aTileSize, aUi, EFalse ), iClockHugeIdx( KErrNotFound ), iClockLargeIdx( KErrNotFound ), 
	      iClockAmPmIdx( KErrNotFound ), iClockAmPmBigIdx( KErrNotFound ), iDateIdx( KErrNotFound ), iAlarmIdx( KErrNotFound )
	{
	}

CEightTileAppClock::~CEightTileAppClock()
	{
    delete iTimer;
    delete iTimerClock;
    delete iChangeNotifier;
    
    if ( iTileSize != ETileSize_3x1 )
        {
        if ( iGpao->IsActive() ) iAlarmSession.NotifyChangeCancel();
        iAlarmSession.Close();
        }
    delete iGpao;
	}

CEightTileAppClock* CEightTileAppClock::NewL( TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi )
	{
	CEightTileAppClock* self = new (ELeave) CEightTileAppClock( aId, aTileSize, aUi );
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); // self;
	return self;
	}

void CEightTileAppClock::ConstructL()
	{
    BaseConstructL();
    
    iTimer = CSGEUtilsGPTimer::NewL( *this );
    
    if ( iUi )
        {
        iClockHugeIdx = ((CEightTileUiStatic*)iUi)->AddTextL( TVgRect(), KFontId_SegoeWP_hugeNum, _L("17:24"), ETrue ); //, ESGEFontAlignHorizontalLeft, ESGEFontAlignVerticalBottom );
        iClockLargeIdx = ((CEightTileUiStatic*)iUi)->AddTextL( TVgRect( TVgPoint( 0, 5 ), TVgSize() ), KFontId_SegoeWP_largeNum, _L("17:24"), ETrue ); //, ESGEFontAlignHorizontalLeft, ESGEFontAlignVerticalBottom );
        iClockAmPmBigIdx = ((CEightTileUiStatic*)iUi)->AddTextL( TVgRect( TVgPoint( 37, 64 ), TVgSize( 70, 40 ) ), KFontId_SegoeWP_medium, _L("AM"), EFalse, ESGEFontAlignHorizontalLeft, ESGEFontAlignVerticalTop );
        iClockAmPmIdx = ((CEightTileUiStatic*)iUi)->AddTextL( TVgRect( TVgPoint( 20, 92 ), TVgPoint( 70, 40 ) ), KFontId_SegoeWP_small, _L("AM"), EFalse, ESGEFontAlignHorizontalLeft, ESGEFontAlignVerticalTop );
        iDateIdx = ((CEightTileUiStatic*)iUi)->AddTextL( TVgRect( TVgPoint( 6, 70 ), TVgPoint( 70, 6 ) ), KFontId_SegoeWP_small, _L("12/05/2013"), EFalse, ESGEFontAlignHorizontalRight, ESGEFontAlignVerticalBottom );
        iAlarmIdx = ((CEightTileUiStatic*)iUi)->AddTextL( TVgRect( TVgPoint( 6, 70 ), TVgPoint( 70, 6 ) ), KFontId_SegoeWP_largeNum, _L(";"), EFalse, ESGEFontAlignHorizontalLeft, ESGEFontAlignVerticalTop );
        
        iTimerClock = CSGEUtilsGPTimer::NewL( *this );
        iTimerClock->Start( 1000000, ETrue );
        
        iCurrentTime.HomeTime();
        UpdateTime();
        UpdateDate();
        
        iChangeNotifier = CEnvironmentChangeNotifier::NewL( CActive::EPriorityHigh, TCallBack( CallbackNotifier, this ) );
        iChangeNotifier->Start();
        
        iGpao = new (ELeave) CSGEUtilsGPAO( this );
        iAlarmSession.Connect();
        iAlarmSession.NotifyChange( iGpao->iStatus, iChangedAlarmId );
        iGpao->SetActive();
        UpdateAlarm();
        }
	}

void CEightTileAppClock::SetRect( const TVgRect& aRect )
    {
    CEightTile::SetRect( aRect );
    
    CEightTileUiStatic* ui = ((CEightTileUiStatic*)iUi);
    if ( ui )
        {
        ui->UpdateRect( iDateIdx, TVgRect( TVgPoint( 6, 70 ), TVgPoint( iRect.Width() - 6, 6 ) ) );
        ui->UpdateRect( iAlarmIdx, TVgRect( TVgPoint( 6, iRect.Height() - 6 ), TVgSize( 20, 20 ) ) );

        if ( iTileSize == ETileSize_1x1 )
            {
            ui->SetItemHide( KIconIdx, EFalse );
            ui->SetItemHide( KTitleIdx, ETrue );
            ui->SetItemHide( iClockHugeIdx, ETrue );
            ui->SetItemHide( iClockLargeIdx, ETrue );
            ui->SetItemHide( iClockAmPmBigIdx, ETrue );
            ui->SetItemHide( iClockAmPmIdx, ETrue );
            ui->SetItemHide( iDateIdx, ETrue );            
            ui->SetItemHide( iAlarmIdx, ETrue );
            }
        else if ( iTileSize == ETileSize_2x2 )
            {
            ui->SetItemHide( KIconIdx, ETrue );
            ui->SetItemHide( KTitleIdx, ETrue );
            ui->SetItemHide( iClockHugeIdx, ETrue );
            ui->SetItemHide( iClockLargeIdx, EFalse );
            ui->SetItemHide( iClockAmPmBigIdx, ETrue );
            ui->SetItemHide( iClockAmPmIdx, EFalse );
            ui->SetItemHide( iDateIdx, EFalse );  
            ui->SetItemHide( iAlarmIdx, EFalse );
            }
        else if ( iTileSize == ETileSize_4x2 )
            {
            ui->SetItemHide( KIconIdx, ETrue );
            ui->SetItemHide( KTitleIdx, ETrue );
            ui->SetItemHide( iClockHugeIdx, EFalse );
            ui->SetItemHide( iClockLargeIdx, ETrue );
            ui->SetItemHide( iClockAmPmBigIdx, EFalse );
            ui->SetItemHide( iClockAmPmIdx, ETrue );
            ui->SetItemHide( iDateIdx, EFalse );
            ui->SetItemHide( iAlarmIdx, EFalse );
            }
        else if ( iTileSize != ETileSize_3x1 ) 
            {
            ui->SetItemHide( KIconIdx, EFalse );
            ui->SetItemHide( KTitleIdx, EFalse );
            ui->SetItemHide( iClockHugeIdx, ETrue );
            ui->SetItemHide( iClockLargeIdx, ETrue );
            ui->SetItemHide( iClockAmPmIdx, ETrue );
            ui->SetItemHide( iClockAmPmBigIdx, ETrue );
            ui->SetItemHide( iDateIdx, ETrue );   
            ui->SetItemHide( iAlarmIdx, ETrue );
            }
        }
    }

TEightTileType CEightTileAppClock::Type() const
    {
    return ETileTypeAppShortcut;
    }

TBool CEightTileAppClock::RunInternal()
    {
    if ( !iTimer->IsTimerStarted() )
        {
        iTimer->Start( 50000 );
        }
    return ETrue;
    }

void CEightTileAppClock::UpdateTime()
    {
    iCurrentTime.HomeTime();
    CEightTileUiStatic* ui = ((CEightTileUiStatic*)iUi);
    if ( ui )
        {
        TBuf<16> buf;

        iCurrentTime.FormatL( buf, _L("%J:%T") );
        ui->UpdateText( iClockLargeIdx, buf );
        ui->UpdateText( iClockHugeIdx, buf );

        iCurrentTime.FormatL( buf, _L("%B") );
        ui->UpdateText( iClockAmPmBigIdx, buf );
        ui->UpdateText( iClockAmPmIdx, buf );
        }
    }

void CEightTileAppClock::UpdateDate()
    {
    CEightTileUiStatic* ui = ((CEightTileUiStatic*)iUi);
    if ( ui )
        {
        TBuf<64> buf;

        iCurrentTime.FormatL( buf, _L("%1%/1%2%/2%3") );
//        iCurrentTime.FormatL( buf, _L("%D/%M/%Y") );
        ui->UpdateText( iDateIdx, buf );
        }
    }

void CEightTileAppClock::UpdateAlarm()
    {
    TAlarmId alarmId;
    TBuf<16> buf;
//    Logger::LogStr( _L("c:\\data\\eclock.txt"), _L("UpdateAlarm()") );
    TInt err = iAlarmSession.GetNextDueAlarmId( alarmId );
//    Logger::LogStrNum( _L("c:\\data\\eclock.txt"), _L("GetNextDueAlarmId "), err );
    if ( err == KErrNone )
        {
        TASShdAlarm alarm;
        err = iAlarmSession.GetAlarmDetails( alarmId, alarm );
//        Logger::LogStrNum( _L("c:\\data\\eclock.txt"), _L("GetAlarmDetails "), err );
        if ( err == KErrNone )
            {
            TTime time = alarm.NextDueTime();
            
            TTimeIntervalSeconds tm;
            if ( time.SecondsFrom( iCurrentTime, tm ) == KErrNone )
                {
//                Logger::LogStrNum( _L("c:\\data\\eclock.txt"), _L("SecondsFrom "), tm.Int() );
                    {
                    if ( tm.Int() <= 60 * 60 * 24 )
                        {
                        buf.Append( _L(";") ); // bell sign
                        }
                    }
                }
            }
        }
    
    CEightTileUiStatic* ui = ((CEightTileUiStatic*)iUi);
    if ( ui )
        {
        ui->UpdateText( iAlarmIdx, buf );
        }
    }

TInt CEightTileAppClock::CallbackNotifier( TAny* aPtr )
    {
    TInt val = ((CEightTileAppClock*)aPtr)->iChangeNotifier->Change();
    if ( ( val & EChangesMidnightCrossover ) == EChangesMidnightCrossover ||
         ( val & EChangesSystemTime ) == EChangesSystemTime ||
         ( val & EChangesLocale ) == EChangesLocale )
        {
        ((CEightTileAppClock*)aPtr)->UpdateTime();
        ((CEightTileAppClock*)aPtr)->UpdateDate();
        }
    return KErrNone;
    }

void CEightTileAppClock::GPTimerEvent( CSGEUtilsGPTimer* aTimer )
    {
    if ( aTimer == iTimerClock )
        {
        TTime time;
        time.HomeTime();
        //TTimeIntervalSeconds tm;
        //if ( time.SecondsFrom( iCurrentTime, tm ) == KErrNone )
        if ( /*time.DateTime().Second() == 59 ||*/ iCurrentTime.DateTime().Minute() != time.DateTime().Minute() )
            {
            //if ( tm.Int() >= 59 )
                {
                iCurrentTime = time;
                UpdateTime();
                }
            }
        return;
        }
    CApaCommandLine* cl = CApaCommandLine::NewLC();
    cl->SetExecutableNameL( _L("clock.exe") );

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
            if ( info.iFullName.FindF( _L("clock.exe") ) != KErrNotFound )
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

void CEightTileAppClock::GPAOObserver( CSGEUtilsGPAO* /*aClient*/, TInt aError )
    {
    if ( aError >= 0 )
        {
        //TAlarmChangeEvent event = aError;
        UpdateAlarm();
        }
    if ( aError != KErrCancel )
        {
        iAlarmSession.NotifyChange( iGpao->iStatus, iChangedAlarmId );
        iGpao->SetActive();
        }
    }


