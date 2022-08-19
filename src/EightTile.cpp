/*
 ============================================================================
 Name		: EightTile.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTile implementation
 ============================================================================
 */

#include "EightTile.h"
#include "EightSettings.h"
#include "EightTileUiDynFlip.h"
#include <vg/vgu.h>
#include <e32math.h>

#define SGN( x ) ( (x) < 0 ? (-1) : (1) )

//const VGfloat KScaleUpdate = 0.01;

const VGfloat KS1 = 81.0 / 75.0 - 69.0 / 75.0;
const VGfloat KS2 = 168.0 / 162.0 - 156.0 / 162.0;
const VGfloat KS3 = 342.0 / 336.0 - 330.0 / 336.0;
const VGfloat KScaleUpdateMod = 0.06; 

const TVgSize KScaleUpdate[ KEightTileSizeCount ] = { TVgSize( KS1, KS1 ), TVgSize( KS2, KS2 ), TVgSize( KS3, KS2 ), TVgSize( KS1, KS1 ) };

const TInt KFlippingTimeMin = 2000000;
const TInt KFlippingTimeMax = 6000000;

const TInt KRunTimeMax = 300000;
const VGfloat KScaleRunUpdateMod = 0.09;
        

CEightTile::CEightTile( TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi, TBool aEmpty ) : iId( aId ), iUi( aUi ), iTileSize( aTileSize ), 
    iEmpty( aEmpty ), iScaleX( 10000, 0 ), iScaleY( 10000, 0 ), iScaleFinalX( 1 ), iScaleFinalY( 1 ),
    iFlipping( EFalse ), iSupportsFlipping( ETrue ), iSupportsCustomColor( ETrue ), iSupportRemoving( ETrue )
    {
    iScaleX.iValue = 1;
    iScaleX.Pause();
    iScaleY.iValue = 1;
    iScaleY.Pause();
    
    switch ( aTileSize )
        {
        case ETileSize_1x1:
                iSize.SetSize( 1, 1 );
            break;
        case ETileSize_2x2:
                iSize.SetSize( 2, 2 );
            break;
        case ETileSize_4x2:
                iSize.SetSize( 4, 2 );
            break;
        case ETileSize_3x1:
                iSize.SetSize( 3, 1 );
            break;
        }
    
    iPalette = CEightSettings::GetInstance()->iPalette;
    }

CEightTile::~CEightTile()
    {
    vgDestroyPath( iPath );
    vgDestroyPaint( iPaint );

    delete iUi;
    }

CEightTile* CEightTile::NewL( TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi, TBool aEmpty )
    {
    CEightTile* self = new ( ELeave ) CEightTile( aId, aTileSize, aUi, aEmpty );
    CleanupStack::PushL( self );
    self->BaseConstructL();
    CleanupStack::Pop(); // self;
    return self;
    }

void CEightTile::BaseConstructL()
    {
	iPath = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 10, 20, VG_PATH_CAPABILITY_ALL );
	
    iPaint = vgCreatePaint();
    vgSetParameteri( iPaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR );
    vgSetParameterfv( iPaint, VG_PAINT_COLOR, 4, iPalette.iTileColor.iColor.Begin() );
    
    if ( iUi )
        {
        iUi->SetTile( this );
        }
    }

TUint CEightTile::Id() const
    {
    return iId;
    }

CEightTileUi* CEightTile::Ui() const
    {
    return iUi;
    }

void CEightTile::SetUi( CEightTileUi* aUi )
    {
    iUi = aUi;
    if ( iUi )
        {
        iUi->SetTile( this );
        }
    }

void CEightTile::SetTileMetrics( TInt /*aTileSizePx*/ )
    {
    //iSize.iWidth = aTileSizePx;
    }

TEightTileType CEightTile::Type() const
    {
    return ETileTypeNormal;
    }

TSize CEightTile::Size() const
    {
    return iSize;
    }

TEightTileSize CEightTile::TileSize() const
    {
    return iTileSize;
    }

void CEightTile::SetTileSize( TEightTileSize aTileSize )
    {
    iTileSize = aTileSize;
    switch ( aTileSize )
        {
        case ETileSize_1x1:
                iSize.SetSize( 1, 1 );
            break;
        case ETileSize_2x2:
                iSize.SetSize( 2, 2 );
            break;
        case ETileSize_4x2:
                iSize.SetSize( 4, 2 );
            break;
        case ETileSize_3x1:
                iSize.SetSize( 3, 1 );
            break;
        }
    }

