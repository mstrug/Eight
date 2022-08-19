/*
 ============================================================================
 Name		: EightTileAds.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileAds implementation
 ============================================================================
 */
//http://www.developer.nokia.com/Community/Wiki/Using_an_already_active_connection_on_Symbian
//https://inneractive.jira.com/wiki/display/DevWiki/Server+API+integration

#include <es_enum.h> 
#include <es_enum_partner.h> 
#include <SGEApplication/SGEApplication.h> 
#include <cryptospi/cryptohashapi.h>
#include <cryptospi/cryptospidef.h>
#include <networking/dnd_err.h> 
#include "EightTileAds.h"
#include "EightTileUiStatic.h"
#include "EightTileUiDynFlip.h"
#include "EightTileUiDynCycle.h"
#include "EightTileUiDynMosaic.h"
#include "EightCompilationSettings.h"
#include "EightSettings.h"
#include "TouchButtonOnOff.h"
#include "logger.h"
#include <APGCLI.H> 
#include <APACMDLN.H> 
#include <APGWGNAM.H> 
#include <aknappui.h> 
#include <bautils.h> 


const TInt KAdRetryTime = 20000000; // 20 sec
const TInt KAdRetryTime2 = 10000000; // 10 sec


#define LOGERRORS( b, c ) if ( iErrorsLog ) Logger::LogStrNum( _L("c:\\data\\eight_errors.txt"), b, c )
//#define LOGERRORS( b, c ) 


/*
<tns:Response Error="OK" xsi:schemaLocation="http://www.inner-active.com/SimpleM2M/M2MResponse http://www.inner-active.com/Public/XML/SimpleM2M/M2MResponse.xsd ">
<tns:Client Id="99999"/>
<tns:Ad>
  <tns:Text/>
  <tns:URL>http://m2m1.inner-active.mobi/simpleM2M/ClientUpdateStatus?cn=MBRSoftware_TestApp_1_Nokia&v=&ci=99999&s=89297940&cip=31-0-106-194&po=551&re=4&lt=0&cc=PL</tns:URL>
  <tns:Image>http://cdeu.buzzcity.net/img.php?contentid=e17621a00171916c</tns:Image>
</tns:Ad>
</tns:Response>
*/

//_LIT( KAppId, "MBRSoftware_TestApp_1_Nokia" );
 
#ifdef EIGHT_PREMIUM
_LIT( KAppId, "MBRSoftware_Eight_Nokia" );
#else
_LIT( KAppId, "MBRSoftware_Eight_free_Nokia" );
#endif


_LIT8(KMimeType, "text/xml");

CEightTileAds::CEightTileAds( TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi )
	: CEightTile( aId, aTileSize, aUi, EFalse ), iForeground( ETrue ), iShowConnectionWarning( ETrue )
	{
	iImageUiIdx[ 0 ] = KErrNotFound;
	iImageUiIdx[ 1 ] = KErrNotFound;
	}

CEightTileAds::~CEightTileAds()
	{
    if ( iHttpClient )
        {
        TRAP_IGNORE( CEightSettings::GetInstance()->SaveToFileL() );
        
        CSGEFileProvider& fp = CEightSettings::GetInstance()->FileProvider();
        TFileName& fn = fp.AppendToPrivateFolder( _L("ad.tmp") );
        BaflUtils::DeleteFile( fp.Fs(), fn );
        }
	
	iAdUrl.Close();
	iAdUrlImage.Close();
	
    iAdUrls[ 0 ].Close();
    iAdUrls[ 1 ].Close();
	
	delete iImeiGetter;
	delete iXmlParser;
	delete iHttpClient;
    delete iTimer;
	}

CEightTileAds* CEightTileAds::NewL( CCoeControl* aControl, TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi )
	{
	CEightTileAds* self = new (ELeave) CEightTileAds( aId, aTileSize, aUi );
	CleanupStack::PushL(self);
	self->ConstructL( aControl );
	CleanupStack::Pop(); // self;
	return self;
	}

