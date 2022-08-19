/*
 ============================================================================
 Name        : EightApplication.cpp
 Author      : Michal Strug
 Company     : 
 Version     : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : Main application class
 ============================================================================
 */

// INCLUDE FILES
#include "EightApplication.h"
#include "EightSettings.h"
#include "EightPhotosProvider.h"
#include "EightMessagingInterface.h"
#include "EightWindowFR1.h"
#include "EightWindowFR2.h"
#include "EightWindowFR3.h"
#include "EightWindowFR4.h"
#include "EightWindowColor.h"
//#include "logger.h"
#include <VG/vgu.h>
#include <VG/openvg.h>
#include <SGEVg/VgPrimitives.h>
#include <SGEVg/VgImage.h>
#include <SGEBitmap/SGEBitmapUtils.h>
#include <SGEBitmap/SGEBitmapItem.h>
#include <bautils.h> 

#define LOG_STARTUP( a ) //Logger::LogStr( _L("c:\\data\\estart.txt"), a );
#define LOG_STARTUP_NUM( a, b ) //Logger::LogStrNum( _L("c:\\data\\estart.txt"), a, b );

//CVgFont* iFont = NULL;

// -----------------------------------------------------------------------------
// CEightApplication::CEightApplication()
// -----------------------------------------------------------------------------
//
CEightApplication::CEightApplication()
    {
    }

// -----------------------------------------------------------------------------
// CEightApplication::~CEightApplication()
// -----------------------------------------------------------------------------
//
CEightApplication::~CEightApplication()
    {
    delete iGestureHelper;
    delete iAppFactory;
    delete iMsgBox;
    delete iFormWindowManager;
    delete iFormWindowManagerFirstStart;
    delete iFrameTimesBuffer;
    delete iTimer;
    iGoomSession.Close();
    
//    RThread thr;
//    TInt a, b;
//    thr.HandleCount( a, b );
//    Logger::LogStrNum( _L("c:\\data\\ehnd.txt"), _L("1: "), a );
//    Logger::LogStrNum( _L("c:\\data\\ehnd.txt"), _L("2: "), b );
    }

void CEightApplication::CheckFirstStartL( TBool aAlways )
	{
	CSGEFileProvider& fp = CEightSettings::GetInstance()->FileProvider();
    TFileName& fn = fp.AppendToPrivateFolder( _L("frun.dat") );

	if ( aAlways || !BaflUtils::FileExists( fp.Fs(), fn ) || BaflUtils::FileExists( fp.Fs(), _L("d:\\system\\temp\\eight.frn") ) )
		{
		delete iFormWindowManagerFirstStart;
		iFormWindowManagerFirstStart = NULL;
		iFormWindowManagerFirstStart = CEightFormWindowManager::NewL();
		iFormWindowManagerFirstStart->SetNextWindowActivateOnClose();
		iFormWindowManagerFirstStart->ShowWindowL( CEightWindowFR4::NewL( Size() ), ETrue );
		iFormWindowManagerFirstStart->ShowWindowL( CEightWindowFR3::NewL( Size() ), ETrue );
		iFormWindowManagerFirstStart->ShowWindowL( CEightWindowFR2::NewL( Size() ), ETrue );
		iFormWindowManagerFirstStart->ShowWindowL( CEightWindowColor::NewL( NULL, Size(), ETrue, ETrue ), ETrue );
		iFormWindowManagerFirstStart->ShowWindowL( CEightWindowFR1::NewL( Size() ), ETrue );
		}
	}

void CEightApplication::InitL()
    {
    CSGEGameApplicationVG::InitL();
    LOG_STARTUP( _L("InitL"));
    TRAPD( err, InitInternalL() );
    if ( err != KErrNone )
    	{
        LOG_STARTUP_NUM( _L("InitL err: "), err );
    	User::Leave( err );
    	}
    }
    
