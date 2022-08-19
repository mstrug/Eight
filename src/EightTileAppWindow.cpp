/*
 ============================================================================
 Name		: EightTileAppWindow.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileAppWindow implementation
 ============================================================================
 */

#include "EightTileAppWindow.h"
#include "EightSettings.h"
#include "EightTileUiStatic.h"
//#include "logger.h"



CEightTileAppWindow::CEightTileAppWindow( CEightFormWindow* aWindow, TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi )
    : CEightTile( aId, aTileSize, aUi, EFalse ), iWindow( aWindow )
    {
    }

CEightTileAppWindow::~CEightTileAppWindow()
    {
    delete iWindow;
    }

CEightTileAppWindow* CEightTileAppWindow::NewL( CEightFormWindow* aWindow, TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi )
    {
    CEightTileAppWindow* self = new ( ELeave ) CEightTileAppWindow( aWindow, aId, aTileSize, aUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self;
    return self;
    }

void CEightTileAppWindow::ConstructL()
    {
    BaseConstructL();
    
    if ( iUi )
        {
        }
    }

TEightTileType CEightTileAppWindow::Type() const
    {
    return ETileTypeAppWindow;
    }

TBool CEightTileAppWindow::RunInternal()
    {
	return EFalse;
    }

CEightFormWindow* CEightTileAppWindow::Window() const
    {
    return iWindow;
    }

TBool CEightTileAppWindow::CanIncreaseSize() const
    {
    return EFalse;
    }

