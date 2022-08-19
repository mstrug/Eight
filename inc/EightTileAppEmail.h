/*
 ============================================================================
 Name		: EightTileAppEmail.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileAppEmail declaration
 ============================================================================
 */

#ifndef EIGHTTILEAPPEMAIL_H
#define EIGHTTILEAPPEMAIL_H

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
 *  CEightTileAppEmail
 * 
 */
class CEightTileAppEmail : public CEightTile, MSGEUtilsGPTimerObserver, MEightMessagingInterface 
    {
public:

    ~CEightTileAppEmail();

    static CEightTileAppEmail* NewL( TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi = NULL );

    TEightTileType Type() const;

    void SetRect( const TVgRect& aRect );
    
    TBool RunInternal();
    
private:

    CEightTileAppEmail( TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi );

    void ConstructL();

    void UpdateRectsL();

private: // from MSGEUtilsGPTimerObserver
    
    void GPTimerEvent( CSGEUtilsGPTimer* aTimer );

private: // from MEightMessagingInterface 

    void MessagingNotifySmsMms( TInt aSmsCount, TInt aMmsCount );
    
    void MessagingNotifyEmail( TInt aEmailCount );
    
private: // data
    
    CSGEUtilsGPTimer* iTimer; // owned
    
    TInt iEMailIdx;
    
    TInt iEmailCount;
    
    };

#endif // EIGHTTILEAPPMESSAGING_H
