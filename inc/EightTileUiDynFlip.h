/*
 ============================================================================
 Name		: EightTileUiDynFlip.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileUiDynFlip declaration
 ============================================================================
 */

#ifndef EIGHTTILEUIDYNFLIP_H
#define EIGHTTILEUIDYNFLIP_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGEVg/VgImage.h>
#include <SGEVg/VgSvgImage.h>
#include <SGEVg/VgFont.h>
#include "EightTileUi.h"
#include "AnimationFragment.h"


// CLASS DECLARATION

/**
 *  CEightTileUiDynFlip
 * 
 */
class CEightTileUiDynFlip : public CEightTileUi
    {
public:

    CEightTileUiDynFlip( TInt aMinTime, TInt aMaxTime );
    
    ~CEightTileUiDynFlip();

    TEightTileUiType Type() const;

    void SetTile( CEightTile* aOwnerTile );

    void SetRect( TVgRect aRect );
    
    void SetTilesUi( CEightTileUi* aFrontTileUi, CEightTileUi* aBackTileUi );

    TBool FrontVisible();

    CEightTileUi* TileUi( TInt aIdx );
    
    CEightTileUi* CurrentTileUi();

    CEightTileUi* NextTileUi();

    void Flip();

    TBool IsFlipping();
    
    TBool DrawBackground() const;
    
    void Draw();
    
    void Update( TTimeIntervalMicroSeconds32 aTimeInterval );
    
private:
    
    CEightTileUi* iTilesUi[ 2 ]; // owned
    
    TAnimationValueProportionalFragment< VGfloat > iFilpAnim;
    
    VGPath iPath;
    
    TReal iValue;
    
    TReal iScale;
    
    TBool iFrontSide;
    
    TInt iMinTime;

    TInt iMaxTime;
    
    TInt iNextTime;
    
    TBool iFlipRequested;

    };

#endif // EIGHTTILEUIDYNFLIP_H