void CEightTileAds::ConstructL( CCoeControl* aControl )
	{
    BaseConstructL();
    
    LOGERRORS( _L("ConstructL 1"), iForeground );

    iControl = aControl;
    
	if ( iUi )
		{
		CEightSettings* settings = CEightSettings::GetInstance();
	    iClientId = settings->iAdClientId;

	    iErrorsLog = BaflUtils::FileExists( settings->FileProvider().Fs(), _L("d:\\system\\temp\\eight.err") );
	    
	    iTimer = CSGEUtilsGPTimer::NewL( *this );
	    
	    iHttpClient = CSGEHttpClient::NewL( settings->iIap );
 
		iXmlParser = CParser::NewL( KMimeType(), *this );

		const TDesC& imei = settings->ImeiMd5();
		if ( imei.Length() == 0 )
		    {
		    iImeiGetter = CSGEImeiGetter::NewL( this );
		    }
		
		iTimer->Start( 5000000, EFalse );
		
		iLastUpdateTime.HomeTime();
		
	    LOGERRORS( _L("ConstructL 2"), iForeground );

//	    iSocketServ.Connect();
//	    iConnection.Open( iSocketServ );
//	    
//	    TUint cntCount = 0;
//	    if ( iConnection.EnumerateConnections( cntCount ) == KErrNone )
//	        {
//	        if ( cntCount == 0 )
//	            {
//#ifdef EIGHT_PREMIUM
//	            // wait for connection
//	            iState = ENotConnected;
//#else
//                // show message about connection need
//	            iState = EConnectionNeededMessage;
//#endif
//	            }
//	        else if ( cntCount == 1 )
//	            {
//#ifdef EIGHT_PREMIUM
//                // download ad
//                iState = EDownloadingAd;
//                iIap = EIAAPNMODE_CURRENT_APN;
//#else
//                // attach to the connection and download ad
//	            TConnectionInfoV2Buf ci;
//	            if ( iConnection.GetConnectionInfo( 1, ci ) == KErrNone )
//	                {
//	                if ( iConnection.Attach( ci, RConnection::EAttachTypeNormal ) == KErrNone )
//	                    {
//	                    iState = EDownloadingAd;
//	                    iIap = ci().iIapId;
//	                    }
//	                else
//	                    {
//	                    iState = ENotConnected;
//	                    }
//	                }
//	            else
//	                {
//	                iState = ENotConnected;
//	                }
//#endif
//	            }
//	        else if ( cntCount > 1 )
//	            {
//	            // find wifi connection if available else get first connection
//	            TInt wifiIdx = KErrNotFound;
//	            for ( TInt i = 1; i <= cntCount; i++ )
//	                {
//	                TConnectionInfoV2Buf ci;
//	                if ( iConnection.GetConnectionInfo( i, ci ) == KErrNone && ci().ConnectionType() == EConnectionWLAN )
//	                    {
//                        if ( iConnection.Attach( ci, RConnection::EAttachTypeNormal ) == KErrNone )
//                            {
//                            wifiIdx = i;
//                            iState = EDownloadingAd;
//                            iIap = ci().iIapId;
//                            break;
//                            }
//	                    }
//	                }
//	            if ( wifiIdx == KErrNotFound )
//	                {
//	                for ( TInt i = 1; i <= cntCount; i++ )
//	                    {
//                        TConnectionInfoV2Buf ci;
//                        if ( iConnection.GetConnectionInfo( i, ci ) == KErrNone )
//                            {
//                            if ( iConnection.Attach( ci, RConnection::EAttachTypeNormal ) == KErrNone )
//                                {
//                                iState = EDownloadingAd;
//                                iIap = ci().iIapId;
//                                break;
//                                }
//                            else
//                                {
//                                iState = ENotConnected;
//                                }
//                            }
//                        else
//                            {
//                            iState = ENotConnected;
//                            }
//	                    }
//	                }
//	            }
//	        }
//	    else
//	        {
//            iState = ENotConnected;
//	        }
//	    
//#ifdef EIGHT_PREMIUM
//        if ( iState == ENotConnected )
//            {
//            // start connection notifications
//            iConnection.Close();
//            iSocketServ.Close();
//            }
//#else
//	    if ( iState == ENotConnected )
//	        {
//            iState = EConnectionNeededMessage;
//	        }
//#endif
//	    
//        if ( iState == EDownloadingAd )
//            {
//            iTimer->Start( 2000000, EFalse );
//            // delay load of ConstructAdL();
//            }
//        else if ( iState == EConnectionNeededMessage )
//            {
//            iConnection.Close();
//            iSocketServ.Close();
//            
//            iIap = EIAAPNMODE_APN_SELECTION_DIALOG;
//
//            iTimer->Start( 5000000, EFalse );
//            }
	        
		}
	}