void CEightApplication::InitInternalL()
	{
    LOG_STARTUP( _L("InitInternalL 0") );
    CEightMessagingInterface::GetInstance();
    LOG_STARTUP( _L("InitInternalL 1") );
    CEightSettings* settings = CEightSettings::GetInstance();
    LOG_STARTUP( _L("InitInternalL 2") );
    CEightPhotosProvider* pp = CEightPhotosProvider::GetInstanceL();
    LOG_STARTUP( _L("InitInternalL 3") );
    pp->RegisterImageSourceL( KPhotosSourceIdInternalFiles, CEightPhotosProvider::EFile );
    pp->RegisterImageSourceL( KPhotosSourceIdContacts, CEightPhotosProvider::EContact );
    pp->RegisterImageSourceL( KPhotosSourceIdDCIM, CEightPhotosProvider::EGallery );
    pp->SetHighFpsIdle( EFalse );

    LOG_STARTUP( _L("InitInternalL 4") );

//    BaflUtils::CopyFile( settings->FileProvider().Fs(), _L("c:\\private\\2006AF94\\settings.dat"), _L("c:\\data\\esettings.dat") );
//    BaflUtils::CopyFile( settings->FileProvider().Fs(), _L("c:\\private\\2006AF94\\tiles.dat"), _L("c:\\data\\etiles.dat") );

    //BaflUtils::CopyFile( settings->FileProvider().Fs(), _L("c:\\private\\2006AF94\\iaSdkSettingsFile_0xE7A4C60E.dat"), _L("c:\\data\\1_iaSdkSettingsFile_0xE7A4C60E.dat") );
    //BaflUtils::CopyFile( settings->FileProvider().Fs(), _L("c:\\private\\2006AF95\\iaSdkSettingsFile_0xE7A4C60E.dat"), _L("c:\\data\\2_iaSdkSettingsFile_0xE7A4C60E.dat") );
    
    
    //LoadSvgImagesL();
    //LoadPhotoImagesL();

    iTileManager[ 0 ] = CEightTileManager::NewL( *this, 12, 75, Size() );
    iTileManager[ 1 ] = CEightTileManager::NewL( *this, 12, 75, Size() );
    //LoadTilesL();
    LOG_STARTUP( _L("InitInternalL 5") );
    iTileManagerFactory = CEightTileManagerFactory::NewL( Size(), MainCoeControl() );
    LOG_STARTUP( _L("InitInternalL 6") );
    iTileManagerFactory->CreateTilesPage( iTileManager[ 0 ] );
    LOG_STARTUP( _L("InitInternalL 7") );
    iTileManagerFactory->CreateShortcutsPage( iTileManager[ 1 ] );
    LOG_STARTUP( _L("InitInternalL 8") );
    iTileManager[ 0 ]->UpdateTilesColors( iTileManager[ 1 ] );
    LOG_STARTUP( _L("InitInternalL 9") );
    iTileManager[ 1 ]->UpdatePinTiles( iTileManager[ 0 ] );
    LOG_STARTUP( _L("InitInternalL 10") );


//    iAppFactory = CEightAppFactory::NewL();
//    iAppFactory->StartL( iTileManager[ 1 ] );
    
    iGestureHelper = CAknTouchGestureFw::NewL( *this, (CCoeControl&)*this->iAppView );
    LOG_STARTUP( _L("InitInternalL 11") );
    iGestureHelper->SetGestureInterestL( EAknTouchGestureFwGroupTap | EAknTouchGestureFwGroupFlick | EAknTouchGestureFwGroupDrag );
    
    LOG_STARTUP( _L("InitInternalL 12") );
    iMsgBox = new (ELeave) CEightMsgBox( Size(), settings->FontManager(), KFontId_SegoeWP_large, KFontId_SegoeWP_small, KFontId_SegoeWP_medium );
    iMsgBox->RegisterObserver( this );
    iTileManager[ 0 ]->SetMsgBox( iMsgBox );
    iTileManager[ 1 ]->SetMsgBox( iMsgBox );
    
    LOG_STARTUP( _L("InitInternalL 13") );
    iFormWindowManager = CEightFormWindowManager::NewL();
    iTileManager[ 0 ]->SetFormWindowManager( iFormWindowManager );
    iTileManager[ 1 ]->SetFormWindowManager( iFormWindowManager );
    
    LOG_STARTUP( _L("InitInternalL 14") );
    iTileManager[ 0 ]->StartReady();

    LOG_STARTUP( _L("InitInternalL 15") );
    iFrameTimesBuffer = CSGECircularBuffer< TInt >::NewL( 60 * 1, 1000000 / 60 );

    CEikonEnv::Static()->SetSystem( ETrue );
    
	SetTitleL( _L("") );
	
	iTimer = CSGEUtilsGPTimer::NewL( *this );
	
    LOG_STARTUP( _L("InitInternalL 16") );
	User::LeaveIfError( iGoomSession.Connect() );
	iGoomSession.ApplicationAboutToStart( ApplicationUid() );
	iGoomSession.SetGOomPriority( RGOomMonitorSession::EGOomPriorityHigh );
	if ( iGoomSession.RequestFreeMemory( 8 * 1024 * 1024 ) != KErrNone )
		{
		if ( iGoomSession.RequestFreeMemory( 6 * 1024 * 1024 ) != KErrNone )
			{
			iGoomSession.RequestFreeMemory( 4 * 1024 * 1024 );
			}
		}	

    LOG_STARTUP( _L("InitInternalL 17") );
    iShowFps = BaflUtils::FileExists( settings->FileProvider().Fs(), _L("d:\\system\\temp\\eight.fps") );
    LOG_STARTUP( _L("InitInternalL 18") );
    iLightDisplay = BaflUtils::FileExists( settings->FileProvider().Fs(), _L("d:\\system\\temp\\eight.lig") );
    LOG_STARTUP( _L("InitInternalL 19") );

#ifndef __WINS__  
    iTouchFeedback = MTouchFeedback::Instance();
#endif
    
    LOG_STARTUP( _L("InitInternalL 20") );
    CheckFirstStartL();
    LOG_STARTUP( _L("InitInternalL 21") );

#ifdef EIGHT_PREMIUM
    if ( settings->iCaptureMenuKey )
        {
        LOG_STARTUP( _L("InitInternalL 22") );
        iHandleMenuKey = CCoeEnv::Static()->RootWin().CaptureKey( EKeyApplication0, 0, 0, 0x7FFFFFFF );
        iHandleMenuKeyUpDown = CCoeEnv::Static()->RootWin().CaptureKeyUpAndDowns( EStdKeyApplication0, 0, 0, 0x7FFFFFFF );
        }
#endif

    SetGameLoopMaxFPS( 999 );
    StartGameLoop();
    LOG_STARTUP( _L("InitInternalL end") );
    }

