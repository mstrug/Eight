/*
 ============================================================================
 Name		: EightTileUiDynMosaic.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileUiDynMosaic implementation
 ============================================================================
 */

#include "EightTileUiDynMosaic.h"
#include "EightSettings.h"
#include <vg/vgu.h>
#include <e32math.h>


CEightTileUiDynMosaic::CEightTileUiDynMosaic( TInt aMinTime, TInt aMaxTime, TInt aParallelFlipCount, TInt aImageSource ) : iMinTime( aMinTime ), iMaxTime( aMaxTime ), 
    iParallelFlipCount( aParallelFlipCount ), iImageSource( aImageSource )
	{
	CEightSettings* settings = CEightSettings::GetInstance();
	for ( TInt i = 0; i < iItems.Count(); i++ )
		{
		for ( TInt j = 0; j < iItems[ i ].Count(); j++ )
			{
			//iItems[ i ][ j ].iFlipIdx = KErrNotFound;
			iItems[ i ][ j ].iPath = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 10, 20, VG_PATH_CAPABILITY_ALL );
			iItems[ i ][ j ].iPaint = vgCreatePaint();
			iItems[ i ][ j ].iImageId = KErrNotFound;
			}
		}
	
    iSelectedPath = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 10, 20, VG_PATH_CAPABILITY_ALL );    
    iSelectedPaint = vgCreatePaint();
    vgSetParameteri( iSelectedPaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR );
    
    if ( iImageSource > KErrNotFound )
        {
        CEightPhotosProvider::GetInstanceL()->GetImageFromSource( this, iImageSource );
        }
	}

CEightTileUiDynMosaic::~CEightTileUiDynMosaic()
	{
	for ( TInt i = 0; i < iItems.Count(); i++ )
		{
		for ( TInt j = 0; j < iItems[ i ].Count(); j++ )
			{
			vgDestroyPath( iItems[ i ][ j ].iPath );
		    vgDestroyPaint( iItems[ i ][ j ].iPaint );
			}
		}
	vgDestroyPath( iSelectedPath );
    vgDestroyPaint( iSelectedPaint );
    
    iFlips.Close();
    iImageIds.Close();
    
    delete iStaticUi;
	}

CEightTileUiDynMosaic::TEightTileUiType CEightTileUiDynMosaic::Type() const
    {
    return EDynMosaic;
    }

void CEightTileUiDynMosaic::UpdateRects()
	{
	TInt dx = iRectScale.Width() / iDivX;
	TInt dy = iRectScale.Height() / iDivY;
	
	for ( TInt y = 0; y < iDivY; y++ )
		{
		for ( TInt x = 0; x < iDivX; x++ )
			{
		    vgClearPath( iItems[ x ][ y ].iPath, VG_PATH_CAPABILITY_ALL );
		    vguRect( iItems[ x ][ y ].iPath, iRectScale.iTl.iX + x * dx, iRectScale.iBr.iY + y * dy, dx, dy );
			}
		}
	}

void CEightTileUiDynMosaic::SetRect( TVgRect aRect )
	{
	iRect = aRect;
	iRectScale = aRect;

	TSize s = iRect.Size();
	
	TInt divxold = iDivX;
	
	iDivY = 3;
	iDivX = 3;
	if ( s.iWidth != s.iHeight )
		{
		iDivX = 6;
		}
	
	if ( divxold == 6 && iDivX != 6 )
	    {
	    for ( TInt y = 0, i = 1; y < iDivY; y++ )
	        {
	        for ( TInt x = 3; x < 6; x++ )
	            {
	            if ( iItems[ x ][ y ].iImageId != KErrNotFound )
	                {
	                SFlip f;
	                f.iStartTime = 200000 * i++;
	                f.iItemIdx.iX = x;
	                f.iItemIdx.iY = y;
	                TReal v = 0.085;
	                f.iAnim = TAnimationValueProportionalFragment< VGfloat >( 1000000 / 30, v );
	                f.iImageId = KErrNotFound;
	                f.iScale = 1;
	                f.iTime = 0;
	                f.iFrontSide = ETrue;

	                iFlips.Append( f );
	                
	                //iImageIds.Append( iItems[ x ][ y ].iImageId );
	                //iItems[ x ][ y ].iImageId = KErrNotFound;
	                }
	            }
	        }
	    }
	
	UpdateRects();
    
	if ( iStaticUi ) iStaticUi->SetRect( aRect );
	
    GenerateFlips();
	}

