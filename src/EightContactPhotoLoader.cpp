/*
 ============================================================================
 Name		: EightContactPhotoLoader.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightContactPhotoLoader implementation
 ============================================================================
 */

#include "EightContactPhotoLoader.h"
//#include "logger.h"


CEightContactPhotoLoader::CEightContactPhotoLoader( MEightContactPhotoLoader* aObserver ) : CActive( EPriorityStandard ), iObserver( aObserver )
    {
    }

CEightContactPhotoLoader::~CEightContactPhotoLoader()
    {
    Cancel();
    delete iBitmapItem;
    delete iImageDecoder;
    iBuf.Close();
    iFs.Close();
    }

CEightContactPhotoLoader* CEightContactPhotoLoader::NewL( MEightContactPhotoLoader* aObserver )
    {
    CEightContactPhotoLoader* self = new ( ELeave ) CEightContactPhotoLoader( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self;
    return self;
    }

void CEightContactPhotoLoader::ConstructL()
    {
    User::LeaveIfError( iFs.Connect() );
    CActiveScheduler::Add( this );
    }

TInt CEightContactPhotoLoader::LoadContactL( HBufC8* aBuffer )
    {
    if ( IsActive() )
        {
        return KErrNotReady;
        }
    iBuf.Close();
    iBuf.Create( aBuffer->Des() );
    
    CImageDecoder::TOptions decodingOptions = (CImageDecoder::TOptions) ( CImageDecoder::EOptionNoDither |
                                              CImageDecoder::EPreferFastDecode |
                                              CImageDecoder::EOptionIgnoreExifMetaData |
                                              CImageDecoder::EOptionAlwaysThread );
    
    delete iImageDecoder;
    iImageDecoder = NULL;
    iImageDecoder = CImageDecoder::DataNewL( iFs, iBuf, decodingOptions );
    iImageDecoder->SetDecoderThreadPriority( EPriorityMore );
    //iImageDecoder->SetImageTypeL( )

    delete iBitmapItem;
    iBitmapItem = NULL;
    TFrameInfo fi = iImageDecoder->FrameInfo();
    if ( fi.iFlags & fi.ETransparencyPossible )
        {
        if ( fi.iFlags & fi.EAlphaChannel )
            {
            iBitmapItem = CSGEBitmapItem::NewL( 1, fi.iOverallSizeInPixels, fi.iFrameDisplayMode, EGray256 );
            }
        else
            {
            iBitmapItem = CSGEBitmapItem::NewL( 1, fi.iOverallSizeInPixels, fi.iFrameDisplayMode, EGray2 );
            }
        
        iImageDecoder->Convert( &iStatus, *iBitmapItem->Bitmap(), *iBitmapItem->BitmapMask(), 0 );
        }
    else
        {
        iBitmapItem = CSGEBitmapItem::NewL( 1, fi.iOverallSizeInPixels, fi.iFrameDisplayMode );

        iImageDecoder->Convert( &iStatus, *iBitmapItem->Bitmap(), 0 );
        }
//    Logger::LogStr( _L("c:\\data\\eight.txt"), _L("cnv started"));
    SetActive();
    return KErrNone;
    }

void CEightContactPhotoLoader::DoCancel()
    {
    if ( iImageDecoder )
        {
        iImageDecoder->Cancel();
        }
    }

void CEightContactPhotoLoader::RunL()
    {
//    Logger::LogStrNum( _L("c:\\data\\eight.txt"), _L("runl "), iStatus.Int());
    iObserver->ContactPhotoLoaded( iStatus.Int(), iBitmapItem );
    }

TInt CEightContactPhotoLoader::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }
