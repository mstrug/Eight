/*
 ============================================================================
 Name		: EightPhotosProvider.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightPhotosProvider implementation
 ============================================================================
 */

#include "EightPhotosProvider.h"
#include "EightContactPhotoLoader.h"
#include "EightSettings.h"
#include "EightFontDef.h"
#include "EightCompilationSettings.h"
//#include "Logger.h"
#include <w32std.h>
#include <imageconversion.h>
#include <SGEBitmap/SGEBitmapUtils.h>
#include <SGEFile/SGEFile.h>
#include <bautils.h>


static CEightPhotosProvider* self = NULL;



CEightPhotosProvider::CEightPhotosProvider() : iHighFpsIdle( ETrue )
	{
	}

CEightPhotosProvider::~CEightPhotosProvider()
	{
    delete iPhotosDCIM;
    iPendingRequests.Close();
    delete iTimer;
    delete iContactsDb;
	delete iImageManager;
	iClients.ResetAndDestroy();
	delete iContactLoader;
	
	RHashMap< TUint, TSourceInfo >::TIter iter( iRegisteredSources );
	iter.Reset();
	while ( iter.NextKey() )
	    {
	    iter.CurrentValue()->iGallerySearchList.Close();
	    }
	iRegisteredSources.Close();
	
    for ( TInt i = 0; i < iImageFiles.Count(); i++ )
        {
        iImageFiles[ i ].iFileName.Close();
        }
	iImageFiles.Close();
	
	iFontLoadClients.Close();
	}

CEightPhotosProvider* CEightPhotosProvider::GetInstanceL()
	{
    if ( !self )
        {
        self = CEightPhotosProvider::NewL();
        }
    return self;
	}

void CEightPhotosProvider::Destroy()
    {
    delete self;
    self = NULL;
    }

CEightPhotosProvider* CEightPhotosProvider::NewL()
	{
	CEightPhotosProvider* self = new (ELeave) CEightPhotosProvider();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); // self;
	return self;
	}

void CEightPhotosProvider::ConstructL()
	{
    iTimer = CSGEUtilsGPTimer::NewL( *this );
    
	iImageManager = CVgImageManager::NewL( this );
	CSGEFile& sf = CEightSettings::GetInstance()->FileProvider().GetSGEFileL( 1 );
    iImageManager->AddImageToLoadL( sf, _L("\\font\\sge_font_2_gray.png"), KFont1image, VG_IMAGE_QUALITY_BETTER );
    iImageManager->AddImageToLoadL( sf, _L("\\font\\font.png"), KFont2image, VG_IMAGE_QUALITY_BETTER );
    iImageManager->AddImageToLoadL( sf, _L("\\img\\about_logo.png"), KLogoImage, VG_IMAGE_QUALITY_BETTER );
#ifdef EIGHT_PREMIUM
    iImageManager->AddImageToLoadL( sf, _L("\\img\\icon.png"), KIconImage, VG_IMAGE_QUALITY_BETTER );
#else
    iImageManager->AddImageToLoadL( sf, _L("\\img\\icon_free.png"), KIconImage, VG_IMAGE_QUALITY_BETTER );
#endif
    iImageManager->StartLoadingImagesL();
	
	iContactsDb = CContactDatabase::OpenL();
	iContactsDb->SetDbViewContactType( KUidContactCard );
	
	iContactLoader = CEightContactPhotoLoader::NewL( this );
	
	iPhotosDCIM = CEightPhotosDCIM::NewL();
	}

void CEightPhotosProvider::SetHighFpsIdle( TBool aIdle )
    {
    iHighFpsIdle = aIdle;
    if ( aIdle )
        {
        iTimer->Stop();
        GPTimerEvent( iTimer );
        }
    }


TBool CEightPhotosProvider::StartDCIMUpdateL( MEightPhotosDCIMObserver* aObserver )
    {
    return iPhotosDCIM->UpdateDBContentL( aObserver );
    }

TBool CEightPhotosProvider::UpdateDBPending() const
    {
    return iPhotosDCIM->UpdateDBPending();
    }

TBool CEightPhotosProvider::DBOpened() const
    {
    return iPhotosDCIM->DBOpened();
    }

void CEightPhotosProvider::RegisterFontLoadClientL( MEightPhotosProviderFontObserver* aObserver )
	{
	iFontLoadClients.AppendL( aObserver );
	}