TBool CEightTileUiDynMosaic::DrawBackground() const
	{
	return EFalse;
	}

TVgColor CEightTileUiDynMosaic::GetColor()
	{
	TVgColor c[ 9 ];
	c[ 0 ] = iTile->Palette().iTileColorLow;
	c[ 1 ] = iTile->Palette().iTileColor;
	c[ 2 ] = iTile->Palette().iTileColorHigh;
	c[ 3 ] = iTile->Palette().iTileColor;
	c[ 4 ] = iTile->Palette().iTileColorLow;
	c[ 5 ] = iTile->Palette().iTileColorHigh;
	c[ 6 ] = iTile->Palette().iTileColorHigh;
	c[ 7 ] = iTile->Palette().iTileColorLow;
	c[ 8 ] = iTile->Palette().iTileColor;
	
    TReal random = Math::Random();
    TReal round;
    if ( Math::Round( round, (TReal)8.0 * random / (TReal) 0xFFFFFFFF, 0 ) == KErrNone )
        {
    	return c[ (TInt) round ];
        }
	return c[ 1 ];
	}

void CEightTileUiDynMosaic::GenerateFlips()
	{
	TInt max = iParallelFlipCount;
	if ( iDivX != iDivY ) max++;
	if ( iFlips.Count() >= max )
		{
		return;
		}
	for ( TInt i = iFlips.Count(); i < max; i++ )
		{
		SFlip f;
		
        TReal random = Math::Random();
        random = Math::Random();
        TReal round;
        TReal div = 1, div2 = 1;
        if ( i % 2 == 0 ) div = 2;
        else div2 = 2;
        if ( Math::Round( round, (TReal)(iMaxTime / div - iMinTime / div2 ) * random / (TReal) 0xFFFFFFFF, 0 ) == KErrNone )
            {
        	f.iStartTime = iMinTime / div + round;
            }

        random = Math::Random();
        if ( Math::Round( round, (TReal)(iDivX - 1) * random / (TReal) 0xFFFFFFFF, 0 ) == KErrNone )
            {
        	f.iItemIdx.iX = round;
            }
		random = Math::Random();
		if ( Math::Round( round, (TReal)(iDivY - 1) * random / (TReal) 0xFFFFFFFF, 0 ) == KErrNone )
			{
			f.iItemIdx.iY = round;
			}
		random = Math::Random();
		TReal v = 0.045 + 0.04f * random / (TReal) 0xFFFFFFFF;
		f.iAnim = TAnimationValueProportionalFragment< VGfloat >( 1000000 / 30, v );
		f.iImageId = KErrNotFound;
        if ( ( Math::Random() % ( iDivX == 6 ? 2 : 3 ) ) == 0 && iImageSource > KErrNotFound )
            {
            CEightPhotosProvider::GetInstanceL()->GetImageFromSource( this, iImageSource );
            if ( iImageIds.Count() > 0 )
                {
                random = Math::Random();
                if ( Math::Round( round, (TReal)(iImageIds.Count() - 1) * random / (TReal) 0xFFFFFFFF, 0 ) == KErrNone )
                    {
                    f.iImageId = iImageIds[ round ];
                    iImageIds.Remove( round );
                    }
                }
            }
        
        f.iScale = 1;
        f.iTime = 0;
        f.iFrontSide = ETrue;

        iFlips.Append( f );
        
        //iItems[ f.iItemIdx.iX ][ f.iItemIdx.iY ].iFlipIdx = iFlips.Count() - 1;
		}

	// clean doubled tile flips
	for ( TInt i = 0; i < iFlips.Count(); i++ )
		{
		for ( TInt j = 0; j < iFlips.Count(); j++ )
			{
			if ( i != j && iFlips[ i ].iItemIdx == iFlips[ j ].iItemIdx )
				{
                if ( iFlips[ j ].iImageId != KErrNotFound )
                   {
                   iImageIds.Append( iFlips[ j ].iImageId );
                   }
				iFlips.Remove( j );
				if ( j < i ) i--;
				j--;
				}
			}		
		}
	}