void CEightTileAds::RequestAd()
	{
    TTime now;
    now.HomeTime();
    if ( !iForeground || now.MicroSecondsFrom( iLastUpdateTime ) > 1000000 ) 
        {
        iRequestedFromBackground = ETrue;
        LOGERRORS( _L("RequestAd1 "), iForeground );
        return;
        }
    
	if ( !iHttpClient->IsConnectionStarted() )
		{
		iHttpClient->StartConnectionL( this );
		}
	else
		{
		TRAPD( err, RequestAdL() );
		if ( err != KErrNone )
			{
			// retry
			LOGERRORS( _L("RequestAd "), err );

			iHttpState = ERetry;
			SheduleNextAdRequest( KAdRetryTime );
			}
		}
	}

void CEightTileAds::RequestAdL()
	{
    //_LIT( KUrl, "http://m2m1.inner-active.mobi/simpleM2M/clientRequestAd?aid=MBRSoftware_TestApp_1_Nokia&v=Sm2m-1.5.3-1.7.0&po=551&hid=354864043521458" );
    RBuf url;
    url.Create( 500 );
    url.Copy( _L("http://m2m1.inner-active.mobi/simpleM2M/clientRequestAd?aid=") );
    url.Append( KAppId() );
    url.Append( _L("&v=Sm2m-1.5.3-1.7.0&po=551&hid=") );
    url.Append( CEightSettings::GetInstance()->ImeiMd5() );
    TTime time;
    time.UniversalTime();
    TTime time1970;
    time1970.Set( _L("19700101:000000") ); //YYYYMMDD:HHMMSS
    url.Append( _L("&time=") );
    url.AppendNum( time.Int64() - time1970.Int64() );
    if ( iClientId != 0 )
        {
        url.Append( _L("&cid=") );
        url.AppendNum( iClientId );
        }
    url.Append( _L("&k=life,fashion,car,fun,business&c=media&w=360&h=640&test=false") );
//    url.Append( _L("&k=life,fashion,car,fun,business&c=media&w=360&h=640&rw=320&rh=50&test=false") );
    
//    Logger::LogStr( _L("c:\\data\\eurl.txt"), url );
    _LIT8( KUA, "Mozilla/5.0 (Symbian/3; Series60/5.3 NokiaE7-00/111.040.1511; Profile/MIDP-2.1 Configuration/CLDC-1.1 ) AppleWebKit/535.1 (KHTML, like Gecko) NokiaBrowser/8.3.1.4 Mobile Safari/535.1" );
    TInt err = iHttpClient->GetL( *this, url, KUA() );
    if ( err > KErrNone )
    	{
	    iHttpState = EXMLDownloading;
    	}
    else
    	{
    	// retry
    	LOGERRORS( _L("RequestAdL "), err );

    	iHttpState = ERetry;
        SheduleNextAdRequest( KAdRetryTime );
    	}
    
    url.Close();
	}

void CEightTileAds::RequestImageL()
	{
	CSGEFileProvider& fp = CEightSettings::GetInstance()->FileProvider();
	TFileName& fn = fp.AppendToPrivateFolder( _L("ad.tmp") );
	TInt err = iAdFile.Replace( fp.Fs(), fn, EFileWrite );
	if ( err == KErrNone )
		{
		RBuf url;
		url.Create( iAdUrlImage.Length() );
		url.Copy( iAdUrlImage );
//	    Logger::LogStr( _L("c:\\data\\eurl.txt"), url );
		err = iHttpClient->GetL( *this, url, iAdFile );
		if ( err > KErrNone )
			{
			iHttpState = EIMGDownloading;
			}
		else
			{
			// retry
			LOGERRORS( _L("RequestImageL1 "), err );
	        
	        iHttpState = EXMLDone;
	        SheduleNextAdRequest( KAdRetryTime2 );
			}
		url.Close();
		}
	else
		{
		// retry
		LOGERRORS( _L("RequestImageL2 "), err );
		
        iHttpState = EXMLDone;
        SheduleNextAdRequest( KAdRetryTime2 );
		}
	}

void CEightTileAds::SheduleNextAdRequest( TInt aTime )
	{
    iTimer->Stop();
	iTimer->Start( aTime, EFalse );
	}

void CEightTileAds::SetMsgBox( CEightMsgBox* aMsgBox )
    {
    iMsgBox = aMsgBox;
    }

TInt CEightTileAds::CustomSettingsButtonsCount() const
    {
//#ifdef EIGHT_PREMIUM
//    return 4;
//#else
//    return 3;
//#endif
    return 3;
    }

