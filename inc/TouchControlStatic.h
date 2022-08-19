/*
 ============================================================================
 Name		: TouchControlStatic.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CTouchControlStatic declaration
 ============================================================================
 */

#ifndef TOUCHCONTROLSTATIC_H
#define TOUCHCONTROLSTATIC_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGEVg/VgPrimitives.h>
#include <SGEVg/VgFontManager.h>
#include <SGEVg/VgImageManager.h>
#include <SGEVg/VgFont.h>
#include "TouchControlBase.h"

// CLASS DECLARATION

/**
 *  CTouchControlStatic
 * 
 */
class CTouchControlStatic : public CTouchControlBase
    {
public:
    
    ~CTouchControlStatic();

    static CTouchControlStatic* NewL( TUint aId, TVgRect aTextRect, const TDesC& aText, TUint aFontId, CVgFontManager& aFontManager );

    static CTouchControlStatic* NewL( TUint aId, TVgRect aImageRect, TUint aImageId, CVgImageManager& aImageManager );

    void SetTextAlign( TSGEFontAlignHorizontal aHorizontalAlign, TSGEFontAlignVertical aVerticalAlign );

    void Draw();
    
private:

    CTouchControlStatic( TUint aId, TVgRect aTextRect, TUint aFontId, CVgFontManager& aFontManager );

    void ConstructL( const TDesC& aText );

    CTouchControlStatic( TUint aId, TVgRect aImageRect, TUint aImageId, CVgImageManager& aImageManager );

    void ConstructL();

private:
    
    CVgImageManager* iImageManager; // not owned
    
    CVgFontManager* iFontManager; // not owned
    
    TUint iFontImageId;
    
    TPtrC iText;
    
    TBool iAlignSet;
    
    TSGEFontAlignHorizontal iHorizontalAlign;
    
    TSGEFontAlignVertical iVerticalAlign;

    enum TType
        {
        EText,
        EImage
        } iType;
        
    };

#endif // TOUCHCONTROLSTATIC_H