void CEightPhotosProvider::UnregisterFontLoadClient( MEightPhotosProviderFontObserver* aObserver )
	{
	TInt idx = iFontLoadClients.Find( aObserver );
	if ( idx > KErrNotFound )
		{
		iFontLoadClients.Remove( idx );
		}
	}

void CEightPhotosProvider::RegisterImageSourceL( TUint aImageSourceId, TImageSourceType aImageSourceType )
    {
    TSourceInfo si( aImageSourceType, *iContactsDb );
    iRegisteredSources.InsertL( aImageSourceId, si );
    }

TInt CEightPhotosProvider::AddImageFileToSource( TUint aImageSourceId, const TDesC& aFileName )
    {
    if ( iRegisteredSources.Find( aImageSourceId ) )
        {
        TImageFile file;
        file.iSourceId = aImageSourceId;
        file.iSgeFileId = KErrNotFound;
        TInt err = file.iFileName.Create( aFileName );
        if ( err != KErrNone )
            {
            return err;
            }
        err = iImageFiles.Append( file );
        if ( err != KErrNone )
            {
            file.iFileName.Close();
            }
        return err;
        }
    return KErrNotFound;
    }

TInt CEightPhotosProvider::AddImageFileToSource( TUint aImageSourceId, TUint aSgeFileId, const TDesC& aFileName )
    {
    if ( iRegisteredSources.Find( aImageSourceId ) )
        {
        TImageFile file;
        file.iSourceId = aImageSourceId;
        file.iSgeFileId = aSgeFileId;
        TInt err = file.iFileName.Create( aFileName );
        if ( err != KErrNone )
            {
            return err;
            }
        err = iImageFiles.Append( file );
        if ( err != KErrNone )
            {
            file.iFileName.Close();
            }
        return err;
        }
    return KErrNotFound;
    }

void CEightPhotosProvider::LoadPendingRequests()
    {
    if ( iPendingRequests.Count() > 0 && !iTimer->IsTimerStarted() )
        {
        iTimer->Start( 250000, EFalse );
        }
    }

void CEightPhotosProvider::NotifyFontLoad()
	{
	for ( TInt i = 0; i < iFontLoadClients.Count(); i++ )
		{
		iFontLoadClients[ i ]->FontsLoaded();
		}
	}

TInt CEightPhotosProvider::GetImageFromSource( MEightPhotosProvider* aClient, TUint aImageSourceId )
    {
    if ( !iHighFpsIdle )
        {
        TBool shedule = ETrue;
        for ( TInt i = 0; i < iPendingRequests.Count(); i++ )
            {
            if ( iPendingRequests[ i ].iClient == aClient && iPendingRequests[ i ].iImageSourceId == aImageSourceId )
                {
                shedule = EFalse;
                break;
                }
            }
        if ( shedule )
            {
            SPendingRequests r = { aClient, aImageSourceId };
            iPendingRequests.AppendL( r );
            }
        LoadPendingRequests();
        
        return KErrNotReady;
        }
    
    TSourceInfo *si = iRegisteredSources.Find( aImageSourceId );
    if ( si )
        {
        switch( si->iSourceType )
            {
            case EFile:
                    return LoadImageFromSourceFileL( aClient, aImageSourceId, si );
                break;
            case EGallery:
                    return LoadImageFromGalleryL( aClient, aImageSourceId, si );
                break;
            case EContact:
                    return LoadContactImageL( aClient, aImageSourceId, si );
                break;
            case EAd:
                break;
            }
        }
    return KErrNotFound;
    }

TInt CEightPhotosProvider::LoadImageFromSourceFileL( MEightPhotosProvider* aClient, TUint aImageSourceId, TSourceInfo* aSi )
    {
    TInt first = -1;
    for ( TInt i = 0, j = 0; i < iImageFiles.Count(); i++ )
        {
        if ( iImageFiles[ i ].iSourceId == aImageSourceId )
            {
            if ( first == -1 ) first = i;
            if ( j == aSi->iCurrentIdx )
                {
                aSi->iCurrentIdx++;
                return LoadImageL( aClient, iImageFiles[ i ].iFileName, aImageSourceId, iImageFiles[ i ].iSgeFileId );
                }
            j++;
            }
        }
    if ( first != -1 )
        {
        aSi->iCurrentIdx = 1;
        return LoadImageL( aClient, iImageFiles[ first ].iFileName, aImageSourceId, iImageFiles[ first ].iSgeFileId );
        }
    return KErrNotFound;
    }

