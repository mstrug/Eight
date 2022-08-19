/*
 ============================================================================
 Name		: EightWindowTileSettings.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightWindowTileSettings implementation
 ============================================================================
 */

#include "EightWindowTileSettings.h"
#include "EightWindowColor.h"
#include "EightFormWindowManager.h"
#include "EightSettings.h"
#include "TouchButtonText.h"
#include "TouchButtonOnOff.h"
#include <SGEVg/VgPrimitives.h>


CEightWindowTileSettings::CEightWindowTileSettings( MEightFormWindowObserver* aObserver, TSize aSize, CEightTile& aTile )
    : CEightFormWindow( aObserver, KEightWindowTileSettingsId, aSize ), iTile( aTile )
    {
    }

CEightWindowTileSettings::~CEightWindowTileSettings()
    {
    iTile.CleanupCustomSettingsButtons();
    }

CEightWindowTileSettings* CEightWindowTileSettings::NewL( MEightFormWindowObserver* aObserver, TSize aSize, CEightTile& aTile )
    {
    CEightWindowTileSettings* self = new ( ELeave ) CEightWindowTileSettings( aObserver, aSize, aTile );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self;
    return self;
    }

void CEightWindowTileSettings::ConstructL()
    {
    CEightFormWindow::ConstructL( _L("Tile settings") );
    
    CEightSettings* settings = CEightSettings::GetInstance();

    TVgRect r1 = TVgRect( TVgPoint( iSize.iWidth - KButtonMargin - KButtonWidth, KButtonHeight + KButtonMargin ), TVgSize( KButtonWidth, KButtonHeight ) );
    TVgRect rt1 = r1; rt1.iBr.iY += KTextOffsetMedium;
    CTouchButtonText* btn = CTouchButtonText::NewL( this, 1, rt1, r1, _L("close"), KFontId_SegoeWP_medium, settings->FontManager() );
    AddButtonL( btn );

    TInt offsetY = WindowContentHeight();
    
    if ( iTile.SupportsCustomColor() )
        {
        TVgRect r3 = TVgRect( TVgPoint( KButtonMargin, offsetY ), TVgSize( iSize.iWidth - 2 * KButtonMargin, KButtonHeight ) );
        CTouchButtonOnOff* btn3 = CTouchButtonOnOff::NewL( this, 3, r3, _L("Tile color same as theme"), KFontId_SegoeWP_medium, settings->FontManager() );
        btn3->SetTextOffset( 4 );
        AddButtonL( btn3 );
    
        offsetY -=  KButtonHeight + 15;
        TVgRect r2 = TVgRect( TVgPoint( KButtonMargin, offsetY ), TVgSize( KButtonWidth, KButtonHeight ) );
        TVgRect rt2 = r2; rt2.iBr.iY += KTextOffsetMedium;
        CTouchButtonText* btn2 = CTouchButtonText::NewL( this, 2, rt2, r2, _L("tile color"), KFontId_SegoeWP_medium, settings->FontManager() );
        btn2->SetBackgroundColor( settings->iPalette.iTileColor ); // iTile.iPalette.iTileColor
        AddButtonL( btn2 );
        offsetY -= KButtonHeight + KButtonMargin + 15;
        }

    if ( iTile.SupportsFlipping() )
        {
        TVgRect r4 = TVgRect( TVgPoint( KButtonMargin, offsetY ), TVgSize( iSize.iWidth - 2 * KButtonMargin, KButtonHeight ) );
        CTouchButtonOnOff* btn4 = CTouchButtonOnOff::NewL( this, 4, r4, _L("Flipping tile"), KFontId_SegoeWP_medium, settings->FontManager() );
        btn4->SetTextOffset( 4 );
        AddButtonL( btn4 );
        offsetY -= KButtonHeight + KButtonMargin + 10;
        }
    
    for ( TInt i = 0; i < iTile.CustomSettingsButtonsCount(); i++ )
        {
        CTouchControlBase* btn = iTile.GetCustomSettingsButtonL( i );
        if ( !btn ) continue;
        TVgRect rc = TVgRect( TVgPoint( KButtonMargin, offsetY ), TVgSize( iSize.iWidth - 2 * KButtonMargin, KButtonHeight ) );
        btn->SetRect( rc );
        btn->SetObserver( this );
        AddButtonL( btn );
        offsetY -= KButtonMargin + btn->Rect().Height() + 15;
        }
    }

