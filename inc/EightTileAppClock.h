/*
 ============================================================================
 Name		: EightTileAppClock.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileAppClock declaration
 ============================================================================
 */

#ifndef EIGHTTILEAPPCLOCK_H
#define EIGHTTILEAPPCLOCK_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <BACNTF.H> 
#include <ASCliSession.h> 
#include <SGEVg/VgSvgImage.h>
#include <SGEVg/VgPrimitives.h>
#include <SGEUtils/SGEUtilsGPTimer.h>
#include <SGEUtils/SGEUtilsGPTimerObserver.h>
#include <SGEUtilsGPAO.h>
#include "EightTile.h"



// CLASS DECLARATION

/**
 *  CEightTileAppClock
 * 
 */
class CEightTileAppClock : public CEightTile, MSGEUtilsGPTimerObserver, MSGEUtilsGPAO
	{
public:

	~CEightTileAppClock();

	static CEightTileAppClock* NewL( TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi );

    TEightTileType Type() const;

    void SetRect( const TVgRect& aRect );
    
    TBool RunInternal();
    
private:

	CEightTileAppClock( TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi );

	void ConstructL();

    void UpdateTime();

    void UpdateDate();

    void UpdateAlarm();
    
    static TInt CallbackNotifier( TAny* aPtr );
    
private: // from MSGEUtilsGPTimerObserver
    
    void GPTimerEvent( CSGEUtilsGPTimer* aTimer );
    
private: // from MSGEUtilsGPAO
    
    void GPAOObserver( CSGEUtilsGPAO* aClient, TInt aError );
    
private: // data
    
    CSGEUtilsGPTimer* iTimer; // owned
    
    CSGEUtilsGPTimer* iTimerClock; // owned

    CEnvironmentChangeNotifier* iChangeNotifier; // owned
    
    RASCliSession iAlarmSession;
    
    CSGEUtilsGPAO* iGpao; // owned
    
    TAlarmId iChangedAlarmId;
    
    TTime iCurrentTime;
    
    TInt iClockHugeIdx;

    TInt iClockLargeIdx;
    
    TInt iClockAmPmIdx;

    TInt iClockAmPmBigIdx;

    TInt iDateIdx;

    TInt iAlarmIdx;
    
	};

#endif // EIGHTTILEAPPCLOCK_H