TInt CEightPhotosProvider::LoadImageFromGalleryL( MEightPhotosProvider* aClient, TUint aImageSourceId, TSourceInfo* /*aSi*/ )
    {
    RBuf fileName;
    fileName.CreateL( 255 );
    iPhotosDCIM->GetFileFromDBL( fileName );
    if ( fileName.Length() != 0 )
        {
        TInt ret = LoadImageL( aClient, fileName, aImageSourceId, KErrNotFound, TSize( 360, 170 ) );
        fileName.Close();
        return ret;
        }
    fileName.Close();
    return KErrNotFound;
    }

TInt CEightPhotosProvider::LoadImageL( MEightPhotosProvider* aClient, const TDesC& aFileName )
	{
    return LoadImageL( aClient, aFileName, KErrNotFound, KErrNotFound );
	}

TInt CEightPhotosProvider::LoadImageL( MEightPhotosProvider* aClient, const TDesC& aFileName, TInt aImageSourceId, TInt aSgeFileId, TSize aSize )
    {
	CSGEFileProvider& fp = CEightSettings::GetInstance()->FileProvider();
	if ( aSgeFileId == KErrNotFound && !BaflUtils::FileExists( fp.Fs(), aFileName ) )
		{
		return KErrNotFound;
		}
	CClient *c = new (ELeave) CClient();
	c->iClient = aClient;
	c->iRequestId = ++iRequestId;
	c->iSource = aImageSourceId;
	c->iFileName.Create( aFileName );
	CleanupStack::PushL( c );
	iClients.AppendL( c );
	CleanupStack::Pop( c );

	if ( aSgeFileId != KErrNotFound )
	    {
	    CSGEFile& sf = fp.GetSGEFileL( aSgeFileId );
	    iImageManager->AddImageToLoadL( sf, aFileName, c->iRequestId, VG_IMAGE_QUALITY_FASTER, aSize );
	    }
	else
	    {
	    iImageManager->AddImageToLoadL( aFileName, c->iRequestId, VG_IMAGE_QUALITY_FASTER, aSize );
	    }
	if ( iFontsLoaded == 3 ) iImageManager->StartLoadingImagesL();
		
	return c->iRequestId;
	}

void CEightPhotosProvider::UnloadImage( TUint aImageId )
	{
	iImageManager->RemoveImage( aImageId );
	}

CVgImageManager& CEightPhotosProvider::ImageManager() const
	{
	return *iImageManager;
	}

TInt CEightPhotosProvider::LoadContactImageL()
    {
    return KErrNone;
    }
    

TInt CEightPhotosProvider::LoadContactImageL( MEightPhotosProvider* aClient, TUint aImageSourceId, TSourceInfo* aSi )
    {
    if ( iContactLoader->IsActive() )
        {
        return KErrNotReady;
        }
    if ( aSi->iContactIteratorEnd )
        {
        return KErrOverflow;
        }
    TInt ret = KErrNotFound;

    TContactItemId contactId;

    while ( ( contactId = aSi->iContactIter.NextL() ) != KNullContactId )
        {
        CContactItem* contactItem = iContactsDb->ReadContactL( contactId );
        CleanupStack::PushL( contactItem );

        CContactItemFieldSet& fieldSet = contactItem->CardFields();

        TInt index = fieldSet.Find( KUidContactFieldVCardMapPHOTO );
        if ( index != KErrNotFound )
            {
            CContactItemField& field = fieldSet[ index ];

            TStorageType storageType = field.StorageType();
            const CContentType& contentType = field.ContentType();

            if ( storageType == KStorageTypeStore && contentType.Mapping() == KUidContactFieldVCardMapPHOTO )
                {
                CContactStoreField* str = field.StoreStorage();
                HBufC8* image = str->Thing();
                
                //delete iContactLoader;
                //iContactLoader = NULL;
                //iContactLoader = CEightContactPhotoLoader::NewL( this, image );
                
                iContactLoader->LoadContactL( image );
                
                CClient *c = new (ELeave) CClient();
                c->iClient = aClient;
                c->iRequestId = ++iRequestId;
                c->iSource = aImageSourceId;
                CleanupStack::PushL( c );
                iClients.AppendL( c );
                CleanupStack::Pop( c );                    
                
                iContactsDb->CloseContactL( contactItem->Id() );
                CleanupStack::PopAndDestroy(); //contactItem
                
                ret = c->iRequestId;
                break;
                }
            }

        iContactsDb->CloseContactL( contactItem->Id() );
        CleanupStack::PopAndDestroy(); //contactItem
        }
    if ( contactId == KNullContactId )
        {
        aSi->iContactIteratorEnd = ETrue;
        }
    return ret;
    }