void CEightApplication::Cleanup()
	{
    iTileManagerFactory->ExternalizeL( iTileManager[ 0 ] );
    delete iTileManager[ 0 ];
    delete iTileManager[ 1 ];
    delete iTileManagerFactory;

    CEightPhotosProvider::GetInstanceL()->Destroy();
    CEightMessagingInterface::GetInstance()->Destroy();
    CEightSettings::GetInstance()->Destroy();
    
    if ( iHandleMenuKey > 0 )
        {
        CCoeEnv::Static()->RootWin().CancelCaptureKey( iHandleMenuKey );
        }
    if ( iHandleMenuKeyUpDown > 0 )
        {
        CCoeEnv::Static()->RootWin().CancelCaptureKeyUpAndDowns( iHandleMenuKeyUpDown );
        }
	}

// -----------------------------------------------------------------------------
// CEightApplication::NewApplication()
// -----------------------------------------------------------------------------
//
CApaApplication* CEightApplication::NewApplication()
    {
    return CSGEGameApplicationVG::NewApplication( new CEightApplication );
    }

// -----------------------------------------------------------------------------
// CEightApplication::ApplicationUid()
// -----------------------------------------------------------------------------
//
TUid CEightApplication::ApplicationUid()
    {
    return KUidEight;
    }

// -----------------------------------------------------------------------------
// CEightApplication::HandleCommandL()
// -----------------------------------------------------------------------------
//
void CEightApplication::HandleCommandL( TInt /*aCommand*/)
    {
    //Exit();
    }

TBool CEightApplication::HandleWsEventL( const TWsEvent& aEvent, CCoeControl* aDestination )
    {
//    if ( aEvent.Type() == KAknUidValueEndKeyCloseEvent )
//        {
//        return ETrue;
//        }
//    else if ( aEvent.Type() == EEventKey )
//        {//TKeyCode
//        TKeyEvent* ke = aEvent.Key();
//        if ( ke->iCode == EKeyApplication0 )
//            {
//            return ETrue;
//            }
//        }
    CEightSettings* settings = CEightSettings::GetInstance();
	
	if ( aEvent.Type() == EEventUser )
        {
        TApaSystemEvent* eventData = reinterpret_cast<TApaSystemEvent*>( aEvent.EventData() );
        if ( (*eventData) == EApaSystemEventShutdown )
            {
            /* taken from goommonitorsession.h (plugin) */
            //const TInt KGoomMemoryLowEvent = 0x10282DBF;
            //const TInt KGoomMemoryGoodEvent = 0x20026790;

            eventData++;
            if ((*eventData) == KGoomMemoryLowEvent)
                {
                //Logger::LogStr( _L("emem.txt"), _L("Low memory") );
                //return ETrue; // stop the event from killing us (not working unless using RGOomSession)
                }
            else if ((*eventData) == KGoomMemoryGoodEvent)
                {
                //Logger::LogStr( _L("emem.txt"), _L("High memory") );
                //return ETrue; // stop the event from killing us (not working unless using RGOomSession)
                }
            else
                {
                //Logger::LogStrNum( _L("emem.txt"), _L("event: "), *eventData );
                }
            }
        }
	else if ( aEvent.Type() == KAknUidValueEndKeyCloseEvent && settings->iCloseOnRedKeyPress )
        {
	    iExitRequested = ETrue;
	    Exit();
        return ETrue;
        }
    
#ifdef EIGHT_PREMIUM
    if ( aEvent.Type() == EEventKey )
        {
        TKeyEvent* ke = aEvent.Key();
        if ( settings->iCaptureMenuKey && ke->iCode == EKeyApplication0 && ke->iRepeats > 0 )
            { 
            iMenuKeyLong = ETrue;
            CEikonEnv::Static()->DisplayTaskList();
            return ETrue;
            }
        else if ( !settings->iCaptureMenuKey && ke->iCode == EKeyApplication0 )
            {
            return EFalse;
            }
        }
    else if ( aEvent.Type() == EEventKeyUp )
        {
        TKeyEvent* ke = aEvent.Key();
        if ( settings->iCaptureMenuKey && !iMenuKeyLong && ke->iScanCode == EStdKeyApplication0 )
            {
            TApaTaskList tasklist( WsSession() );
            TApaTask task(tasklist.FindApp( KUidEight ));
            if(task.Exists())
                {
                task.BringToForeground();
                }       
            
            return ETrue;
            }
        else if ( !settings->iCaptureMenuKey && ke->iScanCode == EStdKeyApplication0 )
            {
            return EFalse;
            }
        }
    else if ( aEvent.Type() == EEventKeyDown )
        {
        TKeyEvent* ke = aEvent.Key();
        if ( settings->iCaptureMenuKey && ke->iScanCode == EStdKeyApplication0 )
            {
            iMenuKeyLong = EFalse;
            if ( iForeground )
                {
                TApaTaskList tasklist( WsSession() );
                TApaTask task(tasklist.FindApp( KUidEight ));
                if(task.Exists())
                    {
                    task.BringToForeground();
                    }       
                }
            
            return ETrue;
            }
        else if ( !settings->iCaptureMenuKey && ke->iScanCode == EStdKeyApplication0 )
            {
            return EFalse;
            }
        }
#endif
	
    return CSGEGameApplicationVG::HandleWsEventL( aEvent, aDestination );
    }

