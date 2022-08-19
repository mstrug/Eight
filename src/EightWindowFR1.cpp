/*
 ============================================================================
 Name		: EightWindowFR1.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightWindowFR1 implementation
 ============================================================================
 */

#include "EightWindowFR1.h"
#include "EightFormWindowManager.h"
#include "EightSettings.h"
#include "EightCompilationSettings.h"
#include "TouchButtonText.h"
#include "TouchControlStatic.h"
#include <SGEVg/VgPrimitives.h>


#ifdef EIGHT_PREMIUM
_LIT( KHelpText1, "Welcome to the Eight application!" );
#else
_LIT( KHelpText1, "Welcome to the Eight free application!" );
#endif

_LIT( KHelpText2,  
"This is the 'first start' wizard which will introduces\n\
you the basic functions of the application and lets\n\
you select theme color.\n\n\
Press the next button or swipe to left.\n" );


CEightWindowFR1::CEightWindowFR1( TSize aSize )
	: CEightFormWindow( NULL, KEightWindowFR1Id, aSize )
	{
	}

CEightWindowFR1::~CEightWindowFR1()
	{
	}

CEightWindowFR1* CEightWindowFR1::NewL( TSize aSize )
	{
	CEightWindowFR1* self = new (ELeave) CEightWindowFR1( aSize );
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); // self;
	return self;
	}

void CEightWindowFR1::ConstructL()
	{
    CEightFormWindow::ConstructL( _L("First step") );
    
    CEightSettings* settings = CEightSettings::GetInstance();

    TVgRect r1 = TVgRect( TVgPoint( iSize.iWidth - KButtonMargin - KButtonWidth, KButtonHeight + KButtonMargin ), TVgSize( KButtonWidth, KButtonHeight ) );
    TVgRect rt1 = r1; rt1.iBr.iY += KTextOffsetMedium;
    CTouchButtonText* btn = CTouchButtonText::NewL( this, 1, rt1, r1, _L("next"), KFontId_SegoeWP_medium, settings->FontManager() );
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

void CEightWindowFR1::TouchButtonPressed( TUint aButtonId )
	{
	if ( aButtonId == 1 )
		{
		iWindowManager->CloseWindow( iWindowId );
		}
	}

void CEightWindowFR1::WindowClosedByGesture()
	{
	iWindowManager->CloseWindow( iWindowId );
	}



