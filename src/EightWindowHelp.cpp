/*
 ============================================================================
 Name		: EightWindowHelp.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightWindowHelp implementation
 ============================================================================
 */

#include "EightWindowHelp.h"
#include "EightSettings.h"
#include "TouchButtonText.h"
#include "TouchControlStatic.h"
#include <SGEVg/VgPrimitives.h>

_LIT( KHelpText,  
"Eight provides Metro UI experience for Symbian users.\n\n\
Each tile represents an application or internal page.\n\
Tiles can be resized, removed, added from supported\n\
application list and layout of the tiles can be freely\n\
customized. All those actions are available in edit mode\n\
by long pressing any of the tile and selecting one of the\n\
button actions. Each tile can have different color and\n\
most of the tiles can flip. To change various options\n\
specific for tiles select tile settings button located\n\
in the upper left corner of the tile in edit mode.\n\n\
Application interface is divided into two sections.\n\
First one visible after application startup represents\n\
customizable tile panel. Second one available after\n\
swapping right or pressing button on the bottom of the\n\
first section provides all supported applications.\n\
Application can be added to the first section by long\n\
pressing on shortcut and then pressing pin button.\n\n\
Application settings are available in Setup tile.\n\
In settings user can select animation options,\n\
autostart, red key press behaviour and theme color.\n\
Theme color can be selected from one of 21 colors." );


CEightWindowHelp::CEightWindowHelp( TSize aSize )
    : CEightFormWindow( NULL, KEightWindowHelpId, aSize )
    {
    }

CEightWindowHelp::~CEightWindowHelp()
    {
    }

CEightWindowHelp* CEightWindowHelp::NewL( TSize aSize )
    {
    CEightWindowHelp* self = new ( ELeave ) CEightWindowHelp( aSize );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self;
    return self;
    }

void CEightWindowHelp::ConstructL()
    {
    CEightFormWindow::ConstructL( _L("Help") );
    
    CEightSettings* settings = CEightSettings::GetInstance();

    TVgRect r1 = TVgRect( TVgPoint( iSize.iWidth - KButtonMargin - KButtonWidth, KButtonHeight + KButtonMargin ), TVgSize( KButtonWidth, KButtonHeight ) );
    TVgRect rt1 = r1; rt1.iBr.iY += KTextOffsetMedium;
    CTouchButtonText* btn = CTouchButtonText::NewL( this, 1, rt1, r1, _L("close"), KFontId_SegoeWP_medium, settings->FontManager() );
    AddButtonL( btn );

    TVgRect r1a = TVgRect( TVgPoint( KButtonMargin, KButtonHeight + KButtonMargin ), TVgSize( KButtonWidth, KButtonHeight ) );
    TVgRect rt1a = r1a; rt1a.iBr.iY += KTextOffsetMedium;
    btn = CTouchButtonText::NewL( this, 2, rt1a, r1a, _L("wizard"), KFontId_SegoeWP_medium, settings->FontManager() );
    AddButtonL( btn );
    
    TInt offsetY = WindowContentHeight();

    TVgRect r2 = TVgRect( TVgPoint( 10, offsetY ), TVgSize( iSize.iWidth - 20, 100 ) );
    CTouchControlStatic* text = CTouchControlStatic::NewL( 2, r2, KHelpText(), KFontId_SegoeWP_small, settings->FontManager() );
    //text->SetTextAlign( ESGEFontAlignHorizontalCenter, ESGEFontAlignVerticalCenter );
    AddButtonL( text );
    }

void CEightWindowHelp::WindowClosedByGesture()
    {
	if ( iObserver ) iObserver->FormWindowButtonAction( iWindowId, 1 ); // simulate close button press
    }


