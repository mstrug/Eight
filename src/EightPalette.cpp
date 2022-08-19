/*
 ============================================================================
 Name		: EightPalette.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : TEightPalette implementation
 ============================================================================
 */

#include "EightPalette.h"


const TUint32 KStreamId = ( '8' | ( 'P' << 8 ) | ( 'A' << 16 ) | ( 'L' << 24 ) );
const TUint16 KStreamVersion = 0x0001;


TEightPalette::TEightPalette() : iTileColor( 0xbf6200ff ), iTilePressedColor( 0xbf6200ff ), iTileSelectedColor( 0xffa800ff ), 
        iTileMovePlaceholderColor( 0xFFFFFF60 ), iTilesFadeColor( 0x00000080 ), iTileColorHigh( 0xee9233ff ), 
        iTileColorLow( 0xa2590eff ), iTileImageColorAlpha( 0.4 ), 
        iMsgBoxBackgroundColor( 0x1b1b1bFF ), iMsgBoxButtonColor( KVgColorWhite ),
        iFormWindowBackgroundColor( KVgColorBlack ), iRadioButtonColor( 0xBFBFBFFF )
    {
    }

TEightPalette::TEightPalette( TInt aColorIdx ) 
    : iTileColor( KEightColorsBase[ aColorIdx ] ), 
      iTilePressedColor( KEightColorsPressed[ aColorIdx ] ), 
      iTileSelectedColor( KEightColorsSelected[ aColorIdx ] ), 
      iTileMovePlaceholderColor( 0xFFFFFF60 ), 
      iTilesFadeColor( 0x00000080 ), 
      iTileColorHigh( KEightColorsHigh[ aColorIdx ] ), 
      iTileColorLow( KEightColorsLow[ aColorIdx ] ), 
      iTileImageColorAlpha( 0.4 ), 
      iMsgBoxBackgroundColor( 0x1b1b1bFF ), 
      iMsgBoxButtonColor( KVgColorWhite ),
      iFormWindowBackgroundColor( KVgColorBlack ), 
      iRadioButtonColor( 0xBFBFBFFF )
    {
    }

void TEightPalette::SetColor( TInt aColorIdx )
    {
    iTileColor = TVgColor( KEightColorsBase[ aColorIdx ] );
    iTilePressedColor = TVgColor( KEightColorsPressed[ aColorIdx ] );
    iTileSelectedColor = TVgColor( KEightColorsSelected[ aColorIdx ] ); 
    iTileColorHigh = TVgColor( KEightColorsHigh[ aColorIdx ] );
    iTileColorLow = TVgColor( KEightColorsLow[ aColorIdx ] );
    }

void TEightPalette::ExternalizeL( RWriteStream& aStream ) const
    {
    aStream.WriteUint32L( KStreamId );
    aStream.WriteUint16L( KStreamVersion );
    
    aStream.WriteUint32L( iTileColor.AsUint32() );
    aStream.WriteUint32L( iTilePressedColor.AsUint32() );
    aStream.WriteUint32L( iTileSelectedColor.AsUint32() );
    aStream.WriteUint32L( iTileMovePlaceholderColor.AsUint32() );
    aStream.WriteUint32L( iTilesFadeColor.AsUint32() );
    aStream.WriteUint32L( iTileColorHigh.AsUint32() );
    aStream.WriteUint32L( iTileColorLow.AsUint32() );
    aStream.WriteReal64L( iTileImageColorAlpha );
    aStream.WriteUint32L( iMsgBoxBackgroundColor.AsUint32() );
    aStream.WriteUint32L( iMsgBoxButtonColor.AsUint32() );
    aStream.WriteUint32L( iFormWindowBackgroundColor.AsUint32() );
    aStream.WriteUint32L( iRadioButtonColor.AsUint32() );
    }

void TEightPalette::InternalizeL( RReadStream& aStream )
    {
    if ( aStream.ReadUint32L() != KStreamId ) return;
    if ( aStream.ReadUint16L() != KStreamVersion ) return;
    
    iTileColor = TVgColor( aStream.ReadUint32L() );
    iTilePressedColor = TVgColor( aStream.ReadUint32L() );
    iTileSelectedColor = TVgColor( aStream.ReadUint32L() );
    iTileMovePlaceholderColor = TVgColor( aStream.ReadUint32L() );
    iTilesFadeColor = TVgColor( aStream.ReadUint32L() );
    iTileColorHigh = TVgColor( aStream.ReadUint32L() );
    iTileColorLow = TVgColor( aStream.ReadUint32L() );
    iTileImageColorAlpha = aStream.ReadReal64L();
    iMsgBoxBackgroundColor = TVgColor( aStream.ReadUint32L() );
    iMsgBoxButtonColor = TVgColor( aStream.ReadUint32L() );
    iFormWindowBackgroundColor = TVgColor( aStream.ReadUint32L() );
    iRadioButtonColor = TVgColor( aStream.ReadUint32L() );
    }