void CEightTileUiDynMosaic::UpdateFlips( TTimeIntervalMicroSeconds32 aTimeInterval )
	{
	for ( TInt i = 0; i < iFlips.Count(); i++ )
		{
		iFlips[ i ].iTime += aTimeInterval.Int();
		if ( iFlips[ i ].iTime >= iFlips[ i ].iStartTime )
			{
			if ( iFlips[ i ].iAnim.Update( aTimeInterval ) )
				{
			    TInt x = iFlips[ i ].iItemIdx.iX;
			    TInt y = iFlips[ i ].iItemIdx.iY;
				TReal old = iFlips[ i ].iScale;
				Math::Cos( iFlips[ i ].iScale, iFlips[ i ].iAnim.iValue );
				iFlips[ i ].iScale = iFlips[ i ].iScale * iFlips[ i ].iScale * iFlips[ i ].iScale; // * iFlips[ i ].iScale * iFlips[ i ].iScale;
                if ( iFlips[ i ].iScale < -1 ) iFlips[ i ].iScale = -1; 
                if ( iFlips[ i ].iScale > 1 ) iFlips[ i ].iScale = 1; 
				if ( iFlips[ i ].iFrontSide && iFlips[ i ].iScale <= 0 )
					{
					iFlips[ i ].iFrontSide = EFalse;
					iFlips[ i ].iAnim.iValueUpdate *= 1.75;
					vgSetParameterfv( iItems[ x ][ y ].iPaint, VG_PAINT_COLOR, 4, GetColor().iColor.Begin() );
					
					if ( iItems[ x ][ y ].iImageId != KErrNotFound )
					    {
					    iImageIds.Append( iItems[ x ][ y ].iImageId );
					    }
					iItems[ x ][ y ].iImageId = iFlips[ i ].iImageId;
					}
				else if ( !iFlips[ i ].iFrontSide && old <= iFlips[ i ].iScale )
					{
					//iItems[ iFlips[ i ].iItemIdx.iX ][ iFlips[ i ].iItemIdx.iY ].iFlipIdx = KErrNotFound;
					iFlips.Remove( i );
					i--;
					}
				}
			}
		}
	
	GenerateFlips();
	}

