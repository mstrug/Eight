/*
 ============================================================================
 Name		: EightPhotosProvider.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightPhotosProvider declaration
 ============================================================================
 */

#ifndef EIGHTPHOTOSPROVIDER_H
#define EIGHTPHOTOSPROVIDER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGEVg/VgImageManager.h>
#include <SGEUtils/SGEUtilsGPTimer.h>
#include <SGEUtils/SGEUtilsGPTimerObserver.h>
#include "EightContactPhotoLoader.h"
#include "EightPhotosDCIM.h"
#include <cntdb.h>
#include <cntitem.h>
#include <cntfield.h>
#include <cntfldst.h>
#include <D32DBMS.H> 


// CLASS DECLARATION

const TInt KPhotosSourceIdInternalFiles = 0x100;
const TInt KPhotosSourceIdContacts = 0x200;
const TInt KPhotosSourceIdDCIM = 0x300;

const TInt KAdsImageF = 0xf011;
const TInt KAdsImageB = 0xf012;

const TInt KFont1image = 0xf021;
const TInt KFont2image = 0xf022;
const TInt KLogoImage = 0xf023;
const TInt KIconImage = 0xf024;

const TInt KWizzardTileImage1 = 0xf031;
const TInt KWizzardTileImage2 = 0xf032;
const TInt KPremiumTileImage = 0xf034;
const TInt KThemeColorTileImage = 0xf035;
const TInt KAboutTileImage = 0xf036;
const TInt KSetupTileImage = 0xf037;
const TInt KHelpTileImage = 0xf038;

const TInt KTileManagerImageArrow = 0xf041;
const TInt KTileManagerImageArrowSel = 0xf042;
const TInt KTileManagerImageRemove = 0xf043;
const TInt KTileManagerImageRemoveSel = 0xf044;
const TInt KTileManagerImageSettings = 0xf045;
const TInt KTileManagerImageSettingsSel = 0xf046;
const TInt KTileManagerImagePin = 0xf047;
const TInt KTileManagerImagePinSel = 0xf048;
const TInt KTileManagerImageUnpin = 0xf049;
const TInt KTileManagerImageUnpinSel = 0xf04a;
const TInt KTileManagerImageExit = 0xf04b;
const TInt KTileManagerImageExitSel = 0xf04c;
const TInt KTileManagerImageArrowRight = 0xf04d;
const TInt KTileManagerImageArrowRightSel = 0xf04e;
const TInt KTileManagerImageArrowLeft = 0xf04f;
const TInt KTileManagerImageArrowLeftSel = 0xf050;



class MEightPhotosProvider
	{
public:
    virtual void ImageLoaded( TInt aImageId, TInt aError, TInt aSource ) = 0;
	};

class MEightPhotosProviderFontObserver
	{
public:
    virtual void FontsLoaded() = 0;
	};

/**
 *  CEightPhotosProvider
 * 
 */