CTouchControlBase* CEightTileAds::GetCustomSettingsButtonL( TInt aIdx )
    {
    if ( aIdx == 0 )
        {
        CTouchButtonText* btn = CTouchButtonText::NewL( NULL, 30, TVgRect(), TVgRect(), _L("reset IAP"), KFontId_SegoeWP_medium, CEightSettings::GetInstance()->FontManager() );
        return btn;
        }
    else if ( aIdx == 1 )
        {
        CTouchButtonText* btn = CTouchButtonText::NewL( NULL, 31, TVgRect(), TVgRect(), _L("reload Ad"), KFontId_SegoeWP_medium, CEightSettings::GetInstance()->FontManager() );
        return btn;
        }
    else if ( aIdx == 2 )
        {
        CTouchButtonOnOff* btn = CTouchButtonOnOff::NewL( NULL, 32, TVgRect(), _L("Internet connection warning"), KFontId_SegoeWP_medium, CEightSettings::GetInstance()->FontManager() );
        btn->SetTextOffset( 4 );
        btn->SetValue( iShowConnectionWarning );
        return btn;
        }
//    else if ( aIdx == 3 )
//        {
//#ifdef EIGHT_PREMIUM
//        CTouchButtonOnOff* btn = CTouchButtonOnOff::NewL( NULL, 33, TVgRect(), _L("Start connection automatically"), KFontId_SegoeWP_medium, CEightSettings::GetInstance()->FontManager() );
//        return btn;
//#endif
//        }
    return NULL;
    }

void CEightTileAds::HandleCustomSettingsButtonState( CTouchControlBase* aButton )
    {
    if ( aButton->Id() == 30 )
        {
        iMsgBox->RegisterObserver( this );
        iMsgBox->ShowL( 0x3010, CEightMsgBox::EMsgBoxButtonYesNo, _L("Do you want to reset stored Internet Access Point?\nThe new one you can select during Ad download.") );
        }
    else if ( aButton->Id() == 31 )
        {
        iMsgBox->ShowL( 0x3011, CEightMsgBox::EMsgBoxButtonOk, _L("New Ad will be loaded in 5 seconds.") );
        SheduleNextAdRequest( 5000000 );
        }
    else if ( aButton->Id() == 32 )
        {
        CTouchButtonOnOff* btn = (CTouchButtonOnOff*) aButton;
        iShowConnectionWarning = btn->IsOn();
        }
//    else if ( aButton->Id() == 33 )
//        {
//        CTouchButtonOnOff* btn = (CTouchButtonOnOff*) aButton;
//        btn->IsOn();
//        }
    }

TBool CEightTileAds::CanIncreaseSize() const
    {
    return EFalse;
    }

TBool CEightTileAds::CanDecreaseSize() const
    {
    return EFalse;
    }

TBool CEightTileAds::RunInternal()
    {
	if ( iHttpState != EIMGReady || iAdUrls[ iAdImageIdx ].Length() == 0 )
		{
	    if ( iHttpClient )
	        {
	        iMsgBox->ShowL( 0x3008, CEightMsgBox::EMsgBoxButtonOk, _L("No Ad available.\n\nWait until Ad download finishes or request Ad\nfrom tile settings.") );
	        }

		return EFalse;
		}

	RBuf url;
	url.Create( iAdUrls[ iAdImageIdx ].Length() );
	url.CleanupClosePushL();
	url.Copy( iAdUrls[ iAdImageIdx ] );

	CSGEApplication::OpenBrowserL( url );
	
	CleanupStack::PopAndDestroy(&url);
	return ETrue;
    }

TEightTileType CEightTileAds::Type() const
    {
    return ETileTypeAppShortcut;
    }

void CEightTileAds::HandleForegroundEventL( TBool aForeground )
    {
    if ( iHttpClient )
        {
        iForeground = aForeground;
        if ( iForeground && iRequestedFromBackground )
            {
            iLastUpdateTime.HomeTime();
            RequestAd();
            iRequestedFromBackground = EFalse;
            }
        }
    }

void CEightTileAds::Update( TTimeIntervalMicroSeconds32 aTimeInterval )
    {
    if ( iHttpClient )
        {
        TTime now;
        now.HomeTime();
        if ( iRequestedFromBackground && now.MicroSecondsFrom( iLastUpdateTime ) > 1000000 ) 
            {
            iLastUpdateTime = now;
            iRequestedFromBackground = EFalse;
            RequestAd();
            }
        else
            {
            iLastUpdateTime = now;
            }
        }
    CEightTile::Update( aTimeInterval );
    }