void CEightTileUiDynMosaic::Draw()
	{
	TReal dx = iRectScale.Width() / iDivX;
	TReal dy = iRectScale.Height() / iDivY;
	
	for ( TInt y = 0; y < iDivY; y++ )
		{
		for ( TInt x = 0; x < iDivX; x++ )
			{
			TInt idx = KErrNotFound;
			for ( TInt i = 0; i < iFlips.Count(); i++ )
				{
				if ( iFlips[ i ].iItemIdx.iX == x && iFlips[ i ].iItemIdx.iY == y )
					{
					idx = i;
					break;
					}
				}
			
			VGfloat m[ 9 ];
            MVgImage* img = NULL;
            if ( iItems[ x ][ y ].iImageId > KErrNotFound )
                {
                img = CEightPhotosProvider::GetInstanceL()->ImageManager().Get( iItems[ x ][ y ].iImageId );
                }
            if ( img )
                {
                vgSeti( VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE );
                vgGetMatrix( m );
                vgTranslate( iRectScale.iTl.iX + (TReal)x * dx, iRectScale.iBr.iY + (TReal)y * dy );
                if ( idx > KErrNotFound )
                    {
                    vgTranslate( dx / 2.0f, dy / 2.0f );
                    if ( iFlips[ idx ].iFrontSide )
                        {
                        vgScale( 1, iFlips[ idx ].iScale );
                        }
                    else
                        {
                        vgScale( 1, -iFlips[ idx ].iScale );
                        }
                    vgTranslate( - dx / 2.0f, - dy / 2.0f );
                    }
                TSize s = img->Size();
                if ( s.iWidth != dx || s.iHeight != dy )
                    {
                    vgScale( dx / s.iWidth, dy / s.iHeight );
                    }
                img->Draw( TVgPoint() );
                vgLoadMatrix( m );
                }
            else
                {
                if ( idx > KErrNotFound )
                    {
                    vgSeti( VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE );
                    vgGetMatrix( m );
                    vgTranslate( iRectScale.iTl.iX  + (TReal)x * dx + dx / 2.0f, iRectScale.iBr.iY + (TReal)y * dy + dy / 2.0f );
                    vgScale( 1, iFlips[ idx ].iScale );
                    vgTranslate( - iRectScale.iTl.iX - (TReal)x * dx - dx / 2.0f, - iRectScale.iBr.iY - (TReal)y * dy - dy / 2.0f );
                    }
    
                vgSetPaint( iItems[ x ][ y ].iPaint, VG_FILL_PATH );
                vgDrawPath( iItems[ x ][ y ].iPath, VG_FILL_PATH );
    
                if ( idx > KErrNotFound )
                    {
                    vgLoadMatrix( m );
                    }
                }
			}
		}
	
    if ( iTile->TileState() == ESelected || iTile->TileState() == EStarting )
        {
        vgSetPaint( iSelectedPaint, VG_FILL_PATH );
        vgDrawPath( iSelectedPath, VG_FILL_PATH );
        }
    
    if ( iStaticUi ) iStaticUi->Draw();

	}

TBool CEightTileUiDynMosaic::UpdateRectScale()
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
    	
        UpdateRects();
    	
        vgClearPath( iSelectedPath, VG_PATH_CAPABILITY_ALL );
        vguRect( iSelectedPath, iRectScale.iTl.iX, iRectScale.iBr.iY, iRectScale.Width(), iRectScale.Height() );

        return ETrue;
        }
	return EFalse;
    }

void CEightTileUiDynMosaic::ImageLoaded( TInt aImageId, TInt aError, TInt aSource )
    {
    if ( aError == KErrNone && aSource == iImageSource )
        {
        iImageIds.Append( aImageId );
        }
    }

void CEightTileUiDynMosaic::Update( TTimeIntervalMicroSeconds32 aTimeInterval )
	{
	UpdateRectScale();
	UpdateFlips( aTimeInterval );
	if ( iStaticUi ) iStaticUi->Update( aTimeInterval );
	}

void CEightTileUiDynMosaic::AddStaticUi( CEightTileUiStatic* aUi )
    {
    iStaticUi = aUi;
    }

CEightTileUiStatic* CEightTileUiDynMosaic::StaticUi() const
    {
    return iStaticUi;
    }

void CEightTileUiDynMosaic::UpdateColorsFromTile()
    {
    for ( TInt i = 0; i < iItems.Count(); i++ )
        {
        for ( TInt j = 0; j < iItems[ i ].Count(); j++ )
            {
            vgSetParameteri( iItems[ i ][ j ].iPaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR );
            vgSetParameterfv( iItems[ i ][ j ].iPaint, VG_PAINT_COLOR, 4, GetColor().iColor.Begin() );
            }
        }
    
    TVgColor c = iTile->Palette().iTileSelectedColor;
    c.SetAlpha( iTile->Palette().iTileImageColorAlpha );
    vgSetParameterfv( iSelectedPaint, VG_PAINT_COLOR, 4, c.iColor.Begin() );
    }

