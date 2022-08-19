/*
 ============================================================================
 Name		: EightTileSpecial.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileSpecial declaration
 ============================================================================
 */

#ifndef EIGHTTILESPECIAL_H
#define EIGHTTILESPECIAL_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGEVg/VgSvgImage.h>
#include <SGEVg/VgPrimitives.h>
#include "EightTile.h"


const TInt KEightTileSpecialIdMask = 0xf000;
// CLASS DECLARATION

/**
 *  CEightTileSpecial
 * 
 */
class CEightTileSpecial : public CEightTile
    {
public:

    ~CEightTileSpecial();

    static CEightTileSpecial* NewL( TEightTileSize aTileSize, TInt aActionId, CVgSvgImage& aImage, CVgSvgImage& aImagePressed );
    
    TEightTileType Type() const;

    TInt ActionId() const;
    
    TBool CanIncreaseSize() const;
    
    TBool CanDecreaseSize() const;
    
    TBool CanBeRemoved() const;
    
    TBool CanBeMoved() const;
    
    TBool CanOpenSettings() const;
    
    void Draw();
    
private:

    CEightTileSpecial( TEightTileSize aTileSize, TInt aActionId, CVgSvgImage& aImage, CVgSvgImage& aImagePressed );

    void ConstructL();
    
private:
    
    CVgSvgImage& iImage;
    
    CVgSvgImage& iImagePressed;
    
    TInt iActionId;
    
    };

#endif // EIGHTTILESPECIAL_H
