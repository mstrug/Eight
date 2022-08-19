/*
 ============================================================================
 Name		: EightTileAppMessaging.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileAppMessaging declaration
 ============================================================================
 */

#ifndef EIGHTTILEAPPMESSAGING_H
#define EIGHTTILEAPPMESSAGING_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGEVg/VgSvgImage.h>
#include <SGEVg/VgPrimitives.h>
#include <SGEUtils/SGEUtilsGPTimer.h>
#include <SGEUtils/SGEUtilsGPTimerObserver.h>
#include "EightTile.h"
#include "EightMessagingInterface.h"
#include <MSVAPI.H> 


// CLASS DECLARATION

/**
 *  CEightTileAppMessaging
 * 
 */
class CEightTileAppMessaging : public CEightTile, MSGEUtilsGPTimerObserver, MEightMessagingInterface 
    {
public:

    ~CEightTileAppMessaging();

    static CEightTileAppMessaging* NewL( TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi = NULL );

    TEightTileType Type() const;

    void SetRect( const TVgRect& aRect );
    
    TBool RunInternal();
    
private:

    CEightTileAppMessaging( TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi );

    void ConstructL();

    void UpdateRectsL();

private: // from MSGEUtilsGPTimerObserver
    
    void GPTimerEvent( CSGEUtilsGPTimer* aTimer );

private: // from MEightMessagingInterface 
    
    void MessagingNotifySmsMms( TInt aSmsCount, TInt aMmsCount );
    
    void MessagingNotifyEmail( TInt aEmailCount );
    
private: // data
    
    CSGEUtilsGPTimer* iTimer; // owned
    
    TInt iSmsIdx;
    
    TInt iSmsCount;
    
    };

#endif // EIGHTTILEAPPMESSAGING_H
