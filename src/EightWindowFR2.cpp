/*
 ============================================================================
 Name		: EightWindowFR2.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightWindowFR2 implementation
 ============================================================================
 */

#include "EightWindowFR2.h"
#include "EightFormWindowManager.h"
#include "EightSettings.h"
#include "EightPhotosProvider.h"
#include "TouchButtonText.h"
#include "TouchControlStatic.h"
#include <SGEVg/VgPrimitives.h>


_LIT( KHelpText,  
"Eight lets you change the layout of the tiles freely.\n\
To do it long touch any of the tiles to enter edit\n\
mode.\n\n\n\n\n\n\n\n\n\
In edit mode you can move, delete and resize tiles.\n\
Also each tile has its own options like tile color which\n\
can be set differently from theme color and flipping\n\
mode. Some of the tiles like Photos or Ads has\n\
additional settings.\n\n\
On the second screen with all supported application\n\
shortcuts, edit mode lets you to pin and unpin the\n\
applications to the first screen.\n\
Second screen is accessible by swapping to left or\n\
pressing arrow button." );


CEightWindowFR2::CEightWindowFR2( TSize aSize )
	: CEightFormWindow( NULL, KEightWindowFR2Id, aSize )
	{
	}

CEightWindowFR2::~CEightWindowFR2()
	{
	delete iTile;
	delete iTile2;
	delete iIcons;
	}

CEightWindowFR2* CEightWindowFR2::NewL( TSize aSize )
	{
	CEightWindowFR2* self = new (ELeave) CEightWindowFR2( aSize );
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); // self;
	return self;
	}

void CEightWindowFR2::ConstructL()
	{
    CEightFormWindow::ConstructL( _L("Tiles") );
    
    CEightSettings* settings = CEightSettings::GetInstance();
    CVgImageManager& iman = CEightPhotosProvider::GetInstanceL()->ImageManager();

    TVgRect r1 = TVgRect( TVgPoint( iSize.iWidth - KButtonMargin - KButtonWidth, KButtonHeight + KButtonMargin ), TVgSize( KButtonWidth, KButtonHeight ) );
    TVgRect rt1 = r1; rt1.iBr.iY += KTextOffsetMedium;
    CTouchButtonText* btn = CTouchButtonText::NewL( this, 1, rt1, r1, _L("next"), KFontId_SegoeWP_medium, settings->FontManager() );
    AddButtonL( btn );
    
    TInt offsetY = WindowContentHeight(); // iSize.iHeight - 30; 

    TVgRect r2 = TVgRect( TVgPoint( 25, offsetY ), TVgSize( iSize.iWidth - 30, 100 ) );
    CTouchControlStatic* text = CTouchControlStatic::NewL( 2, r2, KHelpText(), KFontId_SegoeWP_small, settings->FontManager() );
    //text->SetTextAlign( ESGEFontAlignHorizontalCenter, ESGEFontAlignVerticalCenter );
    AddButtonL( text );

    offsetY -= 70;
    
    TVgRect r3b = TVgRect( TVgPoint( 10, offsetY ), TVgSize( iSize.iWidth - 20, 100 ) );
    TVgPoint p3 = r3b.Center();
    p3.iX -= r3b.Width() / 4;
    TInt s = 82;
    TVgRect r3 = TVgRect( TPoint( p3.iX - s / 2, p3.iY + s / 2 ), TPoint( p3.iX + s / 2, p3.iY - s / 2 ) );
    TVgRect r3ui = TVgRect( TVgPoint( 0, r3.Size().iHeight ), r3.Size() );
    CEightTileUiStatic* ui = new(ELeave) CEightTileUiStatic();
    ui->AddSvgImageL( r3ui, (CVgSvgImage*)iman.Get( KWizzardTileImage1 ), EFalse );
    iTile = CEightTile::NewL( 1, ETileSize_1x1, ui, EFalse );
    iTile->SetRect( r3 );
    
    r3.Shrink( 5, 5 );
    iIcons = new(ELeave) CEightTileUiStatic();
    iIcons->AddSvgImageL( TVgRect( r3.iTl, TVgSize( 1, 1 ) ), (CVgSvgImage*)iman.Get( KTileManagerImageSettings ), EFalse );
    iIcons->AddSvgImageL( TVgRect( TVgPoint( r3.iBr.iX, r3.iTl.iY ), TVgSize( 1, 1 ) ), (CVgSvgImage*)iman.Get( KTileManagerImageRemove ), EFalse );
    iIcons->AddSvgImageL( TVgRect( r3.iBr, TVgSize( 1, 1 ) ), (CVgSvgImage*)iman.Get( KTileManagerImageArrow ), EFalse );
    

    
    TVgPoint p4 = r3b.Center();
    p4.iX += r3b.Width() / 4;
    TVgRect r4 = TVgRect( TPoint( p4.iX - s / 2, p4.iY + s / 2 ), TPoint( p4.iX + s / 2, p4.iY - s / 2 ) );
    TVgRect r4ui = TVgRect( TVgPoint( 0, r4.Size().iHeight ), r4.Size() );
    CEightTileUiStatic* ui2 = new(ELeave) CEightTileUiStatic();
    ui2->AddSvgImageL( r4ui, (CVgSvgImage*)iman.Get( KWizzardTileImage2 ), EFalse );
    iTile2 = CEightTile::NewL( 2, ETileSize_1x1, ui2, EFalse );
    iTile2->SetRect( r4 );

    r4.Shrink( 5, 5 );
    iIcons->AddSvgImageL( TVgRect( TVgPoint( r4.iBr.iX, r4.iTl.iY ), TVgSize( 1, 1 ) ), (CVgSvgImage*)iman.Get( KTileManagerImagePin ), EFalse );
	}

void CEightWindowFR2::TouchButtonPressed( TUint aButtonId )
	{
	if ( aButtonId == 1 )
		{
		iWindowManager->CloseWindow( iWindowId );
		}
	}

void CEightWindowFR2::WindowClosedByGesture()
	{
	iWindowManager->CloseWindow( iWindowId );
	}

void CEightWindowFR2::Activated()
	{
	CEightFormWindow::Activated();
	
    TEightPalette pal = CEightSettings::GetInstance()->iPalette;

    iTile->SetBackgroundColor( pal.iTileSelectedColor );
    iTile2->SetBackgroundColor( pal.iTileSelectedColor );
	}

void CEightWindowFR2::Draw()
	{
	CEightFormWindow::Draw();
	
	iTile->Draw();
	iTile2->Draw();
	iIcons->Draw();
	}