void CEightTileAds::GPTimerEvent( CSGEUtilsGPTimer* /*aTimer*/ )
    {
    CEightSettings* settings = CEightSettings::GetInstance();
    
    LOGERRORS( _L("GPTimerEvent "), iState );

    if ( iState == ENotConnected )
        {
        if ( !iShowConnectionWarning )
            {
            RequestAd();
            }
        else if ( iMsgBox->ShowL( 0x3001, CEightMsgBox::EMsgBoxButtonOk,
#ifdef EIGHT_PREMIUM
                             _L("Eight uses Internet connection to show Ads.")
#else                             
                             _L("Eight free uses Internet connection to show Ads.")
#endif
             ) )
            {
            iMsgBox->RegisterObserver( this );
            }
        else
            {
            iTimer->Start( 2000000, EFalse );
            }
        }
    else if ( iState == EConnectionNeededMessage )
        {
#ifdef EIGHT_PREMIUM
        if ( settings->iAdQueryShowed )
            {
            return;
            }
#endif
        if ( iMsgBox->ShowL( 0x3002, CEightMsgBox::EMsgBoxButtonOk,
#ifdef EIGHT_PREMIUM
                             _L("Eight requires Internet connection for showing Ads.\nIf you want update Ad now, please enable Wi-Fi or\nMobile data in system settings and press ok button.")
#else
                             _L("Eight free requires Internet connection.\nEnable Wi-Fi or Mobile data in system settings and\npress ok button.")
#endif
                             ) )
            {
            iMsgBox->RegisterObserver( this );
            }
        else
            {
            iTimer->Start( 2000000, EFalse );
            }
        }
    else if ( iState == EConnectionNeededMessage2 )
        {
#ifdef EIGHT_PREMIUM
        if ( settings->iAdQueryShowed )
            {
            return;
            }
#endif
#ifdef EIGHT_PREMIUM
        if ( iMsgBox->ShowL( 0x3003, CEightMsgBox::EMsgBoxButtonOk, _L("If you would like to start showing Ads later go to\nAds tile settings and press reload Ad button.") ) )
            {
            settings->iAdQueryShowed = ETrue;
            settings->SaveToFileL();
            iState = ENotConnected;        
#else
        if ( iMsgBox->ShowL( KMsgBoxMessageExit, CEightMsgBox::EMsgBoxButtonOk, _L("Application will exit now.\n\nIf you don't want to use Internet connection please\nbuy Eight premium version which doesn't require\nInternet connection.") ) )
            {
            // handled in CApplication::MsgBoxAction            
#endif
            }
        else
            {
            iTimer->Start( 2000000, EFalse );
            }
        }
    else if ( iState == EConnected )
        {
        if ( iHttpErrorCnt > 3 )
            {
#ifdef EIGHT_PREMIUM
            iHttpErrorCnt = 0;
            iMsgBox->ShowL( 0x3008, CEightMsgBox::EMsgBoxButtonOk,
                             _L("Cannot connect to Ad server. Check your Internet\nconnection to address:\nhttp://m2m1.inner-active.mobi/simpleM2M/") );
            }
#else
            iMsgBox->ShowL( KMsgBoxMessageExit, CEightMsgBox::EMsgBoxButtonOk,
                             _L("Application will exit now.\n\nCannot connect to Ad server. Check your Internet\nconnection to address:\nhttp://m2m1.inner-active.mobi/simpleM2M/") );
            }
        else 
#endif
        if ( iHttpState == EXMLDone )
            {
            RequestImageL();
            }
        else if ( iHttpState == EIMGReady || iHttpState == ERetry )
            {
            RequestAd();
            }
        }	
    }

void CEightTileAds::MsgBoxAction( TUint aMessageId, TInt aActionId )
    {
    iMsgBox->UnregisterObserver( this );
    if ( aMessageId == 0x3001 && aActionId == KMsgBoxButtonOk )
        {
        RequestAd();
        //SheduleNextAdRequest( 5000000 );
        }
    else if ( aMessageId == 0x3002 && aActionId == KMsgBoxButtonOk )
        {
        RequestAd();
        }
    else if ( aMessageId == 0x3010 && aActionId == KMsgBoxButtonYes )
        {
        iHttpClient->SetIap( 0 );
        }
    }

void CEightTileAds::ImageLoaded( TInt aImageId, TInt aError, TInt /*aSource*/ )
	{
	if ( aError == KErrNone )
		{
		CVgImageManager& iman = CEightPhotosProvider::GetInstanceL()->ImageManager();
		if ( iUi )
		    {
            CEightTileUiDynFlip* uid = (CEightTileUiDynFlip*)iUi;
            CEightTileUiStatic* uis = (CEightTileUiStatic*)uid->NextTileUi();
            
            if ( uid->IsFlipping() )
                {
                iman.RemoveImage( iAdImageId[ iAdImageIdx ] );
                iAdImageId[ iAdImageIdx ] = aImageId;
                iAdUrls[ iAdImageIdx ].Close();
                iAdUrls[ iAdImageIdx ].Create( iAdUrl );
                }
            else
                {
                if ( iAdImageIdx == 0 )
                    {
                    iman.RemoveImage( iAdImageId[ 1 ] );
                    iAdImageId[ 1 ] = aImageId;
                    iAdUrls[ 1 ].Close();
                    iAdUrls[ 1 ].Create( iAdUrl );
                    iAdImageIdx = 1;
                    }
                else
                    {
                    iman.RemoveImage( iAdImageId[ 0 ] );
                    iAdImageId[ 0 ] = aImageId;
                    iAdUrls[ 0 ].Close();
                    iAdUrls[ 0 ].Create( iAdUrl );
                    iAdImageIdx = 0;
                    }
                }
		
            MVgImage* ming = iman.Get( aImageId );
            if ( ming )
                {
                TSize s = ming->Size();
                if ( iImageUiIdx[ iAdImageIdx ] == KErrNotFound )
                    {
                    iImageUiIdx[ iAdImageIdx ] = uis->AddImageL( TVgRect(), (TVgImage*)ming, ETrue );
                    //iImageUiIdx[ iAdImageIdx ] = uis->AddImageL( TVgRect( TVgPoint( 0, iSize.iHeight ), TVgSize( iSize ) ), (TVgImage*)ming, ETrue/*, (VGfloat)( 75 * 4 + 12 * 3 ) / (VGfloat)ps.iWidth*/ );
                    }
                else
                    {
                    uis->UpdateImage( iImageUiIdx[ iAdImageIdx ], (TVgImage*)ming );
                    }
                uid->Flip();                
                
                CSGEFileProvider& fp = CEightSettings::GetInstance()->FileProvider();
                TFileName& fn = fp.AppendToPrivateFolder( _L("ad.tmp") );
                BaflUtils::DeleteFile( fp.Fs(), fn );
                
                iHttpState = EIMGReady;
                SheduleNextAdRequest(); // load next ad in specified time
                }
		    }
		}
	else
		{
		// retry
		LOGERRORS( _L("ImageLoaded "), aError );
        iHttpState = EXMLDone;
        SheduleNextAdRequest( KAdRetryTime2 );
		}
	}

void CEightTileAds::HttpConnectionStarted( TInt aError )
	{
	// aError == -1 when mobile data and wifi is disabled
	// aError == -3 when user cancels selection dialog
	// aError == -46 when user disables current iap bearer
    // aError == -30235?
	if ( aError == KErrPermissionDenied || aError == KErrNotFound || aError == KErrCancel || aError == -30235 )
		{
	    if ( iState == EConnectionNeededMessage )
	        {
	        iState = EConnectionNeededMessage2;	        
	        }
	    else
	        {
	        iState = EConnectionNeededMessage;
	        }
        iTimer->Start( 5000000, EFalse );
		//iHttpClient->StartConnectionL( this );
		}
	else if ( aError == KErrNone || aError == KErrAlreadyExists )
		{
        iState = EConnected;
		CEightSettings::GetInstance()->iIap = iHttpClient->Iap();
		RequestAd();
		}
	else
		{
		// retry
		LOGERRORS( _L("HttpConnectionStarted "), aError );
		
        SheduleNextAdRequest( 5000000 );
        //iTimer->Start( 5000000, EFalse );
		}
	}

void CEightTileAds::HttpConnectionClosed()
	{
	
	}

