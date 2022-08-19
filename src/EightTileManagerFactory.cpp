/*
 ============================================================================
 Name		: EightTileManagerFactory.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileManagerFactory implementation
 ============================================================================
 */

#include "EightTileManagerFactory.h"
#include "EightSettings.h"
#include "EightPhotosProvider.h"
#include "EightTile.h"
#include "EightTileSpecial.h"
#include "EightTileAds.h"
#include "EightTileAppShortcut.h"
#include "EightTileAppPhone.h"
#include "EightTileAppCalendar.h"
#include "EightTileAppMessaging.h"
#include "EightTileAppEmail.h"
#include "EightTileAppClock.h"
#include "EightTileAppWindow.h"
#include "EightTileAppPhotos.h"
#include "EightTileUiStatic.h"
#include "EightTileUiDynFlip.h"
#include "EightTileUiDynCycle.h"
#include "EightTileUiDynMosaic.h"
#include "EightWindowAbout.h"
#include "EightWindowHelp.h"
#include "EightWindowSetup.h"
#include "EightWindowColor.h"
#include "EightWindowPremium.h"
#include <BAUTILS.H>
#include <VG/vgu.h>
#include <VG/openvg.h>
#include <SGEVg/VgImageManager.h>


const TUint32 KStreamId = ( '8' | ( 'S' << 8 ) | ( 'T' << 16 ) | ( 'M' << 24 ) );
const TUint16 KStreamVersion = 0x0001;




CEightTileManagerFactory::CEightTileManagerFactory( TSize aSize, CCoeControl* aControl ) : iSize( aSize ), iControl( aControl )
    {
    }

CEightTileManagerFactory::~CEightTileManagerFactory()
    {
    for ( TInt i = 0; i < iSvg.Count(); i++ )
        {
        delete iSvg[ i ].iImage;
        }
    iLs.Close();
    }

CEightTileManagerFactory* CEightTileManagerFactory::NewL( TSize aSize, CCoeControl* aControl )
    {
    CEightTileManagerFactory* self = new ( ELeave ) CEightTileManagerFactory( aSize, aControl );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self;
    return self;
    }

void CEightTileManagerFactory::ConstructL()
    {
    User::LeaveIfError( iLs.Connect() );
    LoadSvgImagesL();
    LoadPhotoImagesL();
    }

void CEightTileManagerFactory::LoadPhotoImagesL()
    {
    CEightPhotosProvider* pp = CEightPhotosProvider::GetInstanceL();

    pp->AddImageFileToSource( 0x100, 1, _L("\\photos\\yellow.jpg") );
    pp->AddImageFileToSource( 0x100, 1, _L("\\photos\\lights.jpg") );
    pp->AddImageFileToSource( 0x100, 1, _L("\\photos\\grass.jpg") );
    pp->AddImageFileToSource( 0x100, 1, _L("\\photos\\bluedrops.jpg") );
    pp->AddImageFileToSource( 0x100, 1, _L("\\photos\\stones.jpg") );
    }

