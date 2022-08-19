/*
 ============================================================================
 Name		: EightTileUiDynCycle.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileUiDynCycle declaration
 ============================================================================
 */

#ifndef EIGHTTILEUIDYNCYCLE_H
#define EIGHTTILEUIDYNCYCLE_H

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
 *  CEightTileUiDynCycle
 * 
 */
class CEightTileUiDynCycle : public CEightTileUi, MEightPhotosProvider
    {
public:

    CEightTileUiDynCycle( TUint aPhotosSourceId );
    
    ~CEightTileUiDynCycle();

    void SetPhotosSourceId( TUint aPhotosSourceId );
    
    TUint PhotosSourceId() const;
    
    TEightTileUiType Type() const;
    
    void SetRect( TVgRect aRect );

    void AddImageL( const TDesC& aFileName );
    
    void AddImage( TVgImage* aImage );
    
    void AddTileUi( CEightTileUi* aTileUi );

    TBool DrawBackground() const;
    
    void Draw();
    
    void Update( TTimeIntervalMicroSeconds32 aTimeInterval );

    void AddStaticUi( CEightTileUiStatic* aUi );

    CEightTileUiStatic* StaticUi() const;
    
    void UpdateColorsFromTile();
    
private:
    
    void CreateCurrentImage();

    void CreateTwoCurrentImages();
    
    TBool UpdateRectScale();

    void LoadNextImage();

private: // MEightPhotosProvider
    
    void ImageLoaded( TInt aImageId, TInt aError, TInt aSource );
    
private:
    
    TBool iReady;
    
    TUint iPhotosSourceId;
    
    VGPath iPath;
    VGPaint iPaint;
    
    RPointerArray< CEightTileUi > iPages;

    //RArray< TVgImage* > iImages;

    RArray< RBuf > iImageFiles;

    TVgImage* iMainImage; // not owned
    TVgImage* iMainImage2; // not owned
    TUint iMainImageId;
    TUint iMainImage2Id;
    TVgImage iMainImageS; // owned
    TVgImage iMainImage2S; // owned
    
    TVgImage iCurrentImage; // owned
    
    TVgImage iCurrentImage2; // owned

    TAnimationValueProportionalFragment< VGfloat > iCycleAnim;
    
    TInt iCurrentImageIdx;

    TInt iNextImageIdx;
    
    TInt iTimeCnt;
    
    TVgRect iRectScale;

    //TRect iRectImage;
    //TRect iRectImage2;

    enum TState
        {
        EPauseStart,
        ECycling,
        EPauseEnd,
        EFastCycling
        } iState;

    CEightTileUiStatic* iStaticUi; // owned
    
    TInt iImageLoadError;
        
    };

#endif // EIGHTTILEUIDYNCYCLE_H
