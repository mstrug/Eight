/*
 ============================================================================
 Name		: SGEUtilsHttpClient.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEHttpClient declaration
 ============================================================================
 */

#ifndef SGEUTILSHTTPCLIENT_H
#define SGEUTILSHTTPCLIENT_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <ES_SOCK.H> 
#include <http.h>
#include <rconnmon.h>
#include <SGEUtilsGPAO.h>

// CLASS DECLARATION


_LIT8( KSGEHttpClientDefaultUserAgent, "Mozilla/5.0 (Symbian/3; Series60/5.3 Nokia; Profile/MIDP-2.1 Configuration/CLDC-1.1 ) AppleWebKit/535.1 (KHTML, like Gecko) NokiaBrowser/8.3.1.4 Mobile Safari/535.1 SGE" );
_LIT8( KSGEHttpClientDefaultContentType, "text/xml" );
_LIT8( KSGEHttpClientDefaultAccept, "*/*" );
_LIT8( KSGEHttpClientDefaultConnectionClose, "Close" );


class MSGEHttpClientObserver
	{
public:
	
	// aStatus - HTTP Status
	// aData - valid only when aStatus == 200
	virtual void HttpResponse( TInt aId, TInt aStatus, const TDesC8& aData ) = 0;

	virtual void HttpDownloadProgress( TInt /*aId*/, TInt /*aCurrentSize*/, TInt /*aTotalSize*/ ) { }

	};

class MSGEHttpClientConnectionObserver
	{
public:
	
	virtual void HttpConnectionStarted( TInt aError ) = 0;

	virtual void HttpConnectionClosed() = 0;

	};

/**
 *  CSGEHttpClient
 * 
 */
class CSGEHttpClient : public CBase, MHTTPTransactionCallback, MConnectionMonitorObserver, MSGEUtilsGPAO
	{

	class TTransaction : public MHTTPDataSupplier 
		{
	public:
		enum TType { EResponseFile, EResponseMemory };
		TTransaction( MSGEHttpClientObserver& aObserver ) : iObserver( aObserver ), iError( KErrGeneral ), 
															   iType( EResponseMemory ) { } 
	public: // from MHTTPDataSupplier
		TBool GetNextDataPart( TPtrC8 &aDataPart ) { aDataPart.Set( iRequestBody ); return ETrue; }		
		void ReleaseData() { }
		TInt OverallDataSize() { return 0; }		
		TInt Reset() { return KErrNotSupported; }
	public:
		MSGEHttpClientObserver& iObserver;
		RHTTPTransaction iHttpTransaction;
		TInt iId;
		TInt iError;
		RBuf8 iResponseBody;
		TPtrC8 iRequestBody;
		RFile iFile;
		TType iType;
		};
	
public:
	// Constructors and destructor
	
	/**
	 * Destructor.
	 */
	~CSGEHttpClient();

	/**
	 * Two-phased constructor.
	 */
	static CSGEHttpClient* NewL( TUint32 aIap = 0 );

	/**
	 * Two-phased constructor.
	 */
	static CSGEHttpClient* NewLC( TUint32 aIap = 0 );

    void StartConnectionL( MSGEHttpClientConnectionObserver* aConnectionObserver = NULL );

    TBool AttachToConnectionL( MSGEHttpClientConnectionObserver* aConnectionObserver = NULL );

    //void CancelAttachMode();

	// returns transaction id
	TInt GetL( MSGEHttpClientObserver& aObserver, const TDesC& aUrl,
				const TDesC8& aUserAgent = KSGEHttpClientDefaultUserAgent(), 
				const TDesC8& aContentType = KSGEHttpClientDefaultContentType(),
				const TDesC8& aAccept = KSGEHttpClientDefaultAccept(), 
				const TDesC8& aConnectionClose = KSGEHttpClientDefaultConnectionClose() );

	// returns transaction id
	// downloads directly to the opened file passed to function
	// file is not closed by the http client
	TInt GetL( MSGEHttpClientObserver& aObserver, const TDesC& aUrl, RFile aFile,
				const TDesC8& aUserAgent = KSGEHttpClientDefaultUserAgent(), 
				const TDesC8& aContentType = KSGEHttpClientDefaultContentType(),
				const TDesC8& aAccept = KSGEHttpClientDefaultAccept(), 
				const TDesC8& aConnectionClose = KSGEHttpClientDefaultConnectionClose() );

	// returns transaction id
	// aBody must be valid until transaction end
	TInt PostL( MSGEHttpClientObserver& aObserver, const TDesC& aUrl, const TDesC8& aBody,
				const TDesC8& aUserAgent = KSGEHttpClientDefaultUserAgent(), 
				const TDesC8& aContentType = KSGEHttpClientDefaultContentType(),
				const TDesC8& aAccept = KSGEHttpClientDefaultAccept(), 
				const TDesC8& aConnectionClose = KSGEHttpClientDefaultConnectionClose() );
	
	void CancelTransaction( TInt aTransactionId );

	void SetAutoCloseConnection( TBool aStatus );

	TBool IsConnectionStarted() const;

	TUint32 Iap() const;
	
	void SetIap( TUint32 aIap );
	
private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CSGEHttpClient( TUint32 aIap );

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();

	void HttpCleanup();

	void SetHeaderL( RHTTPHeaders aHeaders, TInt aHdrField, const TDesC8& aHdrValue );

	TInt ContentLength( RHTTPHeaders aHeaders );

	TTransaction* FindTransaction( RHTTPTransaction aHttpTransaction );

	void NotifyTransaction( TTransaction* aTransaction );

	void CloseTransaction( TTransaction* aTransaction );
	
	void CloseConnection();
	
private:	// from MHTTPTransactionCallback
	
	void MHFRunL( RHTTPTransaction aTransaction, const THTTPEvent &aEvent );
	
	TInt MHFRunError( TInt aError, RHTTPTransaction aTransaction, const THTTPEvent &aEvent );

private: 	// from MConnectionMonitorObserver
	
	void EventL( const CConnMonEventBase &aConnMonEvent );
	
private:	// from MSGEUtilsGPAO
	
	void GPAOObserver( CSGEUtilsGPAO* aClient, TInt aError );
	    
private:
	
	MSGEHttpClientConnectionObserver* iConnectionObserver;
	 
	RSocketServ iSocketServ;
	
	RConnection iConnection;
	
	RConnectionMonitor iConnectionMonitor;
	
	RHTTPSession iHttpSession;

	RArray<TTransaction> iTransactions;
	
	TBool iConnectionStarted;

	TInt iIdCounter;
	
	TBool iAutoCloseConnection;
	
	CSGEUtilsGPAO* iGpao; // owned
	
	TUint iIap;
	
	//TBool iAttachMode;
	
	};

#endif // SGEUTILSHTTPCLIENT_H
