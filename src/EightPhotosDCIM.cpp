/*
 ============================================================================
 Name		: EightPhotosDCIM.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightPhotosDCIM implementation
 ============================================================================
 */

#include "EightPhotosDCIM.h"
#include "EightSettings.h"
#include <bautils.h>
#include <e32math.h>

_LIT( KGalleryDbFileName, "gal.db" );



CEightPhotosDCIM::CEightPhotosDCIM( RFs& aFs ) : iFF( aFs )
    {
    }

CEightPhotosDCIM::~CEightPhotosDCIM()
    {
    iDirs.Close();
    iSql.Close();
    delete iTimer;
    iDb.Close();
    iDbs.Close();
    delete iDir;
    }

CEightPhotosDCIM* CEightPhotosDCIM::NewL()
    {
    CEightPhotosDCIM* self = new ( ELeave ) CEightPhotosDCIM( CEightSettings::GetInstance()->FileProvider().Fs() );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self;
    return self;
    }

void CEightPhotosDCIM::ConstructL()
    {
    iTimer = CSGEUtilsGPTimer::NewL( *this );
    
    CreateDBL();
    }


void CEightPhotosDCIM::CreateDBL()
    {
    CSGEFileProvider& fp = CEightSettings::GetInstance()->FileProvider();
    iDbOpened = EFalse;

    iDbs.Connect();

    TFileName& fn = fp.AppendToPrivateFolder( KGalleryDbFileName() );
    
    if ( iDb.Open( fp.Fs(), fn ) == KErrNone )
        {
        iDbOpened = ETrue;
        }
    else
        {
        //UpdateDBContentL( NULL );
        iDbOpened = EFalse;
        }
    }


TBool CEightPhotosDCIM::UpdateDBContentL( MEightPhotosDCIMObserver* aObserver )
    {
    if ( iDbUpdatePending ) return ETrue;
    
    iObserver = aObserver;
    
    RecreateDBL();
    UpdateDBPrepareFoldersList();

    //UpdateDBContentSyncL();
    return UpdateDBContentAsyncL();
    }

TBool CEightPhotosDCIM::UpdateDBPending() const
    {
    return iDbUpdatePending;
    }

TBool CEightPhotosDCIM::DBOpened() const
    {
    return iDbOpened;
    }

void CEightPhotosDCIM::RecreateDBL()
    {
    CSGEFileProvider& fp = CEightSettings::GetInstance()->FileProvider();
    TFileName& fn = fp.AppendToPrivateFolder( KGalleryDbFileName() );

    RFs fs = fp.Fs();

    iDb.Close();
    if ( iDb.Replace( fp.Fs(), fn ) != KErrNone )
        {
        iDbOpened = EFalse;
        return;
        }
    CDbColSet* colSet = CDbColSet::NewLC();
    colSet->AddL( TDbCol( _L("n"), EDbColUint32 ) );
    colSet->AddL( TDbCol( _L("f"), EDbColText8 ) );
    if ( iDb.CreateTable( _L("pf"), *colSet ) == KErrNone )
        {
        CDbKey *key = CDbKey::NewLC();
        key->AddL( TDbKeyCol( _L("n") ) );
        key->MakeUnique();
        TInt eidx = iDb.CreateIndex( _L("idxName"), _L("pf"), *key );
        CleanupStack::PopAndDestroy( key );
        iDbOpened = ETrue;
        }
    else
        {
        CleanupStack::PopAndDestroy( colSet );
        iDbOpened = EFalse;
        return;
        }
    CleanupStack::PopAndDestroy( colSet );
    }

void CEightPhotosDCIM::UpdateDBPrepareFoldersList()
    {
    CSGEFileProvider& fp = CEightSettings::GetInstance()->FileProvider();
    RFs fs = fp.Fs();
    
    CDir *dirCEF[ 3 ] = { 0 };
    TInt err1 = fs.GetDir( _L("c:\\DCIM\\"), KEntryAttMatchExclusive | KEntryAttDir, EAscending, dirCEF[ 0 ] );
    TInt err2 = fs.GetDir( _L("e:\\DCIM\\"), KEntryAttMatchExclusive | KEntryAttDir, EAscending, dirCEF[ 1 ] );
    TInt err3 = fs.GetDir( _L("f:\\DCIM\\"), KEntryAttMatchExclusive | KEntryAttDir, EAscending, dirCEF[ 2 ] );
    if ( ( err1 == KErrNone || err2 == KErrNone || err3 == KErrNone ) && ( dirCEF[ 0 ] || dirCEF[ 1 ] || dirCEF[ 2 ] ) )
        {
        TInt s = 3 * ( 100 + ( dirCEF[ 0 ] ? dirCEF[ 0 ]->Count() : 0 ) + ( dirCEF[ 1 ] ? dirCEF[ 1 ]->Count() : 0 ) + ( dirCEF[ 2 ] ? dirCEF[ 2 ]->Count() : 0 ) ) * 255; // + aSi->iGallerySearchList.Length();
        iDirs.Close();
        iDirs.CreateL( s );
        iDirs.Append( _L("c:\\DCIM\\;e:\\DCIM\\;f:\\DCIM\\;") );
        for ( TInt d = 0; d < 3; d++ )
            {
            CDir *dir = dirCEF[ d ];
            if ( !dir ) continue;
            for ( TInt i = 0; i < dir->Count(); i++ )
                {
                TChar drive = ( d == 0 ? 'c' : ( d == 1 ? 'e' : 'f' ) );
                iDirs.Append( drive );
                iDirs.Append( _L(":\\DCIM\\") );
                iDirs.Append( (*dir)[ i ].iName );
                iDirs.Append( _L("\\;") );
                
                RBuf subdir;
                CDir *sdir = NULL;
                subdir.Create( 12 + (*dir)[ i ].iName.Length() );
                subdir.Append( drive );
                subdir.Append( _L(":\\DCIM\\") );
                subdir.Append( (*dir)[ i ].iName );
                subdir.Append( _L("\\") );
                TInt err = fs.GetDir( subdir, KEntryAttMatchExclusive | KEntryAttDir, EAscending, sdir );
                if ( err == KErrNone && sdir )
                    {
                    for ( TInt i = 0; i < sdir->Count(); i++ )
                        {
                        iDirs.Append( subdir );
                        iDirs.Append( (*sdir)[ i ].iName );
                        iDirs.Append( _L("\\;") );
                        }
                    delete sdir;
                    sdir = NULL;
                    }            
                subdir.Close();
                }
            }
        
        iDirs.ReAllocL( iDirs.Length() + 1 );
        delete dirCEF[ 0 ];
        delete dirCEF[ 1 ];
        delete dirCEF[ 2 ];
        }
    }

