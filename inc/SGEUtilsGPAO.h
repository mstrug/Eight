/*
 ============================================================================
 Name		: SGEUtilsGPAO.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEUtilsGPAO declaration
 ============================================================================
 */

#ifndef SGEUTILSGPAO_H
#define SGEUTILSGPAO_H

#include <e32base.h>
#include <e32std.h>

class CSGEUtilsGPAO;


class MSGEUtilsGPAO
    {
public:
    virtual void GPAOObserver( CSGEUtilsGPAO* aClient, TInt aError ) = 0;
    virtual void GPAOObserverCancel( CSGEUtilsGPAO* /*aClient*/ ) { };
    };


class CSGEUtilsGPAO : public CActive
    {
public:

    CSGEUtilsGPAO( MSGEUtilsGPAO* aObserver, TInt aPriority = CActive::EPriorityStandard );

    ~CSGEUtilsGPAO();

    void SetObserver( MSGEUtilsGPAO* aObserver );

    void SetActive();

private:

    void RunL();

    void DoCancel();

    TInt RunError( TInt aError );

private:

    MSGEUtilsGPAO* iObserver;
    
    };

#endif // SGEUTILSGPAO_H