void CEightTileAds::HttpResponse( TInt /*aId*/, TInt aStatus, const TDesC8& aData )
	{
    if ( iHttpState == EXMLDownloading )
    	{
    	if ( aStatus == 200 )
    		{
    	    iHttpErrorCnt = 0;

    	    iHttpState = EXMLDone;
    	    
//    	    static int i = 0;
//    	    TBuf<84> fname;
//    	    fname.Copy( _L("c:\\data\\ehttp_") );
//    	    fname.AppendNum( i++ );
//            fname.Append( _L(".txt") );
//            Logger::LogStr( fname, aData );
    	    
    		iXmlParser->ParseBeginL();
    		iXmlParser->ParseL( aData );
    		iXmlParser->ParseEndL();
    		
    		if ( iAdUrlImage.Length() != 0 )
    			{
    			RequestImageL();
    			}
    		else
    			{
    			// retry
    			LOGERRORS( _L("HttpResponse1 "), iAdUrlImage.Length() );
                SheduleNextAdRequest();
    			}
    		}
    	else
    		{
    	    //if ( aStatus >= KErrDndNameNotFound )
    	        {
    	        iHttpErrorCnt++;
    	        }
    		// retry
    		LOGERRORS( _L("HttpResponse2 "), aStatus );
            iHttpState = ERetry;
            SheduleNextAdRequest();
    		}
    	}
    else if ( iHttpState == EIMGDownloading )
    	{
		iAdFile.Close();
    	if ( aStatus == 200 )
    		{
    	    iHttpErrorCnt = 0;

			CSGEFileProvider& fp = CEightSettings::GetInstance()->FileProvider();
			TFileName& fn = fp.AppendToPrivateFolder( _L("ad.tmp") );
			TInt err = CEightPhotosProvider::GetInstanceL()->LoadImageL( this, fn );
			if ( err > KErrNone )
				{
				iHttpState = EIMGDecoding;
				}
			else
				{
	    		// retry
				LOGERRORS( _L("HttpResponse3 "), err );
				
				iHttpState = EXMLDone;
                SheduleNextAdRequest();
	    		}
    		}
    	else
    		{
    	    iHttpErrorCnt++;

    	    // retry
    		LOGERRORS( _L("HttpResponse4 "), aStatus );
            
            iHttpState = EXMLDone;
            SheduleNextAdRequest();
    		}
    	}
	}

void CEightTileAds::OnStartDocumentL(const RDocumentParameters& /*aDocParam*/, TInt /*aErrorCode*/)
	{
	}

void CEightTileAds::OnEndDocumentL(TInt /*aErrorCode*/)
	{
	}

void CEightTileAds::OnStartElementL(const RTagInfo& aElement, const RAttributeArray& aAttributes, TInt /*aErrorCode*/)
	{
	TPtrC8 p1( aElement.Prefix().DesC() );
	TPtrC8 p2( aElement.LocalName().DesC() );
	RDebug::RawPrint( p1 );
	RDebug::RawPrint( p2 );
	
    if ( p2.CompareC( _L8("response") ) == 0 )
        {
        for ( TInt i = 0; i < aAttributes.Count(); i++ )
            {
            TPtrC8 p3( aAttributes[ i ].Attribute().LocalName().DesC() );
            if ( p3.CompareC( _L8("error") ) == 0 )
                {
                TPtrC8 p4( aAttributes[ i ].Value().DesC() );
                if ( p4.CompareC( _L8("ok") ) != 0 )
                    {
                	LOGERRORS( _L("ad error "), p4.Length() );
                    //Logger::LogStr( _L("c:\\data\\eurl.txt"), p4 );
                    //Logger::LogStr( _L("c:\\data\\eurl.txt"), _L("\n") );
                    }
                break;
                }
            }        
        }
    else if ( p2.CompareC( _L8("url") ) == 0 )
        {
        iHttpState = EXMLElementUrl;
        iAdUrl.Close();
        iAdUrl.Create( 2048 );
        }
	else if ( p2.CompareC( _L8("image") ) == 0 )
		{
		iHttpState = EXMLElementImg;
		iAdUrlImage.Close();
		iAdUrlImage.Create( 2048 );
		}
	else if ( p2.CompareC( _L8("client") ) == 0 )
	    {
	    for ( TInt i = 0; i < aAttributes.Count(); i++ )
	        {
	        TPtrC8 p3( aAttributes[ i ].Attribute().LocalName().DesC() );
	        if ( p3.CompareC( _L8("id") ) == 0 )
	            {
	            TLex8 lex;
	            TPtrC8 p4( aAttributes[ i ].Value().DesC() );
	            lex.Assign( p4 );
	            if ( lex.Val( iClientId, EDecimal ) != KErrNone )
	                {
	                iClientId = 0;
	                }
	            else
	                {
	                CEightSettings::GetInstance()->iAdClientId = iClientId;	                
	                break;
	                }
	            }
	        }
	    }
	}