TBool CEightApplication::HandlePointerEventL( const TPointerEvent& aPointerEvent )
	{
    if ( iSwappingTilesManagers ) return EFalse;
    
    TPointerEvent pe( aPointerEvent );
    pe.iPosition.iY = Size().iHeight - pe.iPosition.iY; // flip Y pos
	//Logger::LogStrNum( _L("c:\\data\\akntouch.txt"), _L( "Pointer Event X: "), aPointerEvent.iPosition.iX );
	//Logger::LogStrNum( _L("c:\\data\\akntouch.txt"), _L( "Pointer Event Y: "), aPointerEvent.iPosition.iY );
	//Logger::LogStrNum( _L("c:\\data\\akntouch.txt"), _L( "Pointer Type: "), aPointerEvent.iType );
	iGesturePosition = pe.iPosition;
	
    if ( aPointerEvent.iType == TPointerEvent::EButton1Down /*|| aPointerEvent.iType == TPointerEvent::EButton1Up*/ )
        {
        if ( CEightSettings::GetInstance()->iUseTouchFeedback && iTouchFeedback )
            {
            iTouchFeedback->InstantFeedback( ETouchFeedbackBasicButton );
            }
        }
	
	if ( iFormWindowManagerFirstStart )
		{
		iFormWindowManagerFirstStart->HandlePointerEventL( pe );
		}
	else if ( iMsgBox && iMsgBox->IsVisible() )
        {
        return iMsgBox->HandlePointerEventL( pe );
        }
	else if ( iFormWindowManager && iFormWindowManager->IsVisible() )
        {
	    return iFormWindowManager->HandlePointerEventL( pe );
        }
	else if ( iTileManager[ iActiveTileManaterIdx ] )
	    {
	    return iTileManager[ iActiveTileManaterIdx ]->HandlePointerEventL( pe );
	    }
	return EFalse;
	}

void CEightApplication::HandleForegroundEventL( TBool aForeground )
    {
    iForeground = aForeground;
    if ( !aForeground && iTileManager[ iActiveTileManaterIdx ] )
        {
        iTileManager[ iActiveTileManaterIdx ]->ResetPointerEvents(); // todo: call this before showing of msg box
        }
    iTileManager[ 0 ]->HandleForegroundEventL( aForeground );
    CSGEGameApplicationVG::HandleForegroundEventL( aForeground );
    }

void CEightApplication::StartSwap()
    {
    iTileManager[ iActiveTileManaterIdx ]->ResetPointerEvents();
    if ( iActiveTileManaterIdx == 1 )
        {
        iActiveTileManaterIdx = 0;
        }
    else
        {
        iActiveTileManaterIdx = 1;
        }
    iSwappingTilesManagers = ETrue;
    iSwappingTilesManagersOffset = Size().iWidth;
    }

