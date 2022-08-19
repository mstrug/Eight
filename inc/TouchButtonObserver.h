/*
 ============================================================================
 Name		: TouchButtonObserver.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CTouchMenuItemBase declaration
 ============================================================================
 */

#ifndef TOUCHBUTTONOBSERVER_H
#define TOUCHBUTTONOBSERVER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>


// CLASS DECLARATION
class MTouchButtonObserver
    {
public:
    virtual void TouchButtonPressed( TUint aId ) = 0;
    };

#endif // TOUCHBUTTONOBSERVER_H
