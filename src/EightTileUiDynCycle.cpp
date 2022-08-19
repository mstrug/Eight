/*
 ============================================================================
 Name		: EightTileUiDynCycle.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileUiDynCycle implementation
 ============================================================================
 */

#include "EightTileUiDynCycle.h"
#include "EightSettings.h"
#include "EightPhotosProvider.h"
#include <vg/vgu.h>
#include <e32math.h>


const TInt KMargin = 12;


CEightTileUiDynCycle::CEightTileUiDynCycle( TUint aPhotosSourceId ) : iPhotosSourceId( aPhotosSourceId ), iCycleAnim( 1000000 / 30, 1 )
    {
    iPath = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 10, 20, VG_PATH_CAPABILITY_ALL );
    
    iPaint = vgCreatePaint();
    vgSetParameteri( iPaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR );
    
    //LoadNextImage();
    }

CEightTileUiDynCycle::~CEightTileUiDynCycle()
    {
    vgDestroyPath( iPath );
    vgDestroyPaint( iPaint );
    iPages.ResetAndDestroy();
    //iImages.Close();
    iCurrentImage.DeleteImage();
    iCurrentImage2.DeleteImage();
    
    iMainImageS.DeleteImage();
    iMainImage2S.DeleteImage();
    
    for ( TInt i = 0; i < iImageFiles.Count(); i++ )
    	{
    	iImageFiles[ i ].Close();
    	}
    iImageFiles.Close();
    
    delete iStaticUi;
    }

void CEightTileUiDynCycle::SetPhotosSourceId( TUint aPhotosSourceId )
    {
    iPhotosSourceId = aPhotosSourceId;
    if ( iImageLoadError != KErrNone )
    	{
    	LoadNextImage();
    	}
    }

TUint CEightTileUiDynCycle::PhotosSourceId() const
    {
    return iPhotosSourceId;
    }

CEightTileUiDynCycle::TEightTileUiType CEightTileUiDynCycle::Type() const
    {
    return EDynCycle;
    }

void CEightTileUiDynCycle::SetRect( TVgRect aRect )
    {
    iRect = aRect;
    iRectScale = aRect;
    for ( TInt i = 0; i < iPages.Count(); i++ )
        {
        iPages[ i ]->SetRect( aRect );
        //iPages[ i ]->SetCrop( TVgRect( TVgPoint(), aRect.Size() ) );
        }
    
    UpdateRectScale();
    
    if ( iState == EFastCycling )
        {
        CreateTwoCurrentImages();
        }
    else
        {
        CreateCurrentImage();
        }

    if ( iStaticUi ) iStaticUi->SetRect( aRect );
    }

void CEightTileUiDynCycle::CreateCurrentImage()
    {
    if ( iMainImage )
        {
        iCurrentImage.DeleteImage();
        
//        iRectImage.SetRect( TPoint( KMargin, iCycleAnim.iValue + KMargin ), iRect.Size() );
        
        TSize s( iRectScale.Size() );
        TSize is = iMainImage->Size();
        if ( s.iHeight + iCycleAnim.iValue > is.iHeight ) iCycleAnim.iValue = is.iHeight - s.iHeight;
        if ( s.iWidth > is.iWidth ) s.iWidth = is.iWidth;
        
        
        iCurrentImage = iMainImage->CreateChildImage( TPoint( 0, iCycleAnim.iValue ), s );
//        iCurrentImage = iMainImage->CreateChildImage( TPoint( iRectImage.iTl.iX, iRectImage.iBr.iY ), s );
        }
    }

void CEightTileUiDynCycle::CreateTwoCurrentImages()
    {
    if ( iMainImage && iMainImage2 )
        {
        iCurrentImage.DeleteImage();
        TSize is = iMainImage->Size();
        TSize is2 = iMainImage2->Size();
        TSize rs = iRectScale.Size();
        TSize s( rs.iWidth, is.iHeight - iCycleAnim.iValue );
        if ( s.iHeight > rs.iHeight ) 
            {
            iCycleAnim.iValue = is.iHeight - rs.iHeight;
            s.iHeight = is.iHeight - iCycleAnim.iValue;
            }
        if ( s.iWidth > is.iWidth ) s.iWidth = is.iWidth;

        iCurrentImage = iMainImage->CreateChildImage( TPoint( 0, iCycleAnim.iValue), s );

        iCurrentImage2.DeleteImage();
        TSize s2( rs.iWidth, rs.iHeight - ( is.iHeight - iCycleAnim.iValue ) );
        if ( s2.iWidth > is2.iWidth ) s2.iWidth = is2.iWidth;
        iCurrentImage2 = iMainImage2->CreateChildImage( TPoint(), s2 );
        }
    }