void CEightTileAds::OnEndElementL(const RTagInfo& aElement, TInt /*aErrorCode*/)
	{
	TPtrC8 p2( aElement.LocalName().DesC() );
	if ( iHttpState == EXMLElementUrl && p2.CompareC( _L8("url") ) == 0 )
		{
		iAdUrl.ReAlloc( iAdUrl.Length() + 1 );
		iHttpState = EXMLDownloading;
		RDebug::RawPrint( iAdUrl );
		}
	else if ( iHttpState == EXMLElementImg && p2.CompareC( _L8("image") ) == 0 )
		{
		iAdUrlImage.ReAlloc( iAdUrlImage.Length() + 1 );
		iHttpState = EXMLDownloading;
		RDebug::RawPrint( iAdUrlImage );
		}
	//RDebug::Print( _L("end: %s / %s\n"), aElement.Prefix().DesC(), aElement.LocalName().DesC() );
	}

void CEightTileAds::OnContentL(const TDesC8& aBytes, TInt /*aErrorCode*/)
	{
	if ( iHttpState == EXMLElementUrl )
		{
		iAdUrl.Append( aBytes );
		}
	else if ( iHttpState == EXMLElementImg )
		{
		iAdUrlImage.Append( aBytes );
		}
	//RDebug::RawPrint( aBytes );
	}

void CEightTileAds::OnStartPrefixMappingL(const RString& /*aPrefix*/, const RString& /*aUri*/, TInt /*aErrorCode*/)
	{
	}

void CEightTileAds::OnEndPrefixMappingL(const RString& /*aPrefix*/, TInt /*aErrorCode*/)
	{
	}

void CEightTileAds::OnIgnorableWhiteSpaceL(const TDesC8& /*aBytes*/, TInt /*aErrorCode*/)
	{
	}

void CEightTileAds::OnSkippedEntityL(const RString& /*aName*/, TInt /*aErrorCode*/)
	{
	}

void CEightTileAds::OnProcessingInstructionL(const TDesC8& /*aTarget*/, const TDesC8& /*aData*/, TInt /*aErrorCode*/)
	{
	}

void CEightTileAds::OnError(TInt /*aErrorCode*/)
	{
	// todo
	}

TAny* CEightTileAds::GetExtendedInterface(const TInt32 /*aUid*/)
	{
	return NULL;
	}

void CEightTileAds::GotImeiL( const TDesC& aIMEI, TInt /*aError*/ )
    {
    using namespace CryptoSpi;
    CHash* hash = NULL;
    TRAPD( err, CHashFactory::CreateHashL( hash, KMd5Uid, KHashModeUid, NULL, NULL ) );
    if ( err == KErrNone )
        {    
        TBuf8<50> imei;
        //imei.Copy( aIMEI );
        //imei.Copy( _L("357482010609641") );
        
        if ( aIMEI.Length() == 0 )
            {
            imei.Append( 1 );          
            imei.Append( 0 );          
            imei.Append( 2 );          
            imei.Append( 3 );          
            imei.Append( 4 );          
            imei.Append( 5 );          
            imei.Append( 6 );          
            imei.Append( 7 );          
            imei.Append( 8 );          
            imei.Append( 9 );          
            }
        else
            {
            for ( TInt i = 0; i < aIMEI.Length(); i++ )
                {
                imei.Append( aIMEI[ i ] - 0x30 );
                }
            }
        
        TPtrC8 hashData( hash->Hash( imei ) );
        
        imei.Zero();
        for ( TInt i = 0; i < hashData.Length(); i++ )
            {
            imei.AppendNum( hashData[ i ], EHex );
            }

        CEightSettings* settings = CEightSettings::GetInstance();
        settings->SetImeiMd5( imei );

        delete hash;

        settings->SaveToFileL();
        }
    }

void CEightTileAds::ExternalizeL( RWriteStream& aStream ) const
    {
    aStream.WriteUint32L( 1 ); // size of additional data
    aStream.WriteUint8L( iShowConnectionWarning );
    }

void CEightTileAds::InternalizeL( RReadStream& aStream )
    {
    TUint size = aStream.ReadUint32L();
    if ( size == 1 )
        {
        iShowConnectionWarning = aStream.ReadUint8L();
        }
    }
