/*
 ============================================================================
 Name		: EightTileUi.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileUi implementation
 ============================================================================
 */

#include "EightTileUi.h"
#include "EightTile.h"


CEightTileUi::CEightTileUi()
    {
    }

CEightTileUi::~CEightTileUi()
    {
    }

void CEightTileUi::SetTile( CEightTile* aOwnerTile )
    {
    iTile = aOwnerTile;
    UpdateColorsFromTile();
    }

CEightTileUi::TEightTileUiType CEightTileUi::Type() const
    {
    return EBase;
    }

void CEightTileUi::SetCrop( TVgRect aRect )
    {
    iCropRect = aRect;
    }

void CEightTileUi::SetRect( TVgRect aRect )
    {
    iRect = aRect;
    }

TBool CEightTileUi::DrawBackground() const
    {
    return ETrue;
    }

void CEightTileUi::UpdateColorsFromTile()
    {
    }

void CEightTileUi::Draw()
    {
    }

void CEightTileUi::Update( TTimeIntervalMicroSeconds32 /*aTimeInterval*/ )
    {
    }

void CEightTileUi::ExternalizeL( RWriteStream& aStream ) const
    {
    aStream.WriteUint32L( 0 ); // size of additional data
    }

void CEightTileUi::InternalizeL( RReadStream& aStream )
    {
    TUint size = aStream.ReadUint32L();
    aStream.ReadL( size );
    }