void CEightTile::SetMsgBox( CEightMsgBox* /*aMsgBox*/ )
    {
    // do nothing by default
    }

TEightTileState CEightTile::TileState() const
    {
    return iState;
    }

void CEightTile::SetBackgroundColor( TVgColor aColor )
    {
    iPalette.iTileColor = aColor;
    if ( iState == ENormal )
        {
        vgSetParameterfv( iPaint, VG_PAINT_COLOR, 4, aColor.iColor.Begin() );
        }
    }

void CEightTile::SetSelectedColor( TVgColor aColor )
    {
    iPalette.iTileSelectedColor = aColor;
    if ( iState == ESelected )
        {
        vgSetParameterfv( iPaint, VG_PAINT_COLOR, 4, aColor.iColor.Begin() );
        }
    }

void CEightTile::SetPressedColor( TVgColor aColor )
    {
    iPalette.iTilePressedColor = aColor;
    if ( iState == EPressed )
        {
        vgSetParameterfv( iPaint, VG_PAINT_COLOR, 4, aColor.iColor.Begin() );
        }
    }

void CEightTile::UpdateColorsFromTheme()
    {
    if ( iCustomColor ) return;
    iPalette = CEightSettings::GetInstance()->iPalette;
    switch ( iState )
        {
        case ENormal:
        case EFaded:
                vgSetParameterfv( iPaint, VG_PAINT_COLOR, 4, iPalette.iTileColor.iColor.Begin() );
            break;
        case EPressed:
                vgSetParameterfv( iPaint, VG_PAINT_COLOR, 4, iPalette.iTilePressedColor.iColor.Begin() );
            break;
        case EStarting:
        case ESelected:
                vgSetParameterfv( iPaint, VG_PAINT_COLOR, 4, iPalette.iTileSelectedColor.iColor.Begin() );
            break;
        }
    if ( iUi )
        {
        iUi->UpdateColorsFromTile();
        }
    }

void CEightTile::UpdateColorsFromIndex( TInt aColorIndex )
    {
    SetBackgroundColor( KEightColorsBase[ aColorIndex ] );
    SetSelectedColor( KEightColorsSelected[ aColorIndex ] );
    SetPressedColor( KEightColorsPressed[ aColorIndex ] );
    iPalette.iTileColorLow = KEightColorsLow[ aColorIndex ];
    iPalette.iTileColorHigh = KEightColorsHigh[ aColorIndex ];

    if ( iUi )
        {
        iUi->UpdateColorsFromTile();
        }
    }

void CEightTile::UpdateColorsFromPalette( const TEightPalette& aPalette )
    {
    SetBackgroundColor( aPalette.iTileColor );
    SetSelectedColor( aPalette.iTileSelectedColor );
    SetPressedColor( aPalette.iTilePressedColor );
    iPalette.iTileColorLow = aPalette.iTileColorLow;
    iPalette.iTileColorHigh = aPalette.iTileColorHigh;

    if ( iUi )
        {
        iUi->UpdateColorsFromTile();
        }
    }

void CEightTile::SetCustomColor( TBool aCustom )
    {
    iCustomColor = aCustom;
    }

TBool CEightTile::CustomColor() const
    {
    return iCustomColor;
    }

TBool CEightTile::SupportsCustomColor() const
    {
    return iSupportsCustomColor;
    }

void CEightTile::SetCustomColorSupport( TBool aSupport )
    {
    iSupportsCustomColor = aSupport;
    if ( !iSupportsCustomColor && iCustomColor )
        {
        SetCustomColor( EFalse );
        }
    }

const TEightPalette& CEightTile::Palette() const
    {
    return iPalette;
    }

void CEightTile::SetFlipping( TBool aFlipping )
    {
    if ( !iSupportsFlipping || iFlipping == aFlipping )
        {
        return;
        }
    iFlipping = aFlipping;
    if ( aFlipping )
        {
        CEightTileUi* ui = iUi;
        iUi = new CEightTileUiDynFlip( KFlippingTimeMin, KFlippingTimeMax );
        if ( iUi )
            {
            iUi->SetTile( this );
            ((CEightTileUiDynFlip*)iUi)->SetTilesUi( ui, ui );
            iUi->SetRect( iRect );
            }
        else
            {
            iUi = ui;
            }
        }
    else
        {
        CEightTileUi* ui = ((CEightTileUiDynFlip*)iUi)->TileUi( 0 );
        ((CEightTileUiDynFlip*)iUi)->SetTilesUi( NULL, NULL );
        delete iUi;
        iUi = ui;
        }
    }

