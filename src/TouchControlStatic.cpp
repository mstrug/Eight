/*
 ============================================================================
 Name		: TouchControlStatic.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CTouchControlStatic implementation
 ============================================================================
 */

#include "TouchControlStatic.h"
#include <vg/vgu.h>


CTouchControlStatic::CTouchControlStatic( TUint aId, TVgRect aTextRect, TUint aFontId, CVgFontManager& aFontManager ) 
    : CTouchControlBase( NULL, aId, aTextRect ), iFontManager( &aFontManager ), iFontImageId( aFontId ), 
      iHorizontalAlign( ESGEFontAlignHorizontalCenter ), iVerticalAlign( ESGEFontAlignVerticalCenter ), iType( EText )
    {
    }

CTouchControlStatic::CTouchControlStatic( TUint aId, TVgRect aImageRect, TUint aImageId, CVgImageManager& aImageManager ) 
    : CTouchControlBase( NULL, aId, aImageRect ), iImageManager( &aImageManager ), iFontImageId( aImageId ), iType( EImage )
    {
    }

CTouchControlStatic::~CTouchControlStatic()
    {
    }

CTouchControlStatic* CTouchControlStatic::NewL( TUint aId, TVgRect aTextRect, const TDesC& aText, TUint aFontId, CVgFontManager& aFontManager )
    {
    CTouchControlStatic* self = new ( ELeave ) CTouchControlStatic( aId, aTextRect, aFontId, aFontManager );
    CleanupStack::PushL( self );
    self->ConstructL( aText );
    CleanupStack::Pop(); // self;
    return self;
    }

CTouchControlStatic* CTouchControlStatic::NewL( TUint aId, TVgRect aImageRect, TUint aImageId, CVgImageManager& aImageManager )
    {
    CTouchControlStatic* self = new ( ELeave ) CTouchControlStatic( aId, aImageRect, aImageId, aImageManager );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self;
    return self;
    }

void CTouchControlStatic::ConstructL( const TDesC& aText )
    {
    iText.Set( aText );
    }

void CTouchControlStatic::ConstructL()
    {
    }

void CTouchControlStatic::SetTextAlign( TSGEFontAlignHorizontal aHorizontalAlign, TSGEFontAlignVertical aVerticalAlign )
    {
    iAlignSet = ETrue;
    iHorizontalAlign = aHorizontalAlign;
    iVerticalAlign = aVerticalAlign;
    }

void CTouchControlStatic::Draw()
    {
    if ( iType == EText )
        {
        CVgFont* fnt = iFontManager->Get( iFontImageId );
        if ( fnt )
            {
            if ( iAlignSet )
                {
                fnt->DrawText( iRect, iText, iHorizontalAlign, iVerticalAlign );
                }
            else
                {
                fnt->DrawText( iRect.iTl, iText, ETrue );
                }
            }
        }
    else
        {
        MVgImage* img = iImageManager->Get( iFontImageId );
        if ( img )
            {
            VGfloat m[9];
            if ( img->ImageType() == MVgImage::EVgImageTypeSvg ) vgSeti( VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE );
            else vgSeti( VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE );
            vgGetMatrix( m );
            img->DrawCentered( iRect.Center() );
            vgLoadMatrix( m );
            }
        }
    }