void CEightTileUiDynCycle::AddImageL( const TDesC& /*aFileName*/ )
	{
//	RBuf buf;
//	buf.Create( aFileName );
//	iImageFiles.Append( buf );
//	if ( iImageFiles.Count() == 1 )
//		{
//		TInt requestId = CEightPhotosProvider::GetInstanceL()->LoadImageL( this, iImageFiles[ iCurrentImageIdx ] );
//		}
	}

void CEightTileUiDynCycle::AddImage( TVgImage* /*aImage*/ )
    {
    //iImages.Append( aImage );
    }

void CEightTileUiDynCycle::AddTileUi( CEightTileUi* aTileUi )
    {
    iPages.Append( aTileUi );
    }

void CEightTileUiDynCycle::LoadNextImage()
	{
    TInt err = CEightPhotosProvider::GetInstanceL()->GetImageFromSource( this, iPhotosSourceId );
    if ( err < KErrNone && err != KErrNotReady /* iHighFpsIdle */ )
        {
        iImageLoadError = err;
        }

//	TInt requestId = CEightPhotosProvider::GetInstanceL()->LoadImageL( this, iImageFiles[ iNextImageIdx ] );
	}

TBool CEightTileUiDynCycle::DrawBackground() const
    {
    return ( !iMainImage );
    }

void CEightTileUiDynCycle::Draw()
    {
	if ( !iMainImage )
		{
	    if ( iStaticUi ) iStaticUi->Draw();

		return;
		}

    if ( iState == EFastCycling )
        {
        VGfloat m[ 9 ];
        TVgPoint p( iRectScale.BottomLeft() );
        
        vgGetMatrix( m );
        if ( iCurrentImage.IsChildImage() )
            {
            iCurrentImage.Draw( p );
            }
        TSize is = iMainImage->Size();
        p.iY += (TInt) ( is.iHeight - iCycleAnim.iValue );
        vgLoadMatrix( m );
        if ( iCurrentImage2.IsChildImage() )
            {
            iCurrentImage2.Draw( p );
            }
        }
    else
        {
        if ( iCurrentImage.IsChildImage() )
            {
            iCurrentImage.Draw( iRectScale.BottomLeft() );
            }
        }
    
    if ( iTile->TileState() == ESelected || iTile->TileState() == EStarting )
        {
        vgSetPaint( iPaint, VG_FILL_PATH );
        vgDrawPath( iPath, VG_FILL_PATH );
        }

    if ( iStaticUi ) iStaticUi->Draw();
    }

TBool CEightTileUiDynCycle::UpdateRectScale()
    {
    TSize rr = iRect.Size();
    TSize rs = iRectScale.Size();    
    TVgSize ss = iTile->TileScale();
    if ( ss.iWidth != rs.iWidth || ss.iHeight != rs.iHeight )
        {
        iRectScale = iRect;
        TReal w = rr.iWidth * ss.iWidth;
        TReal h = rr.iHeight * ss.iHeight;
        iRectScale.Grow( ( w - rr.iWidth ) / 2, ( h - rr.iHeight ) / 2 );
        vgClearPath( iPath, VG_PATH_CAPABILITY_ALL );
        vguRect( iPath, iRectScale.iTl.iX, iRectScale.iBr.iY, iRectScale.Width(), iRectScale.Height() );
        return ETrue;
        }
    //vguRect( iPath, iRectScale.iTl.iX, iRectScale.iBr.iY, iRectScale.Width(), iRectScale.Height() );
    return EFalse;
    }

