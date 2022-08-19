/*
 ============================================================================
 Name		: EightWindowHelp.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightWindowHelp declaration
 ============================================================================
 */

#ifndef EIGHTWINDOWTILEHELP_H
#define EIGHTWINDOWTILEHELP_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "EightFormWindow.h"
#include "EightTile.h"

// CLASS DECLARATION

const TInt KEightWindowHelpId = 0x1024;

/**
 *  CEightWindowHelp
 * 
 */
class CEightWindowHelp : public CEightFormWindow
    {
public:

    ~CEightWindowHelp();

    static CEightWindowHelp* NewL( TSize aSize );

private:

    CEightWindowHelp( TSize aSize );

    void ConstructL();

    void WindowClosedByGesture();

private:
    
    };

#endif // EIGHTWINDOWTILEHELP_H