TBool CEightTile::IsFlipping() const
    {
    return iFlipping;
    }

void CEightTile::SetFlippingSupport( TBool aSupportFlipping )
    {
    if ( iSupportsFlipping && !aSupportFlipping )
        {
        SetFlipping( EFalse );
        }
    iSupportsFlipping = aSupportFlipping;
    }

TBool CEightTile::SupportsFlipping() const
    {
    return iSupportsFlipping;
    }

TBool CEightTile::IsPinned() const
    {
    return iPinned;
    }

void CEightTile::SetPinned( TBool aPinned )
    {
    iPinned = aPinned;
    }

VGPaint& CEightTile::TilePaint()
    {
    return iPaint;
    }

VGPath& CEightTile::TilePath()
    {
    return iPath;
    }

TVgSize CEightTile::TileScale()
    {
    return TVgSize( iScaleX.iValue, iScaleY.iValue );
    }

void CEightTile::UpdateScaleNow()
    {
    CEightSettings* settings = CEightSettings::GetInstance();
    VGfloat scalex = settings->iScale[ iState ][ iTileSize ].iWidth, scaley = settings->iScale[ iState ][ iTileSize ].iHeight;
    iScaleFinalX = iScaleX.iValue = scalex;
    iScaleX.Pause();
    iScaleFinalY = iScaleY.iValue = scaley;
    iScaleY.Pause();
    }

void CEightTile::IncreaseTileSize()
    {
    if ( iTileSize == ETileSize_1x1 )
        {
        iTileSize = ETileSize_2x2;
        iSize.SetSize( 2, 2 );
        }
    else if ( iTileSize == ETileSize_2x2 )
        {
        iTileSize = ETileSize_4x2;
        iSize.SetSize( 4, 2 );
        }
    }

void CEightTile::DecreaseTileSize()
    {
    iTileSize = ETileSize_1x1;
    iSize.SetSize( 1, 1 );
    }

TBool CEightTile::IsEmpty() const
    {
    return iEmpty;
    }

void CEightTile::SetEmpty()
    {
    iEmpty = ETrue;
    }

TVgRect& CEightTile::Rect()
    {
    return iRect;
    }

void CEightTile::SetRect( const TVgRect& aRect )
    {
    iRect = aRect;
    vgClearPath( iPath, VG_PATH_CAPABILITY_ALL );
    if ( iTileSize == ETileSize_3x1 )
        {
        vguRect( iPath, iRect.iTl.iX, iRect.iBr.iY, iRect.Height(), iRect.Height() );
        }
    else
        {
        vguRect( iPath, iRect.iTl.iX, iRect.iBr.iY, iRect.Width(), iRect.Height() );
        }
    UpdateScaleNow();
    if ( iUi )
        {
        iUi->SetRect( aRect );
        }
    }

void CEightTile::SetPressed( TBool aPressed )
    {
	if ( iState == EStarting ) return;
	
    iState = ( aPressed ? EPressed : ENormal );
    
    if ( aPressed )
    	{
        vgSetParameterfv( iPaint, VG_PAINT_COLOR, 4, iPalette.iTilePressedColor.iColor.Begin() );
    	}
    else
    	{
        vgSetParameterfv( iPaint, VG_PAINT_COLOR, 4, iPalette.iTileColor.iColor.Begin() );
    	}
    }

void CEightTile::SetSelected( TBool aSelected )
    {
	if ( iState == EStarting ) return;

	iState = ( aSelected ? ESelected : ENormal );
    
    if ( aSelected )
    	{
        vgSetParameterfv( iPaint, VG_PAINT_COLOR, 4, iPalette.iTileSelectedColor.iColor.Begin() );
    	}
    else
    	{
        vgSetParameterfv( iPaint, VG_PAINT_COLOR, 4, iPalette.iTileColor.iColor.Begin() );
    	}
    }

