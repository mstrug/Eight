/*
 ============================================================================
 Name		: EightSettings.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightSettings implementation
 ============================================================================
 */

#include "EightSettings.h"
#include "EightCompilationSettings.h"
#include <SGEBitmap/SGEBitmapItem.h>
#include <SGEBitmap/SGEBitmapUtils.h>
#include <BAUTILS.H>
#include <dscstore.h> // dscstore.dll
#include <dscitem.h>
//#include "logger.h"


static CEightSettings* self = NULL;

const TUint32 KStreamId = ( '8' | ( 'S' << 8 ) | ( 'E' << 16 ) | ( 'T' << 24 ) );
const TUint16 KStreamVersion = 0x0001;


CEightSettings::CEightSettings() : iThemeColorIdx( KEightColorsDefaultIdx ), iPalette( iThemeColorIdx )
	{
	iScale[ ENormal ][ ETileSize_1x1 ].SetSize( 1, 1 );
	iScale[ ENormal ][ ETileSize_2x2 ].SetSize( 1, 1 );
    iScale[ ENormal ][ ETileSize_4x2 ].SetSize( 1, 1 );
    iScale[ ENormal ][ ETileSize_3x1 ].SetSize( 1, 1 );
	iScale[ EPressed ][ ETileSize_1x1 ].SetSize( 81.0f/75.0f, 81.0f/75.0f );
	iScale[ EPressed ][ ETileSize_2x2 ].SetSize( 168.0f/162.0f, 168.0f/162.0f );
    iScale[ EPressed ][ ETileSize_4x2 ].SetSize( 342.0f/336.0f, 168.0f/162.0f );
    iScale[ EPressed ][ ETileSize_3x1 ].SetSize( 81.0f/75.0f /*255.0f/249.0f*/, 81.0f/75.0f );
	iScale[ ESelected ][ ETileSize_1x1 ].SetSize( 81.0f/75.0f, 81.0f/75.0f );
	iScale[ ESelected ][ ETileSize_2x2 ].SetSize( 168.0f/162.0f, 168.0f/162.0f );
	iScale[ ESelected ][ ETileSize_4x2 ].SetSize( 342.0f/336.0f, 168.0f/162.0f );
    iScale[ ESelected ][ ETileSize_3x1 ].SetSize( 81.0f/75.0f /*255.0f/249.0f*/, 81.0f/75.0f );
	iScale[ EFaded ][ ETileSize_1x1 ].SetSize( 69.0f/75.0f, 69.0f/75.0f );
	iScale[ EFaded ][ ETileSize_2x2 ].SetSize( 156.0f/162.0f, 156.0f/162.0f );
	iScale[ EFaded ][ ETileSize_4x2 ].SetSize( 330.0f/336.0f, 156.0f/162.0f );
    iScale[ EFaded ][ ETileSize_3x1 ].SetSize( 69.0f/75.0f /*243.0f/249.0f*/, 69.0f/75.0f );
    iScale[ EStarting ][ ETileSize_1x1 ].SetSize( 1, 1);
    iScale[ EStarting ][ ETileSize_2x2 ].SetSize( 1, 1 );
    iScale[ EStarting ][ ETileSize_4x2 ].SetSize( 1, 1 );
    iScale[ EStarting ][ ETileSize_3x1 ].SetSize( 1, 1 );
//	iScale[ EStarting ][ ETileSize_1x1 ].SetSize( 83.0f/75.0f, 83.0f/75.0f );
//	iScale[ EStarting ][ ETileSize_2x2 ].SetSize( 170.0f/162.0f, 170.0f/162.0f );
//	iScale[ EStarting ][ ETileSize_4x2 ].SetSize( 344.0f/336.0f, 170.0f/162.0f );
//    iScale[ EStarting ][ ETileSize_3x1 ].SetSize( 83.0f/75.0f /*255.0f/249.0f*/, 83.0f/75.0f );
    ResetToDefaults();
	}

CEightSettings::~CEightSettings()
	{
	delete iFontManager;
	delete iSGEFileProvider;
	iImeiMd5.Close();
	}

void CEightSettings::ConstructL()
	{
	iFontManager = CVgFontManager::NewL();
	iSGEFileProvider = CSGEFileProvider::NewL();
	
	TFileName& fn = iSGEFileProvider->AppendToPrivateFolder( _L("eight.sge") );
	iSGEFileProvider->AddSGEFileL( fn, 1 );
	
	LoadFromFileL();
	}

