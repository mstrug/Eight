/*
 ============================================================================
 Name		: EightWindowPremium.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightWindowPremium implementation
 ============================================================================
 */

#include "EightWindowPremium.h"
#include "EightFormWindowManager.h"
#include "EightSettings.h"
#include "TouchButtonText.h"
#include "TouchControlStatic.h"
#include <SGEVg/VgPrimitives.h>


_LIT( KHelpText,  
"Premium version of the Eight application lets you:\n\n\
- remove or add Ads tile freely (Internet connection\n\
  is not mandatory in Premium version)\n\
- remove About tile\n\
- integrate phone's Menu key" );


CEightWindowPremium::CEightWindowPremium( TSize aSize )
	: CEightFormWindow( NULL, KEightWindowPremiumId, aSize )
	{
	}

CEightWindowPremium::~CEightWindowPremium()
	{
	}

CEightWindowPremium* CEightWindowPremium::NewL( TSize aSize )
	{
	CEightWindowPremium* self = new (ELeave) CEightWindowPremium( aSize );
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); // self;
	return self;
	}

void CEightWindowPremium::ConstructL()
	{
    CEightFormWindow::ConstructL( _L("Premium information") );
    
    CEightSettings* settings = CEightSettings::GetInstance();

    TVgRect r1 = TVgRect( TVgPoint( iSize.iWidth - KButtonMargin - KButtonWidth, KButtonHeight + KButtonMargin ), TVgSize( KButtonWidth, KButtonHeight ) );
    TVgRect rt1 = r1; rt1.iBr.iY += KTextOffsetMedium;
    CTouchButtonText* btn = CTouchButtonText::NewL( this, 1, rt1, r1, _L("close"), KFontId_SegoeWP_medium, settings->FontManager() );
    AddButtonL( btn );
    
    TInt offsetY = WindowContentHeight();

    TVgRect r2 = TVgRect( TVgPoint( 10, offsetY ), TVgSize( iSize.iWidth - 20, 30 ) );
    CTouchControlStatic* text = CTouchControlStatic::NewL( 2, r2, KHelpText(), KFontId_SegoeWP_small, settings->FontManager() );
    //text->SetTextAlign( ESGEFontAlignHorizontalCenter, ESGEFontAlignVerticalCenter );
    AddButtonL( text );
	}

void CEightWindowPremium::WindowClosedByGesture()
	{
    if ( iObserver ) iObserver->FormWindowButtonAction( iWindowId, 1 ); // simulate close button press
	}



