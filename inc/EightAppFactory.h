/*
 ============================================================================
 Name		: EightAppFactory.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightAppFactory declaration
 ============================================================================
 */

#ifndef EIGHTAPPFACTORY_H
#define EIGHTAPPFACTORY_H

#include <e32base.h>	// For CActive, link against: euser.lib
#include <e32std.h>		// For RTimer, link against: euser.lib
#include <apgcli.h>     // link against apgrfx.lib, apparc.lib

class CEightTileManager;


class CEightAppFactory : public CActive
    {
public:

    ~CEightAppFactory();

    static CEightAppFactory* NewL();

    static CEightAppFactory* NewLC();

public:
    void StartL( CEightTileManager* aTileManager );
    void Start2L( CEightTileManager* aTileManager );

private:

    CEightAppFactory();

    void ConstructL();

private:

    void RunL();

    void DoCancel();

    TInt RunError( TInt aError );

private:
    
    RFs iFs;

    RApaLsSession iLsSession;
    
    RArray<TUid> iUids;

    CEightTileManager* iTileManager;  
    
    RTimer iTimer;
    
    TInt iType;

    };


#endif // EIGHTAPPFACTORY_H