void CEightTile::SetFaded( TBool aFaded )
	{
	if ( iState == EStarting ) return;
	
	if ( iState == ESelected && !aFaded )
		{
		return;
		}
    iState = ( aFaded ? EFaded : ENormal );
    
	vgSetParameterfv( iPaint, VG_PAINT_COLOR, 4, iPalette.iTileColor.iColor.Begin() );
	}

TInt CEightTile::CustomSettingsButtonsCount() const
    {
    return 0;
    }

CTouchControlBase* CEightTile::GetCustomSettingsButtonL( TInt /*aIdx*/ )
    {
    return NULL;
    }

void CEightTile::HandleCustomSettingsButtonState( CTouchControlBase* /*aButton*/ )
    {
    }

void CEightTile::CleanupCustomSettingsButtons()
    {
    }

TBool CEightTile::CanIncreaseSize() const
    {
    return ( iTileSize <= ETileSize_2x2 );
    }

TBool CEightTile::CanDecreaseSize() const
    {
    return ( iTileSize >= ETileSize_2x2 ) && ( iTileSize != ETileSize_3x1 );
    }

void CEightTile::SetCanBeRemovedSupport( TBool aSupportRemoving )
    {
    iSupportRemoving = aSupportRemoving;
    }

TBool CEightTile::SupportsRemoving() const
    {
    return iSupportRemoving;
    }

TBool CEightTile::CanBeRemoved() const
    {
    return ( iTileSize != ETileSize_3x1 ) && iSupportRemoving;
    //return ETrue;
    }

TBool CEightTile::CanBeMoved() const
	{
    return ( iTileSize != ETileSize_3x1 );
	//return ETrue;
	}

TBool CEightTile::CanOpenSettings() const
	{
    return ( iTileSize != ETileSize_3x1 );
	//return ETrue;
	}

void CEightTile::HandleForegroundEventL( TBool /*aForeground*/ )
    {    
    }

TBool CEightTile::HandlePointerEventL( const TPointerEvent& aPointerEvent )
    {
    if ( aPointerEvent.iType == TPointerEvent::EButton1Down )
        {
        iRectGrowed = ETrue;
        }
    else if ( aPointerEvent.iType == TPointerEvent::EButton1Up )
        {
        iRectGrowed = EFalse;
        }
    return ETrue;
    }

void CEightTile::Run()
    {
	iState = EStarting;
    vgSetParameterfv( iPaint, VG_PAINT_COLOR, 4, iPalette.iTileSelectedColor.iColor.Begin() );
	iRunTime = 0;
    // RunInternal(); // called from update
    }

void CEightTile::ResetRunState()
	{
	if ( iState == EStarting )
		{
		iState = ENormal;
		SetSelected( EFalse ); // to update iPaint
        RunInternal();
		}	
	}

TBool CEightTile::RunInternal()
	{
    return EFalse;
	}

void CEightTile::DrawDebug()
	{
	CVgFont* font = CEightSettings::GetInstance()->FontManager().Get( 1 );
	
	if ( font )
		{
		font->DrawText( iRect.BottomLeft(), _L("rr") );
		}
	}

void CEightTile::Draw()
    {
    if ( ( iUi && iUi->DrawBackground() ) || !iUi )
        {
        VGfloat m[ 9 ];
        vgSeti( VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE );

        if ( iScaleX.iValue != 1 || iScaleY.iValue != 1 )
            {
            vgGetMatrix( m );
            VGfloat w = iRect.Width();
            if ( iTileSize == ETileSize_3x1 ) w = iRect.Height();
            vgTranslate( iRect.iTl.iX + w / 2, iRect.iBr.iY + iRect.Height() / 2 );
            
//            if ( Abs( scalex - iScaleX ) > 0.001 )
//                {
//                if ( scalex > 1 ) iScaleX += 0.001;
//                else iScaleX -= 0.001;
//                }
//            else iScaleX = scalex;
//            
//            if ( Abs( scaley - iScaleY ) > 0.001 )
//                {
//                if ( scaley > 1 ) iScaleY += 0.001;
//                else iScaleY -= 0.001;
//                }
//            else iScaleY = scaley;
            
            vgScale( iScaleX.iValue, iScaleY.iValue );
            vgTranslate( - iRect.iTl.iX - w / 2, - iRect.iBr.iY - iRect.Height() / 2 );
            }
    
        vgSetPaint( iPaint, VG_FILL_PATH );
        vgDrawPath( iPath, VG_FILL_PATH );
        if ( iScaleX.iValue != 1 || iScaleY.iValue != 1 )
            {
            vgLoadMatrix( m );
            }
        }
    
    if ( iUi )
        {
        iUi->Draw();
        }
    }