void CEightTileManagerFactory::LoadSvgImagesL()
    {
    CSGEFileProvider& fp = CEightSettings::GetInstance()->FileProvider();

    CSGEFile& sf = fp.GetSGEFileL( 1 );
    
    iSvg.Reset();
    iSvg[ KIdxTileAbout ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\about.svg") );
    iSvg[ KIdxTileAbout ].iTitle.Set( _L("About") );
    iSvg[ KIdxTileAbout ].iShortcut = ETrue;
    iSvg[ KIdxTileCalc ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\calc.svg") );
    iSvg[ KIdxTileCalc ].iTitle.Set( _L("Calculator") );
    iSvg[ KIdxTileCalc ].iShortcut = ETrue;
    iSvg[ KIdxTileCalc ].iExeName.Set( _L("calcsoft.exe") );
    iSvg[ KIdxTileCalendar ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\calendar.svg") );
    iSvg[ KIdxTileCalendar ].iTitle.Set( _L("Calendar") );
    iSvg[ KIdxTileCalendar ].iShortcut = ETrue;
    iSvg[ KIdxTileCalendar ].iExeName.Set( _L("calendar.exe") );
    iSvg[ KIdxTileCamera ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\camera.svg") );
    iSvg[ KIdxTileCamera ].iTitle.Set( _L("Camera") );
    iSvg[ KIdxTileCamera ].iShortcut = ETrue;
    iSvg[ KIdxTileCamera ].iExeName.Set( _L("cameraapp.exe") );
    iSvg[ KIdxTileClock ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\clock.svg") );
    iSvg[ KIdxTileClock ].iTitle.Set( _L("Clock") );
    iSvg[ KIdxTileClock ].iShortcut = ETrue;
    iSvg[ KIdxTileClock ].iExeName.Set( _L("clock.exe") );
    iSvg[ KIdxTileEmail ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\email.svg") );
    iSvg[ KIdxTileEmail ].iTitle.Set( _L("E-mail") );
    iSvg[ KIdxTileEmail ].iShortcut = ETrue;
    iSvg[ KIdxTileEmail ].iExeName.Set( _L("freestyleemailui.exe") );
    iSvg[ KIdxTileFacebook ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\facebook.svg") );
    iSvg[ KIdxTileFacebook ].iTitle.Set( _L("Facebook") );
    iSvg[ KIdxTileFacebook ].iShortcut = ETrue;
    iSvg[ KIdxTileFacebook ].iExeName.Set( _L("launcher_0x2002f957.exe") );
    iSvg[ KIdxTileFolder ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\folder.svg") );
    iSvg[ KIdxTileFolder ].iTitle.Set( _L("Files") );
    iSvg[ KIdxTileFolder ].iShortcut = ETrue;
    iSvg[ KIdxTileFolder ].iExeName.Set( _L("filemanager.exe") );
    iSvg[ KIdxTileHeadphones ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\headphones.svg") );
    iSvg[ KIdxTileHeadphones ].iTitle.Set( _L("Music") );
    iSvg[ KIdxTileHeadphones ].iShortcut = ETrue;
    iSvg[ KIdxTileHeadphones ].iExeName.Set( _L("mpx.exe") );
    iSvg[ KIdxTileHelp ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\help.svg") );
    iSvg[ KIdxTileHelp ].iTitle.Set( _L("Help") );
    iSvg[ KIdxTileHelp ].iShortcut = ETrue;
    iSvg[ KIdxTileWeb ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\inet.svg") );
    iSvg[ KIdxTileWeb ].iTitle.Set( _L("Web") );
    iSvg[ KIdxTileWeb ].iShortcut = ETrue;
    iSvg[ KIdxTileWeb ].iExeName.Set( _L("browserng.exe") );
    iSvg[ KIdxTileMaps ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\map.svg") );
    iSvg[ KIdxTileMaps ].iTitle.Set( _L("Maps") );
    iSvg[ KIdxTileMaps ].iShortcut = ETrue;
    iSvg[ KIdxTileMaps ].iExeName.Set( _L("map_0x20001f63.exe") );
    iSvg[ KIdxTileMaps ].iExeName2.Set( _L("smart2go.exe") );
    iSvg[ KIdxTileMenu ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\menu.svg") );
    iSvg[ KIdxTileMenu ].iTitle.Set( _L("Menu") );
    iSvg[ KIdxTileMenu ].iShortcut = ETrue;
    iSvg[ KIdxTileMenu ].iExeName.Set( _L("homescreen.exe") );
    iSvg[ KIdxTileMenu ].iExeName2.Set( _L("matrixmenu.exe") );
    iSvg[ KIdxTileRecorder ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\mic.svg") );
    iSvg[ KIdxTileRecorder ].iTitle.Set( _L("Recorder") );
    iSvg[ KIdxTileRecorder ].iShortcut = ETrue;
    iSvg[ KIdxTileRecorder ].iExeName.Set( _L("voicerecorder.exe") );
    iSvg[ KIdxTileMusic ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\music.svg") );
    iSvg[ KIdxTileMusic ].iTitle.Set( _L("Music") );
    iSvg[ KIdxTileMusic ].iShortcut = EFalse;
    iSvg[ KIdxTileNotes ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\notes.svg") );
    iSvg[ KIdxTileNotes ].iTitle.Set( _L("Notes") );
    iSvg[ KIdxTileNotes ].iShortcut = ETrue;
    iSvg[ KIdxTileNotes ].iExeName.Set( _L("notepad.exe") );
    iSvg[ KIdxTilePeople ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\people.svg") );
    iSvg[ KIdxTilePeople ].iTitle.Set( _L("People") );
    iSvg[ KIdxTilePeople ].iShortcut = ETrue;
    iSvg[ KIdxTilePeople ].iExeName.Set( _L("phonebook2.exe") );
    iSvg[ KIdxTilePeoples ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\peoples.svg") );
    iSvg[ KIdxTilePeoples ].iTitle.Set( _L("Peoples") );
    iSvg[ KIdxTilePeoples ].iShortcut = EFalse;
    iSvg[ KIdxTilePhone ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\phone.svg") );
    iSvg[ KIdxTilePhone ].iTitle.Set( _L("Phone") );
    iSvg[ KIdxTilePhone ].iShortcut = ETrue;
    iSvg[ KIdxTilePhone ].iExeName.Set( _L("phoneui.exe") );
    iSvg[ KIdxTilePlay ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\play.svg") );
    iSvg[ KIdxTilePlay ].iTitle.Set( _L("Play") );
    iSvg[ KIdxTilePlay ].iShortcut = EFalse;
    iSvg[ KIdxTilePoint ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\point.svg") );
    iSvg[ KIdxTilePoint ].iTitle.Set( _L("Maps") );
    iSvg[ KIdxTilePoint ].iShortcut = EFalse;
    iSvg[ KIdxTilePoint ].iExeName.Set( _L("landmarks.exe") );
    iSvg[ KIdxTileRadio ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\radio.svg") );
    iSvg[ KIdxTileRadio ].iTitle.Set( _L("FM Radio") );
    iSvg[ KIdxTileRadio ].iShortcut = ETrue;
    iSvg[ KIdxTileRadio ].iExeName.Set( _L("fmradio.exe") );
    iSvg[ KIdxTileSearch ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\search.svg") );
    iSvg[ KIdxTileSearch ].iTitle.Set( _L("Search") );
    iSvg[ KIdxTileSearch ].iShortcut = ETrue;
    iSvg[ KIdxTileSearch ].iExeName.Set( _L("srchuiapp.exe") );
    iSvg[ KIdxTileSearch ].iExeName2.Set( _L("smartsearch_0x2002ae6c.exe") );
    iSvg[ KIdxTileSearch ].iRunTwoTimes = ETrue;
    iSvg[ KIdxTileSettings ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\settings.svg") );
    iSvg[ KIdxTileSettings ].iTitle.Set( _L("Settings") );
    iSvg[ KIdxTileSettings ].iShortcut = ETrue;
    iSvg[ KIdxTileSettings ].iExeName.Set( _L("gsapp.exe") );
    iSvg[ KIdxTileSetup ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\setup.svg") );
    iSvg[ KIdxTileSetup ].iTitle.Set( _L("Setup") );
    iSvg[ KIdxTileSetup ].iShortcut = ETrue;
    iSvg[ KIdxTileStore ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\shop.svg") );
    iSvg[ KIdxTileStore ].iTitle.Set( _L("Store") );
    iSvg[ KIdxTileStore ].iShortcut = ETrue;
    iSvg[ KIdxTileStore ].iExeName.Set( _L("ovistore_2002D07F.exe") );
    iSvg[ KIdxTileStore ].iExeName2.Set( _L("nokiastoreclient.exe") );
    iSvg[ KIdxTileSkype ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\skype.svg") );
    iSvg[ KIdxTileSkype ].iTitle.Set( _L("Skype") );
    iSvg[ KIdxTileSkype ].iShortcut = ETrue;
    iSvg[ KIdxTileSkype ].iExeName.Set( _L("skypeui.exe") ); // 0x20024d7a
    iSvg[ KIdxTileMessaging ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\sms.svg") );
    iSvg[ KIdxTileMessaging ].iTitle.Set( _L("Messaging") );
    iSvg[ KIdxTileMessaging ].iShortcut = ETrue;
    iSvg[ KIdxTileMessaging ].iExeName.Set( _L("mce.exe") );
//    iSvg[ KIdxTileMessaging ].iExeName.Set( _L("smsviewer.exe") );
    iSvg[ KIdxTileSocial ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\social.svg") );
    iSvg[ KIdxTileSocial ].iTitle.Set( _L("Social") );
    iSvg[ KIdxTileSocial ].iShortcut = ETrue;
    iSvg[ KIdxTileSocial ].iExeName.Set( _L("launcher_0x2002699F.exe") );
    iSvg[ KIdxTileTwitter ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\twiter.svg") );
    iSvg[ KIdxTileTwitter ].iTitle.Set( _L("Twitter") );
    iSvg[ KIdxTileTwitter ].iShortcut = ETrue;
    iSvg[ KIdxTileTwitter ].iExeName.Set( _L("launcher_0x2002f958.exe") );
    iSvg[ KIdxTileVideos ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\video.svg") );
    iSvg[ KIdxTileVideos ].iTitle.Set( _L("Videos") );
    iSvg[ KIdxTileVideos ].iShortcut = ETrue;
    iSvg[ KIdxTileVideos ].iExeName.Set( _L("mpxvideoplayer.exe") );
    iSvg[ KIdxTileThemeColor ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\brush.svg") );
    iSvg[ KIdxTileThemeColor ].iTitle.Set( _L("Theme color") );
    iSvg[ KIdxTileThemeColor ].iShortcut = ETrue;
    iSvg[ KIdxTileThemeColor ].iExeName.Set( KNullDesC() );
    iSvg[ KIdxTilePhotos ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\postcard.svg") );
    iSvg[ KIdxTilePhotos ].iTitle.Set( _L("Photos") );
    iSvg[ KIdxTilePhotos ].iShortcut = ETrue;
    iSvg[ KIdxTilePhotos ].iExeName.Set( _L("glx.exe") );
    iSvg[ KIdxTileAds ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\ad.svg") );
    iSvg[ KIdxTileAds ].iTitle.Set( _L("Ads") );
    iSvg[ KIdxTileAds ].iShortcut = ETrue;
#ifndef EIGHT_PREMIUM
    iSvg[ KIdxTilePremium ].iImage = CVgSvgImage::NewL( sf, _L("\\icons\\star.svg") );
    iSvg[ KIdxTilePremium ].iTitle.Set( _L("Eight Premium") );
    iSvg[ KIdxTilePremium ].iShortcut = ETrue;
    iSvg[ KIdxTilePremium ].iExeName.Set( KNullDesC() );
    CEightPhotosProvider::GetInstanceL()->ImageManager().AddImageL( KPremiumTileImage, iSvg[ KIdxTilePremium ].iImage, EFalse );
#endif
    CEightPhotosProvider::GetInstanceL()->ImageManager().AddImageL( KThemeColorTileImage, iSvg[ KIdxTileThemeColor ].iImage, EFalse );
    CEightPhotosProvider::GetInstanceL()->ImageManager().AddImageL( KAboutTileImage, iSvg[ KIdxTileAbout ].iImage, EFalse );
    CEightPhotosProvider::GetInstanceL()->ImageManager().AddImageL( KSetupTileImage, iSvg[ KIdxTileSetup ].iImage, EFalse );
    CEightPhotosProvider::GetInstanceL()->ImageManager().AddImageL( KHelpTileImage, iSvg[ KIdxTileHelp ].iImage, EFalse );

    CEightPhotosProvider::GetInstanceL()->ImageManager().AddImageL( KWizzardTileImage1, iSvg[ KIdxTilePhone ].iImage, EFalse );
    CEightPhotosProvider::GetInstanceL()->ImageManager().AddImageL( KWizzardTileImage2, iSvg[ KIdxTileEmail ].iImage, EFalse );
    }

void CEightTileManagerFactory::CreateTilesPage( CEightTileManager* aTileManager )
    {
    CSGEFileProvider& fp = CEightSettings::GetInstance()->FileProvider();
    TFileName& fn = fp.AppendToPrivateFolder( _L("tiles.dat") );
    
    if ( BaflUtils::FileExists( fp.Fs(), fn ) )
        {
        RFileReadStream fileStream;
        if ( fileStream.Open( fp.Fs(), fn, EFileRead ) == KErrNone )
            {
            TRAPD( err, InternalizeL( aTileManager, fileStream ) );
            if ( err != KErrNone )
                {
                fileStream.Close();
                fp.Fs().Delete( fn );

                RPointerArray< CEightTile >& tiles = aTileManager->Tiles();
                tiles.ResetAndDestroy();
                RArray< TFixedArray< TInt, 4 > >& board = aTileManager->Board();
                board.Reset();
                
                CreateDefaultTilesL( aTileManager );
                }
            else
                {
                fileStream.Close();
                aTileManager->SetupBoardAfterInternalize( 1 );
                }
            }
        else
            {
            CreateDefaultTilesL( aTileManager );
            }
        }
    else
        {
        CreateDefaultTilesL( aTileManager );
        }
    }

void CEightTileManagerFactory::CreateShortcutsPage( CEightTileManager* aTileManager )
    {
    for ( TInt i = 0; i < iSvg.Count(); i++ )
        {
        if ( iSvg[ i ].iShortcut )
            {
            if ( !CheckAppExists( i ) ) continue;

            CEightTile* tile = CreateTile( i, NULL, EFalse );
            
            //aTileManager->AddIconTileL( i, iSvg[ i ].iImage, iSvg[ i ].iTitle, iSvg[ i ].iPinned );
            
            aTileManager->AddIconTileL( tile, iSvg[ i ].iImage, iSvg[ i ].iTitle, iSvg[ i ].iPinned );
            }
        }
    aTileManager->SetupBoard( 2 );
    }

void CEightTileManagerFactory::ResetStoredFile()
	{
    CSGEFileProvider& fp = CEightSettings::GetInstance()->FileProvider();
    TFileName& fn = fp.AppendToPrivateFolder( _L("tiles.dat") );
    BaflUtils::DeleteFile( fp.Fs(), fn );
	}

TBool CEightTileManagerFactory::AddTile( CEightTileManager* aTileManager, TUint aTileId, TBool aCustomColor, const TEightPalette& aPalette )
    {
    if ( aTileId < (TUint)iSvg.Count() )
        {
//        CEightTileUiStatic* tileUi = new (ELeave) CEightTileUiStatic();
//        tileUi->AddSvgImageL( TVgRect(), iSvg[ aTileId ].iImage );
//    
//        aTileManager->InsertNewTileL( CEightTile::NewL( aTileId, ETileSize_1x1, tileUi ) );
        
        CEightTileUi* ui = CreateTileUi( aTileId );
        CEightTile* tile = CreateTile( aTileId, ui, EFalse );
        if ( aCustomColor )
        	{
        	tile->SetCustomColor( aCustomColor );
        	tile->UpdateColorsFromPalette( aPalette );
        	}
        aTileManager->InsertNewTileL( tile );
        
        return ETrue;
        }  
    return EFalse;
    }

CEightTileUi* CEightTileManagerFactory::CreateTileUi( TInt idx )
    {
    switch ( idx )
        {
        case KIdxTilePhone:
                {
                CEightTileUiStatic* phone = new (ELeave) CEightTileUiStatic();
                phone->AddSvgImageL( TVgRect(), iSvg[ KIdxTilePhone ].iImage );
                return phone;
                }
            break;
        case KIdxTilePeople:
                {
                CEightTileUiDynMosaic* people = new (ELeave) CEightTileUiDynMosaic( 3000000, 10000000, 3, KPhotosSourceIdContacts );
                CEightTileUiStatic* text = new (ELeave) CEightTileUiStatic();
                text->AddTextL( TVgRect( TVgPoint( 6, 70 ), TVgPoint( 70, 6 ) ), KFontId_SegoeWP_medium, iSvg[ idx ].iTitle, EFalse, ESGEFontAlignHorizontalLeft, ESGEFontAlignVerticalBottom );
                people->AddStaticUi( text );
                return people;
                }
            break;
        case KIdxTileCalendar:
                {
                CEightTileUiStatic* calendar = new (ELeave) CEightTileUiStatic();
                calendar->AddSvgImageL( TVgRect(), iSvg[ KIdxTileCalendar ].iImage );
                return calendar;
                }
            break;
        case KIdxTilePhotos:
                {
                CEightTileUiDynCycle* photos = new (ELeave) CEightTileUiDynCycle( KPhotosSourceIdInternalFiles );
                CEightTileUiStatic* text = new (ELeave) CEightTileUiStatic();
                text->AddTextL( TVgRect( TVgPoint( 6, 70 ), TVgPoint( 70, 6 ) ), KFontId_SegoeWP_medium, _L("Photos"), EFalse, ESGEFontAlignHorizontalLeft, ESGEFontAlignVerticalBottom );
                photos->AddStaticUi( text );
                return photos;
                }
            break;
//        case KIdxTileClock:
//                {
//                CEightTileUiStatic* clock = new (ELeave) CEightTileUiStatic();
//                clock->AddSvgImageL( TVgRect(), iSvg[ KIdxTileClock ].iImage );
//                return clock;
//                }
//            break;
        case KIdxTileAds:
                {
                CEightTileUiDynFlip* ads = new (ELeave) CEightTileUiDynFlip( 0, 0 );
                CEightTileUiStatic* adsf = new (ELeave) CEightTileUiStatic();
                adsf->AddTextL( TVgRect( TVgPoint( 6, 70 ), TVgPoint( 70, 6 ) ),  KFontId_SegoeWP_small, _L("Ads"), EFalse, ESGEFontAlignHorizontalLeft, ESGEFontAlignVerticalBottom );
                CEightTileUiStatic* adsb = new (ELeave) CEightTileUiStatic();
                adsb->AddTextL( TVgRect( TVgPoint( 6, 70 ), TVgPoint( 70, 6 ) ),  KFontId_SegoeWP_small, _L("Ads"), EFalse, ESGEFontAlignHorizontalLeft, ESGEFontAlignVerticalBottom );
                ads->SetTilesUi( adsf, adsb );
                return ads;
                }
            break;
        case KIdxTileSetup:
        case KIdxTileThemeColor:
        case KIdxTileAbout:
#ifndef EIGHT_PREMIUM        	
        case KIdxTilePremium:
#endif
        case KIdxTileHelp:
                {
                CEightTileUiStatic* tile = new (ELeave) CEightTileUiStatic();
                tile->AddSvgImageL( TVgRect(), iSvg[ idx ].iImage );
                return tile;
                }
            break;
        default:
                {
            	if ( idx >= KTilesDefsCount ) return NULL;
                CEightTileUiStatic* tile = new (ELeave) CEightTileUiStatic();
                tile->AddSvgImageL( TVgRect(), iSvg[ idx ].iImage );
                if ( idx < iSvg.Count() ) tile->AddTextL( TVgRect( TVgPoint( 6, 70 ), TVgPoint( 70, 6 ) ), KFontId_SegoeWP_medium, iSvg[ idx ].iTitle, EFalse, ESGEFontAlignHorizontalLeft, ESGEFontAlignVerticalBottom );
                return tile;
                }
            break;
        }
    }

CEightTile* CEightTileManagerFactory::CreateTile( TUint idx, CEightTileUi* ui, TBool aEmpty )
    {
    if ( aEmpty )
        {
        return CEightTile::NewL( idx, ETileSize_1x1, ui, aEmpty );
        }
    CEightTile *tile = NULL;
    
    switch ( idx )
        {
        case KIdxTilePhone:
                {
                tile = CEightTileAppPhone::NewL( KIdxTilePhone, ETileSize_2x2, ui );
                tile->SetFlippingSupport( EFalse );
                }
            break;
        case KIdxTilePeople:
                {
                tile = CEightTileAppShortcut::NewL( iSvg[ idx ].iExeName, KIdxTilePeople, ETileSize_2x2, ui );
                if ( iSvg[ idx ].iExeName2.Length() > 0 ) ((CEightTileAppShortcut*)tile)->SetExeName2L( iSvg[ idx ].iExeName2 );
                tile->SetFlippingSupport( EFalse );
                }
            break;
        case KIdxTileCalendar:
                {
                tile = CEightTileAppCalendar::NewL( KIdxTileCalendar, ETileSize_4x2, ui );
                //CEightTileAppShortcut::NewL( iSvg[ idx ].iExeName, KIdxTileCalendar, ETileSize_4x2, ui );
                //if ( iSvg[ idx ].iExeName2.Length() > 0 ) ((CEightTileAppShortcut*)tile)->SetExeName2L( iSvg[ idx ].iExeName2 );
                tile->SetFlippingSupport( EFalse );
                }
            break;
        case KIdxTileMessaging:
                {
                tile = CEightTileAppMessaging::NewL( KIdxTileMessaging, ETileSize_1x1, ui );
                //CEightTileAppShortcut::NewL( iSvg[ idx ].iExeName, KIdxTileCalendar, ETileSize_4x2, ui );
                //if ( iSvg[ idx ].iExeName2.Length() > 0 ) ((CEightTileAppShortcut*)tile)->SetExeName2L( iSvg[ idx ].iExeName2 );
                tile->SetFlippingSupport( EFalse );
                }
            break;
        case KIdxTileEmail:
                {
                tile = CEightTileAppEmail::NewL( KIdxTileEmail, ETileSize_1x1, ui );
                tile->SetFlippingSupport( EFalse );
                }
            break;
        case KIdxTilePhotos:
                {
                tile = CEightTileAppPhotos::NewL( iSvg[ idx ].iExeName, KIdxTilePhotos, ETileSize_2x2, ui );
                //((CEightTileAppPhotos*)tile)->SetCanBeRemoved( EFalse );
                //if ( iSvg[ idx ].iExeName2.Length() > 0 ) ((CEightTileAppShortcut*)tile)->SetExeName2L( iSvg[ idx ].iExeName2 );
                tile->SetFlippingSupport( EFalse );
                tile->SetCustomColorSupport( EFalse );
                }
            break;
        case KIdxTileClock:
                {
                tile = CEightTileAppClock::NewL( KIdxTileClock, ETileSize_2x2, ui );
                tile->SetFlippingSupport( EFalse );
                }
            break;
        case KIdxTileAds:
                {
                tile = CEightTileAds::NewL( iControl, KIdxTileAds, ETileSize_4x2, ui );
                tile->SetFlippingSupport( EFalse );
#ifndef EIGHT_PREMIUM
                tile->SetCanBeRemovedSupport( EFalse );
#endif
                }
            break;
        case KIdxTileAbout:
                {
                CEightWindowAbout* wnd = CEightWindowAbout::NewL( iSize );
                tile = CEightTileAppWindow::NewL( wnd, KIdxTileAbout, ETileSize_1x1, ui );
#ifndef EIGHT_PREMIUM
                tile->SetCanBeRemovedSupport( EFalse );
#endif
                }
            break;
        case KIdxTileSetup:
                {
                CEightWindowSetup* wnd = CEightWindowSetup::NewL( iSize );
                tile = CEightTileAppWindow::NewL( wnd, KIdxTileSetup, ETileSize_1x1, ui );
                }
            break;
        case KIdxTileHelp:
                {
                CEightWindowHelp* wnd = CEightWindowHelp::NewL( iSize );
                tile = CEightTileAppWindow::NewL( wnd, KIdxTileHelp, ETileSize_1x1, ui );
                }
            break;
        case KIdxTileThemeColor:
                {
                CEightWindowColor* wnd = CEightWindowColor::NewL( NULL, iSize );
                tile = CEightTileAppWindow::NewL( wnd, KIdxTileThemeColor, ETileSize_1x1, ui );
                }
            break;
#ifndef EIGHT_PREMIUM        	
        case KIdxTilePremium:
                {
                CEightWindowPremium* wnd = CEightWindowPremium::NewL( iSize );
                tile = CEightTileAppWindow::NewL( wnd, KIdxTilePremium, ETileSize_1x1, ui );
                tile->SetCanBeRemovedSupport( EFalse );
                }
            break;
#endif
        default:
                {
                if ( idx < KTilesDefsCount && iSvg[ idx ].iExeName.Length() > 0 )
                    {
                    tile = CEightTileAppShortcut::NewL( iSvg[ idx ].iExeName, idx, ETileSize_1x1, ui );
                    if ( iSvg[ idx ].iExeName2.Length() > 0 ) ((CEightTileAppShortcut*)tile)->SetExeName2L( iSvg[ idx ].iExeName2 );
                    if ( iSvg[ idx ].iRunTwoTimes ) ((CEightTileAppShortcut*)tile)->EnableRunTwoTimes();
                    }
                else
                    {
                    tile = CEightTile::NewL( idx, ETileSize_1x1, ui );
                    }
                }
            break;
        }
    
    if ( idx == KIdxTileFacebook )
        {
        tile->SetCustomColorSupport( EFalse );
        tile->SetCustomColor( ETrue );
        tile->SetBackgroundColor( TVgColor( 0x33518fff ) );
        tile->SetSelectedColor( TVgColor( 0x33518fff ) );
        tile->SetPressedColor( TVgColor( 0x33518fff ) );
        }

    return tile;
    }

TBool CEightTileManagerFactory::CheckAppExists( TInt idx )
    {
    if ( idx >= iSvg.Count() ) return ETrue;
    
    iLs.GetAllApps();
    TApaAppInfo info;
    while ( iLs.GetNextApp( info ) == KErrNone )
        {
        if ( info.iFullName.FindF( iSvg[ idx ].iExeName ) != KErrNotFound || ( iSvg[ idx ].iExeName2.Length() > 0 && info.iFullName.FindF( iSvg[ idx ].iExeName2 ) != KErrNotFound ) )
            {
            return ETrue;
            }
        }
    return EFalse;
    }

void CEightTileManagerFactory::AddTileTo0( CEightTileManager* aTileManager, TInt idx )
    {
    if ( !CheckAppExists( idx ) ) return;
    CEightTileUi* ui = CreateTileUi( idx );
    CEightTile* tile = CreateTile( idx, ui, EFalse );
    switch ( idx )
        {
        case KIdxTilePhotos:
        case KIdxTilePeople:
        case KIdxTileAds:
                {
                if ( idx < iSvg.Count() ) iSvg[ idx ].iPinned = ETrue;
                aTileManager->AddTileL( tile );
                }
            break;
        default:
                {
                iSvg[ idx ].iPinned = ETrue;
                aTileManager->AddTileL( tile );
                }
            break;
        }
    }

void CEightTileManagerFactory::CreateDefaultTilesL( CEightTileManager* aTileManager )
    {    
    AddTileTo0( aTileManager, KIdxTileClock );
    AddTileTo0( aTileManager, KIdxTilePhone );
    AddTileTo0( aTileManager, KIdxTilePeople );
    AddTileTo0( aTileManager, KIdxTileCalendar );
    AddTileTo0( aTileManager, KIdxTilePhotos );
    AddTileTo0( aTileManager, KIdxTileMessaging );
    AddTileTo0( aTileManager, KIdxTileHeadphones ); // KIdxTileMusic 
    AddTileTo0( aTileManager, KIdxTileSearch );
    AddTileTo0( aTileManager, KIdxTileEmail );
    AddTileTo0( aTileManager, KIdxTileMaps );
    AddTileTo0( aTileManager, KIdxTileCalc );
    AddTileTo0( aTileManager, KIdxTileSettings );
    AddTileTo0( aTileManager, KIdxTileWeb );
#ifndef EIGHT_PREMIUM
    AddTileTo0( aTileManager, KIdxTileAds );
#endif
    AddTileTo0( aTileManager, KIdxTileAbout );
    AddTileTo0( aTileManager, KIdxTileHelp );
    AddTileTo0( aTileManager, KIdxTileSetup );
    AddTileTo0( aTileManager, KIdxTileThemeColor );
#ifndef EIGHT_PREMIUM
    AddTileTo0( aTileManager, KIdxTilePremium );
    AddTileTo0( aTileManager, KIdxTileMenu );
    AddTileTo0( aTileManager, KIdxTileStore );
    AddTileTo0( aTileManager, KIdxTileRecorder );
#endif
    aTileManager->SetupBoard( 1 );
    }

void CEightTileManagerFactory::ExternalizeInternalL( CEightTileManager* aTileManager, RWriteStream& aStream ) const
    {
    aStream.WriteUint32L( KStreamId );
    aStream.WriteUint16L( KStreamVersion );
    
    aTileManager->PrepareForExternalize();
    const RPointerArray< CEightTile >& tiles = aTileManager->Tiles(); 
    const RArray< TFixedArray< TInt, 4 > >& board = aTileManager->Board();

    TInt cnt = 0;
    for ( TInt i = 0; i < tiles.Count(); i++ )
        {
        if ( !tiles[ i ]->IsEmpty() ) cnt++;
        }
    RHashMap< TInt, TInt > newIdx;
    aStream.WriteUint32L( cnt );
    for ( TInt i = 0, k = 0; i < tiles.Count(); i++ )
        {
        if ( tiles[ i ]->IsEmpty() ) continue;
        newIdx.Insert( i, k++ );
        aStream.WriteUint32L( tiles[ i ]->Id() );
        aStream.WriteUint32L( tiles[ i ]->Type() );
        aStream.WriteUint32L( tiles[ i ]->TileSize() );
        aStream.WriteUint32L( tiles[ i ]->IsEmpty() );
        aStream.WriteUint32L( tiles[ i ]->CustomColor() );
        aStream.WriteUint32L( tiles[ i ]->IsFlipping() );
        tiles[ i ]->SetFlipping( EFalse ); // for propper internalization
        aStream << tiles[ i ]->Palette();
        if ( tiles[ i ]->Ui() )
            {
            aStream.WriteUint32L( tiles[ i ]->Ui()->Type() );
            }
        else
            {
            aStream.WriteUint32L( 0 );
            }
        aStream << *tiles[ i ];
        if ( tiles[ i ]->Ui() )
            {
            aStream << *tiles[ i ]->Ui();
            }
        else
            {
            aStream.WriteUint32L( 0 );
            }
        }
    
    aStream.WriteUint32L( board.Count() );
    for ( TInt i = 0; i < board.Count(); i++ )
        {
        for ( TInt j = 0; j < 4; j++ )
            {
            if ( board[ i ][ j ] == KErrNotFound || ( ( board[ i ][ j ] & KTileSpecialTile ) == KTileSpecialTile ) )
                {
                aStream.WriteInt32L( board[ i ][ j ] );
                }
            else
                {
                TInt idx = ( board[ i ][ j ] & KTilePlaceholderMask );
                TInt* val = newIdx.Find( idx );
                if ( val )
                    {
                    TInt p = ( board[ i ][ j ] & KTilePlaceholder );
                    aStream.WriteInt32L( *val | p );
                    }
                else
                    {
                    aStream.WriteInt32L( KErrNotFound );
                    }
                }
            
            //aStream.WriteInt32L( board[ i ][ j ] );
            }
        }
    
    newIdx.Close();
    }

void CEightTileManagerFactory::ExternalizeL( CEightTileManager* aTileManager ) const
    {
    RFileWriteStream fileStream;

    CSGEFileProvider& fp = CEightSettings::GetInstance()->FileProvider();
    TFileName& fn = fp.AppendToPrivateFolder( _L("tiles.dat") );
    
    BaflUtils::EnsurePathExistsL( fp.Fs(), fn );
    
    if ( fileStream.Replace( fp.Fs(), fn, EFileWrite ) == KErrNone )
        {
        ExternalizeInternalL( aTileManager, fileStream );        
        fileStream.Close();
        }    
    }

void CEightTileManagerFactory::InternalizeL( CEightTileManager* aTileManager, RReadStream& aStream )
    {
    if ( aStream.ReadUint32L() != KStreamId ) User::Leave( KErrNotSupported );
    if ( aStream.ReadUint16L() != KStreamVersion ) User::Leave( KErrNotSupported );

    RPointerArray< CEightTile >& tiles = aTileManager->Tiles(); 
    RArray< TFixedArray< TInt, 4 > >& board = aTileManager->Board();
    
    TBool b = CEightSettings::GetInstance()->iShowBottomButtons;
    TInt c = aStream.ReadUint32L();
    for ( TInt i = 0; i < c; i++ )
        {
        TUint id = aStream.ReadUint32L();
        TEightTileType type = (TEightTileType) aStream.ReadUint32L();
        TEightTileSize ts = (TEightTileSize) aStream.ReadUint32L();
        TBool empty = aStream.ReadUint32L();
        TBool customColor = aStream.ReadUint32L();
        TBool flipping = aStream.ReadUint32L();
        TEightPalette palette;
        aStream >> palette;
        
        CEightTileUi::TEightTileUiType uiType = (CEightTileUi::TEightTileUiType) aStream.ReadUint32L();
        
        CEightTileUi* ui = NULL;
        if ( uiType != CEightTileUi::ENone && ( id & KEightTileSpecialIdMask ) == 0 )
            {
            ui = CreateTileUi( id );
            }
        
        CEightTile* tile = NULL;
        
        if ( ( id & KEightTileSpecialIdMask ) == (TUint) KEightTileSpecialIdMask )
            {
            tile = CEightTile::NewL( id, ts, ui, empty );
            }
        else
            {
            tile = CreateTile( id, ui, empty );
            tile->SetTileSize( ts );
            }
        tile->UpdateColorsFromPalette( palette );
        tile->SetCustomColor( customColor );
        aStream >> *tile;
        if ( ui )
            {
            aStream >> *ui;
            }
        else
            {
            aStream.ReadUint32L();
            }
        tile->SetFlipping( flipping );
    
        if ( ( id & KEightTileSpecialIdMask ) == (TUint) KEightTileSpecialIdMask && !b )
        	{
        	delete tile;
        	}
        else
        	{
        	tiles.AppendL( tile );
        	}
        }
    
    TInt r = aStream.ReadUint32L();
    for ( TInt i = 0; i < r; i++ )
        {
        TFixedArray< TInt, 4 > row;
        for ( TInt j = 0; j < 4; j++ )
            {
            TInt32 v = aStream.ReadInt32L();
            if ( v == KErrNotFound || ( ( v & KTileSpecialTile ) == KTileSpecialTile ) || ( ( v & KTilePlaceholderMask ) < c ) )
                {
                row[ j ] = v;
                }
            else
                {
                row[ j ] = KErrNotFound;
                }
            
//            TInt v = aStream.ReadInt32L();
//            if ( v == KErrNotFound || ( ( v & KTileSpecialTile ) == KTileSpecialTile ) )
//                {
//                row[ j ] = v;
//                }
//            else
//                {
//                TInt id = ( v & KTilePlaceholderMask );
//                TInt p = ( v & KTilePlaceholder );
//                
//                for ( TInt )
//                
//                }
            
//            row[ j ] = aStream.ReadInt32L();
            }

        if ( !b && row[ 1 ] == KTileSpecialTile && row[ 2 ] == KTileSpecialTile )
            {
            if ( row[ 0 ] >= 0 && row[ 0 ] < tiles.Count() )
                {
                tiles[ row[ 0 ] ]->SetEmpty();
                }
            if ( row[ 3 ] >= 0 && row[ 3 ] < tiles.Count() )
                {
                tiles[ row[ 3 ] ]->SetEmpty();
                }
            }
        else
            {
            board.AppendL( row );
            }
        }
    }

