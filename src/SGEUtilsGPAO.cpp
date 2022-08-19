/*
 ============================================================================
 Name		: SGEUtilsGPAO.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEUtilsGPAO implementation
 ============================================================================
 */

#include "SGEUtilsGPAO.h"


CSGEUtilsGPAO::CSGEUtilsGPAO( MSGEUtilsGPAO* aObserver, TInt aPriority ) : CActive( aPriority ), iObserver( aObserver )
    {
    CActiveScheduler::Add( this );
    }

CSGEUtilsGPAO::~CSGEUtilsGPAO()
    {
    Cancel();
    }

void CSGEUtilsGPAO::DoCancel()
    {
    if ( iObserver )
        {
        iObserver->GPAOObserverCancel( this );
        }
    }

void CSGEUtilsGPAO::SetObserver( MSGEUtilsGPAO* aObserver )
    {
    iObserver = aObserver;
    }

void CSGEUtilsGPAO::SetActive()
    {
    CActive::SetActive();
    }

void CSGEUtilsGPAO::RunL()
    {
    if ( iObserver )
        {
        iObserver->GPAOObserver( this, iStatus.Int() );
        }
    }

TInt CSGEUtilsGPAO::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }
