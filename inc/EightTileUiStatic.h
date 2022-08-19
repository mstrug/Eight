/*
 ============================================================================
 Name		: EightTileUiStatic.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileUiStatic declaration
 ============================================================================
 */

#ifndef EIGHTTILEUISTATIC_H
#define EIGHTTILEUISTATIC_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGEVg/VgImage.h>
#include <SGEVg/VgSvgImage.h>
#include <SGEVg/VgFont.h>
#include "EightTileUi.h"

// CLASS DECLARATION

/**
 *  CEightTileUiStatic
 * 
 */
class CEightTileUiStatic : public CEightTileUi
    {
public:
    
    ~CEightTileUiStatic();

    TEightTileUiType Type() const;

    void SetCrop( TVgRect aRect );
    
    TInt AddImageL( TVgRect aRect, TVgImage* aImage, TBool aCenterParent = ETrue, VGfloat aScale = 1 );
    
    TInt AddSvgImageL( TVgRect aRect, CVgSvgImage* aSvgImage, TBool aCenterParent = ETrue, VGfloat aScale = 1 );
    
    TInt AddTextL( TVgRect aRect, TUint aFontId, const TDesC& aText, TBool aCenterParent = ETrue, 
                   TSGEFontAlignHorizontal aHorizontalAlign = ESGEFontAlignHorizontalCenter,
                   TSGEFontAlignVertical aVerticalAlign = ESGEFontAlignVerticalCenter );
    
    TInt UpdateText( TInt aItemIdx, const TDesC& aText );
    
    TInt UpdateRect( TInt aItemIdx, TVgRect aRect );

    TInt UpdateImage( TInt aItemIdx, TVgImage* aNewImage );

    TInt SetItemHide( TInt aItemIdx, bool aHide );
    
    void Draw();
    
private:
    
    void TruncateTextToRect( TInt aItemIdx );
    
private:
    
    class CItem : public CBase
    {
    public:
        TBool iCenterParent;
        TBool iHidden;
        TVgRect iRect;
        TVgRect iCropRect;
        TVgImage *iImage; // not owned
        TVgImage iCurrentChildImage; // owned
        CVgSvgImage *iSvgImage; // not owned
        CVgFont *iFont; // not owned
        TInt iFontId;
        RBuf iText;
        TSGEFontAlignHorizontal iHorizontalAlign;
        TSGEFontAlignVertical iVerticalAlign;
        VGfloat iScale;
        CItem() : iHidden(EFalse), iFontId( KErrNotFound ), iHorizontalAlign( ESGEFontAlignHorizontalCenter ), iVerticalAlign( ESGEFontAlignVerticalCenter ) { }
        ~CItem() { /*delete iImage; delete iSvgImage; delete iFont;*/ iText.Close(); iCurrentChildImage.DeleteImage(); }
    };
    
    RPointerArray< CItem > iItems; 

    };

#endif // EIGHTTILEUISTATIC_H
