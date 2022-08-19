/*
 ============================================================================
 Name		: EightTileUiDynFlip.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileUiDynFlip implementation
 ============================================================================
 */

#include "EightTileUiDynFlip.h"
#include "EightSettings.h"
#include <vg/vgu.h>
#include <e32math.h>


CEightTileUiDynFlip::CEightTileUiDynFlip( TInt aMinTime, TInt aMaxTime ) : iFilpAnim( 1000000 / 30, 0.09 ), iScale( 1 ), iFrontSide( ETrue ), 
        iMinTime( aMinTime ), iMaxTime( aMaxTime )
    {
    iFilpAnim.Pause();
    
    iPath = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 10, 20, VG_PATH_CAPABILITY_ALL );
    }

CEightTileUiDynFlip::~CEightTileUiDynFlip()
    {
    vgDestroyPath( iPath );
    if ( iTilesUi[ 0 ] == iTilesUi[ 1 ] )
        {
        delete iTilesUi[ 0 ];
        }
    else
        {
        delete iTilesUi[ 0 ];
        delete iTilesUi[ 1 ];
        }
    }

CEightTileUiDynFlip::TEightTileUiType CEightTileUiDynFlip::Type() const
    {
    return EDynFilp;
    }

void CEightTileUiDynFlip::SetTile( CEightTile* aOwnerTile )
    {
    CEightTileUi::SetTile( aOwnerTile );
    if ( iTilesUi[ 0 ] ) iTilesUi[ 0 ]->SetTile( aOwnerTile );
    if ( iTilesUi[ 1 ] && iTilesUi[ 0 ] != iTilesUi[ 1 ] ) iTilesUi[ 1 ]->SetTile( aOwnerTile );
    }

void CEightTileUiDynFlip::SetRect( TVgRect aRect )
    {
    iRect = aRect;
    vgClearPath( iPath, VG_PATH_CAPABILITY_ALL );
    vguRect( iPath, iRect.iTl.iX, iRect.iBr.iY, iRect.Width(), iRect.Height() );
    
    if ( iTilesUi[ 0 ] ) iTilesUi[ 0 ]->SetRect( aRect );
    if ( iTilesUi[ 1 ] && iTilesUi[ 0 ] != iTilesUi[ 1 ] ) iTilesUi[ 1 ]->SetRect( aRect );
    }

void CEightTileUiDynFlip::SetTilesUi( CEightTileUi* aFrontTileUi, CEightTileUi* aBackTileUi )
    {
    iTilesUi[ 0 ] = aFrontTileUi;
    iTilesUi[ 1 ] = aBackTileUi;
    }

TBool CEightTileUiDynFlip::FrontVisible()
    {
    return iFrontSide;
    }

CEightTileUi* CEightTileUiDynFlip::TileUi( TInt aIdx )
    {
    return iTilesUi[ aIdx ];
    }

CEightTileUi* CEightTileUiDynFlip::CurrentTileUi()
    {
    TInt idx = ( iFrontSide ? 0 : 1 );
    return iTilesUi[ idx ];
    }

CEightTileUi* CEightTileUiDynFlip::NextTileUi()
    {
    TInt idx = ( iFrontSide ? 1 : 0 );
    return iTilesUi[ idx ];
    }

void CEightTileUiDynFlip::Flip()
    {
    if ( !iFilpAnim.IsStarted() )
        {
        iFilpAnim.Start();
        }
    else
        {
        if ( iFlipRequested )
            {
            iFlipRequested = EFalse;
            }
        else
            {
            iFlipRequested = ETrue;
            }
        }
    }

TBool CEightTileUiDynFlip::IsFlipping()
    {
    return iFilpAnim.IsStarted() || iFlipRequested;
    }

TBool CEightTileUiDynFlip::DrawBackground() const
    {
    return EFalse;
    }

void CEightTileUiDynFlip::Draw()
    {
    VGfloat m[ 3 ][ 9 ];
    TVgSize s = iTile->TileScale();
    TBool s1 = ( iScale != 1 );
    TBool s2 = ( s.iWidth != 1 || s.iHeight * iScale != 1 );
    if ( s1 )
        {
        vgSeti( VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE );
        vgGetMatrix( m[ 0 ] );
        vgTranslate( iRect.iTl.iX + iRect.Width() / 2, iRect.iBr.iY + iRect.Height() / 2 );
        vgScale( 1, iScale );
        //vgScale( s.iWidth, s.iHeight * iScale );
        vgTranslate( - iRect.iTl.iX - iRect.Width() / 2, - iRect.iBr.iY - iRect.Height() / 2 );
        vgSeti( VG_MATRIX_MODE, VG_MATRIX_GLYPH_USER_TO_SURFACE );
        vgGetMatrix( m[ 1 ] );
        vgTranslate( iRect.iTl.iX + iRect.Width() / 2, iRect.iBr.iY + iRect.Height() / 2 );
        vgScale( 1, iScale );
        //vgScale( s.iWidth, s.iHeight * iScale );
        vgTranslate( - iRect.iTl.iX - iRect.Width() / 2, - iRect.iBr.iY - iRect.Height() / 2 );
        }
    if ( s2 )
        {
        vgSeti( VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE );
        vgGetMatrix( m[ 2 ] );
        vgTranslate( iRect.iTl.iX + iRect.Width() / 2, iRect.iBr.iY + iRect.Height() / 2 );
        vgScale( s.iWidth, s.iHeight * iScale );
        vgTranslate( - iRect.iTl.iX - iRect.Width() / 2, - iRect.iBr.iY - iRect.Height() / 2 );
        }

    TInt idx = ( iFrontSide ? 0 : 1 );
    if ( ( iTilesUi[ idx ] && iTilesUi[ idx ]->DrawBackground() ) || !iTilesUi[ idx ] )
        {
        vgSetPaint( iTile->TilePaint(), VG_FILL_PATH );
        vgDrawPath( iPath, VG_FILL_PATH );
        }    
    if ( iTilesUi[ idx ] ) iTilesUi[ idx ]->Draw();
    
    if ( s1 )
        {
        vgSeti( VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE );
        vgLoadMatrix( m[ 0 ] );
        vgSeti( VG_MATRIX_MODE, VG_MATRIX_GLYPH_USER_TO_SURFACE );
        vgLoadMatrix( m[ 1 ] );
        }
    if ( s2 )
        {
        vgSeti( VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE );
        vgLoadMatrix( m[ 2 ] );
        }
    }

void CEightTileUiDynFlip::Update( TTimeIntervalMicroSeconds32 aTimeInterval )
    {
//    TReal v = aTimeInterval.Int();
//    v /= 1000000;
    
    if ( iFilpAnim.Update( aTimeInterval ) )
        {    
        Math::Cos( iScale, iFilpAnim.iValue );
        iScale = iScale * iScale * iScale * iScale * iScale;
        if ( !iFrontSide )
            {
            iScale = 1 - iScale;
            }
        if ( ( iFrontSide && iScale <= 0 ) ||
             ( !iFrontSide && iScale >= 1 ) )
            {
            iFrontSide = !iFrontSide;
            iValue = 0;
            iFilpAnim.iValue = 0;
            if ( iScale < 0 )
                {
                iScale = 0;
                }
            if ( iScale >= 1 )
                {
                iScale = 1;
                iFilpAnim.Pause();

                CEightTileUi* u = iTilesUi[ 0 ];
                iTilesUi[ 0 ] = iTilesUi[ 1 ];
                iTilesUi[ 1 ] = u;
                
                if ( iFlipRequested )
                    {
                    iFilpAnim.Start();
                    iFlipRequested = EFalse;
                    }
                }
            }
        else
            {
    //        iValue += v;
            }
        }
    else if ( iMaxTime != 0 && iMinTime != 0 )
        {
        if ( iNextTime == 0 )
            {
            TReal random = Math::Random();
            TReal round;
            if ( Math::Round( round, (TReal)(iMaxTime - iMinTime) * random / (TReal) 0xFFFFFFFF, 0 ) == KErrNone )
                {
                iNextTime = iMinTime + round;
                }
            }
        else
            {
            iNextTime -= aTimeInterval.Int();
            if ( iNextTime <= 0 )
                {
                iNextTime = 0;
                iFilpAnim.Start();
                }
            }        
        }

    }


