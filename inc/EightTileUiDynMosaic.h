/*
 ============================================================================
 Name		: EightTileUiDynMosaic.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileUiDynMosaic declaration
 ============================================================================
 */

#ifndef EIGHTTILEUIDYNMOSAIC_H
#define EIGHTTILEUIDYNMOSAIC_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGEVg/VgImage.h>
#include <SGEVg/VgSvgImage.h>
#include <SGEVg/VgFont.h>
#include "EightTileUi.h"
#include "EightTileUiStatic.h"
#include "EightPhotosProvider.h"
#include "AnimationFragment.h"

// CLASS DECLARATION

/**
 *  CEightTileUiDynMosaic
 * 
 */
class CEightTileUiDynMosaic : public CEightTileUi, MEightPhotosProvider
	{
public:

	CEightTileUiDynMosaic( TInt aMinTime, TInt aMaxTime, TInt aParallelFlipCount, TInt aImageSource = KErrNotFound );

	~CEightTileUiDynMosaic();

	TEightTileUiType Type() const;

    void SetRect( TVgRect aRect );

    TBool DrawBackground() const;
    
    void Draw();
    
    void Update( TTimeIntervalMicroSeconds32 aTimeInterval );
    
    void AddStaticUi( CEightTileUiStatic* aUi );

    CEightTileUiStatic* StaticUi() const;

    void UpdateColorsFromTile();

private:
    
    TVgColor GetColor();
    
    void GenerateFlips();

    void UpdateFlips( TTimeIntervalMicroSeconds32 aTimeInterval );
    
    void UpdateRects();
    
    TBool UpdateRectScale();
    
private: // from MEightPhotosProvider

    void ImageLoaded( TInt aImageId, TInt aError, TInt aSource );

private:

    VGPath iSelectedPath;
    VGPaint iSelectedPaint;

    typedef struct
		{
    	VGPath iPath;
    	VGPaint iPaint;
    	TInt iImageId;
		} SItem;

	TFixedArray< TFixedArray< SItem, 3 >, 6 > iItems;

	TInt iDivX;
	TInt iDivY;
	
	TVgRect iRectScale;
	
    
    TInt iMinTime;

    TInt iMaxTime;
    
    TInt iParallelFlipCount;
    
    class SFlip
		{
    public:
    	TPoint iItemIdx;
    	TReal iScale;
    	TAnimationValueProportionalFragment< VGfloat > iAnim;
    	TInt iStartTime;
    	TInt iTime;
    	TBool iFrontSide;
    	TInt iImageId;
    	SFlip() : iAnim( 0, 0 ) { }
		};
    
	RArray< SFlip > iFlips;
	
	TInt iImageSource;
	
    RArray< TUint > iImageIds;

    CEightTileUiStatic* iStaticUi; // owned
    
	};

#endif // EIGHTTILEUIDYNMOSAIC_H
