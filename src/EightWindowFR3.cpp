/*
 ============================================================================
 Name		: EightWindowFR3.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightWindowFR3 implementation
 ============================================================================
 */

#include "EightWindowFR3.h"
#include "EightFormWindowManager.h"
#include "EightSettings.h"
#include "EightPhotosProvider.h"
#include "TouchButtonText.h"
#include "TouchControlStatic.h"
#include "EightTileUiStatic.h"
#include "EightCompilationSettings.h"
#include <SGEVg/VgPrimitives.h>


#ifndef EIGHT_PREMIUM
_LIT( KHelpText,  
"Eight supports shortcuts for almost each system\n\
application. Shortcuts to the user installed\n\
applications currently are not supported.\n\n\
Apart the applications shortcuts by pressing\n\
following tiles you can open custom application\n\
windows like: about, help, settings, theme color\n\
and information about premium version:\n" );
#else
_LIT( KHelpText,  
"Eight supports shortcuts for almost each system\n\
application. Shortcuts to the user installed\n\
applications currently are not supported.\n\n\
Apart the applications shortcuts by pressing\n\
following tiles you can open custom application\n\
windows like: about, help, settings and theme\n\
color:" );
#endif


CEightWindowFR3::CEightWindowFR3( TSize aSize )
	: CEightFormWindow( NULL, KEightWindowFR3Id, aSize )
	{
	}

CEightWindowFR3::~CEightWindowFR3()
	{
	for ( TInt i = 0; i < iTiles.Count(); i++ ) delete iTiles[ i ];
	}

CEightWindowFR3* CEightWindowFR3::NewL( TSize aSize )
	{
	CEightWindowFR3* self = new (ELeave) CEightWindowFR3( aSize );
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); // self;
	return self;
	}

void CEightWindowFR3::ConstructL()
	{
    CEightFormWindow::ConstructL( _L("Eight tiles") );
    
    CEightSettings* settings = CEightSettings::GetInstance();
    CVgImageManager& iman = CEightPhotosProvider::GetInstanceL()->ImageManager();

    TVgRect r1 = TVgRect( TVgPoint( iSize.iWidth - KButtonMargin - KButtonWidth, KButtonHeight + KButtonMargin ), TVgSize( KButtonWidth, KButtonHeight ) );
    TVgRect rt1 = r1; rt1.iBr.iY += KTextOffsetMedium;
    CTouchButtonText* btn = CTouchButtonText::NewL( this, 1, rt1, r1, _L("next"), KFontId_SegoeWP_medium, settings->FontManager() );
    AddButtonL( btn );
    
    TInt offsetY = WindowContentHeight();

    TVgRect r2 = TVgRect( TVgPoint( 25, offsetY ), TVgSize( iSize.iWidth - 30, 100 ) );
    CTouchControlStatic* text = CTouchControlStatic::NewL( 2, r2, KHelpText(), KFontId_SegoeWP_small, settings->FontManager() );
    //text->SetTextAlign( ESGEFontAlignHorizontalCenter, ESGEFontAlignVerticalCenter );
    AddButtonL( text );
    
    offsetY -= 165;
    
    //TVgRect r3b = TVgRect( TVgPoint( 12, offsetY ), TVgSize( iSize.iWidth - 20, 100 ) );
    //TVgPoint p3 = r3b.Center();
    //p3.iX -= r3b.Width() / 4;
    TInt s = 75;
    TVgRect r3 = TVgRect( TPoint( 12, offsetY ), TSize( s, s ) );
    TVgRect r3ui = TVgRect( TVgPoint( 0, r3.Size().iHeight ), r3.Size() );
    CEightTileUiStatic* ui = new(ELeave) CEightTileUiStatic();
    ui->AddSvgImageL( r3ui, (CVgSvgImage*)iman.Get( KAboutTileImage ), EFalse );
    iTiles[ 0 ] = CEightTile::NewL( 1, ETileSize_1x1, ui, EFalse );
    iTiles[ 0 ]->SetRect( r3 );
    
    TVgRect r4 = TVgRect( TPoint( 2 * 12 + s, offsetY ), TSize( s, s ) );
    ui = new(ELeave) CEightTileUiStatic();
    ui->AddSvgImageL( r3ui, (CVgSvgImage*)iman.Get( KHelpTileImage ), EFalse );
    iTiles[ 1 ] = CEightTile::NewL( 1, ETileSize_1x1, ui, EFalse );
    iTiles[ 1 ]->SetRect( r4 );
    
    TVgRect r5 = TVgRect( TPoint( 3 * 12 + 2 * s, offsetY ), TSize( s, s ) );
    ui = new(ELeave) CEightTileUiStatic();
    ui->AddSvgImageL( r3ui, (CVgSvgImage*)iman.Get( KSetupTileImage ), EFalse );
    iTiles[ 2 ] = CEightTile::NewL( 1, ETileSize_1x1, ui, EFalse );
    iTiles[ 2 ]->SetRect( r5 );
    
    TVgRect r6 = TVgRect( TPoint( 4 * 12 + 3 * s, offsetY ), TSize( s, s ) );
    ui = new(ELeave) CEightTileUiStatic();
    ui->AddSvgImageL( r3ui, (CVgSvgImage*)iman.Get( KThemeColorTileImage ), EFalse );
    iTiles[ 3 ] = CEightTile::NewL( 1, ETileSize_1x1, ui, EFalse );
    iTiles[ 3 ]->SetRect( r6 );

#ifndef EIGHT_PREMIUM
    TVgRect r7 = TVgRect( TPoint( 12, offsetY - 12 - s ), TSize( s, s ) );
    ui = new(ELeave) CEightTileUiStatic();
    ui->AddSvgImageL( r3ui, (CVgSvgImage*)iman.Get( KPremiumTileImage ), EFalse );
    iTiles[ 4 ] = CEightTile::NewL( 1, ETileSize_1x1, ui, EFalse );
    iTiles[ 4 ]->SetRect( r7 );
#endif
    }

void CEightWindowFR3::TouchButtonPressed( TUint aButtonId )
	{
	if ( aButtonId == 1 )
		{
		iWindowManager->CloseWindow( iWindowId );
		}
	}

void CEightWindowFR3::WindowClosedByGesture()
	{
	iWindowManager->CloseWindow( iWindowId );
	}

void CEightWindowFR3::Activated()
	{
	CEightFormWindow::Activated();
	
    TEightPalette pal = CEightSettings::GetInstance()->iPalette;

    for ( TInt i = 0; i < iTiles.Count(); i++ ) iTiles[ i ]->SetBackgroundColor( pal.iTileColor );
	}

void CEightWindowFR3::Draw()
	{
	CEightFormWindow::Draw();
	
	for ( TInt i = 0; i < iTiles.Count(); i++ ) iTiles[ i ]->Draw();
	}