void CEightSettings::SetIncreaseTileSizeOnTouch( TBool aEnabled )
	{
	iIncreaseTileSizeOnTouch = aEnabled;
	if ( iIncreaseTileSizeOnTouch )
		{
		iScale[ EPressed ][ ETileSize_1x1 ].SetSize( 81.0f/75.0f, 81.0f/75.0f );
		iScale[ EPressed ][ ETileSize_2x2 ].SetSize( 168.0f/162.0f, 168.0f/162.0f );
	    iScale[ EPressed ][ ETileSize_4x2 ].SetSize( 342.0f/336.0f, 168.0f/162.0f );
	    iScale[ EPressed ][ ETileSize_3x1 ].SetSize( 81.0f/75.0f /*255.0f/249.0f*/, 81.0f/75.0f );
		}
	else
		{
		iScale[ EPressed ][ ETileSize_1x1 ].SetSize( 1, 1 );
		iScale[ EPressed ][ ETileSize_2x2 ].SetSize( 1, 1 );
	    iScale[ EPressed ][ ETileSize_4x2 ].SetSize( 1, 1 );
	    iScale[ EPressed ][ ETileSize_3x1 ].SetSize( 1, 1 );
		}
	}

TBool CEightSettings::IncreaseTileSizeOnTouch() const
	{
	return iIncreaseTileSizeOnTouch;
	}

void CEightSettings::SetImeiMd5( const TDesC8& aImeiMd5 )
    {
    iImeiMd5.Close();
    iImeiMd5.Create( aImeiMd5.Length() );
    iImeiMd5.Copy( aImeiMd5 );
    }

const TDesC& CEightSettings::ImeiMd5() const
    {
    return iImeiMd5;
    }

void CEightSettings::UpdateAutostartL()
	{
	const TUid KUidDsc =
	    {
		KDefaultSymbianDsc
	    };
//	TBuf<64> name;
//	name.Append( iSGEFileProvider->InstallDrive() );
//	name.Append( _L(":\\sys\\bin\\Eight_0x2006AF94.exe") );

#ifdef EIGHT_PREMIUM
	CDscItem* item = CDscItem::NewLC( _L("Eight_0x2006AF94.exe"), KNullDesC() );
#else
    CDscItem* item = CDscItem::NewLC( _L("Eight_0x2006AF95.exe"), KNullDesC() );
#endif
	item->SetDscId( KUidDsc );
	item->SetNoOfRetriesL( 0 );
	item->SetStartInBackground( EFalse );
	item->SetStartMethodL( EFireAndForget );
	//item->SetStartupType( EStartApp );
	item->SetViewless( EFalse );
	
	RDscStore store;
	store.OpenL();
	CleanupClosePushL( store );
	
	TBool exist = store.ItemExistsL( *item );
	if ( iAutostart && !exist )
		{
		if ( !store.DscExistsL( KUidDsc ) )
			{
			store.CreateDscL( KUidDsc, _L("Default Symbian DSC") );
			}
		store.AddItemL( *item, ELast );
		}
	else if ( !iAutostart && exist )
		{
		store.DeleteItemL( *item );
		}
	
//	store.EnumOpenLC();
//	while ( CDscItem* itm = store.EnumReadNextL() )
//		{
//		Logger::LogStr( _L("c:\\data\\autostr8.txt"), itm->FileName() );
//		Logger::LogNum( _L("c:\\data\\autostr8.txt"), itm->DscId().iUid );
//		Logger::LogNum( _L("c:\\data\\autostr8.txt"), itm->ItemId() );
//		delete itm;
//		itm = NULL;
//		}
//	Logger::LogStr( _L("c:\\data\\autostr8.txt"), _L("") );
//	store.EnumClose();	
//	CleanupStack::PopAndDestroy(); //store.EnumClose()
	
	store.Close();
	CleanupStack::PopAndDestroy( 2 );
	}

void CEightSettings::ResetSettingsFile()
	{
    TFileName& fn = iSGEFileProvider->AppendToPrivateFolder( _L("settings.dat") );
    BaflUtils::DeleteFile( iSGEFileProvider->Fs(), fn );
	}

void CEightSettings::LoadFromFileL()
	{
    TFileName& fn = iSGEFileProvider->AppendToPrivateFolder( _L("settings.dat") );
    if ( BaflUtils::FileExists( iSGEFileProvider->Fs(), fn ) )
        {
        RFileReadStream fileStream;
        if ( fileStream.Open( iSGEFileProvider->Fs(), fn, EFileRead ) == KErrNone )
            {
            TRAPD( err, InternalizeL( fileStream ) );
            fileStream.Close();
            if ( err != KErrNone )
                {
                iSGEFileProvider->Fs().Delete( fn );
                }
            }
        }
	}

