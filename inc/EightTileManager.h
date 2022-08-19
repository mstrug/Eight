/*
 ============================================================================
 Name		: EightTileManager.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileManager declaration
 ============================================================================
 */

#ifndef EIGHTTILEMANAGER_H
#define EIGHTTILEMANAGER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <w32std.h>
#include <apaid.h>
#include <SGEVg/VgSvgImage.h>
#include "EightTile.h"
#include "EightMsgBox.h"
#include "EightFormWindowManager.h"
#include "TouchButtonPath.h"
#include "TouchButtonObserver.h"
#include <touchfeedback.h>


const TInt KTileSpecialTile = 0x10000;
const TInt KTilePlaceholder = 0x1000;
const TInt KTilePlaceholderMask = 0x0FFF;


// CLASS DECLARATION

#define DRAW_DEBUGx

class CEightTileManager;
class CVgImageManager;

class MEightTileManagerObserver
	{
public:
	typedef enum
	{
        EExit,
		ESwapLeft,
		ESwapRight,
		ESettigsUpdated,
		ETileColorChanged,
		EReset,
        ESettigsCaptureMenuKey,
        EExitAnimFinished,
        EActivateFRWizzard,
        EStartingApp
	} TTileManagerAction;
public:
    virtual void TileManagerAction( CEightTileManager& aTileManager, TTileManagerAction aAction, TInt aParam = 0 ) = 0;
    virtual void TileManagerRemovedTile( CEightTileManager& aTileManager, TUint aTileId ) = 0;
    virtual void TileManagerAddTile( CEightTileManager& aTileManager, TUint aTileId ) = 0;
	};

/**
 *  CEightTileManager
 * 
 */
class CEightTileManager : public CBase, MTouchButtonObserver, MEightFormWindowObserver
    {
    typedef enum
    {
        ENormal,
        EScrollingDown,
        EScrollingUp,
        EScreenBouncingDown,
        EScreenBouncingUp,
        ETileMoving
    } TState;
    
    typedef enum
    {
        EStart,
        ENormalDrawing,
        EAppStart,
        EEnd
    } TDrawingState;
    
public:

    ~CEightTileManager();

    static CEightTileManager* NewL( MEightTileManagerObserver& aObserver, TInt aMargin, TInt aTileSize, TSize aSize );

    void SetMsgBox( CEightMsgBox* aMsgBox );

    void SetFormWindowManager( CEightFormWindowManager* aWndMan );

    void SetupBoard( TInt aSpecialTilesType = KErrNotFound );

    void SetupBoardAfterInternalize( TInt aSpecialTilesType = KErrNotFound );
    
    void InsertNewTileL( CEightTile* aTile );
    
    void AddTileL( CEightTile* aTile );

    void AddIconTileL( TUint aId, CVgSvgImage* aImage, const TDesC& aTitle, TBool aPinned );
    
    void AddIconTileL( CEightTile* aTile, CVgSvgImage* aImage, const TDesC& aTitle, TBool aPinned );

    void AddAppTileL( TApaAppInfo appInfo );
    
    CEightTile* GetAppSetupTile(); // takes ownership, used for reset
    TBool SetAppSetupTile( CEightTile* aTile ); // takes ownership, used for reset
    
    void HandleForegroundEventL( TBool aForeground );
    
    void ResetPointerEvents();
    
    TBool HandlePointerEventL( const TPointerEvent& aPointerEvent );
    
    void TouchGestureTap( TPoint aPosition );
    
    void TouchGestureLongTap( TPoint aPosition );
    
    void TouchGestureFlickUp( TInt aValue ); // pix per second

    void TouchGestureFlickDown( TInt aValue ); // pix per second

    void TouchGestureDragStart( TPoint aStartPoint, TPoint aCurrentPoint );

    TBool TouchGestureDrag( TSize aOffset, TBool anyState = EFalse );

    void TouchGestureDragStop();
    
    void RemoveSelectedTile();
    
    void RemoveTile( TUint aTileId );
    
    TBool CanSwapLeftRight();
    
    void SetXOffset( VGfloat aOffset );
    
    void SetPinTile( TUint aTIleId, TBool aPinned );

    void UpdatePinTiles( CEightTileManager* aManager );
    
    RPointerArray< CEightTile >& Tiles(); 

    RArray< TFixedArray< TInt, 4 > >& Board(); 
    
    CEightTile* Tile( TUint aId );
    
    void StartReady(); // todo rename functions

    void NormalReady();

    void EndReady();

    void RunTimeout();

    TBool RunTile( TUint aId );

    void ResetTilesColor();

    void UpdateTilesColor();

    void UpdateTileColor( TUint aTileId, TBool aCustomColor, const TEightPalette& aPalette );

    void UpdateTilesColors( CEightTileManager* aManager );

    void UpdateSpcialRow();

    void UpdateClosestTileCoords();

    void Draw();
    
    void Update( TTimeIntervalMicroSeconds32 aTimeInterval );

    void ExternalizeL( RWriteStream& aStream ) const;

    void InternalizeL( RReadStream& aStream );

    void PrepareForExternalize();
    
private:

    CEightTileManager( MEightTileManagerObserver& aObserver, TInt aMargin, TInt aTileSize, TSize aSize );

    MVgImage* ConstructImageL( CVgImageManager& aImageManager, TUint aImageId, const TDesC8& aImageData );

    void ConstructL();
    
    void SetState( TState aNewState );

    TBool IsHeightSmallerThanScreen();
    
    TVgImage* GetAppIconL( TApaAppInfo appInfo );
    
    TBool SelectTile( TPoint aPosition );

    void SetupTile( TPoint aPlace, TInt aIdx );

    void SelectionModeStop();
    
    TBool IsSelectionMode() const;

    void AddSpecialTiles( TInt aSpecialTilesType );
    
    void CompressBoard();
    
    void UpdateTilesRects();

    TBool ButtonsContainsPoint( TPoint aPoint );
    
    TInt FindTile( TPoint aScreenCoords );

    TInt FindTile( TUint aId ); // returns idx

    TPoint FindTileInBoard( CEightTile* aPtr );
    
    TVgRect GetTileRect( TInt aX, TInt aY, TEightTileSize aSize );
    
    TPoint FindPlace1x1();
    
    TPoint FindPlace2x2();
    
    TPoint FindPlace4x2();

    TPoint FindPlace3x1();
    
    void CheckScreenBounce();
    
    TInt BottomBorder( TBool aWithSpecialRow = ETrue ) const;
    
    TVgRect FindNearestMovingTile();

    TBool MoveTile( TPoint& aStartIdx, TPoint& aEndIdx );
    
    void InsertRow( TInt aOffsetY );
    
    void RemoveLastRow();
    
    void FadeAllTiles( TBool aFade );
    
    void SetButtonsCoords();
    
    void IncreaseSelectedTileSize();
    
    void DecreaseSelectedTileSize();
    
    void DrawTileDebug( TInt aX, TInt aY );
    
    void DrawTiles();
    
private: // from MTouchButtonObserver
    
    void TouchButtonPressed( TUint aId );
    
private: // from MEightFormWindowObserver
    
    void FormWindowButtonAction( TUint aWindowId, TUint aButtonId );
    
private:
    
    MEightTileManagerObserver& iObserver;
    
    RPointerArray< CEightTile > iTiles;
    
    RArray< TFixedArray< TInt, 4 > > iBoard;
    
    CEightMsgBox* iMsgBox; // not owned
    
    CEightFormWindowManager* iWndMan; // not owned
    
    TInt iTilesWidth;
    
    TInt iMargin;
    
    TInt iTileSize;
    
    TSize iScreenSize;
    
    TPoint iSelectedTile;
    
    TBool iTilesVisible;	// oherwise apps list visible
    
    TVgPoint iTilesOffset;
    
    TState iState;
    
    TDrawingState iDrawingState;
    TReal iDrawingEndIdx;
    
    TState iOldState;
    
    TTime iStateChangeTimestamp;
    
    TReal iScrollSpeed;
    
    TReal iScrollAcceleration;
    
    TReal iScrollTime;
    
    TReal iScrollOffset;
    
    TReal iScrollOffsetOld;
    
    TReal iBouncingValue;
    
    TVgPoint iSelectionTileOffset;
    
    TVgRect iSelectionClosestTile;
    
    TVgPoint iSelectionClosestTileAnim;

    TPoint iSelectionClosestTileIndexes;
    
    TInt iNanokernelTickPeriod;
    
    TUint32 iNanokernelTickCountDragStop;
    
    MVgImage* iIconArrow;	// not owend
    MVgImage* iIconArrowSel;	// not owend
    MVgImage* iIconRemove;	// not owend
    MVgImage* iIconRemoveSel;	// not owend
    MVgImage* iIconSettings;	// not owend
    MVgImage* iIconSettingsSel;	// not owend
    MVgImage* iIconPin;	// not owend
    MVgImage* iIconPinSel;	// not owend
    MVgImage* iIconUnpin;	// not owend
    MVgImage* iIconUnpinSel;	// not owend
    
    MVgImage* iIconExit;	// not owend
    MVgImage* iIconExitSel;	// not owend
    MVgImage* iIconArrowRight;	// not owend
    MVgImage* iIconArrowRightSel;	// not owend
    MVgImage* iIconArrowLeft;	// not owend
    MVgImage* iIconArrowLeftSel;	// not owend
    
    CTouchButtonPath* iButtonArrow;
    CTouchButtonPath* iButtonRemove;
    CTouchButtonPath* iButtonSettings;
    CTouchButtonPath* iButtonPin;
    CTouchButtonPath* iButtonUnpin;

    TBool iSpecialRow;
    TInt iSpecialTilesType;
    
    TInt iPressedDownTileIdx;
    
    RPointerArray< TVgImage > iAppsImgs;
    
    MTouchFeedback* iTouchFeedback;
    };

#endif // EIGHTTILEMANAGER_H