void CEightApplication::HandleTouchGestureL( MAknTouchGestureFwEvent& aEvent )
	{
    if ( !iTileManager[ iActiveTileManaterIdx ] || iSwappingTilesManagers ) return;
	if ( iFormWindowManagerFirstStart )
		{
		iFormWindowManagerFirstStart->HandleTouchGestureL( aEvent );
		return;
		}
    if ( iMsgBox && iMsgBox->IsVisible() ) return;
    if ( iFormWindowManager && iFormWindowManager->IsVisible() ) 
        {
        iFormWindowManager->HandleTouchGestureL( aEvent );
        return;
        }
	//Logger::LogStrNum( _L("c:\\data\\akntouch.txt"), _L( "TG Event group: "), aEvent.Group() );
	//Logger::LogStrNum( _L("c:\\data\\akntouch.txt"), _L( "TG Event type: "), aEvent.Type() );
	if ( aEvent.Group() == EAknTouchGestureFwGroupTap )
		{
	    TPoint p = ((MAknTouchGestureFwTapEvent&)aEvent).Position();
	    p.iY = Size().iHeight - ((MAknTouchGestureFwTapEvent&)aEvent).Position().iY;
		switch ( aEvent.Type() )
			{
			case EAknTouchGestureFwTap:
					iTileManager[ iActiveTileManaterIdx ]->TouchGestureTap( p );
				break;
			case EAknTouchGestureFwDoubleTap:
				break;
			case EAknTouchGestureFwLongTap:
                    if ( CEightSettings::GetInstance()->iUseTouchFeedback && iTouchFeedback )
                        {
                        iTouchFeedback->InstantFeedback( ETouchFeedbackLongTap );
                        }
					iTileManager[ iActiveTileManaterIdx ]->TouchGestureLongTap( p );
				break;
			default:
				break;
			}
		}
	else if ( aEvent.Group() == EAknTouchGestureFwGroupDrag && aEvent.Type() == EAknTouchGestureFwDrag )
		{
	    MAknTouchGestureFwDragEvent& ev = (MAknTouchGestureFwDragEvent&)aEvent;
	    if ( ev.State() == EAknTouchGestureFwStart )
	        {
            TPoint p = ev.StartPosition();
            p.iY = Size().iHeight - p.iY;
            TPoint pc = ev.CurrentPosition();
            pc.iY = Size().iHeight - pc.iY;
	        iTileManager[ iActiveTileManaterIdx ]->TouchGestureDragStart( p, pc );
	        }
	    else if ( ev.State() == EAknTouchGestureFwOn )
            {
	        // todo: add screen height offset!
	        TPoint p2 = ev.PreviousPosition();
	        TPoint p1 = ev.CurrentPosition();
            iTileManager[ iActiveTileManaterIdx ]->TouchGestureDrag( TSize( p2.iX - p1.iX, p2.iY - p1.iY ) );
            }
	    else if ( ev.State() == EAknTouchGestureFwStop )
            {
            if ( CEightSettings::GetInstance()->iUseTouchFeedback && iTouchFeedback )
                {
                iTouchFeedback->InstantFeedback( ETouchFeedbackBasicButton );
                }
            iTileManager[ iActiveTileManaterIdx ]->TouchGestureDragStop();	                
            }
		}
	else if ( aEvent.Group() == EAknTouchGestureFwGroupFlick )
		{
	    MAknTouchGestureFwFlickEvent& ev = (MAknTouchGestureFwFlickEvent&)aEvent;
        TPoint p = ev.Position();
        p.iY = Size().iHeight - ev.Position().iY;
		switch ( aEvent.Type() )
			{
			case EAknTouchGestureFwFlickLeft:
                    //iTileManager[ iActiveTileManaterIdx ]->TouchGestureFlickLeft();
                    if ( iActiveTileManaterIdx == 0 && iTileManager[ iActiveTileManaterIdx ]->CanSwapLeftRight() )
                        {
                        StartSwap();
                        }
				break;
			case EAknTouchGestureFwFlickRight:
					//iTileManager[ iActiveTileManaterIdx ]->TouchGestureFlickRight();
                    if ( iActiveTileManaterIdx == 1 && iTileManager[ iActiveTileManaterIdx ]->CanSwapLeftRight() )
                        {
                        StartSwap();
                        }
				break;
			case EAknTouchGestureFwFlickUp:
                    iTileManager[ iActiveTileManaterIdx ]->TouchGestureFlickUp( ev.Speed().iY );
				break;
			case EAknTouchGestureFwFlickDown:
                    iTileManager[ iActiveTileManaterIdx ]->TouchGestureFlickDown( ev.Speed().iY );
				break;
			default:
				break;
			}
		}
//	iGesturePosition.SetXY( -1, -1 );
	}

void CEightApplication::UpdateCaptureMenuKey()
    {
#ifdef EIGHT_PREMIUM
        if ( CEightSettings::GetInstance()->iCaptureMenuKey )
            {
            if ( iHandleMenuKey == 0 )
                {
                iHandleMenuKey = CCoeEnv::Static()->RootWin().CaptureKey( EKeyApplication0, 0, 0, 0x7FFFFFFF );
                }
            if ( iHandleMenuKeyUpDown == 0 )
                {
                iHandleMenuKeyUpDown = CCoeEnv::Static()->RootWin().CaptureKeyUpAndDowns( EStdKeyApplication0, 0, 0, 0x7FFFFFFF );
                }
            }
        else
            {
            if ( iHandleMenuKey > 0 )
                {
                CCoeEnv::Static()->RootWin().CancelCaptureKey( iHandleMenuKey );
                iHandleMenuKey = 0;
                }
            if ( iHandleMenuKeyUpDown > 0 )
                {
                CCoeEnv::Static()->RootWin().CancelCaptureKeyUpAndDowns( iHandleMenuKeyUpDown );
                iHandleMenuKeyUpDown = 0;
                }
            }
#endif
    }