void CEightWindowTileSettings::WindowClosedByGesture()
    {
	if ( iObserver ) iObserver->FormWindowButtonAction( iWindowId, 1 ); // simulate close button press
    }

void CEightWindowTileSettings::Activated()
    {
    CEightFormWindow::Activated();

    UpdateButtonColor();
    
    CTouchButtonOnOff* btn = (CTouchButtonOnOff*) GetButton( 3 );
    if ( btn )
        {
        btn->SetValue( !iTile.CustomColor() );
        CTouchButtonText* btn2 = ( CTouchButtonText* ) GetButton( 2 );
        if ( btn2 ) 
            {
            btn2->SetEnabled( !btn->IsOn() );
            }
        }
    btn = (CTouchButtonOnOff*) GetButton( 4 );
    if ( btn )
        {
        btn->SetValue( iTile.IsFlipping() );
        }
    }

void CEightWindowTileSettings::Deactivated()
    {
    CEightFormWindow::Deactivated();

//    CTouchButtonOnOff* btn = (CTouchButtonOnOff*) GetButton( 3 );
//    if ( btn )
//        {
//        if ( btn->IsOn() )
//            {
//            iTile.SetCustomColor( EFalse );
//            iTile.UpdateColorsFromTheme();
//            }
//        }
    if ( iTile.SupportsFlipping() )
        {
        CTouchButtonOnOff* btn = (CTouchButtonOnOff*) GetButton( 4 );
        if ( btn )
            {
            iTile.SetFlipping( btn->IsOn() );
            }
        }
    }

void CEightWindowTileSettings::UpdateButtonColor()
    {
    CTouchButtonText* btn2 = ( CTouchButtonText* ) GetButton( 2 );
    if ( btn2 ) 
        {
        btn2->SetBackgroundColor( iTile.Palette().iTileColor );
        }
    
	CTouchButtonOnOff* btn = (CTouchButtonOnOff*) GetButton( 3 );
	if ( btn )
		{
		btn->UpdateColorsFromTheme(); //SetActiveColor( iTile.Palette().iTileColor );
		}
    }

void CEightWindowTileSettings::TouchButtonPressed( TUint aId )
    {
    if ( aId == 2 )
        {
        CEightWindowColor* wnd = CEightWindowColor::NewL( this, iSize, EFalse );
        wnd->SelectColor( iTile.Palette().iTileColor );
        iWindowManager->ShowWindowL( wnd );
        }
    else if ( aId == 3 )
    	{
        CTouchButtonText* btn2 = ( CTouchButtonText* ) GetButton( 2 );
    	CTouchButtonOnOff* btn3 = (CTouchButtonOnOff*) GetButton( 3 );
    	btn2->SetEnabled( !btn3->IsOn() );
        iTile.SetCustomColor( !btn3->IsOn() );
        iTile.UpdateColorsFromTheme();
        if ( btn3->IsOn() )
            {
            btn2->SetBackgroundColor( iTile.Palette().iTileColor  );
            }
    	}
    else
        {
        TInt customBtns = iTile.CustomSettingsButtonsCount();
        if ( customBtns > 0 )
            {
            for ( TInt i = iButtons.Count() - customBtns; i < iButtons.Count(); i++ )
                {
                if ( iButtons[ i ]->Id() == aId )
                    {
                    iTile.HandleCustomSettingsButtonState( iButtons[ i ] );
                    return;
                    }
                }
            }
        CEightFormWindow::TouchButtonPressed( aId );
        }
    }

void CEightWindowTileSettings::FormWindowButtonAction( TUint aWindowId, TUint aButtonId )
    {
    if ( aWindowId == KEightWindowColorId && aButtonId > 1 )
        {
        TInt idx = aButtonId & 0xff;
        if ( idx >= 0 && idx < KEightColorsCount )
            {
            iTile.SetCustomColor( ETrue );
            iTile.UpdateColorsFromIndex( idx );
            UpdateButtonColor();
            }
        }
    iWindowManager->CloseWindow( KEightWindowColorId );
    }

