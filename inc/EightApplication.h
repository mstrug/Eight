/*
 ============================================================================
 Name        : EightApplication.h
 Author      : Michal Strug
 Company     : 
 Version     : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : Declares main application class.
 ============================================================================
 */

#ifndef __EIGHTAPPLICATION_H__
#define __EIGHTAPPLICATION_H__

// INCLUDES
#include <SGEApplication/SGEGameApplicationVG.h>
#include <SGEVg/VgSvgImage.h>
#include <SGEVg/VgFont.h>
#include <SGESensors/SGECircularBuffer.h>
#include <SGEUtils/SGEUtilsGPTimer.h>
#include <SGEUtils/SGEUtilsGPTimerObserver.h>
#include "EightTileManager.h"
#include "EightTileManagerFactory.h"
#include "EightAppFactory.h"
#include "EightPhotosProvider.h"
#include "EightMsgBox.h"
#include "EightFormWindowManager.h"
#include "EightCompilationSettings.h"
#include <mw/akntouchgesturefwobserver.h>
#include <mw/akntouchgesturefw.h>
#include <mw/akntouchgesturefwevents.h>
#include <goommonitorsession.h>
#include <touchfeedback.h>


using namespace AknTouchGestureFw;

// CONSTANTS
// UID of the application
const TUid KUidEight =
    {
#ifdef EIGHT_PREMIUM
    0x2006AF94
#else
    0x2006AF95
#endif
    };

// CLASS DECLARATION
/**
 * CEightApplication
 *
 */
class CEightApplication : public CSGEGameApplicationVG, public MAknTouchGestureFwObserver, public MEightTileManagerObserver, MEightPhotosProvider, MEightMsgBoxObserver, MSGEUtilsGPTimerObserver
    {

public:

    CEightApplication();

    ~CEightApplication();

    void InitL();
    
    void CheckFirstStartL( TBool aAlways = EFalse );
    
    void Cleanup();
    
    static CApaApplication* NewApplication();

    TUid ApplicationUid();

    void HandleCommandL( TInt aCommand );

    TBool HandleWsEventL( const TWsEvent& aEvent, CCoeControl *aDestination );
    
    TBool HandlePointerEventL( const TPointerEvent& aPointerEvent );
    
    void HandleForegroundEventL( TBool aForeground );

    void Draw( const TRect& aRect ) const;

    TBool FullScreen() const;
    
    TSGEApplicationOrientation Orientation() const;

    TInt SampleBuffers();

    TInt SampleSize();

    TInt DepthSize();
    
    TBool GoBackgroundOnEndKey() const;

    void ExitApplication();
    
private:

    void InitInternalL();

    void DoGameLoopEvent( TTimeIntervalMicroSeconds32 aTimeInterval );

    TBool UpdateScene( TTimeIntervalMicroSeconds32 aTimeInterval );

    void DrawScene();
    
    void DrawFPS();
    
    void LoadSvgImagesL();

    void LoadPhotoImagesL();

    void LoadTilesL();

    void AddTileTo0( TInt idx );

    void CreateDefaultTilesL();

    void CreateShortcutTilesL();
    
    void StartSwap();

    void UpdateCaptureMenuKey();

private: // from MAknTouchGestureFwObserver

    void HandleTouchGestureL( MAknTouchGestureFwEvent& aEvent );
    
private: // from MEightTileManagerObserver
    
    void TileManagerAction( CEightTileManager& aTileManager, TTileManagerAction aAction, TInt aParam );

    void TileManagerRemovedTile( CEightTileManager& aTileManager, TUint aTileId );

    void TileManagerAddTile( CEightTileManager& aTileManager, TUint aTileId );

private: // from MEightPhotosProvider
    
    void ImageLoaded( TInt aImageId, TInt aError, TInt aSource );
    
private: // from MEightMsgBoxObserver
    
    void MsgBoxAction( TUint aMessageId, TInt aActionId );

    void MsgBoxWillShow();

private: // from MSGEUtilsGPTimerObserver
    
    void GPTimerEvent( CSGEUtilsGPTimer* aTimer );
    
private:    // data

    CEightTileManagerFactory* iTileManagerFactory;
    
    CEightTileManager* iTileManager[ 2 ];
    
    CAknTouchGestureFw* iGestureHelper; // = AknTouchGestureFw::CAknTouchGestureFw::NewL(*this, *this);
    
    CEightMsgBox* iMsgBox; // owned

    CEightFormWindowManager* iFormWindowManagerFirstStart; // owned

    CEightFormWindowManager* iFormWindowManager; // owned

    CSGECircularBuffer< TInt > *iFrameTimesBuffer; // owned
    
    TPoint iGesturePosition;
    
    TInt iActiveTileManaterIdx;
    
    CEightAppFactory* iAppFactory;
    
    TBool iExitRequested;

	RGOomMonitorSession iGoomSession;

    CSGEUtilsGPTimer* iTimer; // owned

    TBool iMenuKeyLong;
    
    TBool iForeground;
    
    TInt32 iHandleMenuKey;

    TInt32 iHandleMenuKeyUpDown;
    
    TBool iShowFps;

    TBool iLightDisplay;
    
    TBool iSwappingTilesManagers;
    
    VGfloat iSwappingTilesManagersOffset;

    MTouchFeedback* iTouchFeedback;
    };

#endif // __EIGHTAPPLICATION_H__