void CEightTile::Update( TTimeIntervalMicroSeconds32 aTimeInterval )
    {
    CEightSettings* settings = CEightSettings::GetInstance();
    VGfloat scalex = settings->iScale[ iState ][ iTileSize ].iWidth, scaley = settings->iScale[ iState ][ iTileSize ].iHeight;
	if ( iState == EStarting )
		{
		iScaleMod = KScaleRunUpdateMod;
		}
	else
		{
	    iScaleMod = KScaleUpdateMod;
		}

    if ( scalex != iScaleFinalX )
        {
    	if ( iState != ESelected && iState != EPressed && iState != EStarting )
            {
            TReal random = Math::Random();
            TReal round;
            const VGfloat KTab[ 10 ] = { 0.05, 0.04, 0.03, 0.02, 0.025, 0.06, 0.07, 0.08, 0.075, 0.35 };
            //const VGfloat KTab[ 10 ] = { 0.05, 0.04, 0.03, 0.02, 0.01, 0.06, 0.07, 0.08, 0.09, 0.1 };
            if ( Math::Round( round, (TReal)9 * random / (TReal) 0xFFFFFFFF, 0 ) == KErrNone )
                {
                if ( round >= 0 && round <= 9 )
                    {
                    iScaleMod = KTab[ (TInt) round ];
                    }
                }
            }
        
        iScaleFinalX = scalex;
        if ( iScaleFinalX != iScaleX.iValue )
            {
            iScaleX.iValueUpdate = ( iScaleFinalX < iScaleX.iValue ? - KScaleUpdate[ iTileSize ].iWidth : KScaleUpdate[ iTileSize ].iWidth );
            iScaleX.iValueUpdate *= iScaleMod;
            iScaleX.Start();
            }
        }
    if ( scaley != iScaleFinalY )
        {
        iScaleFinalY = scaley;
        if ( iScaleFinalY != iScaleY.iValue )
            {
            iScaleY.iValueUpdate = ( iScaleFinalY < iScaleY.iValue ? - KScaleUpdate[ iTileSize ].iHeight : KScaleUpdate[ iTileSize ].iHeight );
            iScaleY.iValueUpdate *= iScaleMod;
            iScaleY.Start();
            }
        }
    if ( iScaleX.Update( aTimeInterval ) )
        {
        //if ( Abs( iScaleX.iValue - iScaleFinalX ) < KScaleUpdate[ iTileSize ].iWidth * iScaleMod )
        if ( ( iScaleX.iValueUpdate > 0 && iScaleX.iValue >= iScaleFinalX ) ||
             ( iScaleX.iValueUpdate < 0 && iScaleX.iValue <= iScaleFinalX ) )
            {
            iScaleX.iValue = iScaleFinalX;
            iScaleX.Pause();
            }
        }
    if ( iScaleY.Update( aTimeInterval ) )
        {
        //if ( Abs( iScaleY.iValue - iScaleFinalY ) < KScaleUpdate[ iTileSize ].iHeight * iScaleMod )
        if ( ( iScaleY.iValueUpdate > 0 && iScaleY.iValue >= iScaleFinalY ) ||
             ( iScaleY.iValueUpdate < 0 && iScaleY.iValue <= iScaleFinalY ) )      
            {
            iScaleY.iValue = iScaleFinalY;
            iScaleY.Pause();
            }
        }
    
    if ( iState == EStarting && iScaleY.iValue == iScaleFinalY && iScaleX.iValue == iScaleFinalX )
    	{
    	if ( iRunTime == 0 )
    		{
    		}
    	iRunTime += aTimeInterval.Int();
    	if ( iRunTime >= KRunTimeMax )
    		{
    		ResetRunState();
    		}
    	}
    
    if ( iUi )
        {
        iUi->Update( aTimeInterval );
        }
    }

void CEightTile::ExternalizeL( RWriteStream& aStream ) const
    {
    aStream.WriteUint32L( 0 ); // size of additional data
    }

void CEightTile::InternalizeL( RReadStream& aStream )
    {
    TUint size = aStream.ReadUint32L();
    aStream.ReadL( size );
    }

