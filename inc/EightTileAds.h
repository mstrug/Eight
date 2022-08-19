/*
 ============================================================================
 Name		: EightTileAds.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileAds declaration
 ============================================================================
 */

#ifndef EIGHTTILEADS_H
#define EIGHTTILEADS_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <ES_SOCK.H> 
#include <xml/parser.h>
#include <xml/contenthandler.h>
#include <SGEVg/VgSvgImage.h>
#include <SGEVg/VgPrimitives.h>
#include <SGEUtils/SGEUtilsGPTimer.h>
#include <SGEUtils/SGEUtilsGPTimerObserver.h>
#include <SGEUtilsHttpClient.h>
#include <SGESecurity/SGEImeiGetter.h>
#include "EightTile.h"
#include "EightMsgBox.h"
#include "EightPhotosProvider.h"

// CLASS DECLARATION
using namespace Xml;

const TInt KAdUpdateTime = 30000000; // 30 sec


/**
 *  CEightTileAds
 * 
 */
class CEightTileAds : public CEightTile, MSGEUtilsGPTimerObserver, MEightMsgBoxObserver, MEightPhotosProvider, MSGEHttpClientObserver, MSGEHttpClientConnectionObserver, MContentHandler, MSGEImeiGetterObserver
	{
public:

	~CEightTileAds();

	static CEightTileAds* NewL( CCoeControl* aControl, TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi = NULL );
	
    void SetMsgBox( CEightMsgBox* aMsgBox );

    TInt CustomSettingsButtonsCount() const;

    CTouchControlBase* GetCustomSettingsButtonL( TInt aIdx );

    void HandleCustomSettingsButtonState( CTouchControlBase* aButton );

	TEightTileType Type() const;
	
	TBool RunInternal();
    
	TBool CanIncreaseSize() const;
    
    TBool CanDecreaseSize() const;
    
    void HandleForegroundEventL( TBool aForeground );

    void Update( TTimeIntervalMicroSeconds32 aTimeInterval );
    
    void ExternalizeL( RWriteStream& aStream ) const;

    void InternalizeL( RReadStream& aStream );
    
private:

	CEightTileAds( TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi );

	void ConstructL( CCoeControl* aControl );
	
	void RequestAd();
	
	void RequestAdL();

	void RequestImageL();

	void SheduleNextAdRequest( TInt aTime = KAdUpdateTime );
	
private: // from MSGEUtilsGPTimerObserver
	
    void GPTimerEvent( CSGEUtilsGPTimer* aTimer );	
	
private: // from MEightMsgBoxObserver
    
    void MsgBoxAction( TUint aMessageId, TInt aActionId );
    
private: // from MEightPhotosProvider
    
    void ImageLoaded( TInt aImageId, TInt aError, TInt aSource );
    
private: // from MSGEHttpClientObserver
    
    void HttpResponse( TInt aId, TInt aStatus, const TDesC8& aData );
    
private: // from MSGEHttpClientConnectionObserver

	void HttpConnectionStarted( TInt aError );

	void HttpConnectionClosed();

private: // from MContentHandler
	
	void OnStartDocumentL(const RDocumentParameters& aDocParam, TInt aErrorCode);
	void OnEndDocumentL(TInt aErrorCode);
	void OnStartElementL(const RTagInfo& aElement, const RAttributeArray& aAttributes,TInt aErrorCode);
	void OnEndElementL(const RTagInfo& aElement, TInt aErrorCode);
	void OnContentL(const TDesC8& aBytes, TInt aErrorCode);
	void OnStartPrefixMappingL(const RString& aPrefix, const RString& aUri, TInt aErrorCode);
	void OnEndPrefixMappingL(const RString& aPrefix, TInt aErrorCode);
	void OnIgnorableWhiteSpaceL(const TDesC8& aBytes, TInt aErrorCode);
	void OnSkippedEntityL(const RString& aName, TInt aErrorCode);
	void OnProcessingInstructionL(const TDesC8& aTarget, const TDesC8& aData, TInt aErrorCode);
	void OnError(TInt aErrorCode);
	TAny* GetExtendedInterface(const TInt32 aUid);

private: // from MSGEImeiGetterObserver
	
	void GotImeiL(const TDesC& aIMEI,TInt aError);
	
private: // data
    
    CSGEHttpClient* iHttpClient; // owned
    
	CSGEUtilsGPTimer* iTimer; // owned
	
	CCoeControl* iControl; // not owned 

	CEightMsgBox* iMsgBox; // not owned
		
	CSGEImeiGetter* iImeiGetter; // owned
	
	typedef enum
	{
	    ENotConnected,
        EConnectionNeededMessage,
        EConnectionNeededMessage2,
	    EConnected
	} TConnectionState;
	
	TConnectionState iState;
	
	//TUint32 iIap;
	
	
	typedef enum
	{
	    EXMLDownloading,
	    EXMLElementUrl,
        EXMLElementImg,
        EXMLDone,
	    EIMGDownloading,
	    EIMGDecoding,
	    EIMGReady,
	    ERetry
	} THttpState;
	
	THttpState iHttpState;
	
	CParser* iXmlParser; // owned

	RBuf8 iAdUrl;
	
	RBuf8 iAdUrlImage;
	
	RFile iAdFile;
	
    TInt iAdImageId[ 2 ];
    RBuf8 iAdUrls[ 2 ];
	
	TInt iAdImageIdx; // 0 or 1
	
	TInt iImageUiIdx[ 2 ];
	
	TUint32 iClientId;

	TBool iForeground;
	
	TBool iRequestedFromBackground;
	
	TBool iShowConnectionWarning;
	
	TTime iLastUpdateTime;
	
	TBool iErrorsLog;
	
	TInt iHttpErrorCnt;
	
	};

#endif // EIGHTTILEADS_H
