/*
 ============================================================================
 Name		: EightTileAppWindow.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileAppWindow declaration
 ============================================================================
 */

#ifndef EIGHTTILEAPPWINDOW_H
#define EIGHTTILEAPPWINDOW_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGEVg/VgSvgImage.h>
#include <SGEVg/VgPrimitives.h>
#include <SGEUtils/SGEUtilsGPTimer.h>
#include <SGEUtils/SGEUtilsGPTimerObserver.h>
#include "EightTile.h"
#include "EightFormWindow.h"


// CLASS DECLARATION

/**
 *  CEightTileAppWindow
 * 
 */
class CEightTileAppWindow : public CEightTile 
    {
public:

    ~CEightTileAppWindow();

    static CEightTileAppWindow* NewL( CEightFormWindow* aWindow, TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi = NULL );

    TEightTileType Type() const;
    
    TBool RunInternal();
    
    CEightFormWindow* Window() const;
    
    TBool CanIncreaseSize() const;
    
private:

    CEightTileAppWindow( CEightFormWindow* aWindow, TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi );

    void ConstructL();
 
private: // data
    
    CEightFormWindow* iWindow; // owned
    
    };

#endif // EIGHTTILEAPPWINDOW_H