class CEightPhotosProvider : public CBase, public MVgImageManagerObserver, MEightContactPhotoLoader, MSGEUtilsGPTimerObserver
	{
public:
    typedef enum
    {
        EFile,
        EGallery,
        EContact,
        EAd
    } TImageSourceType;
public:

    // singelton functions
	static CEightPhotosProvider* GetInstanceL();
	
	void Destroy();

	void RegisterFontLoadClientL( MEightPhotosProviderFontObserver* aObserver );
	
	void UnregisterFontLoadClient( MEightPhotosProviderFontObserver* aObserver );
	
	// image source provider API
    void RegisterImageSourceL( TUint aImageSourceId, TImageSourceType aImageSourceType );

    TInt AddImageFileToSource( TUint aImageSourceId, const TDesC& aFileName );
    
    TInt AddImageFileToSource( TUint aImageSourceId, TUint aSgeFileId, const TDesC& aFileName );
    
    TInt GetImageFromSource( MEightPhotosProvider* aClient, TUint aImageSourceId );

    // universal API
	TInt LoadImageL( MEightPhotosProvider* aClient, const TDesC& aFileName );

	void UnloadImage( TUint aImageId );
	
	CVgImageManager& ImageManager() const;

    TInt LoadContactImageL();
    
    void SetHighFpsIdle( TBool aIdle );

    TBool StartDCIMUpdateL( MEightPhotosDCIMObserver* aObserver );
    
    TBool UpdateDBPending() const;
    
    TBool DBOpened() const;
    
private:

	CEightPhotosProvider();

	~CEightPhotosProvider();

	void ConstructL();

	static CEightPhotosProvider* NewL();
	
    class TSourceInfo
        {
    public:
	    TSourceInfo( TImageSourceType aSourceType, CContactDatabase& aDatabase ) : iSourceType( aSourceType ), iCurrentIdx(0),
            iContactIter( aDatabase ), iContactIteratorEnd(EFalse), iGallerySearchListCnt(0), iGallerySearchListCall(0),
	        iGallerySearchListSortAsc(ETrue) { }
	    
        TImageSourceType iSourceType;
        TInt iCurrentIdx;
        
        TContactIter iContactIter; // for contacts
        TBool iContactIteratorEnd; // for contacts
        
        RBuf iGallerySearchList; // for gallery
        TInt iGallerySearchListCnt; // for gallery
        TInt iGallerySearchListCall; // for gallery
        TBool iGallerySearchListSortAsc; // for gallery
        };
    
    TInt LoadImageFromSourceFileL( MEightPhotosProvider* aClient, TUint aImageSourceId, TSourceInfo* aSi );

    TInt RefrestGalleryFolderSearchList( TSourceInfo* aSi );

    TInt LoadImageFromGalleryL( MEightPhotosProvider* aClient, TUint aImageSourceId, TSourceInfo* aSi );

    TInt LoadImageL( MEightPhotosProvider* aClient, const TDesC& aFileName, TInt aImageSourceId, TInt aSgeFileId, TSize size = TSize() );

    TInt LoadContactImageL( MEightPhotosProvider* aClient, TUint aImageSourceId, TSourceInfo* aSi );
    
    void LoadFont1L();

    void LoadFont2L();

    void LoadPendingRequests();
    
    void NotifyFontLoad();

private: // MVgImageManagerObserver
	
    void VgImageLoaded( TUint aImageId, TInt aError );

    void VgAllImagesLoaded();
    
private: // MEightContactPhotoLoader 
    
    void ContactPhotoLoaded( TInt aError, CSGEBitmapItem* aBitmapItem );
    
private: // from MSGEUtilsGPTimerObserver
    
    void GPTimerEvent( CSGEUtilsGPTimer* aTimer );
    
private:

    CSGEUtilsGPTimer* iTimer; // owned
    
    CVgImageManager* iImageManager; // owned

    CEightContactPhotoLoader* iContactLoader; // owned
    
    CContactDatabase* iContactsDb; // owned
    
    CEightPhotosDCIM* iPhotosDCIM; // owned

    class CClient
		{
    public:
    	MEightPhotosProvider* iClient;
    	RBuf iFileName;
    	TInt iRequestId;
    	TInt iSource;
    	~CClient() { iFileName.Close(); }
		};
		
	RPointerArray< CClient > iClients;

    TInt iRequestId;
    
    // source images api

	RHashMap< TUint, TSourceInfo > iRegisteredSources;
	
	class TImageFile
	    {
	public:
	    TUint iSourceId;
	    RBuf iFileName;
	    TInt iSgeFileId;
	    };
    RArray< TImageFile > iImageFiles;
    	
    // used for loading font images first
    TInt iFontsLoaded; // 1, 2, 3
    
    

    struct SPendingRequests
        {
        MEightPhotosProvider* iClient;
        TUint iImageSourceId;
        };
        
    RArray< SPendingRequests > iPendingRequests;
    
    TBool iHighFpsIdle;
    
    RPointerArray< MEightPhotosProviderFontObserver > iFontLoadClients;
    
	};

#endif // EIGHTPHOTOSPROVIDER_H