void CEightTileUiDynCycle::Update( TTimeIntervalMicroSeconds32 aTimeInterval )
    {
	if ( !iReady )
		{
		LoadNextImage();
		iReady = ETrue;
		}
	if ( !iMainImage )
		{
	    if ( iImageLoadError != KErrNone )
	        {
	        iTimeCnt += aTimeInterval.Int();
	        if ( iTimeCnt > 7000000 )
	            {
	            iTimeCnt = 0;
	            LoadNextImage();
	            }
	        }
		return;
		}
//    TSize rr = iRect.Size();
//    TVgSize ss = iTile->TileScale();
    TBool updateImages = UpdateRectScale();
    TSize rs = iRectScale.Size();    
    
    if ( iState == EPauseStart )
        {
        iTimeCnt += aTimeInterval.Int();
        if ( iTimeCnt > 1000000 )
            {
            iTimeCnt = 0;
            iState = ECycling;
            }
        }
    else if ( iState == ECycling )
        {
        if ( iCycleAnim.Update( aTimeInterval ) )
            {
            TSize is = iMainImage->Size();
            if ( iCycleAnim.iValue + rs.iHeight < is.iHeight )
                {
                CreateCurrentImage();
                updateImages = EFalse;
                }
            else
                {
                iCycleAnim.iValue = is.iHeight - rs.iHeight;
                //iCycleAnim.iValue = (TInt) iCycleAnim.iValue;
                iState = EPauseEnd;
                iTimeCnt = 0;
                CreateCurrentImage();
                updateImages = EFalse;
                
                iNextImageIdx = iCurrentImageIdx + 1;
                if ( iNextImageIdx >= iImageFiles.Count() )
                    {
                    iNextImageIdx = 0;
                    }
                //LoadNextImage();
                }
            }
        }
    else if ( iState == EPauseEnd )
        {
        if ( iTimeCnt == 0 )
        	{
        	LoadNextImage();
        	iTimeCnt++;
        	}
        iTimeCnt += aTimeInterval.Int();
        if ( iTimeCnt > 1000000 && iMainImage2 )
            {
            iTimeCnt = 0;
            iState = EFastCycling;
            iCycleAnim.iBaseUpdateTime = 1000000 / 300;
            //iCycleAnim.Update( aTimeInterval );
            CreateTwoCurrentImages();
            updateImages = EFalse;
            }
        else if ( iTimeCnt > 5000000 && !iMainImage2 && iImageLoadError != KErrNone )
            {
            iTimeCnt = 1;
            iImageLoadError = KErrNone;
            LoadNextImage();
            }
        }
    else if ( iState == EFastCycling )
        {
        if ( iCycleAnim.Update( aTimeInterval ) )
            {
            TSize is = iMainImage->Size();
            if ( iCycleAnim.iValue < is.iHeight )
                {
                CreateTwoCurrentImages();
                updateImages = EFalse;
                }
            else
                {
                iState = EPauseStart;
                iCycleAnim.iValue = 0;
                iCycleAnim.iBaseUpdateTime = 1000000 / 30;
                iCurrentImageIdx = iNextImageIdx;
                iMainImageS.DeleteImage();
                iMainImageS = iMainImage2S;
                CEightPhotosProvider::GetInstanceL()->UnloadImage( iMainImageId );
                //iMainImage = iMainImage2;
                iMainImage = &iMainImageS;
                iMainImageId = iMainImage2Id;
                iMainImage2 = NULL;
                iCurrentImage2.DeleteImage();
                CreateCurrentImage();
                updateImages = EFalse;
                }
            }
        }

    if ( updateImages )
        {
        if ( iState == EFastCycling )
            {
            CreateTwoCurrentImages();
            }
        else
            {
            CreateCurrentImage();
            }
        }
    
    if ( iStaticUi ) iStaticUi->Update( aTimeInterval );
    }

void CEightTileUiDynCycle::ImageLoaded( TInt aImageId, TInt aError, TInt /*aSource*/ )
	{
    iImageLoadError = aError;
	if ( aError == KErrNone )
		{
		TVgImage* img = (TVgImage*) CEightPhotosProvider::GetInstanceL()->ImageManager().Get( aImageId );
		TSize s = img->Size();
		if ( img )
			{
			if ( !iMainImage )
				{
				iMainImageId = aImageId;
				iMainImageS = img->CreateChildImage( TPoint( 0, 1 ), TSize( s.iWidth, s.iHeight - 2 ) );
				iMainImage = &iMainImageS;
				}
			else
				{
				iMainImage2Id = aImageId;
				iMainImage2S = img->CreateChildImage( TPoint( 0, 1 ), TSize( s.iWidth, s.iHeight - 2 ) );
                iMainImage2 = &iMainImage2S;
				//iMainImage2 = img;
				}
			//iImages.Append( img );
			}
		}
	}


void CEightTileUiDynCycle::AddStaticUi( CEightTileUiStatic* aUi )
    {
    iStaticUi = aUi;
    }

CEightTileUiStatic* CEightTileUiDynCycle::StaticUi() const
    {
    return iStaticUi;
    }

void CEightTileUiDynCycle::UpdateColorsFromTile()
    {
    TVgColor c = iTile->Palette().iTileSelectedColor;
    c.SetAlpha( iTile->Palette().iTileImageColorAlpha );
    vgSetParameterfv( iPaint, VG_PAINT_COLOR, 4, c.iColor.Begin() );
    }

