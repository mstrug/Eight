/*
 ============================================================================
 Name		: EightTileAppShortcut.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileAppShortcut declaration
 ============================================================================
 */

#ifndef EIGHTTILEAPPSHORTCUT_H
#define EIGHTTILEAPPSHORTCUT_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGEVg/VgSvgImage.h>
#include <SGEVg/VgPrimitives.h>
#include <SGEUtils/SGEUtilsGPTimer.h>
#include <SGEUtils/SGEUtilsGPTimerObserver.h>
#include "EightTile.h"

// CLASS DECLARATION

/**
 *  CEightTileAppShortcut
 * 
 */
class CEightTileAppShortcut : public CEightTile, MSGEUtilsGPTimerObserver
    {
public:

    ~CEightTileAppShortcut();

    static CEightTileAppShortcut* NewL( const TDesC& aExeName, TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi = NULL );

    TEightTileType Type() const;

    void SetRect( const TVgRect& aRect );
    
    TBool RunInternal();
    
    void SetExeName2L( const TDesC& aExeName );
    
    void EnableRunTwoTimes();
    
    void SetCanBeRemoved( TBool aValue );
    
    TBool CanBeRemoved() const;
    
protected:

    CEightTileAppShortcut( TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi );

    void ConstructL( const TDesC& aExeName );

private: // from MSGEUtilsGPTimerObserver
    
    void GPTimerEvent( CSGEUtilsGPTimer* aTimer );
    
private: // data
    
    RBuf iExeName;
    
    RBuf iExeName2;

    CSGEUtilsGPTimer* iTimer; // owned
    
    TBool iRunTwoTimes;
    
    TBool iSecondRun;
    
    TBool iCanBeRemoved;

    TBool iCanBeRemovedSet;
    
    };

#endif // EIGHTTILEAPPSHORTCUT_H
