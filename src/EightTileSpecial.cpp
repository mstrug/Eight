/*
 ============================================================================
 Name		: EightTileSpecial.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileSpecial implementation
 ============================================================================
 */

#include "EightTileSpecial.h"


CEightTileSpecial::CEightTileSpecial( TEightTileSize aTileSize, TInt aActionId, CVgSvgImage& aImage, CVgSvgImage& aImagePressed ) 
    : CEightTile( KEightTileSpecialIdMask | aActionId, aTileSize, NULL, EFalse ), iImage( aImage ), iImagePressed( aImagePressed ), iActionId( aActionId )
    {
    }

CEightTileSpecial::~CEightTileSpecial()
    {
    }

CEightTileSpecial* CEightTileSpecial::NewL( TEightTileSize aTileSize, TInt aActionId, CVgSvgImage& aImage, CVgSvgImage& aImagePressed )
    {
    CEightTileSpecial* self = new ( ELeave ) CEightTileSpecial( aTileSize, aActionId, aImage, aImagePressed );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self;
    return self;
    }

void CEightTileSpecial::ConstructL()
    {

    }

TEightTileType CEightTileSpecial::Type() const
    {
    return ETileTypeSpecial;
    }

TInt CEightTileSpecial::ActionId() const
    {
    return iActionId;
    }

TBool CEightTileSpecial::CanIncreaseSize() const
	{
	return EFalse;
	}

TBool CEightTileSpecial::CanDecreaseSize() const
	{
	return EFalse;
	}

TBool CEightTileSpecial::CanBeRemoved() const
	{
	return EFalse;
	}

TBool CEightTileSpecial::CanBeMoved() const
	{
	return EFalse;
	}

TBool CEightTileSpecial::CanOpenSettings() const
	{
	return EFalse;
	}

void CEightTileSpecial::Draw()
    {
    VGfloat m[ 9 ];
    vgSeti( VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE );
    vgGetMatrix( m );
    
    TVgSize is = iImage.Size();
    
    TVgPoint c = iRect.Center();
    vgTranslate( c.iX, c.iY );
//    vgRotate( 45 );
//  vgScale( 0.8 * iRect.Width() / is.iWidth, 0.8 * iRect.Height() / is.iHeight );
    vgScale( 1.5, 1.5 );
    vgTranslate( -is.iWidth / 2, -is.iHeight / 2 );
    if ( iState == EPressed || iState == ESelected )
        {
        iImagePressed.Draw( TVgPoint() );
        }
    else
        {
        iImage.Draw( TVgPoint() );
        }
    
    vgLoadMatrix( m );
    }


