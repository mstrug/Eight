/*
 ============================================================================
 Name		: EightTileAppCalendar.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileAppCalendar declaration
 ============================================================================
 */

#ifndef EIGHTTILEAPPCALENDAR_H
#define EIGHTTILEAPPCALENDAR_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <BACNTF.H> 
#include <calsession.h> 
#include <calentryview.h>
#include <calinstanceview.h> 
#include <calinstance.h> 
#include <SGEVg/VgSvgImage.h>
#include <SGEVg/VgPrimitives.h>
#include <SGEUtils/SGEUtilsGPTimer.h>
#include <SGEUtils/SGEUtilsGPTimerObserver.h>
#include "EightTile.h"
#include "EightPhotosProvider.h"
#include "SGEUtilsGPAO.h"


// CLASS DECLARATION

/**
 *  CEightTileAppCalendar
 * 
 */
class CEightTileAppCalendar : public CEightTile, MSGEUtilsGPTimerObserver, MCalChangeCallBack2, MEightPhotosProviderFontObserver
    {
public:
    
    ~CEightTileAppCalendar();

    static CEightTileAppCalendar* NewL( TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi = NULL );

    TEightTileType Type() const;

    void SetRect( const TVgRect& aRect );

    TBool RunInternal();
    
private:

    CEightTileAppCalendar( TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi );

    void ConstructL();

    void UpdateDate();
    
    void UpdateEvents();
    
    static TInt CallbackNotifier( TAny* aPtr );
    
private: // from MSGEUtilsGPTimerObserver
    
    void GPTimerEvent( CSGEUtilsGPTimer* aTimer );

private: // from MCalChangeCallBack2
    
    void CalChangeNotification(RArray<TCalChangeEntry>& aChangeItems);
    
private: // from MEightPhotosProviderFontObserver
    
    void FontsLoaded();
    
private: // data
    
    CSGEUtilsGPTimer* iTimer; // owned

    CCalSession* iCalSession; // owned
    
    CCalChangeNotificationFilter* iCalFilter; // owned

    CEnvironmentChangeNotifier* iChangeNotifier; // owned
    
    TInt iNameIdx;
    
    TInt iDayIdx;
    
    TInt iDayNameIdx;

    TInt iEventTitleIdx;
    TInt iEventLine2Idx;
    TInt iEventLine3Idx;
    
    };

#endif // EIGHTTILEAPPCALENDAR_H
