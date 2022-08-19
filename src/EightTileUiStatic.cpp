/*
 ============================================================================
 Name		: EightTileUiStatic.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileUiStatic implementation
 ============================================================================
 */

#include "EightTileUiStatic.h"
#include "EightSettings.h"



CEightTileUiStatic::~CEightTileUiStatic()
    {
    iItems.ResetAndDestroy();
    }

CEightTileUiStatic::TEightTileUiType CEightTileUiStatic::Type() const
    {
    return EStatic;
    }

void CEightTileUiStatic::SetCrop( TVgRect aRect )
    {
    if ( iCropRect == aRect )
        {
        return;
        }
    iCropRect = aRect;
    for ( TInt i = 0; i < iItems.Count(); i++ )
        {
        if ( iItems[ i ]->iImage )
            {
            TSize is = iItems[ i ]->iImage->Size();
            TSize rs = aRect.Size();
            if ( is.iWidth >= rs.iWidth && is.iHeight >= rs.iHeight )
                {
                iItems[ i ]->iCurrentChildImage.DeleteImage();
                iItems[ i ]->iCurrentChildImage = iItems[ i ]->iImage->CreateChildImage( aRect.iTl, rs );
                }
            }
        if ( iItems[ i ]->iSvgImage )
            {
            // todo
            }
        if ( iItems[ i ]->iFont && iItems[ i ]->iText.Length() > 0 )
            {
            // todo
            }
        }
    }

TInt CEightTileUiStatic::AddImageL( TVgRect aRect, TVgImage* aImage, TBool aCenterParent, VGfloat aScale )
    {
    CItem* item = new (ELeave) CItem();
    item->iCenterParent = aCenterParent;
    item->iRect = aRect;
    item->iImage = aImage;
    item->iScale = aScale;
    iItems.AppendL( item );
    return iItems.Count() - 1;
    }

TInt CEightTileUiStatic::AddSvgImageL( TVgRect aRect, CVgSvgImage* aSvgImage, TBool aCenterParent, VGfloat aScale )
    {
    CItem* item = new (ELeave) CItem();
    item->iCenterParent = aCenterParent;
    item->iRect = aRect;
    item->iSvgImage = aSvgImage;
    item->iScale = aScale;
    iItems.AppendL( item );
    return iItems.Count() - 1;
    }
    
TInt CEightTileUiStatic::AddTextL( TVgRect aRect, TUint aFontId, const TDesC& aText, TBool aCenterParent, 
                                   TSGEFontAlignHorizontal aHorizontalAlign, TSGEFontAlignVertical aVerticalAlign )
    {
    CItem* item = new (ELeave) CItem();
    item->iCenterParent = aCenterParent;
    item->iRect = aRect;
    item->iFont = CEightSettings::GetInstance()->FontManager().Get( aFontId );
    item->iFontId = aFontId;    
    item->iHorizontalAlign = aHorizontalAlign;
    item->iVerticalAlign = aVerticalAlign;
    item->iText.CreateL( aText );
    iItems.AppendL( item );
    return iItems.Count() - 1;
    }

TInt CEightTileUiStatic::UpdateText( TInt aItemIdx, const TDesC& aText )
    {
    if ( aItemIdx >= 0 && aItemIdx < iItems.Count() )
        {
        iItems[ aItemIdx ]->iText.Close();
        iItems[ aItemIdx ]->iText.Create( aText );
        return KErrNone;
        }
    return KErrArgument;
    }

TInt CEightTileUiStatic::UpdateRect( TInt aItemIdx, TVgRect aRect )
    {
    if ( aItemIdx >= 0 && aItemIdx < iItems.Count() )
        {
        iItems[ aItemIdx ]->iRect = aRect;
        return KErrNone;
        }
    return KErrArgument;
    }

TInt CEightTileUiStatic::UpdateImage( TInt aItemIdx, TVgImage* aNewImage )
	{
    if ( aItemIdx >= 0 && aItemIdx < iItems.Count() )
        {
        iItems[ aItemIdx ]->iImage = aNewImage;
        return KErrNone;
        }
    return KErrArgument;
	}

TInt CEightTileUiStatic::SetItemHide( TInt aItemIdx, bool aHide )
    {
    if ( aItemIdx >= 0 && aItemIdx < iItems.Count() )
        {
        iItems[ aItemIdx ]->iHidden = aHide;
        return KErrNone;
        }
    return KErrArgument;
    }

void CEightTileUiStatic::Draw()
    {
    for ( TInt i = 0; i < iItems.Count(); i++ )
        {
        if ( iItems[ i ]->iHidden )
            {
            continue;
            }
        if ( iItems[ i ]->iImage )
            {
            VGfloat m[ 9 ];
            vgSeti( VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE );
            vgGetMatrix( m );
            if ( iItems[ i ]->iCenterParent )
                { // allign to pixel
                vgTranslate( (TInt)(iRect.iTl.iX + iRect.Width() / 2), (TInt)(iRect.iBr.iY + iRect.Height() / 2) );
                }
            else
                {
                vgTranslate( iRect.iTl.iX, iRect.iBr.iY );
                }
            if ( iItems[ i ]->iScale != 1 )
                {
                vgScale( iItems[ i ]->iScale, iItems[ i ]->iScale );
                }
            
            if ( iItems[ i ]->iCurrentChildImage.IsChildImage() )
                {
                iItems[ i ]->iCurrentChildImage.DrawCentered( iItems[ i ]->iRect.Center() );
                }
            else
                {
                iItems[ i ]->iImage->DrawCentered( iItems[ i ]->iRect.Center() );
                }
            
            vgLoadMatrix( m );
            }
        if ( iItems[ i ]->iSvgImage )
            {
            VGfloat m[ 9 ];
            vgSeti( VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE );
            vgGetMatrix( m );
            if ( iItems[ i ]->iCenterParent )
                { // don't need to be alligned
                vgTranslate( iRect.iTl.iX + iRect.Width() / 2, iRect.iBr.iY + iRect.Height() / 2 );
                }
            else
                {
                vgTranslate( iRect.iTl.iX, iRect.iBr.iY );
                }

            iItems[ i ]->iSvgImage->DrawCentered( iItems[ i ]->iRect.Center() );
            
            vgLoadMatrix( m );
            }
        if ( iItems[ i ]->iFont && iItems[ i ]->iText.Length() > 0 )
            {
            VGfloat m[ 9 ];
            vgSeti( VG_MATRIX_MODE, VG_MATRIX_GLYPH_USER_TO_SURFACE );
            vgGetMatrix( m );
            TVgRect r( iRect );
            //r.Shrink( 6, 6 );
            if ( iItems[ i ]->iCenterParent )
                { // allign to pixel
                vgTranslate( (TInt)(r.iTl.iX + r.Width() / 2), (TInt)(r.iBr.iY + r.Height() / 2) );
                }
            else
                {
                vgTranslate( r.iTl.iX, r.iBr.iY );
                }
            
            iItems[ i ]->iFont->DrawText( iItems[ i ]->iRect, iItems[ i ]->iText, iItems[ i ]->iHorizontalAlign, iItems[ i ]->iVerticalAlign, ETrue );
            
            vgLoadMatrix( m );
            }
        else if ( !iItems[ i ]->iFont && iItems[ i ]->iFontId > KErrNotFound )
            {
            iItems[ i ]->iFont = CEightSettings::GetInstance()->FontManager().Get( iItems[ i ]->iFontId );
            }
        }
    }