void CEightPhotosProvider::LoadFont1L()
    {
    TVgImage* img = (TVgImage*) iImageManager->Get( KFont1image );
    TSize gc( 16, 8 );
    TSize s = img->Size();
    CVgFont* font = CVgFont::NewL( img->Image(), gc.iWidth * gc.iHeight );
    font->AddGlyphs( ' ', '~', s, TSize( 8, 16 ), gc );
    iImageManager->RemoveImage( KFont1image );
    CEightSettings::GetInstance()->FontManager().AddFontL( KFontId_SGE, font );
    }

void CEightPhotosProvider::LoadFont2L()
    {
    TVgImage* img = (TVgImage*) iImageManager->Get( KFont2image );
    CVgFontManager& fm = CEightSettings::GetInstance()->FontManager();
    
    TSize s = img->Size();
    CVgFont* font1 = CVgFont::NewL( img->Image(), 9 * 13 );    
    RArray< TSize > esc( 9 * 13 );
    for ( TInt i = 0; i < 9 * 13; i++ ) esc.Append( TSize( KFont_SegoeWP_14x19_Escapements[ i ], 0 ) );
    font1->AddGlyphs( ' ', 128, TSize( 112, s.iHeight ), TSize( 14, 19 ), TSize( 9, 13 ), TPoint( 0, 4 ), esc );
    esc.Reset();
    fm.AddFontL( KFontId_SegoeWP_small, font1 );
    
    CVgFont* font2 = CVgFont::NewL( img->Image(), 9 * 11 );
    for ( TInt i = 0; i < 9 * 11; i++ ) esc.Append( TSize( KFont_SegoeWP_17x23_Escapements[ i ], 0 ) );
    font2->AddGlyphs( ' ', 128, TSize( 265 - 112, s.iHeight ), TSize( 17, 23 ), TSize( 9, 11 ), TPoint( 0, 4 ), esc, TPoint( 112, 0 ) );
    esc.Reset();
    fm.AddFontL( KFontId_SegoeWP_medium, font2 );

    CVgFont* font3 = CVgFont::NewL( img->Image(), 8 * 2 );
    for ( TInt i = 0; i < 8 * 2; i++ ) esc.Append( TSize( KFont_SegoeWP_largeNum_Escapements[ i ], 0 ) );
    font3->AddGlyphs( '0', ';', TSize( 307 - 265, s.iHeight ), TSize( 21, 30 ), TSize( 2, 8 ), TPoint(), esc, TPoint( 265, 0 ) );
    esc.Reset();
    fm.AddFontL( KFontId_SegoeWP_largeNum, font3 );
    
    CVgFont* font4 = CVgFont::NewL( img->Image(), 7 * 14 );
    for ( TInt i = 0; i < 7 * 14; i++ ) esc.Append( TSize( KFont_SegoeWP_26x36_Escapements[ i ], 0 ) );
    font4->AddGlyphs( ' ', 128, TSize( 673 - 307, s.iHeight ), TSize( 26, 36 ), TSize( 14, 7 ), TPoint( 0, 7 ), esc, TPoint( 307, 0 ) );
    esc.Reset();
    fm.AddFontL( KFontId_SegoeWP_large, font4 );
    
    CVgFont* font5 = CVgFont::NewL( img->Image(), 4 * 3 );
    for ( TInt i = 0; i < 4 * 3; i++ ) esc.Append( TSize( KFont_SegoeWP_hugeNum_Escapements[ i ], 0 ) );
    font5->AddGlyphs( '0', ':', TSize( 893 - 673, s.iHeight ), TSize( 55, 85 ), TSize( 4, 3 ), TPoint(), esc, TPoint( 673, 0 ) );
    esc.Close();
    fm.AddFontL( KFontId_SegoeWP_hugeNum, font5 );
    
    iImageManager->RemoveImage( KFont2image );
    }

