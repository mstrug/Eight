/*
 ============================================================================
 Name		: EightTileAppPhone.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileAppPhone implementation
 ============================================================================
 */

#include "EightTileAppPhone.h"
#include "EightSettings.h"
#include "EightTileUiStatic.h"
//#include "logger.h"
#include <aknappui.h> 
#include <APGCLI.H> 
#include <APACMDLN.H> 
#include <APGWGNAM.H> 


CEightTileAppPhone::CEightTileAppPhone( TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi )
    : CEightTile( aId, aTileSize, aUi, EFalse ), iNetworkNamePckg( iNetworkName ), iShowNetworkName( ETrue )
    {
    }

CEightTileAppPhone::~CEightTileAppPhone()
    {
    delete iTelephony;
    delete iTimer;
    delete iGPAO;
    delete iMissedCallCalc;
    }

CEightTileAppPhone* CEightTileAppPhone::NewL( TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi )
    {
    CEightTileAppPhone* self = new ( ELeave ) CEightTileAppPhone( aId, aTileSize, aUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self;
    return self;
    }

void CEightTileAppPhone::ConstructL()
    {
    BaseConstructL();
    
    iTimer = CSGEUtilsGPTimer::NewL( *this );
    
    if ( iUi )
        {
        iGPAO = new (ELeave) CSGEUtilsGPAO( this, CActive::EPriorityStandard );
    
        iTelephony = CTelephony::NewL();
    
        iTelephony->GetCurrentNetworkName( iGPAO->iStatus, iNetworkNamePckg );
        iGPAO->SetActive();
    
        iNetworkNameIdx = ((CEightTileUiStatic*)iUi)->AddTextL( TVgRect( TVgPoint( 6, 70 ), TVgPoint( 70, 6 ) ), KFontId_SegoeWP_medium, _L("Phone"), EFalse, ESGEFontAlignHorizontalLeft, ESGEFontAlignVerticalBottom );
        
        iMissedCallCalc = CEightMissedCallCalc::NewL();
        iMissedCallsCount = iMissedCallCalc->Count();
        iMissedCallsIdx = ((CEightTileUiStatic*)iUi)->AddTextL( TVgRect( TVgPoint( 20, 3 ), TVgSize() ), KFontId_SegoeWP_largeNum, _L(""), ETrue ); //, ESGEFontAlignHorizontalLeft, ESGEFontAlignVerticalBottom );
        }
    }

void CEightTileAppPhone::UpdateMissedCallsCount()
    {
    TInt val = iMissedCallCalc->Count();
    if ( iMissedCallsCount != val )
        {
        iMissedCallsCount = val;
        if ( iMissedCallsCount == 0 )
            {
            ((CEightTileUiStatic*)iUi)->UpdateText( iMissedCallsIdx, _L("") );
            ((CEightTileUiStatic*)iUi)->UpdateRect( 0, TVgRect() );
            }
        else
            {
            if ( iMissedCallsCount > 9 && iTileSize == ETileSize_1x1 )
                {
                ((CEightTileUiStatic*)iUi)->UpdateText( iMissedCallsIdx, _L("9") );
                ((CEightTileUiStatic*)iUi)->UpdateRect( 0, TVgRect( TVgPoint( -10, 0 ), TVgSize() ) ); // icon
                }
            else
                {
                TBuf<10> num;
                num.Num( iMissedCallsCount );
                ((CEightTileUiStatic*)iUi)->UpdateText( iMissedCallsIdx, num );
                if ( iMissedCallsCount > 9 )
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
    }

void CEightTileAppPhone::SetRect( const TVgRect& aRect )
    {
    CEightTile::SetRect( aRect );
    
    CEightTileUiStatic* ui = ((CEightTileUiStatic*)iUi);
    if ( ui && iTileSize != ETileSize_3x1 )
        {
        ui->UpdateRect( iNetworkNameIdx, TVgRect( TVgPoint( 6, 70 ), TVgPoint( iRect.Width() - 6, 6 ) ) );
        ui->SetItemHide( iNetworkNameIdx, ( iTileSize == ETileSize_1x1 ) );
        
        iMissedCallsCount = -1; // wymusza update
        UpdateMissedCallsCount();
        }
    }

TEightTileType CEightTileAppPhone::Type() const
    {
    return ETileTypeAppShortcut;
    }

TBool CEightTileAppPhone::CanBeRemoved() const
    {
    return EFalse;
    }

TBool CEightTileAppPhone::RunInternal()
    {
    //const TUid KUidViewId = { 0x10282D81 };
    //const TUid KUidAppId = { 0x100058B3 };
    //const TUid KTouchDiallerViewCommand = { 1 };
    //TVwsViewId tv( KUidAppId, KUidViewId );
    //CCoeEnv::Static()->AppUi()->ActivateViewL( tv, KTouchDiallerViewCommand, KNullDesC8() );

//    RWsSession ws( CCoeEnv::Static()->WsSession() );
//    TApaTask task( ws );
//    task.SendToBackground();
    
    if ( !iTimer->IsTimerStarted() )
        {
        iTimer->Start( 50000 );
        }
    return ETrue;
    }

void CEightTileAppPhone::Update( TTimeIntervalMicroSeconds32 aTimeInterval )
    {
    if ( iTileSize != ETileSize_3x1 ) UpdateMissedCallsCount();
    CEightTile::Update( aTimeInterval );
    }

void CEightTileAppPhone::StartLogs()
    { // from LogsUiCmdStarter.inl NOT WORKING ON SYMBIAN BELLE
    _LIT8( KMissedView, "missed" );
    const TUid KLogsUID3    =   { 270486741 };      //101F4CD5 
    
    TPtrC8 msg;
    msg.Set( KMissedView );

    // Check that Logs application exists
    TApaAppInfo appInfo;
    RApaLsSession lsSession;
    User::LeaveIfError( lsSession.Connect() );        
    CleanupClosePushL( lsSession );
    User::LeaveIfError( lsSession.GetAppInfo( appInfo, /*LogsUiCmdStarterConsts::*/KLogsUID3 ) );

    // Create command line for starting Logs
    CApaCommandLine* cmdLine = CApaCommandLine::NewLC();
    cmdLine->SetExecutableNameL( appInfo.iFullName );
    cmdLine->SetCommandL( EApaCommandRun );

    // Check if Logs is already running
    TApaTaskList taskList( CCoeEnv::Static()->WsSession() );
    TApaTask task = taskList.FindApp( /*LogsUiCmdStarterConsts::*/KLogsUID3 );

    if( task.Exists() )
        {
        // Logs already running. Request it to activate the correct view
        const TUid dummyUID = { 0x0 };
        task.SendMessage( dummyUID, msg );
        
        // The below is just to secure an unlikely case that StartApp() would in future anyhow 
        // send command line parameter to application that is already running (currently
        // it doesn't). The "dontactivate" just prevents double attempt of activation of
        // view requested in msg SendMessage'd to Logs.
        _LIT8(dontActivate, "dontactivate");
        TPtrC8 dontActivateMsg = dontActivate(); 
        cmdLine->SetTailEndL( dontActivateMsg );  
        }
    else
        {
        // Logs not yet running. Request it to activate the correct view
        cmdLine->SetTailEndL( msg );  
        // Start Logs 
        lsSession.StartApp( *cmdLine );
        }
  
    CleanupStack::PopAndDestroy( cmdLine );
    CleanupStack::PopAndDestroy();  //lsSession      
    }

void CEightTileAppPhone::StartLogsNew()
    { // from pdk 4.0\sf\app\recents\recents_plat\logs_services_api\inc\logsservices.h
    /*XQApplicationManager appMgr;
    QScopedPointer<XQAiwRequest> request(appMgr.create(XQI_LOGS_VIEW, XQOP_LOGS_SHOW, false));   
    if (!request.isNull()) {
        int retValue = -1; 
        QList<QVariant> arglist;
        QVariantMap map;
        map.insert(XQLOGS_VIEW_INDEX, QVariant((int)XQService::LogsViewMissed));
        map.insert(XQLOGS_SHOW_DIALPAD, QVariant(true));
        map.insert(XQLOGS_DIALPAD_TEXT, QVariant(QString()));
        arglist.append(QVariant(map));
        request->setArguments(arglist);
        QVariant ret(retValue);        
        request->send(ret);
    }*/
    }

void CEightTileAppPhone::GPTimerEvent( CSGEUtilsGPTimer* /*aTimer*/ )
    {
    if ( iMissedCallsCount == 0 )
        {
        CCoeEnv::Static()->AppUi()->ActivateViewL( TVwsViewId( TUid::Uid( 0x100058B3 ), TUid::Uid( 0x10282D81 ) ), TUid::Uid( 1 ), KNullDesC8() );
        }
    else
        {    
        // LogsUiCmdStarter::CmdStartL( LogsUiCmdStarterConsts::KMissedView() );  
        StartLogs();

        CApaCommandLine* cl = CApaCommandLine::NewLC();
        cl->SetExecutableNameL( _L("logs.exe") );
        cl->SetCommandL( EApaCommandRun );
        cl->SetTailEndL( _L8("missed") );  // this is not working on Symbian Belle because of qt app
        
        RApaLsSession ls;
        ls.Connect();
        
        TInt err = ls.StartApp( *cl );
        
        ls.Close();
        CleanupStack::PopAndDestroy();
        }
    }

void CEightTileAppPhone::GPAOObserver( CSGEUtilsGPAO* /*aClient*/, TInt aError )
    {
    //Logger::LogStrNum( _L("c:\\data\\e.txt"), iNetworkNamePckg().iNetworkName, aError );
    if ( aError == KErrNone )
        {
        if ( iNetworkName.iNetworkName.Length() > 0 )
            {
            ((CEightTileUiStatic*)iUi)->UpdateText( iNetworkNameIdx, iNetworkName.iNetworkName );
            }
        }
    else if ( aError == KErrAccessDenied )
        { // offline?
        ((CEightTileUiStatic*)iUi)->UpdateText( iNetworkNameIdx, _L("Offline") );
        }
    }

void CEightTileAppPhone::GPAOObserverCancel( CSGEUtilsGPAO* /*aClient*/ )
    {
    iTelephony->CancelAsync( CTelephony::EGetCurrentNetworkNameCancel );
    }


