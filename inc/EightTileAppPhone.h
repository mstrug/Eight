/*
 ============================================================================
 Name		: EightTileAppPhone.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileAppPhone declaration
 ============================================================================
 */

#ifndef EIGHTTILEAPPPHONE_H
#define EIGHTTILEAPPPHONE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <Etel3rdParty.h> 
#include <SGEVg/VgSvgImage.h>
#include <SGEVg/VgPrimitives.h>
#include <SGEUtils/SGEUtilsGPTimer.h>
#include <SGEUtils/SGEUtilsGPTimerObserver.h>
#include "EightTile.h"
#include "EightMissedCallCalc.h"
#include "SGEUtilsGPAO.h"


// CLASS DECLARATION

/**
 *  CEightTileAppPhone
 * 
 */
class CEightTileAppPhone : public CEightTile, MSGEUtilsGPTimerObserver, MSGEUtilsGPAO
    {
public:

    ~CEightTileAppPhone();

    static CEightTileAppPhone* NewL( TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi = NULL );

    TEightTileType Type() const;
    
    TBool CanBeRemoved() const;

    void SetRect( const TVgRect& aRect );
    
    TBool RunInternal();

    void Update( TTimeIntervalMicroSeconds32 aTimeInterval );
    
private:

    CEightTileAppPhone( TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi );

    void ConstructL();

    void UpdateMissedCallsCount();
    
    void StartLogs();

    void StartLogsNew();
    
private: // from MSGEUtilsGPTimerObserver
    
    void GPTimerEvent( CSGEUtilsGPTimer* aTimer );
    
private: // from MSGEUtilsGPAO
    
    void GPAOObserver( CSGEUtilsGPAO* aClient, TInt aError );
    
    void GPAOObserverCancel( CSGEUtilsGPAO* aClient );
    
private: // data
    
    CTelephony *iTelephony; // owned
    
    CSGEUtilsGPTimer* iTimer; // owned
    
    CEightMissedCallCalc* iMissedCallCalc; // owned
    
    CTelephony::TNetworkNameV1 iNetworkName;
    
    CTelephony::TNetworkNameV1Pckg iNetworkNamePckg;
    
    CSGEUtilsGPAO* iGPAO;
    
    TInt iNetworkNameIdx;
    
    TInt iMissedCallsIdx;
    
    TInt iMissedCallsCount;
    
    TBool iShowNetworkName;
    
    };

#endif // EIGHTTILEAPPPHONE_H
