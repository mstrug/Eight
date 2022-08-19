/*
 ============================================================================
 Name		: EightTileUi.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileUi declaration
 ============================================================================
 */

#ifndef EIGHTTILEUI_H
#define EIGHTTILEUI_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGEVg/VgPrimitives.h>

// CLASS DECLARATION
class CEightTile;

/**
 *  CEightTileUi
 * 
 */
class CEightTileUi : public CBase
    {
public:
    
    typedef enum
    {
        ENone,
        EBase,
        EStatic,
        EDynCycle,
        EDynFilp,
        EDynMosaic
    } TEightTileUiType;
    
public:

    ~CEightTileUi();
    
    virtual void SetTile( CEightTile* aOwnerTile );

    virtual TEightTileUiType Type() const;
    
    virtual void SetCrop( TVgRect aRect );
    
    virtual void SetRect( TVgRect aRect );
    
    virtual TBool DrawBackground() const;
    
    virtual void UpdateColorsFromTile();

    virtual void Draw();
    
    virtual void Update( TTimeIntervalMicroSeconds32 aTimeInterval );

    virtual void ExternalizeL( RWriteStream& aStream ) const;

    virtual void InternalizeL( RReadStream& aStream );
    
protected:

    CEightTileUi();

protected:
    
    CEightTile* iTile; // not owned
    
    TVgRect iRect;
    
    TVgRect iCropRect;
    
    };

#endif // EIGHTTILEUI_H
