/*
 ============================================================================
 Name		: SGEHttpClient.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEHttpClient implementation
 ============================================================================
 */

#include "SGEUtilsHttpClient.h"
#include <es_sock.h>
#include <es_enum_partner.h> 
#include <ConnPref.h>
#include <UriUtils.h>
#include <httpstringconstants.h>
#include <commdbconnpref.h>
//#include "logger.h"


CSGEHttpClient::CSGEHttpClient( TUint32 aIap ) : iAutoCloseConnection( ETrue ), iIap( aIap )
	{
	}

CSGEHttpClient::~CSGEHttpClient()
	{
	for ( TInt i = iTransactions.Count() - 1; i >= 0; i-- )
		{
		CloseTransaction( &iTransactions[ i ] );
		}

	delete iGpao;
	
	iTransactions.Close();
	iHttpSession.Close();
	iConnection.Close();
	iSocketServ.Close();
	
	iConnectionMonitor.Close();
	}

CSGEHttpClient* CSGEHttpClient::NewLC( TUint32 aIap )
	{
	CSGEHttpClient* self = new (ELeave) CSGEHttpClient( aIap );
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CSGEHttpClient* CSGEHttpClient::NewL( TUint32 aIap )
	{
	CSGEHttpClient* self = CSGEHttpClient::NewLC( aIap );
	CleanupStack::Pop(); // self;
	return self;
	}

void CSGEHttpClient::ConstructL()
	{
	User::LeaveIfError( iSocketServ.Connect() );
	
	iGpao = new (ELeave) CSGEUtilsGPAO( this );
			
	iHttpSession.OpenL();
    iHttpSession.ConnectionInfo().SetPropertyL( iHttpSession.StringPool().StringF( HTTP::EHttpSocketServ,
                                                RHTTPSession::GetTable() ),
                                                THTTPHdrVal( iSocketServ.Handle() ) );

    iConnectionMonitor.ConnectL();
    iConnectionMonitor.NotifyEventL( *this );
	}

void CSGEHttpClient::StartConnectionL( MSGEHttpClientConnectionObserver* aConnectionObserver )
	{
	if ( !iConnectionStarted )
		{
		if ( aConnectionObserver )
			{
			iConnectionObserver = aConnectionObserver;
			if ( iGpao->IsActive() )
				{
				User::Leave( KErrAlreadyExists );
				}
			
			if ( !AttachToConnectionL() )
				{
	            User::LeaveIfError( iConnection.Open( iSocketServ ) );
	            
				TCommDbConnPref pref;
				pref.SetIapId( iIap );
				if ( iIap == 0 )
					{
					pref.SetDialogPreference( ECommDbDialogPrefPrompt );
					}
				iConnection.Start( pref, iGpao->iStatus );
				iGpao->SetActive();
				}
			}
		else
			{
            User::LeaveIfError( iConnection.Open( iSocketServ ) );
            TCommDbConnPref pref;
			pref.SetIapId( iIap );
			if ( iIap == 0 )
				{
				pref.SetDialogPreference( ECommDbDialogPrefPrompt );
				}
			User::LeaveIfError( iConnection.Start( pref ) );
			iConnectionStarted = ETrue;
			}
		}
	else
		{
		if ( iConnectionObserver )
			{
			iConnectionObserver->HttpConnectionStarted( KErrNone );
			}
		}
	}

TBool CSGEHttpClient::AttachToConnectionL( MSGEHttpClientConnectionObserver* aConnectionObserver )
    {
    TUint cntCount = 0;
    
    //iAttachMode = ETrue;
    
    if ( aConnectionObserver )
        {
        iConnectionObserver = aConnectionObserver;
        }
    
    if ( iConnectionStarted )
        { // error
        return EFalse;
        }
        
    User::LeaveIfError( iConnection.Open( iSocketServ ) );
    
    if ( iConnection.EnumerateConnections( cntCount ) == KErrNone )
        {
        if ( cntCount != 0 )
            {
            TBool attached = EFalse;
            for ( TUint i = 1; i <= cntCount; i++ )
                {
                TConnectionInfoV2Buf ci;
                if ( iConnection.GetConnectionInfo( i, ci ) == KErrNone )
                    {
                    if ( iConnection.Attach( ci, RConnection::EAttachTypeNormal ) == KErrNone )
                        {
                        iIap = ci().iIapId;
                        attached = ETrue;
                        iConnectionStarted = ETrue;                        

                        iHttpSession.ConnectionInfo().SetPropertyL( iHttpSession.StringPool().StringF( HTTP::EHttpSocketConnection,
                                                                    RHTTPSession::GetTable() ),
                                                                    THTTPHdrVal( reinterpret_cast<TInt>( &iConnection ) ) );
                        
                        iConnectionObserver->HttpConnectionStarted( KErrNone );
                        return ETrue;
                        }
                    }
                }
            }
        }
    iConnection.Close();
    return EFalse;
    }

//void CSGEHttpClient::CancelAttachMode()
//    {
//    iAttachMode = EFalse;
//    }

void CSGEHttpClient::MHFRunL( RHTTPTransaction aTransaction, const THTTPEvent &aEvent )
	{
	TTransaction* tran = FindTransaction( aTransaction );
	
	if ( tran )
		{
		if ( aEvent.iStatus < 0 )
			{
			tran->iError = aEvent.iStatus;
			}
		else
			{
			switch ( aEvent.iStatus )
				{
				case THTTPEvent::EFailed:
						NotifyTransaction( tran );
						CloseTransaction( tran );
					break;
				case THTTPEvent::ECancel:
						tran->iError = KErrCancel;
						NotifyTransaction( tran );
						CloseTransaction( tran );
					break;
				case THTTPEvent::ESucceeded:
						NotifyTransaction( tran );
						CloseTransaction( tran );
					break;
				case THTTPEvent::EGotResponseHeaders:
						{
						RHTTPResponse resp = tran->iHttpTransaction.Response();
						tran->iError = resp.StatusCode();
						
						if ( !( resp.HasBody() && ( tran->iError >= 200 ) && ( tran->iError < 300 ) ) )
							{
							NotifyTransaction( tran );
							CloseTransaction( tran );
							}
						else
							{
							TInt cl = ContentLength( tran->iHttpTransaction.Response().GetHeaderCollection() );
							if ( cl > 0 )
								{
								tran->iObserver.HttpDownloadProgress( tran->iId, 0, cl );
								}
							}
						}
					break;
				case THTTPEvent::EGotResponseBodyData:
						{
						MHTTPDataSupplier* respBodyData = tran->iHttpTransaction.Response().Body();
						TPtrC8 ptr;
						respBodyData->GetNextDataPart( ptr );
						if ( ptr.Size() > 0 )
							{
							TInt cl = ContentLength( tran->iHttpTransaction.Response().GetHeaderCollection() );
							if ( tran->iType == TTransaction::EResponseFile )
								{
								tran->iFile.Write( ptr );
								if ( cl > 0 )
									{
									TInt size;
									if ( tran->iFile.Size( size ) == KErrNone )
										{
										tran->iObserver.HttpDownloadProgress( tran->iId, size, cl );
										}
									}
								}
							else
								{
								tran->iResponseBody.ReAllocL( tran->iResponseBody.MaxLength() + ptr.Size() );
								tran->iResponseBody.Append( ptr );
								if ( cl > 0 )
									{
									tran->iObserver.HttpDownloadProgress( tran->iId, 
																		  tran->iResponseBody.MaxLength(), 
																		  cl );
									}
								}
							}
						respBodyData->ReleaseData();
						}
					break;
				case THTTPEvent::EResponseComplete:
					{
					TInt cl = ContentLength( tran->iHttpTransaction.Response().GetHeaderCollection() );
					if ( cl > 0 )
						{
						tran->iObserver.HttpDownloadProgress( tran->iId, cl, cl );
						}
					}
					break;
				default:
					break;
				}
			}
		}
	else
		{
		aTransaction.Close();
		CloseConnection();
		}
	}

TInt CSGEHttpClient::MHFRunError( TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& /*aEvent*/ )
	{
	TTransaction* tran = FindTransaction( aTransaction );
	
	if ( tran )
		{
		tran->iError = aError;
		NotifyTransaction( tran );
		CloseTransaction( tran );
		}
	else
		{
		aTransaction.Close();
		CloseConnection();
		}
	
	return KErrNone;
	}

void CSGEHttpClient::EventL( const CConnMonEventBase &aConnMonEvent )
	{
	if ( aConnMonEvent.EventType() == EConnMonCreateConnection )
		{
//		if ( iAttachMode )
//		    {
//	        iConnectionObserver->HttpConnectionStarted( KErrNone );
//		    }
		}
	else if ( aConnMonEvent.EventType() == EConnMonDeleteConnection )
		{
		iConnection.Close();
		iConnectionStarted = EFalse;
		if ( iConnectionObserver )
			{
			iConnectionObserver->HttpConnectionClosed();
			}
		}
	}

void CSGEHttpClient::GPAOObserver( CSGEUtilsGPAO* /*aClient*/, TInt aError )
	{
	if ( aError == KErrNone || aError == KErrAlreadyExists )
		{
		TUint cntCount = 0;
		if ( iConnection.EnumerateConnections( cntCount ) == KErrNone )
			{
			if ( cntCount != 0 )
				{
				TConnectionInfoV2Buf ci;
				if ( iConnection.GetConnectionInfo( 1, ci ) == KErrNone )
					{
					iIap = ci().iIapId;
					}
				}
			}
		
		iHttpSession.ConnectionInfo().SetPropertyL( iHttpSession.StringPool().StringF( HTTP::EHttpSocketConnection,
													RHTTPSession::GetTable() ),
													THTTPHdrVal( reinterpret_cast<TInt>( &iConnection ) ) );

		iConnectionStarted = ETrue;
		}
	else
		{
		iConnection.Close();
		iIap = 0;
		iConnectionStarted = EFalse;
		}
	
	if ( iConnectionObserver )
		{
		iConnectionObserver->HttpConnectionStarted( aError );
		}
	}

TInt CSGEHttpClient::GetL( MSGEHttpClientObserver& aObserver, const TDesC& aUrl,
							const TDesC8& aUserAgent, const TDesC8& aContentType, const TDesC8& aAccept, const TDesC8& aConnectionClose )
	{
	if ( !iConnectionStarted )
		{
		return KErrNotReady;
		}
	
	RStringPool stringPool = iHttpSession.StringPool();
	RStringF stringMethodGet = stringPool.StringF( HTTP::EGET, RHTTPSession::GetTable() );

	CUri8* uri = UriUtils::CreateUriL( aUrl );
	CleanupStack::PushL( uri );
	
	TTransaction tran( aObserver );
	tran.iHttpTransaction = iHttpSession.OpenTransactionL( uri->Uri(), *this, stringMethodGet );
	tran.iId = ++iIdCounter;
            
	CleanupStack::PopAndDestroy( uri );

    RHTTPHeaders hdr = tran.iHttpTransaction.Request().GetHeaderCollection();
	SetHeaderL( hdr, HTTP::EUserAgent, aUserAgent );
	SetHeaderL( hdr, HTTP::EContentType, aContentType );
	SetHeaderL( hdr, HTTP::EAccept, aAccept );
	SetHeaderL( hdr, HTTP::EConnection, aConnectionClose );
	
	tran.iHttpTransaction.SubmitL();
	
	iTransactions.AppendL( tran );

    
//    THTTPHdrFieldIter i = hdr.Fields();
//    i.First();
//    for ( i.First(); !(i.AtEnd()); ++i )
//        {
//        RStringF header = iHttpSession.StringPool().StringF( i() );
//        
//        THTTPHdrVal val;
//        hdr.GetField( header, 0, val );
//        RStringF value = val.StrF();
//        
//        Logger::LogStr( _L("c:\\data\\eurl.txt"), header.DesC() );
//        Logger::LogStr( _L("c:\\data\\eurl.txt"), _L(": ") );
//        Logger::LogStr( _L("c:\\data\\eurl.txt"), value.DesC() );
//        Logger::LogStr( _L("c:\\data\\eurl.txt"), _L("\n") );
//        }
//    Logger::LogStr( _L("c:\\data\\eurl.txt"), _L("====\n") );
	
	return tran.iId;
	}

TInt CSGEHttpClient::GetL( MSGEHttpClientObserver& aObserver, const TDesC& aUrl, RFile aFile,
							const TDesC8& aUserAgent, const TDesC8& aContentType, const TDesC8& aAccept, const TDesC8& aConnectionClose )
	{
	if ( !iConnectionStarted )
		{
		return KErrNotReady;
		}
	
	RStringPool stringPool = iHttpSession.StringPool();
	RStringF stringMethodGet = stringPool.StringF( HTTP::EGET, RHTTPSession::GetTable() );

	CUri8* uri = UriUtils::CreateUriL( aUrl );
	CleanupStack::PushL( uri );
	
	TTransaction tran( aObserver );
	tran.iType = TTransaction::EResponseFile;
	tran.iFile = aFile;
	tran.iHttpTransaction = iHttpSession.OpenTransactionL( uri->Uri(), *this, stringMethodGet );
	tran.iId = ++iIdCounter;

	CleanupStack::PopAndDestroy( uri );

    RHTTPHeaders hdr = tran.iHttpTransaction.Request().GetHeaderCollection();
	SetHeaderL( hdr, HTTP::EUserAgent, aUserAgent );
	SetHeaderL( hdr, HTTP::EContentType, aContentType );
	SetHeaderL( hdr, HTTP::EAccept, aAccept );
	SetHeaderL( hdr, HTTP::EConnection, aConnectionClose );
	
	tran.iHttpTransaction.SubmitL();
	
	iTransactions.AppendL( tran );
	
	return tran.iId;
	}

TInt CSGEHttpClient::PostL( MSGEHttpClientObserver& aObserver, const TDesC& aUrl, const TDesC8& aBody,
							const TDesC8& aUserAgent, const TDesC8& aContentType, const TDesC8& aAccept, const TDesC8& aConnectionClose )
	{
	if ( !iConnectionStarted )
		{
		return KErrNotReady;
		}
	
	RStringPool stringPool = iHttpSession.StringPool();
	RStringF stringMethodPost = stringPool.StringF( HTTP::EPOST, RHTTPSession::GetTable() );

	CUri8* uri = UriUtils::CreateUriL( aUrl );
	CleanupStack::PushL( uri );
	
	TTransaction tran( aObserver );
	tran.iHttpTransaction = iHttpSession.OpenTransactionL( uri->Uri(), *this, stringMethodPost );
	tran.iId = ++iIdCounter;
	tran.iRequestBody.Set( aBody );

	CleanupStack::PopAndDestroy( uri );

	RHTTPHeaders hdr = tran.iHttpTransaction.Request().GetHeaderCollection();
	SetHeaderL( hdr, HTTP::EUserAgent, aUserAgent );
	SetHeaderL( hdr, HTTP::EContentType, aContentType );
	SetHeaderL( hdr, HTTP::EAccept, aAccept );
	SetHeaderL( hdr, HTTP::EConnection, aConnectionClose );
	
	tran.iHttpTransaction.Request().SetBody( tran );

	tran.iHttpTransaction.SubmitL();
	
	iTransactions.AppendL( tran );
	
	return tran.iId;
	}

void CSGEHttpClient::CancelTransaction( TInt aTransactionId )
	{
	for ( TInt i = 0; i < iTransactions.Count(); i++ )
		{
		if ( iTransactions[ i ].iId == aTransactionId )
			{
			iTransactions[ i ].iHttpTransaction.Cancel();
			CloseTransaction( &iTransactions[ i ] );
			return;
			}
		}
	}

void CSGEHttpClient::SetAutoCloseConnection( TBool aStatus )
	{
	if ( aStatus && !iAutoCloseConnection )
		{
		iAutoCloseConnection = aStatus;
		CloseConnection();
		}
	else
		{
		iAutoCloseConnection = aStatus;
		}
	}

TBool CSGEHttpClient::IsConnectionStarted() const
	{
	return iConnectionStarted;
	}

TUint32 CSGEHttpClient::Iap() const
	{
	return iIap;
	}

void CSGEHttpClient::SetIap( TUint32 aIap )
    {
    iIap = aIap;
    }

CSGEHttpClient::TTransaction* CSGEHttpClient::FindTransaction( RHTTPTransaction aHttpTransaction )
	{
	for ( TInt i = 0; i < iTransactions.Count(); i++ )
		{
		if ( iTransactions[ i ].iHttpTransaction == aHttpTransaction )
			{
			return &iTransactions[ i ];
			}
		}
	
	return NULL;
	}

void CSGEHttpClient::NotifyTransaction( CSGEHttpClient::TTransaction* aTransaction )
	{
	aTransaction->iHttpTransaction.Close();
	aTransaction->iObserver.HttpResponse( aTransaction->iId, aTransaction->iError, aTransaction->iResponseBody );
	}

void CSGEHttpClient::CloseTransaction( CSGEHttpClient::TTransaction* aTransaction )
	{
	TInt idx = iTransactions.Find( *aTransaction );
	
	ASSERT( idx >= 0 );
	
	/*if ( aTransaction->iType == TTransaction::EResponseFile )
		{ // file is handled by the client!
		aTransaction->iFile.Close();
		}*/
	
	aTransaction->iResponseBody.Close();
	iTransactions.Remove( idx );
	
	CloseConnection();
	}

void CSGEHttpClient::CloseConnection()
	{
	if ( iAutoCloseConnection && iTransactions.Count() == 0 )
		{
		iConnectionStarted = EFalse;
		iConnection.Close();
		if ( iConnectionObserver )
			{
			iConnectionObserver->HttpConnectionClosed();
			}
		}
	}

void CSGEHttpClient::SetHeaderL( RHTTPHeaders aHeaders, TInt aHdrField, const TDesC8& aHdrValue )
	{
	RStringF valStr = iHttpSession.StringPool().OpenFStringL( aHdrValue );
	THTTPHdrVal val(valStr);
	aHeaders.SetFieldL( iHttpSession.StringPool().StringF(aHdrField,RHTTPSession::GetTable()), val );
	valStr.Close();
	}

TInt CSGEHttpClient::ContentLength( RHTTPHeaders aHeaders )
	{
	THTTPHdrVal val;
	TInt err = aHeaders.GetField( iHttpSession.StringPool().StringF(HTTP::EContentLength,RHTTPSession::GetTable()), 0, val );
	if ( err )
		{
		return err;
		}
	return val.Int();
	}

