/*
 ============================================================================
 Name		: EightPalette.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : TEightPalette declaration
 ============================================================================
 */

#ifndef EIGHTPALETTE_H
#define EIGHTPALETTE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <sgevg/VgPrimitives.h>

// CLASS DECLARATION

const TInt KEightColorsCount = 21;
const TInt KEightColorsDefaultIdx = 20;

const TUint32 KEightColorsBase[ KEightColorsCount ] = { 0xA4C400FF, 0x60A917FF, 0x008A00FF, 0x00ABA9FF, 0x1BA1E2FF, 0x0050EFFF, 0x6A00FFFF,
                                                        0xAA00FFFF, 0xF472D0FF, 0xD80073FF, 0xA20025FF, 0xE51400FF, 0xFA6800FF, 0xF0A30AFF,
                                                        0xD8C100FF, 0x825A2CFF, 0x6D8764FF, 0x647687FF, 0x76608AFF, 0x7A3B3FFF, 0xBF6200FF };

const TUint32 KEightColorsPressed[ KEightColorsCount ] = { 0xA4C400FF, 0x60A917FF, 0x008A00FF, 0x00ABA9FF, 0x1BA1E2FF, 0x0050EFFF, 0x6A00FFFF,
                                                           0xAA00FFFF, 0xF472D0FF, 0xD80073FF, 0xA20025FF, 0xE51400FF, 0xFA6800FF, 0xF0A30AFF,
                                                           0xD8C100FF, 0x825A2CFF, 0x6D8764FF, 0x647687FF, 0x76608AFF, 0x7A3B3FFF, 0xBF6200FF };

const TUint32 KEightColorsSelected[ KEightColorsCount ] = { 0xd5ff00FF, 0x8ef12bFF, 0x0aef0aFF, 0x00fffcFF, 0x62ccffFF, 0x229bffFF, 0xad72ffFF,
                                                            0xd787ffFF, 0xff97e2FF, 0xff4aaaFF, 0xff003aFF, 0xff5342FF, 0xff8d3cFF, 0xffc34dFF,
                                                            0xffe400FF, 0xd09450FF, 0x9fc193FF, 0x99b1c7FF, 0xae92c7FF, 0xc1676dFF, 0xFFA800ff };

const TUint32 KEightColorsHigh[ KEightColorsCount ] = { 0xcdeb35FF, 0x94d652FF, 0x55cf55FF, 0x41e9e7FF, 0x5fbeecFF, 0x6699ffFF, 0x8f3fffFF,
                                                        0xce6cffFF, 0xf2a2dcFF, 0xfe67b8FF, 0xd50031FF, 0xf54e3eFF, 0xfe8935FF, 0xffc656FF,
                                                        0xf2de35FF, 0xba8d5aFF, 0x8ea287FF, 0x96a1abFF, 0x9a8ca6FF, 0xa2676aFF, 0xEE9233FF };

const TUint32 KEightColorsLow[ KEightColorsCount ] = { 0x8ca50cFF, 0x589020FF, 0x196819FF, 0x239695FF, 0x1e84b5FF, 0x1b4dafFF, 0x540cbaFF,
                                                       0x7912acFF, 0xdc4fb5FF, 0xac1b68FF, 0x80001dFF, 0xb51c0dFF, 0xd6610eFF, 0xca8b0eFF,
                                                       0xb4a31bFF, 0x5d4224FF, 0x5a6e54FF, 0x53606cFF, 0x584a65FF, 0x5d3235FF, 0xA2590EFF };


/**
 *  CEightPalette
 * 
 */
class TEightPalette
    {
public:
    
    TEightPalette();

    TEightPalette( TInt aColorIdx );

    void SetColor( TInt aColorIdx );

    virtual void ExternalizeL( RWriteStream& aStream ) const;

    virtual void InternalizeL( RReadStream& aStream );
    
public:
    
    TVgColor iTileColor;
    
    TVgColor iTilePressedColor;
    
    TVgColor iTileSelectedColor;
    
    TVgColor iTileMovePlaceholderColor;

    TVgColor iTilesFadeColor;
    
    TVgColor iTileColorHigh;

    TVgColor iTileColorLow;

    VGfloat iTileImageColorAlpha;

    TVgColor iMsgBoxBackgroundColor;

    TVgColor iMsgBoxButtonColor;

    TVgColor iFormWindowBackgroundColor;
    
    TVgColor iRadioButtonColor;

    };

#endif // EIGHTPALETTE_H