void CEightApplication::TileManagerAction( CEightTileManager& aTileManager, TTileManagerAction aAction, TInt aParam )
	{
    if ( aAction == MEightTileManagerObserver::EExit )
        {
        iMsgBox->ShowL( KMsgBoxMessageExit, CEightMsgBox::EMsgBoxButtonYesNo, _L("Do you want to quit?") );
        }
    else if ( aAction == MEightTileManagerObserver::EExitAnimFinished )
        {
        iExitRequested = ETrue;
        }
    else if ( aAction == MEightTileManagerObserver::ESwapLeft )
        {
        StartSwap();
        }
    else if ( aAction == MEightTileManagerObserver::ESwapRight )
        {
        StartSwap();
        }
    else if ( aAction == MEightTileManagerObserver::ESettigsCaptureMenuKey )
        {
        UpdateCaptureMenuKey();
        }
    else if ( aAction == MEightTileManagerObserver::ESettigsUpdated )
        {
        UpdateCaptureMenuKey();
        
        if ( &aTileManager == iTileManager[ 0 ] )
            {
            iTileManager[ 1 ]->UpdateTilesColor();
            }
        else
            {
            iTileManager[ 0 ]->UpdateTilesColor();
            }
        iTileManager[ 0 ]->UpdateSpcialRow();
        }
    else if ( aAction == MEightTileManagerObserver::ETileColorChanged )
        {
        if ( &aTileManager == iTileManager[ 0 ] )
            {
            CEightTile* tile = iTileManager[ 0 ]->Tile( aParam );
            if ( tile )
                {
                iTileManager[ 1 ]->UpdateTileColor( aParam, tile->CustomColor(), tile->Palette() );
                }
            }
        }
    else if ( aAction == MEightTileManagerObserver::EReset )
        {
        if ( &aTileManager == iTileManager[ 0 ] )
            {
            CEightTile* setup = iTileManager[ 0 ]->GetAppSetupTile(); 
            delete iTileManager[ 0 ];
            iTileManager[ 0 ] = NULL;
            iTileManager[ 0 ] = CEightTileManager::NewL( *this, 12, 75, Size() );
            iTileManagerFactory->ResetStoredFile();
            iTileManagerFactory->CreateTilesPage( iTileManager[ 0 ] );
            //CEightSettings::GetInstance()->ResetSettingsFile();
            iTileManager[ 0 ]->SetMsgBox( iMsgBox );
            iTileManager[ 0 ]->SetFormWindowManager( iFormWindowManager );    
            iTileManager[ 0 ]->NormalReady();
            setup->UpdateColorsFromTheme();
            if ( !iTileManager[ 0 ]->SetAppSetupTile( setup ) )
                {
                delete setup;
                }
            }
        else
            {
            delete iTileManager[ 0 ];
            iTileManager[ 0 ] = NULL;
            iTileManager[ 0 ] = CEightTileManager::NewL( *this, 12, 75, Size() );
            iTileManagerFactory->ResetStoredFile();
            iTileManagerFactory->CreateTilesPage( iTileManager[ 0 ] );
            //CEightSettings::GetInstance()->ResetSettingsFile();
            iTileManager[ 0 ]->SetMsgBox( iMsgBox );
            iTileManager[ 0 ]->SetFormWindowManager( iFormWindowManager );    
            iTileManager[ 0 ]->NormalReady();
            }
        iTileManager[ 1 ]->ResetTilesColor();
        iTileManager[ 1 ]->UpdatePinTiles( iTileManager[ 0 ] );
        }
    else if ( aAction == MEightTileManagerObserver::EStartingApp )
    	{
        if ( aParam == KIdxTileAds && &aTileManager == iTileManager[ 1 ] )
            {
            if ( !iTileManager[ 0 ]->Tile( aParam ) )
                {
                iMsgBox->ShowL( KMsgBoxMessagePinAds, CEightMsgBox::EMsgBoxButtonOk, _L("Pin Ads tile to the first screen.") );
                }
            else if ( iTileManager[ 0 ]->RunTile( aParam ) )
                {
                SetTitleL( _L("Launching...") );
                iTimer->Start( 600000 );
                }
            }
        else
            {
            SetTitleL( _L("Launching...") );
            iTimer->Start( 600000 );
            }
    	}
    else if ( aAction == MEightTileManagerObserver::EActivateFRWizzard )
    	{
    	CheckFirstStartL( ETrue );
    	}
	}

void CEightApplication::TileManagerRemovedTile( CEightTileManager& aTileManager, TUint aTileId )
    {
    if ( &aTileManager == iTileManager[ 0 ] )
        {
        iTileManager[ 1 ]->SetPinTile( aTileId, EFalse );
        }
    else if ( &aTileManager == iTileManager[ 1 ] )
        {
        iTileManager[ 0 ]->RemoveTile( aTileId );
        iTileManager[ 1 ]->SetPinTile( aTileId, EFalse );
        }
    }

void CEightApplication::TileManagerAddTile( CEightTileManager& aTileManager, TUint aTileId )
    {
    if ( &aTileManager == iTileManager[ 1 ] )
        {
    	CEightTile* tile = aTileManager.Tile( aTileId );
        if ( iTileManagerFactory->AddTile( iTileManager[ 0 ], aTileId, tile->CustomColor(), tile->Palette() ) )
            {
            iTileManager[ 1 ]->SetPinTile( aTileId, ETrue );
        
            StartSwap();
            //iActiveTileManaterIdx = 0;
            }
        }
    }

void CEightApplication::ImageLoaded( TInt /*aImageId*/, TInt /*aError*/, TInt /*aSource*/ )
    {
    }

void CEightApplication::MsgBoxAction( TUint aMessageId, TInt aActionId )
    {
    if ( aMessageId == KMsgBoxMessageExit && aActionId == KMsgBoxButtonYes )
        {
        iFormWindowManager->CloseAllWindows();
        iTileManager[ 0 ]->EndReady();
        iActiveTileManaterIdx = 0;
        iMsgBox->SetBlock();
        //iExitRequested = ETrue;
        }
    else if ( aMessageId == KMsgBoxMessageRemoveTile && aActionId == KMsgBoxButtonYes )
        {
        iTileManager[ iActiveTileManaterIdx ]->RemoveSelectedTile();
        }
    }

void CEightApplication::MsgBoxWillShow()
    {
    iTileManager[ iActiveTileManaterIdx ]->ResetPointerEvents();
    }

void CEightApplication::GPTimerEvent( CSGEUtilsGPTimer* aTimer )
	{
    if ( aTimer == iTimer )
        {
        SetTitleL( _L("" ) );
        iTileManager[ iActiveTileManaterIdx ]->RunTimeout();
        //DrawScene();
        }
	}

