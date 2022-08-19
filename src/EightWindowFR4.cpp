/*
 ============================================================================
 Name		: EightWindowFR4.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightWindowFR4 implementation
 ============================================================================
 */

#include "EightWindowFR4.h"
#include "EightFormWindowManager.h"
#include "EightSettings.h"
#include "TouchButtonText.h"
#include "TouchControlStatic.h"
#include "EightCompilationSettings.h"
#include <SGEVg/VgPrimitives.h>

_LIT( KHelpText1, "Now you can start using the application!" );

#ifndef EIGHT_PREMIUM
_LIT( KHelpText2,  
"You can always run this wizard from Eight help\n\
window.\n\n\
Free version requires Internet connection for\n\
showing ads." );
#else
_LIT( KHelpText2,  
"You can always run this wizard from Eight help\n\
window." );
#endif


CEightWindowFR4::CEightWindowFR4( TSize aSize )
	: CEightFormWindow( NULL, KEightWindowFR4Id, aSize )
	{
	}

CEightWindowFR4::~CEightWindowFR4()
	{
	}

CEightWindowFR4* CEightWindowFR4::NewL( TSize aSize )
	{
	CEightWindowFR4* self = new (ELeave) CEightWindowFR4( aSize );
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); // self;
	return self;
	}

void CEightWindowFR4::ConstructL()
	{
    CEightFormWindow::ConstructL( _L("Start!") );
    
    CEightSettings* settings = CEightSettings::GetInstance();

    TVgRect r1 = TVgRect( TVgPoint( iSize.iWidth - KButtonMargin - KButtonWidth, KButtonHeight + KButtonMargin ), TVgSize( KButtonWidth, KButtonHeight ) );
    TVgRect rt1 = r1; rt1.iBr.iY += KTextOffsetMedium;
    CTouchButtonText* btn = CTouchButtonText::NewL( this, 1, rt1, r1, _L("close"), KFontId_SegoeWP_medium, settings->FontManager() );
    AddButtonL( btn );
    
    TInt offsetY = WindowContentHeight() - 25;

    TVgRect r2 = TVgRect( TVgPoint( 25, offsetY ), TVgSize( iSize.iWidth - 30, 30 ) );
    CTouchControlStatic* text = CTouchControlStatic::NewL( 2, r2, KHelpText1(), KFontId_SegoeWP_medium, settings->FontManager() );
    //text->SetTextAlign( ESGEFontAlignHorizontalCenter, ESGEFontAlignVerticalCenter );
    AddButtonL( text );
    
    offsetY -= 60;

    TVgRect r3 = TVgRect( TVgPoint( 25, offsetY ), TVgSize( iSize.iWidth - 30, 100 ) );
    text = CTouchControlStatic::NewL( 3, r3, KHelpText2(), KFontId_SegoeWP_small, settings->FontManager() );
    AddButtonL( text );

	}

void CEightWindowFR4::TouchButtonPressed( TUint aButtonId )
	{
	if ( aButtonId == 1 )
		{
		iWindowManager->CloseWindow( iWindowId );
		}
	}

void CEightWindowFR4::WindowClosedByGesture()
	{
	iWindowManager->CloseWindow( iWindowId );
	}



