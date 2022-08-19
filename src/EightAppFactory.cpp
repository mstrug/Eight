/*
 ============================================================================
 Name		: EightAppFactory.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightAppFactory implementation
 ============================================================================
 */

#include "EightAppFactory.h"
#include "EightTileManager.h"



CEightAppFactory::CEightAppFactory() : CActive( EPriorityStandard )
    {
    }

CEightAppFactory* CEightAppFactory::NewLC()
    {
    CEightAppFactory* self = new ( ELeave ) CEightAppFactory();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CEightAppFactory* CEightAppFactory::NewL()
    {
    CEightAppFactory* self = CEightAppFactory::NewLC();
    CleanupStack::Pop(); // self;
    return self;
    }

void CEightAppFactory::ConstructL()
    {
    User::LeaveIfError( iLsSession.Connect() );
    User::LeaveIfError( iFs.Connect() );

    User::LeaveIfError( iTimer.CreateLocal() );
    CActiveScheduler::Add( this );
    }

CEightAppFactory::~CEightAppFactory()
    {
    iLsSession.Close();
    iFs.Close();
    Cancel();
    iTimer.Close();
    iUids.Close();
    }

void CEightAppFactory::DoCancel()
    {
    iTimer.Cancel();
    }

void CEightAppFactory::StartL( CEightTileManager* aTileManager )
    {
    iTileManager = aTileManager;
    Cancel();
    iTimer.After( iStatus, 1000 );
    iLsSession.GetAllApps();
    SetActive();
    iType = 1;
    }

void CEightAppFactory::Start2L( CEightTileManager* aTileManager )
    {
    CDir* dir = NULL;
    TChar drives[ 6 ] = { 'c', 'd', 'e', 'f', 'g', 'z' };
    for ( TInt d = 0; d < 6; d++ )
        {
        RBuf paths;
        paths.Create( 256 );
        paths.Append( drives[ d ] );
        paths.Append( _L(":\\private\\10003a3f\\import\\apps\\*.rsc") );
        if ( iFs.GetDir( paths, KEntryAttNormal, ESortByName, dir ) == KErrNone )
            {
            for ( TInt i = 0; i < dir->Count(); i++ )
                {
                RBuf path;
                path.Create( 512 );
                path.Append( drives[ d ] );
                path.Append( _L(":\\private\\10003a3f\\import\\apps\\") );
                path.Append( (*dir)[ i ].iName );
                
                RFile f;
                if ( f.Open( iFs, path, EFileRead ) == KErrNone )
                    {
                    TBuf8<12> b;
                    f.Read( b );
                    f.Close();
                    if ( b.Length() == 12 )
                        {
                        TUid u;
                        u.iUid  = ( b[ 11 ] << 24 ) | ( b[ 10 ] << 16 ) | ( b[ 9 ] << 8 ) | b [ 8 ];
                        iUids.Append( u );
                        }
                    }
                path.Close();
                }
            delete dir;
            }
        paths.Close();
        }
    if ( iUids.Count() > 0 )
        {
        iTileManager = aTileManager;
        Cancel();
        iTimer.After( iStatus, 1000 );
        SetActive();
        iType = 2;
        }
    }

void CEightAppFactory::RunL()
    {
    TApaAppInfo appInfo;
    TInt err = KErrNone;
    if ( iType == 1 )
        {
        err = iLsSession.GetNextApp( appInfo );
        }
    else if ( iType == 2 )
        {
        if ( iUids.Count() > 0 )
            {
            for ( TInt i = 0; i < iUids.Count(); i++ )
                {
                err = iLsSession.GetAppInfo( appInfo, iUids[ 0 ] );
                iUids.Remove( 0 );
                if ( err != KErrNone )
                    {
                    i--;
                    }
                else
                    {
                    break;
                    }
                }
            }
        else
            {
            err = KErrNotFound;
            }
        }
    if ( err == KErrNone )
        {
        TRAP_IGNORE( iTileManager->AddAppTileL( appInfo ) ); 
        iTimer.After( iStatus, 100000 );
        SetActive();
        }
    }

TInt CEightAppFactory::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }


