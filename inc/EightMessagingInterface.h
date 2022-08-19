/*
 ============================================================================
 Name		: EightMessagingInterface.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightMessagingInterface declaration
 ============================================================================
 */

#ifndef EIGHTMESSAGINGINTERFACE_H
#define EIGHTMESSAGINGINTERFACE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <MSVAPI.H> 
#include <SGEUtils/SGEUtilsGPTimer.h>
#include <SGEUtils/SGEUtilsGPTimerObserver.h>

// CLASS DECLARATION

class MEightMessagingInterface
    {
public:
    virtual void MessagingNotifySmsMms( TInt aSmsCount, TInt aMmsCount ) = 0;
    virtual void MessagingNotifyEmail( TInt aEmailCount ) = 0;
    };

/**
 *  CEightMessagingInterface
 * 
 */
class CEightMessagingInterface : public CBase, MSGEUtilsGPTimerObserver, MMsvSessionObserver 
    {
public:

    static CEightMessagingInterface* GetInstance();

    void Destroy();
    
    void RegisterClientL( MEightMessagingInterface* aClient );

    TInt SmsCount() const;
    
    TInt MmsCount() const;
    
    TInt EmailCount() const;
        
private:

    CEightMessagingInterface();

    ~CEightMessagingInterface();

    void ConstructL();

    void UpdateCountersL();
    
private: // from MSGEUtilsGPTimerObserver
    
    void GPTimerEvent( CSGEUtilsGPTimer* aTimer );

private: // from MMsvSessionObserver 
    
    void HandleSessionEventL( MMsvSessionObserver::TMsvSessionEvent aEvent, TAny *aArg1, TAny *aArg2, TAny *aArg3 );
    
private: // data
    
    CSGEUtilsGPTimer* iTimer; // owned
    
    CMsvSession* iMsvSession; // owned
    
    RPointerArray< MEightMessagingInterface > iClients;
    
    TInt iSmsCount;
    
    TInt iMmsCount;
    
    TInt iEmailCount;
    
    };

#endif // EIGHTMESSAGINGINTERFACE_H