void CEightSettings::SaveToFileL()
	{
    TFileName& fn = iSGEFileProvider->AppendToPrivateFolder( _L("settings.dat") );
	RFileWriteStream fileStream;
	if ( fileStream.Replace( iSGEFileProvider->Fs(), fn, EFileWrite ) == KErrNone )
		{
		TRAPD( err, ExternalizeL( fileStream ) );
		fileStream.Close();
		if ( err != KErrNone )
			{
			iSGEFileProvider->Fs().Delete( fn );
			}
		}
	
	TRAPD( err, UpdateAutostartL() );
	if ( err )
		{
		}
//	Logger::LogStrNum( _L("c:\\data\\autostr8.txt"), _L("error: "), err );
	}

void CEightSettings::InternalizeL( RReadStream& aStream )
	{
    if ( aStream.ReadUint32L() != KStreamId ) User::Leave( KErrNotSupported );
    if ( aStream.ReadUint16L() != KStreamVersion ) User::Leave( KErrNotSupported );

    iThemeColorIdx = aStream.ReadUint32L();
    iPalette = TEightPalette( iThemeColorIdx );
    
    iShowBottomButtons = aStream.ReadUint32L();
    iAnimateAppStartStop = aStream.ReadUint32L();
    iAnimateWindows = aStream.ReadUint32L();
    SetIncreaseTileSizeOnTouch( aStream.ReadUint32L() );
    iAutostart = aStream.ReadUint32L();
    iCloseOnRedKeyPress = aStream.ReadUint32L();
    iUseTouchFeedback = aStream.ReadUint32L();
    iCaptureMenuKey = aStream.ReadUint32L();
    iAdClientId = aStream.ReadUint32L();
    iIap = aStream.ReadUint32L();
    iAdQueryShowed = aStream.ReadUint32L();
    
    TUint16 len = aStream.ReadUint16L();
    iImeiMd5.Close();
    iImeiMd5.Create( len );
    aStream.ReadL( iImeiMd5 );
	}

void CEightSettings::ExternalizeL( RWriteStream& aStream )
	{
	aStream.WriteUint32L( KStreamId );
	aStream.WriteUint16L( KStreamVersion );

	aStream.WriteUint32L( iThemeColorIdx );
	aStream.WriteUint32L( iShowBottomButtons );
    aStream.WriteUint32L( iAnimateAppStartStop );
    aStream.WriteUint32L( iAnimateWindows );
	aStream.WriteUint32L( iIncreaseTileSizeOnTouch );
	aStream.WriteUint32L( iAutostart );
    aStream.WriteUint32L( iCloseOnRedKeyPress );
    aStream.WriteUint32L( iUseTouchFeedback );
    aStream.WriteUint32L( iCaptureMenuKey );
    aStream.WriteUint32L( iAdClientId );
    aStream.WriteUint32L( iIap );
    aStream.WriteUint32L( iAdQueryShowed );

    aStream.WriteUint16L( iImeiMd5.Length() );
    aStream.WriteL( iImeiMd5 );
	}

CEightSettings* CEightSettings::GetInstance()
	{
    if ( !self )
        {
        self = new (ELeave) CEightSettings();
        self->ConstructL();
        }
    return self;
	}

void CEightSettings::Destroy()
    {
    delete self;
    self = NULL;
    }

void CEightSettings::ResetToDefaults()
	{
	iIncreaseTileSizeOnTouch = ETrue;
	iShowBottomButtons = ETrue;
	iAnimateAppStartStop = ETrue;
	iAnimateWindows = ETrue;
	iAutostart = EFalse;
	iCloseOnRedKeyPress = ETrue;
	iUseTouchFeedback = ETrue;
	iCaptureMenuKey = EFalse;
	iAdClientId = 0;
	iIap = 0;
	iAdQueryShowed = EFalse;
	
	UpdateAutostartL();
	}

void CEightSettings::SetThemeColorIndex( TInt aIdx )
    {
    if ( aIdx >= 0 && aIdx < KEightColorsCount )
        {
        iThemeColorIdx = aIdx;
        iPalette.SetColor( iThemeColorIdx );
        }
    }

TInt CEightSettings::ThemeColorIndex() const
    {
    return iThemeColorIdx;
    }

CVgFontManager& CEightSettings::FontManager()
	{
	return *iFontManager;
	}

CSGEFileProvider& CEightSettings::FileProvider()
    {
    return *iSGEFileProvider;
    }



