/*
 ============================================================================
 Name		: EightWindowColor.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightWindowColor implementation
 ============================================================================
 */

#include "EightWindowColor.h"
#include "EightFormWindowManager.h"
#include "EightSettings.h"
#include "EightPalette.h"
#include "TouchButtonText.h"
#include <SGEVg/VgPrimitives.h>



CEightWindowColor::CEightWindowColor( MEightFormWindowObserver* aObserver, TSize aSize, TBool aUpdatePalette, TBool aButtonNext )
    : CEightFormWindow( aObserver, KEightWindowColorId, aSize ), iUpdatePalette( aUpdatePalette ), iButtonNext( aButtonNext )
    {
    }

CEightWindowColor::~CEightWindowColor()
    {
    }

CEightWindowColor* CEightWindowColor::NewL( MEightFormWindowObserver* aObserver, TSize aSize, TBool aUpdatePalette, TBool aButtonNext )
    {
    CEightWindowColor* self = new ( ELeave ) CEightWindowColor( aObserver, aSize, aUpdatePalette, aButtonNext );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self;
    return self;
    }

void CEightWindowColor::ConstructL()
    {
    CEightFormWindow::ConstructL( _L("Select theme color") );
    
    CEightSettings* settings = CEightSettings::GetInstance();

    TVgRect r1 = TVgRect( TVgPoint( iSize.iWidth - KButtonMargin - KButtonWidth, KButtonHeight + KButtonMargin ), TVgSize( KButtonWidth, KButtonHeight ) );
    TVgRect rt1 = r1; rt1.iBr.iY += KTextOffsetMedium;
    CTouchButtonText* btn = CTouchButtonText::NewL( this, 1, rt1, r1, ( iButtonNext ? _L("next") : _L("cancel") ), KFontId_SegoeWP_medium, settings->FontManager() );
    AddButtonL( btn );
    
//    TInt w = 2 * KButtonHeight + 19;
//    TInt h = KButtonHeight;
//    TInt s = 3;
//    TInt t = 0;
    TInt w = 2 * KButtonHeight + 19 + 10;
    TInt h = KButtonHeight + 22;
    TInt s = 3 + 15;
    TInt t = 3;
    for ( TInt i = 0, c = 0x200; i < 3; i++ )
        {
        for ( TInt j = 0; j < 7; j++ )
            {
            TVgRect r1 = TVgRect( TVgPoint( KButtonMargin + i * ( w + KButtonMargin - s ), WindowContentHeight() + t - j * ( h + KButtonMargin - s ) ), TVgSize( w, h ) );
            CTouchButtonText* btn = CTouchButtonText::NewL( this, c, r1, r1, _L(""), KFontId_SegoeWP_medium, settings->FontManager() );
            btn->SetBackgroundColor( KEightColorsBase[ c & 0xff ] );
            if ( settings->ThemeColorIndex() == ( c & 0xff ) )
                {
                btn->SetBorderWidth( 4 );
                iDefaultColorBtnId = c; 
                }
            else
                {
                btn->SetBorderWidth( 0 );
                }
            AddButtonL( btn );
            c++;
            }
        }
    }

void CEightWindowColor::WindowClosedByGesture()
    {
	if ( iObserver ) iObserver->FormWindowButtonAction( iWindowId, 1 ); // simulate close button press
	else if ( iButtonNext ) iWindowManager->CloseWindow( iWindowId );
    }

void CEightWindowColor::SelectColor( TVgColor aColor )
    {
    for ( TInt i = 0; i < iButtons.Count(); i++ )
        {
        if ( iButtons[ i ]->Id() >= 0x200 )
            {
            if ( KEightColorsBase[ iButtons[ i ]->Id() & 0xff ] == aColor.AsUint32() )
                {
                ((CTouchButtonText*)iButtons[ i ])->SetBorderWidth( 4 );
                iDefaultColorBtnId = iButtons[ i ]->Id(); 
                }
            else
                {
                ((CTouchButtonText*)iButtons[ i ])->SetBorderWidth( 0 );
                }
            }
        }
    }

void CEightWindowColor::TouchButtonPressed( TUint aId )
    {
    CEightSettings* settings = CEightSettings::GetInstance();

    for ( TInt i = 0; i < iButtons.Count(); i++ )
        {
        if ( iButtons[ i ]->Id() >= 0x200 )
            {
            CTouchButtonText* btn = (CTouchButtonText*) iButtons[ i ];
            if ( iButtons[ i ]->Id() == aId )
                {
                btn->SetBorderWidth( 4 );
                if ( iUpdatePalette ) 
                	{
                	settings->SetThemeColorIndex( aId & 0xff );
                	CEightSettings::GetInstance()->SaveToFileL();
                	}
                iDefaultColorBtnId = iButtons[ i ]->Id();
                }
            else
                {
                btn->SetBorderWidth( 0 );
                }
            }
        }
    if ( aId == 1 )
    	{ // cancel
        if ( iButtonNext )
        	{
    		iWindowManager->CloseWindow( iWindowId );
    		return;
        	}
        else
        	{
			CTouchButtonText* btn = (CTouchButtonText*) GetButton( iDefaultColorBtnId );
			if ( btn )
				{
				btn->SetBorderWidth( 4 );
				}
        	}
    	}

    
    CEightFormWindow::TouchButtonPressed( aId );

    }

