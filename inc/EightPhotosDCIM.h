/*
 ============================================================================
 Name		: EightPhotosDCIM.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightPhotosDCIM declaration
 ============================================================================
 */

#ifndef EIGHTPHOTOSDCIM_H
#define EIGHTPHOTOSDCIM_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <D32DBMS.H> 
#include <SGEUtils/SGEUtilsGPTimer.h>
#include <SGEUtils/SGEUtilsGPTimerObserver.h>


// CLASS DECLARATION

class MEightPhotosDCIMObserver
    {
public:
    virtual void PhotosDCIMUpdateFinished( TInt aError ) = 0;
    };

/**
 *  CEightPhotosDCIM
 * 
 */
class CEightPhotosDCIM : public CBase, MSGEUtilsGPTimerObserver
    {
public:

    ~CEightPhotosDCIM();

    static CEightPhotosDCIM* NewL();

    void GetFileFromDBL( TDes& aFile );

    TBool UpdateDBContentL( MEightPhotosDCIMObserver* aObserver );
    
    TBool UpdateDBPending() const;

    TBool DBOpened() const;
    
private:

    CEightPhotosDCIM( RFs& aFs );

    void ConstructL();
    
    void CreateDBL();

    TBool UpdateDBContentSyncL();

    TBool UpdateDBContentAsyncL();

    void UpdateDBPrepareFoldersList();

    void RecreateDBL();
    
private: // from MSGEUtilsGPTimerObserver
    
    void GPTimerEvent( CSGEUtilsGPTimer* aTimer );
    
private:

    MEightPhotosDCIMObserver* iObserver; // not owned
    
    RDbs iDbs;
    
    RDbNamedDatabase iDb;

    TBool iDbOpened;
    
    TBool iDbUpdatePending;
    
    CSGEUtilsGPTimer* iTimer; // owned

    RBuf iDirs;
    
    RBuf iSql;
    
    TInt iFilesCount;
    
    TInt iCurrentFileIdx;

    TFindFile iFF;
    
    CDir* iDir; // owned
    
    };

#endif // EIGHTPHOTOSDCIM_H