TBool CEightPhotosDCIM::UpdateDBContentSyncL()
    {
    CSGEFileProvider& fp = CEightSettings::GetInstance()->FileProvider();
    RFs fs = fp.Fs();
    
    iFilesCount = 0;

    TFindFile ff( fs );
    CDir *dir = NULL;
    TInt err = ff.FindWildByPath( _L("*.jpg"), &iDirs, dir );
    RBuf sql;
    sql.CreateL( 300 );
    while ( err == KErrNone )
        {
        if ( dir )
            {
            for ( TInt i = 0; i < dir->Count(); i++ )
                {
                if ( !(*dir)[ i ].IsDir() )
                    {
                    TParse parse;
                    parse.Set( ff.File(), NULL, NULL );
                    
                    sql.Copy( _L("INSERT INTO pf VALUES( ") );
                    sql.AppendNum( iFilesCount++ );
                    sql.Append( _L(" , '") );
                    sql.Append( parse.DriveAndPath() );
                    sql.Append( (*dir)[ i ].iName );
                    sql.Append( _L("' )") );
                    TInt err = iDb.Execute( sql );
                    if ( err != 1 )
                        {
                        // error
                        }
                    }
                }
            delete dir;
            dir = NULL;
            }
        err = ff.FindWild( dir );
        }
    sql.Close();
    iDirs.Close();
    delete dir;
    dir = NULL;
    
    return EFalse;
    }

TBool CEightPhotosDCIM::UpdateDBContentAsyncL()
    {
    CSGEFileProvider& fp = CEightSettings::GetInstance()->FileProvider();
    RFs fs = fp.Fs();
    
    iFilesCount = 0;
    iCurrentFileIdx = 0;

    delete iDir;
    iDir = NULL;
    TInt err = iFF.FindWildByPath( _L("*.jpg"), &iDirs, iDir );
    if ( err == KErrNone && iDir )
        {
        iSql.Close();
        iSql.CreateL( 300 );
        iDbUpdatePending = ETrue;
        iTimer->Start( 300 );
        return ETrue;
        }
    else
        {
        iDbUpdatePending = EFalse;
        delete iDir;
        iDir = NULL;
        iDirs.Close();
        return EFalse;
        }
    }


void CEightPhotosDCIM::GPTimerEvent( CSGEUtilsGPTimer* /*aTimer*/ )
    {
    for ( TInt i = 0; iCurrentFileIdx < iDir->Count(); iCurrentFileIdx++, i++ )
        {
        if ( !(*iDir)[ iCurrentFileIdx ].IsDir() )
            {
            TParse parse;
            parse.Set( iFF.File(), NULL, NULL );
            
            iSql.Copy( _L("INSERT INTO pf VALUES( ") );
            iSql.AppendNum( iFilesCount++ );
            iSql.Append( _L(" , '") );
            iSql.Append( parse.DriveAndPath() );
            iSql.Append( (*iDir)[ iCurrentFileIdx ].iName );
            iSql.Append( _L("' )") );
            TInt err = iDb.Execute( iSql );
            if ( err != 1 )
                {
                // error
                }
            }
        
        if ( i == 15 )
            {
            iTimer->Start( 250 );
            return;
            }
        }
    
    iCurrentFileIdx = 0;
    delete iDir;
    iDir = NULL;
    TInt err = iFF.FindWild( iDir );
    if ( err == KErrNone && iDir )
        {
        iTimer->Start( 250 );
        }
    else
        {
        delete iDir;
        iDir = NULL;
        iDbUpdatePending = EFalse;
        iDirs.Close();
        iSql.Close();
        
        if ( iObserver )
            {
            iObserver->PhotosDCIMUpdateFinished( err );
            }
        }
    }


void CEightPhotosDCIM::GetFileFromDBL( TDes& aFile )
    {
    if ( !iDbOpened )
        {
        aFile.Zero();
        return;
        }
    
    RDbTable table;
    if ( table.Open( iDb, _L("pf"), RDbTable::EUpdatable ) == KErrNone )
        {
        TInt cnt = table.CountL( RDbTable::EQuick );
        if ( cnt == 0 ) return;
        TUint32 idx = Math::Random() % cnt;

        TInt err = table.SetIndex( _L( "idxName" ) );
        TBool found = EFalse;
        if ( err == KErrNone )
            {
            found = table.SeekL( TDbSeekKey( (TUint)idx ) );
            }
        if ( !found )
            {
            TInt i = 0;
            table.FirstL();
            while ( i++ < idx && table.NextL() ) ;
            if ( !table.AtRow() )
                {
                table.FirstL();
                }
            }
        table.GetL();
        aFile.Zero();
        TPtrC8 data = table.ColDes8( 2 );
        aFile.Copy( data );
        table.Close();
        }
    }
