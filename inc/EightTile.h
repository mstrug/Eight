/*
 ============================================================================
 Name		: EightTile.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTile declaration
 ============================================================================
 */

#ifndef EIGHTTILE_H
#define EIGHTTILE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <w32std.h>
#include <SGEVg/VgPrimitives.h>
#include <SGEVg/VgFont.h>
#include "EightTileUi.h"
#include "EightPalette.h"
#include "AnimationFragment.h"
#include "TouchControlBase.h"

// CLASS DECLARATION
class CEightMsgBox;
    
typedef enum
{
	ENormal,
	EPressed,
	ESelected,
	EFaded,
	EStarting
} TEightTileState;
const TInt KEightTileStateCount = EStarting + 1;

typedef enum
{
    ETileSize_1x1,
    ETileSize_2x2,
    ETileSize_4x2,
    ETileSize_3x1
} TEightTileSize;
const TInt KEightTileSizeCount = ETileSize_3x1 + 1;

typedef enum
{
    ETileTypeNormal,
    ETileTypeSpecial,
    ETileTypeAppShortcut,
    ETileTypeAppWindow    
} TEightTileType;

/**
 *  CEightTile
 * 
 */
class CEightTile : public CBase
    {
    
public:

    ~CEightTile();

    static CEightTile* NewL( TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi = NULL, TBool aEmpty = EFalse );
    
    TUint Id() const;

    CEightTileUi* Ui() const;

    void SetUi( CEightTileUi* aUi );
    
    void SetTileMetrics( TInt aTileSizePx );
    
    virtual TEightTileType Type() const;
    
    TSize Size() const;

    TEightTileSize TileSize() const;

    // used only during internalization!
    void SetTileSize( TEightTileSize aTileSize );
    
    virtual void SetMsgBox( CEightMsgBox* aMsgBox );

    TEightTileState TileState() const;

    void SetBackgroundColor( TVgColor aColor );

    void SetSelectedColor( TVgColor aColor );

    void SetPressedColor( TVgColor aColor );

    void UpdateColorsFromTheme();

    void UpdateColorsFromIndex( TInt aColorIndex );

    void UpdateColorsFromPalette( const TEightPalette& aPalette );

    void SetCustomColor( TBool aCustom );
    
    TBool CustomColor() const;

    TBool SupportsCustomColor() const;
    
    void SetCustomColorSupport( TBool aSupport );
    
    const TEightPalette& Palette() const; 

    void SetFlipping( TBool aFlipping );

    TBool IsFlipping() const;

    void SetFlippingSupport( TBool aSupportFlipping );

    TBool SupportsFlipping() const;

    TBool IsPinned() const;

    void SetPinned( TBool aPinned );

    VGPaint& TilePaint();
    
    VGPath& TilePath();
    
    TVgSize TileScale();
    
    void IncreaseTileSize();

    void DecreaseTileSize();
    
    TBool IsEmpty() const;
    
    // permanently hides tile
    void SetEmpty();
    
    TVgRect& Rect();
    
    virtual void SetRect( const TVgRect& aRect );

    void SetPressed( TBool aPressed );
    
    void SetSelected( TBool aSelected );
    
    void SetFaded( TBool aFaded );

    virtual TInt CustomSettingsButtonsCount() const;

    virtual CTouchControlBase* GetCustomSettingsButtonL( TInt aIdx );

    virtual void HandleCustomSettingsButtonState( CTouchControlBase* aButton );

    virtual void CleanupCustomSettingsButtons();

    virtual TBool CanIncreaseSize() const;
    
    virtual TBool CanDecreaseSize() const;
    
    void SetCanBeRemovedSupport( TBool aSupportRemoving );
    
    TBool SupportsRemoving() const;
    
    virtual TBool CanBeRemoved() const;
    
    virtual TBool CanBeMoved() const;
    
    virtual TBool CanOpenSettings() const;

    virtual void HandleForegroundEventL( TBool aForeground );

    virtual TBool HandlePointerEventL( const TPointerEvent& aPointerEvent );

    void Run();

    // no animation
    virtual TBool RunInternal();

    void ResetRunState();

    virtual void DrawDebug();
    
    virtual void Draw();
    
    virtual void Update( TTimeIntervalMicroSeconds32 aTimeInterval );

    virtual void ExternalizeL( RWriteStream& aStream ) const;

    virtual void InternalizeL( RReadStream& aStream );

protected:

    CEightTile( TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi, TBool aEmpty );

    void BaseConstructL();

    void UpdateScaleNow();

protected:
    
    TUint iId;
    
    VGPath iPath;
    VGPaint iPaint;
    
    CEightTileUi* iUi; // owned
    
    TInt iCoord;
    
    TEightTileSize iTileSize;
    
    TSize iSize;
    
    TSize iPixSize;
    
    TBool iEmpty;
    
    TVgRect iRect;
    
    TBool iRectGrowed;
    
    TEightTileState iState;
    
    TBool iPinned;
    
    TEightPalette iPalette;
    
    TBool iCustomColor;
    
    TAnimationValueProportionalFragment< VGfloat > iScaleX;
    TAnimationValueProportionalFragment< VGfloat > iScaleY;
    
    VGfloat iScaleFinalX, iScaleFinalY;
    VGfloat iScaleMod;
    
    TBool iFlipping;

    TBool iSupportsFlipping;
    
    TBool iSupportsCustomColor;
    
    TInt iRunTime;
    
    TBool iSupportRemoving;
    
public:
    VGfloat iDrawingOffset;
    VGfloat iDrawingScale;
    };

#endif // EIGHTTILE_H
