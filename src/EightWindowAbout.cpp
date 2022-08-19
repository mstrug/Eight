/*
 ============================================================================
 Name		: EightWindowAbout.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightWindowAbout implementation
 ============================================================================
 */

#include "EightWindowAbout.h"
#include "EightSettings.h"
#include "EightPhotosProvider.h"
#include "EightCompilationSettings.h"
#include "TouchButtonText.h"
#include "TouchControlStatic.h"
#include <SGEVg/VgPrimitives.h>
#include <SGEApplication/SGEApplication.h>


CEightWindowAbout::CEightWindowAbout( TSize aSize )
    : CEightFormWindow( NULL, KEightWindowAboutId, aSize )
    {
    }

CEightWindowAbout::~CEightWindowAbout()
    {
    }

CEightWindowAbout* CEightWindowAbout::NewL( TSize aSize )
    {
    CEightWindowAbout* self = new ( ELeave ) CEightWindowAbout( aSize );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self;
    return self;
    }

void CEightWindowAbout::ConstructL()
    {
    CEightFormWindow::ConstructL( _L("") );
    
    CEightSettings* settings = CEightSettings::GetInstance();
    CVgImageManager& imgMan = CEightPhotosProvider::GetInstanceL()->ImageManager();

    TVgRect r1 = TVgRect( TVgPoint( iSize.iWidth - KButtonMargin - KButtonWidth, KButtonHeight + KButtonMargin ), TVgSize( KButtonWidth, KButtonHeight ) );
    TVgRect rt1 = r1; rt1.iBr.iY += KTextOffsetMedium;
    CTouchButtonText* btn = CTouchButtonText::NewL( this, 1, rt1, r1, _L("close"), KFontId_SegoeWP_medium, settings->FontManager() );
    AddButtonL( btn );

    TInt offsetY = WindowContentHeight() + 40;

    TVgRect r2 = TVgRect( TVgPoint( 0, offsetY ), TVgSize( iSize.iWidth, 100 ) );
    CTouchControlStatic* icon = CTouchControlStatic::NewL( 2, r2, KIconImage, imgMan );
    AddButtonL( icon );
    
    offsetY -= 110;

    TVgRect r3 = TVgRect( TVgPoint( 0, offsetY ), TVgSize( iSize.iWidth, 20 ) );
    CTouchControlStatic* appname = CTouchControlStatic::NewL( 3, r3,
#ifdef EIGHT_PREMIUM
                                                              _L("Eight 1.0.0"),
#else
                                                                      _L("Eight free 1.0.0"),
#endif
                                                              KFontId_SegoeWP_medium, settings->FontManager() );
    appname->SetTextAlign( ESGEFontAlignHorizontalCenter, ESGEFontAlignVerticalCenter );
    AddButtonL( appname );

    offsetY -= 70;

    TVgRect r4 = TVgRect( TVgPoint( 0, offsetY ), TVgSize( iSize.iWidth, 64 ) );
    CTouchControlStatic* logo = CTouchControlStatic::NewL( 4, r4, KLogoImage, imgMan );
    AddButtonL( logo );

    offsetY -= 100;

    TVgRect r5 = TVgRect( TVgPoint( 0, offsetY ), TVgSize( iSize.iWidth, 30 ) );
    CTouchControlStatic* name = CTouchControlStatic::NewL( 5, r5, _L("Micha\x7f Strug"), KFontId_SegoeWP_large, settings->FontManager() );
    name->SetTextAlign( ESGEFontAlignHorizontalCenter, ESGEFontAlignVerticalCenter );
    AddButtonL( name );

    offsetY -= 45;

    TVgRect r6 = TVgRect( TVgPoint( 0, offsetY ), TVgSize( iSize.iWidth, 30 ) );
    CTouchControlStatic* cpr = CTouchControlStatic::NewL( 6, r6, _L("\x80 2013"), KFontId_SegoeWP_large, settings->FontManager() );
    cpr->SetTextAlign( ESGEFontAlignHorizontalCenter, ESGEFontAlignVerticalCenter );
    AddButtonL( cpr );

    offsetY -= 80;

    TVgRect r7 = TVgRect( TVgPoint( 30, offsetY ), TVgSize( iSize.iWidth - 30 - 30, 50 ) );
    TVgRect r7u = TVgRect( TVgPoint( 43, offsetY - 2 ), TVgSize( iSize.iWidth - 40 - 42, 36 ) );
    TVgRect rt7 = r7; rt7.iBr.iY += 16; //rt7.iTl.iY += 8;
    
    CTouchButtonText* www = CTouchButtonText::NewL( this, 7, rt7, r7, _L("www.mbrsoftware.com"), KFontId_SegoeWP_large, settings->FontManager() );
    www->SetBorderWidth( 0 );
    TVgColor link( 0x1E74FFFF );
    www->SetLinkStyle( r7u, link );
    AddButtonL( www );
    }

void CEightWindowAbout::WindowClosedByGesture()
    {
    if ( iObserver ) iObserver->FormWindowButtonAction( iWindowId, 1 ); // simulate close button press
    }

void CEightWindowAbout::TouchButtonPressed( TUint aId )
	{
	if ( aId == 7 )
		{
		CSGEApplication::OpenBrowserL( _L("http://www.mbrsoftware.com") );
		}
	else
		{
		CEightFormWindow::TouchButtonPressed( aId );
		}
	}

void CEightWindowAbout::Draw()
    {
    CEightFormWindow::Draw();
    }

