/*
 ============================================================================
 Name		: EightMessagingInterface.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightMessagingInterface implementation
 ============================================================================
 */

#include "EightMessagingInterface.h"
#include <senduimtmuids.h>
#include <msvids.h>


static CEightMessagingInterface* self = NULL;



CEightMessagingInterface::CEightMessagingInterface()
    {
    }

CEightMessagingInterface::~CEightMessagingInterface()
    {
    iClients.Close();
    delete iTimer;
    delete iMsvSession;
    }

CEightMessagingInterface* CEightMessagingInterface::GetInstance()
    {
    if ( !self )
        {
        self = new (ELeave) CEightMessagingInterface();
        self->ConstructL();
        }
    return self;
    }

void CEightMessagingInterface::ConstructL()
    {
    iTimer = CSGEUtilsGPTimer::NewL( *this );
    iMsvSession = CMsvSession::OpenAsyncL( *this );
    }

void CEightMessagingInterface::Destroy()
    {
    delete self;
    self = NULL;
    }

void CEightMessagingInterface::RegisterClientL( MEightMessagingInterface* aClient )
    {
    iClients.AppendL( aClient );
    }

TInt CEightMessagingInterface::SmsCount() const
    {
    return iSmsCount;
    }

TInt CEightMessagingInterface::MmsCount() const
    {
    return iMmsCount;
    }

TInt CEightMessagingInterface::EmailCount() const
    {
    return iEmailCount;
    }

void CEightMessagingInterface::UpdateCountersL()
    {
    TInt tmp[] = { iSmsCount, iMmsCount, iEmailCount };
    iSmsCount = iMmsCount = iEmailCount = 0;
    
    CMsvEntry* entry = iMsvSession->GetEntryL( KMsvGlobalInBoxIndexEntryId );
    CleanupStack::PushL( entry );
    if ( entry->Count() )
        {
        CMsvEntrySelection* entries = entry->ChildrenL();
        CleanupStack::PushL( entries );

        for ( TInt i = 0; i < entry->Count(); i++ )
            {
            TMsvEntry msventry = entry->ChildDataL( entries->At( i ) );
            if ( msventry.Unread() )
                {
                switch ( msventry.iMtm.iUid )
                    {
                    case KSenduiMtmSmsUidValue:
                            iSmsCount++;
                        break;
                    case KSenduiMtmMmsUidValue:
                            iMmsCount++;
                        break;
                    case KSenduiMtmPop3UidValue:
                    case KSenduiMtmImap4UidValue:
                            iEmailCount++;
                        break;
                    default:
                        break;
                    }
                }
            }

        CleanupStack::PopAndDestroy( entries );
        }

    CleanupStack::PopAndDestroy( 1 ); //entry
    
    if ( iMmsCount != tmp[ 1 ] || iSmsCount != tmp[ 0 ] )
        {
        for ( TInt i = 0; i < iClients.Count(); i++ ) iClients[ i ]->MessagingNotifySmsMms( iSmsCount, iMmsCount );
        }
    if ( iEmailCount != tmp[ 2 ] )
        {
        for ( TInt i = 0; i < iClients.Count(); i++ ) iClients[ i ]->MessagingNotifyEmail( iEmailCount );
        }
    }

void CEightMessagingInterface::GPTimerEvent( CSGEUtilsGPTimer* /*aTimer*/ )
    {
    TRAP_IGNORE( UpdateCountersL() );
    }

void CEightMessagingInterface::HandleSessionEventL( MMsvSessionObserver::TMsvSessionEvent aEvent, TAny */*aArg1*/, TAny */*aArg2*/, TAny */*aArg3*/ )
    {
    if ( aEvent == EMsvServerReady )
        {
        iMsvSession->SetReceiveEntryEvents( ETrue );
        
        iTimer->Start( 1000000, EFalse );
        }
    else if ( aEvent < 4 )
        {
        if ( !iTimer->IsTimerStarted() )
            {
            UpdateCountersL();
            }
        }
    }