void CEightPhotosProvider::VgImageLoaded( TUint aImageId, TInt aError )
	{
    if ( aImageId == KFont1image )
        {
        LoadFont1L();
        iFontsLoaded = 1;
        return;
        }
    else if ( aImageId == KFont2image )
        {
        LoadFont2L();
        iFontsLoaded = 2;
        NotifyFontLoad();
        return;
        }
    else if ( aImageId == KIconImage || aImageId == KLogoImage )
        {
        return;
        }    
    
    LoadPendingRequests();
    
//    Logger::LogStrNum( _L("c:\\data\\egal.txt"), _L("VgImageLoaded: "), aImageId );
//    Logger::LogStrNum( _L("c:\\data\\egal.txt"), _L("VgImageLoaded: "), aError );
//    Logger::LogStr( _L("c:\\data\\egal.txt"), _L("") );

	for ( TInt i = 0; i < iClients.Count(); i++ )
		{
		if ( (TUint)iClients[ i ]->iRequestId == aImageId )
			{
			iClients[ i ]->iClient->ImageLoaded( aImageId, aError, iClients[ i ]->iSource );
			delete iClients[ i ];
			iClients.Remove( i );
			return;
			}
		}
	}

void CEightPhotosProvider::VgAllImagesLoaded()
	{
    if ( iFontsLoaded == 2 ) iFontsLoaded = 3;
	}

void CEightPhotosProvider::ContactPhotoLoaded( TInt aError, CSGEBitmapItem* aBitmapItem )
    {
    LoadPendingRequests();

    for ( TInt i = 0; i < iClients.Count(); i++ )
        {
        if ( iClients[ i ]->iSource > KErrNotFound )
            {
            TSourceInfo *si = iRegisteredSources.Find( iClients[ i ]->iSource );
            if ( si && si->iSourceType == EContact )
                {
                
                if ( aError == KErrNone )
                    {
                    CFbsBitmap* bmp;                
                    if ( aBitmapItem->BitmapMask() )
                        {
                        bmp = SGEBitmapUtils::CombineBitmapsL( *aBitmapItem->Bitmap(), *aBitmapItem->BitmapMask(), EFalse, ETrue );
                        }
                    else
                        {
                        bmp = SGEBitmapUtils::ConvertTo16MAL( *aBitmapItem->Bitmap(), EFalse, ETrue );
                        //bmp->SetDisplayMode( EColor16MU ); // propper VG image mode
                        }
                    TVgImage img;
                    img.CreateImage( bmp, VG_IMAGE_QUALITY_FASTER );
                    
                    TSize sb = bmp->SizeInPixels();
                    if ( sb.iHeight > sb.iWidth )
                        {
                        TVgImage img2 = img.CreateChildImage( TPoint( 0, ( sb.iHeight - sb.iWidth ) / 2 ), TSize( sb.iWidth, sb.iWidth ) );
                        img.DeleteImage();
                        iImageManager->AddImageL( iClients[ i ]->iRequestId, img2 );
                        }
                    else if ( sb.iHeight < sb.iWidth )
                        {
                        TVgImage img2 = img.CreateChildImage( TPoint( ( sb.iWidth - sb.iHeight ) / 2, 0 ), TSize( sb.iHeight, sb.iHeight ) );
                        img.DeleteImage();
                        iImageManager->AddImageL( iClients[ i ]->iRequestId, img2 );
                        }
                    else
                        {
                        iImageManager->AddImageL( iClients[ i ]->iRequestId, img );
                        }
                    delete bmp;
                    }
                iClients[ i ]->iClient->ImageLoaded( iClients[ i ]->iRequestId, aError, iClients[ i ]->iSource );
                delete iClients[ i ];
                iClients.Remove( i );
                return;
                }
            }
        }
    
//    delete iContactLoader;
//    iContactLoader = NULL;
    }

void CEightPhotosProvider::GPTimerEvent( CSGEUtilsGPTimer* /*aTimer*/ )
    {
    if ( iPendingRequests.Count() > 0 )
        {
        TInt err = GetImageFromSource( iPendingRequests[ 0 ].iClient, iPendingRequests[ 0 ].iImageSourceId );
        if ( err != KErrNotReady )
            {
            iPendingRequests[ 0 ].iClient->ImageLoaded( KErrNotFound, err, iPendingRequests[ 0 ].iImageSourceId );
            iPendingRequests.Remove( 0 );
            }
        if ( err != KErrNone )
            {
            LoadPendingRequests();
            }
        }
    }