// -----------------------------------------------------------------------------
// CEightApplication::Draw()
// -----------------------------------------------------------------------------
//
void CEightApplication::Draw( const TRect& /*aRect*/) const
    {
    const_cast<CEightApplication*> ( this )->DrawScene();
    }

// -----------------------------------------------------------------------------
// CEightApplication::FullScreen()
// -----------------------------------------------------------------------------
//
TBool CEightApplication::FullScreen() const
    {
    return EFalse;
    }

TSGEApplicationOrientation CEightApplication::Orientation() const
    {
    return ESGEApplicationOrientationPortrait;
    }

// -----------------------------------------------------------------------------
// CEightApplication::SampleBuffers()
// -----------------------------------------------------------------------------
//
TInt CEightApplication::SampleBuffers()
    {
    return 0;
    }

// -----------------------------------------------------------------------------
// CEightApplication::SampleSize()
// -----------------------------------------------------------------------------
//
TInt CEightApplication::SampleSize()
    {
    return 0;
    }

TInt CEightApplication::DepthSize()
    {
    return 0;
    }

TBool CEightApplication::GoBackgroundOnEndKey() const
	{
    //CEightSettings* settings = CEightSettings::GetInstance();
    //return !settings->iCloseOnRedKeyPress;
    return EFalse;
	}

void CEightApplication::ExitApplication()
    {
    Exit();
    }

// -----------------------------------------------------------------------------
// CEightApplication::DoGameLoopEvent()
// -----------------------------------------------------------------------------
//
#ifdef DUMP_FRAMES
TInt fileCnt = 0;
#endif
void CEightApplication::DoGameLoopEvent( TTimeIntervalMicroSeconds32 aTimeInterval )
    {
#ifdef DUMP_FRAMES
    aTimeInterval = 1000000 / 60;
#endif
    
    if ( UpdateScene( aTimeInterval ) )
        {
        DrawScene();
        
#ifdef DUMP_FRAMES
        if ( fileCnt > 2000 ) return;
        TInt w = Size().iWidth;
        TInt h = Size().iHeight;
        TInt size = sizeof( TUint8 ) * w * h * 4;
        TUint8* buffer = (TUint8*) User::Alloc( size );
        vgReadPixels( buffer, w * 4, VG_sARGB_8888, 0, 0, w, h );
        VGErrorCode error = vgGetError();
        if ( error == VG_NO_ERROR )
            {
            TInt fsize = size + 14 + 40;
            const TUint8 header[] = { 'B', 'M', ( fsize & 0xff ), ( fsize & 0xff00 ) >> 8, ( fsize & 0xff0000 ) >> 16, 
                                      ( fsize & 0xff000000 ) >> 24, 0, 0, 0, 0, 14 + 40, 0, 0, 0,
                                      40, 0, 0, 0, ( w & 0xff ), ( w & 0xff00 ) >> 8, 0, 0, ( h & 0xff ), ( h & 0xff00 ) >> 8, 0, 0, 1, 0, 32, 0, 0, 0, 0, 0, 
                                      ( size & 0xff ), ( size & 0xff00 ) >> 8, ( size & 0xff0000 ) >> 16, ( size & 0xff000000 ) >> 24,
                                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; 
            RFs& fs( CEightSettings::GetInstance()->FileProvider().Fs() );
            RFile file;
            TBuf<200> fname;
            fname.Format( _L("e:\\data\\eimg\\frame%04d.bmp"), fileCnt++ );
            file.Replace( fs, fname, EFileWrite );
            TPtrC8 ptr1( header, sizeof( header ) );
            file.Write( ptr1 );
            TPtrC8 ptr2( buffer, size );
            file.Write( ptr2 );
            file.Close();
            }
        User::Free( buffer );
#endif
        }
    }

// -----------------------------------------------------------------------------
// CEightApplication::UpdateScene()
// -----------------------------------------------------------------------------
//
TBool CEightApplication::UpdateScene( TTimeIntervalMicroSeconds32 aTimeInterval )
    {
    TInt cval = iFrameTimesBuffer->CurrentValue();
    if ( Abs( cval - aTimeInterval.Int() ) > 1000 ) iFrameTimesBuffer->AddNewValue( ( aTimeInterval.Int() > cval ? cval + 1000 : cval - 1000 ) );
    else iFrameTimesBuffer->AddNewValue( aTimeInterval.Int() );
    cval = iFrameTimesBuffer->CurrentValue();

    if ( iLightDisplay )
        {
        User::ResetInactivityTime();
        }
    
	if ( iExitRequested )
		{
	    ExitApplication();
		return EFalse;
		}
	
	if ( iFormWindowManagerFirstStart )
		{
		iFormWindowManagerFirstStart->Update( cval );
		if ( !iFormWindowManagerFirstStart->IsVisible() )
			{
			delete iFormWindowManagerFirstStart;
			iFormWindowManagerFirstStart = NULL;
			iTileManager[ 0 ]->UpdateTilesColor();
			iTileManager[ 1 ]->UpdateTilesColor();
			
			CSGEFileProvider& fp = CEightSettings::GetInstance()->FileProvider();
		    TFileName& fn = fp.AppendToPrivateFolder( _L("frun.dat") );
		    RFile file;
		    if ( file.Replace( fp.Fs(), fn, EFileWrite ) == KErrNone )
		    	{
		    	file.Close();
		    	}			
			}
		else
			{
			return ETrue;
			}
		}
	
	if ( iSwappingTilesManagers )
	    {
	    VGfloat w = Size().iWidth;
	    if ( iActiveTileManaterIdx == 1 )
	        { // swapping to apps list
//	        iTileManager[ 0 ]->SetXOffset( -iSwappingTilesManagersOffset );
//	        iTileManager[ 1 ]->SetXOffset( w - iSwappingTilesManagersOffset );
          iTileManager[ 0 ]->SetXOffset( - ( w - iSwappingTilesManagersOffset ) );
          iTileManager[ 1 ]->SetXOffset( iSwappingTilesManagersOffset );
	        }
	    else
	        { // swapping to tiles
//            iTileManager[ 0 ]->SetXOffset( - ( w - iSwappingTilesManagersOffset ) );
//            iTileManager[ 1 ]->SetXOffset( iSwappingTilesManagersOffset );
              iTileManager[ 0 ]->SetXOffset( -iSwappingTilesManagersOffset );
              iTileManager[ 1 ]->SetXOffset( w - iSwappingTilesManagersOffset );
	        }

        //iSwappingTilesManagersOffset *= 1.5;
        //if ( iSwappingTilesManagersOffset >= w )
        iSwappingTilesManagersOffset /= 1.5;
	    if ( iSwappingTilesManagersOffset <= 1 )
            {
            iSwappingTilesManagers = EFalse;
            iSwappingTilesManagersOffset = 0;
            iTileManager[ 0 ]->SetXOffset( 0 );
            iTileManager[ 1 ]->SetXOffset( 0 );
            }
        
        iTileManager[ 0 ]->Update( cval );
        iTileManager[ 1 ]->Update( cval );
        
	    return ETrue;
	    }
	
    if ( iFormWindowManager && iFormWindowManager->IsVisible() )
        {
        iFormWindowManager->Update( cval );
        }
    else if ( iTileManager[ iActiveTileManaterIdx ] )
        {
        iTileManager[ iActiveTileManaterIdx ]->Update( cval );
        }
    if ( iMsgBox && iMsgBox->IsVisible() )
        {
        iMsgBox->Update( cval );
        }
    return ETrue;
    }

void CEightApplication::DrawFPS()
    {
    CEightSettings* settings = CEightSettings::GetInstance();
    CVgFont* font = settings->FontManager().Get( 1 );
    if ( font )
		{
		TBuf<32> buf;
		buf.AppendNum( GameLoopFPS() );
		buf.Append( _L("/") );
        buf.AppendNum( iFrameTimesBuffer->CurrentValue() );
	    vgSeti( VG_MATRIX_MODE, VG_MATRIX_GLYPH_USER_TO_SURFACE );
	    vgLoadIdentity();
		font->DrawText( TVgPoint( 0, Size().iHeight - font->GlyphSize().iHeight ), buf );
		}
    }

// -----------------------------------------------------------------------------
// CEightApplication::DrawScene()
// -----------------------------------------------------------------------------
//
void CEightApplication::DrawScene()
    {
    float clearColor[] = { 0, 0, 0, 1 };
    vgSetfv( VG_CLEAR_COLOR, 4, clearColor );
    vgSeti( VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE );
    TSize s = Size();
    vgClear( 0, 0, s.iWidth, s.iHeight );
    vgLoadIdentity();
	
	if ( iFormWindowManagerFirstStart )
		{
        vgSeti( VG_MATRIX_MODE, VG_MATRIX_GLYPH_USER_TO_SURFACE );
        vgLoadIdentity();
        vgSeti( VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE );
        vgLoadIdentity();
        vgSeti( VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE );
        vgLoadIdentity();
        iFormWindowManagerFirstStart->Draw();
        if ( iShowFps )
            {
            DrawFPS();  
            }
		return;
		}
	
    if ( iSwappingTilesManagers )
        {
        iTileManager[ 0 ]->Draw();
        iTileManager[ 1 ]->Draw();
        if ( iShowFps )
            {
            DrawFPS();  
            }
        return;
        }
    
    if ( iFormWindowManager && iFormWindowManager->IsVisible() )
        {
        if ( iFormWindowManager->IsShowinOrHidingWindow() )
            {
            if ( iFormWindowManager->WindowsCount() == 1 )
                {
                iTileManager[ iActiveTileManaterIdx ]->Draw();
                }
            }
        vgSeti( VG_MATRIX_MODE, VG_MATRIX_GLYPH_USER_TO_SURFACE );
        vgLoadIdentity();
        vgSeti( VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE );
        vgLoadIdentity();
        vgSeti( VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE );
        vgLoadIdentity();
        iFormWindowManager->Draw();
        }
    else if ( iTileManager[ iActiveTileManaterIdx ] )
        {
        iTileManager[ iActiveTileManaterIdx ]->Draw();
        }
    if ( iMsgBox && iMsgBox->IsVisible() )
        {
        vgSeti( VG_MATRIX_MODE, VG_MATRIX_GLYPH_USER_TO_SURFACE );
        vgLoadIdentity();
        vgSeti( VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE );
        vgLoadIdentity();
        iMsgBox->Draw();
        }
    
    if ( iShowFps )
    	{
    	DrawFPS();  
    	}
    }

// End of file
