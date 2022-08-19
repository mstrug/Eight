/*
 ============================================================================
 Name		: EightWindowSetup.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightWindowSetup implementation
 ============================================================================
 */

#include "EightWindowSetup.h"
#include "EightWindowColor.h"
#include "EightFormWindowManager.h"
#include "EightSettings.h"
#include "EightCompilationSettings.h"
#include "TouchButtonText.h"
#include "TouchButtonOnOff.h"
#include <SGEVg/VgPrimitives.h>

#ifdef EIGHT_PREMIUM
const TInt KButtonOnOffMargin = 15;
#else
const TInt KButtonOnOffMargin = 20;
#endif


CEightWindowSetup::CEightWindowSetup( TSize aSize )
    : CEightFormWindow( NULL, KEightWindowSetupId, aSize )
    {
    }

CEightWindowSetup::~CEightWindowSetup()
    {
    }

CEightWindowSetup* CEightWindowSetup::NewL( TSize aSize )
    {
    CEightWindowSetup* self = new ( ELeave ) CEightWindowSetup( aSize );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self;
    return self;
    }

void CEightWindowSetup::ConstructL()
    {
    CEightFormWindow::ConstructL( _L("Setup") );
    
    CEightSettings* settings = CEightSettings::GetInstance();

    TVgRect r1 = TVgRect( TVgPoint( iSize.iWidth - KButtonMargin - KButtonWidth, KButtonHeight + KButtonMargin ), TVgSize( KButtonWidth, KButtonHeight ) );
    TVgRect rt1 = r1; rt1.iBr.iY += KTextOffsetMedium;
    CTouchButtonText* btn = CTouchButtonText::NewL( this, 1, rt1, r1, _L("close"), KFontId_SegoeWP_medium, settings->FontManager() );
    AddButtonL( btn );

    TVgRect r11 = TVgRect( TVgPoint( KButtonMargin, KButtonHeight + KButtonMargin ), TVgSize( KButtonWidth, KButtonHeight ) );
//    TVgRect r11 = TVgRect( TVgPoint( iSize.iWidth - KButtonMargin - KButtonWidth, 2 * KButtonHeight + 2 * KButtonMargin ), TVgSize( KButtonWidth, KButtonHeight ) );
    TVgRect rt11 = r11; rt11.iBr.iY += KTextOffsetMedium;
    CTouchButtonText* btn11 = CTouchButtonText::NewL( this, 11, rt11, r11, _L("reset"), KFontId_SegoeWP_medium, settings->FontManager() );
    AddButtonL( btn11 );

#ifdef EIGHT_PREMIUM
    TInt offsetY = WindowContentHeight() + 5;
#else
    TInt offsetY = WindowContentHeight();
#endif
    
    TVgRect r2 = TVgRect( TVgPoint( KButtonMargin, offsetY ), TVgSize( KButtonWidth, KButtonHeight ) );
    TVgRect rt2 = r2; rt2.iBr.iY += KTextOffsetMedium;
    CTouchButtonText* btn2 = CTouchButtonText::NewL( this, 2, rt2, r2, _L("theme color"), KFontId_SegoeWP_medium, settings->FontManager() );
    btn2->SetBackgroundColor( settings->iPalette.iTileColor ); // iTile.iPalette.iTileColor
    AddButtonL( btn2 );

    TVgSize onOffSize( iSize.iWidth - 2 * KButtonMargin, KButtonHeight );
    
#ifdef EIGHT_PREMIUM
    offsetY -= KButtonHeight + KButtonMargin;
#else
    offsetY -= KButtonHeight + KButtonMargin + 10;
#endif
    TVgRect r3 = TVgRect( TVgPoint( KButtonMargin, offsetY ), onOffSize );
    CTouchButtonOnOff* btn3 = CTouchButtonOnOff::NewL( this, 3, r3, _L("Show bottom buttons"), KFontId_SegoeWP_medium, settings->FontManager() );
    btn3->SetTextOffset( 4 );
    btn3->SetValue( settings->iShowBottomButtons );
    AddButtonL( btn3 );

    offsetY -= KButtonHeight + KButtonOnOffMargin;
    TVgRect r4 = TVgRect( TVgPoint( KButtonMargin, offsetY ), onOffSize );
    CTouchButtonOnOff* btn4 = CTouchButtonOnOff::NewL( this, 4, r4, _L("Animate applications start"), KFontId_SegoeWP_medium, settings->FontManager() );
    btn4->SetTextOffset( 4 );
    btn4->SetValue( settings->iAnimateAppStartStop );
    AddButtonL( btn4 );

    offsetY -= KButtonHeight + KButtonOnOffMargin;
    TVgRect r5 = TVgRect( TVgPoint( KButtonMargin, offsetY ), onOffSize );
    CTouchButtonOnOff* btn5 = CTouchButtonOnOff::NewL( this, 5, r5, _L("Animate window open/close"), KFontId_SegoeWP_medium, settings->FontManager() );
    btn5->SetTextOffset( 4 );
    btn5->SetValue( settings->iAnimateWindows );
    AddButtonL( btn5 );

    offsetY -= KButtonHeight + KButtonOnOffMargin;
    TVgRect r6 = TVgRect( TVgPoint( KButtonMargin, offsetY ), onOffSize );
    CTouchButtonOnOff* btn6 = CTouchButtonOnOff::NewL( this, 6, r6, _L("Increase tile size on touch"), KFontId_SegoeWP_medium, settings->FontManager() );
    btn6->SetTextOffset( 4 );
    btn6->SetValue( settings->IncreaseTileSizeOnTouch() );
    AddButtonL( btn6 );

    offsetY -= KButtonHeight + KButtonOnOffMargin;
    TVgRect r7 = TVgRect( TVgPoint( KButtonMargin, offsetY ), onOffSize );
    CTouchButtonOnOff* btn7 = CTouchButtonOnOff::NewL( this, 7, r7, _L("Autostart after phone reboot"), KFontId_SegoeWP_medium, settings->FontManager() );
    btn7->SetTextOffset( 4 );
    btn7->SetValue( settings->iAutostart );
    AddButtonL( btn7 );

    offsetY -= KButtonHeight + KButtonOnOffMargin;
    TVgRect r8 = TVgRect( TVgPoint( KButtonMargin, offsetY ), onOffSize );
    CTouchButtonOnOff* btn8 = CTouchButtonOnOff::NewL( this, 8, r8, _L("Close on red key press"), KFontId_SegoeWP_medium, settings->FontManager() );
    btn8->SetTextOffset( 4 );
    btn8->SetValue( settings->iCloseOnRedKeyPress );
    AddButtonL( btn8 );

    offsetY -= KButtonHeight + KButtonOnOffMargin;
    TVgRect r9 = TVgRect( TVgPoint( KButtonMargin, offsetY ), onOffSize );
    CTouchButtonOnOff* btn9 = CTouchButtonOnOff::NewL( this, 9, r9, _L("Use touch feedback"), KFontId_SegoeWP_medium, settings->FontManager() );
    btn9->SetTextOffset( 4 );
    btn9->SetValue( settings->iUseTouchFeedback );
    AddButtonL( btn9 );
    
#ifdef EIGHT_PREMIUM
    offsetY -= KButtonHeight + KButtonOnOffMargin;
    TVgRect r10 = TVgRect( TVgPoint( KButtonMargin, offsetY ), onOffSize );
    CTouchButtonOnOff* btn10 = CTouchButtonOnOff::NewL( this, KEightWindowSetupMenuButtonId, r10, _L("Handle menu key"), KFontId_SegoeWP_medium, settings->FontManager() );
    btn10->SetTextOffset( 4 );
    btn10->SetValue( settings->iCaptureMenuKey );
    AddButtonL( btn10 );
#endif
    }

void CEightWindowSetup::UpdateButtonsColors()
    {
    for ( TInt i = 3; i <= 10; i++ )
        {
        CTouchControlBase* btn = GetButton( i );
        if ( btn ) btn->UpdateColorsFromTheme();
        }

    }

void CEightWindowSetup::WindowClosedByGesture()
    {
	if ( iObserver ) iObserver->FormWindowButtonAction( iWindowId, 1 ); // simulate close button press
    }

void CEightWindowSetup::Activated()
    {
    CEightFormWindow::Activated();
    
    CEightSettings* settings = CEightSettings::GetInstance();
    CTouchButtonText* btn2 = ( CTouchButtonText* ) GetButton( 2 );
    if ( btn2 ) 
        {
        btn2->SetBackgroundColor( settings->iPalette.iTileColor );
        }
    UpdateButtonsColors();
    UpdateButtonStates();
    
    if ( iMsgBox )
    	{
    	iMsgBox->RegisterObserver( this );
    	}
    }

void CEightWindowSetup::Deactivated()
    {
    CEightFormWindow::Deactivated();

    if ( iMsgBox ) iMsgBox->UnregisterObserver( this );
    }

void CEightWindowSetup::UpdateButtonStates()
	{
    CEightSettings* settings = CEightSettings::GetInstance();
    
    CTouchButtonOnOff* btn3 = (CTouchButtonOnOff*) GetButton( 3 );
    btn3->SetValue( settings->iShowBottomButtons );

    CTouchButtonOnOff* btn4 = (CTouchButtonOnOff*) GetButton( 4 );
    btn4->SetValue( settings->iAnimateAppStartStop );

    CTouchButtonOnOff* btn5 = (CTouchButtonOnOff*) GetButton( 5 );
    btn5->SetValue( settings->iAnimateWindows );

    CTouchButtonOnOff* btn6 = (CTouchButtonOnOff*) GetButton( 6 );
    btn6->SetValue( settings->IncreaseTileSizeOnTouch() );

    CTouchButtonOnOff* btn7 = (CTouchButtonOnOff*) GetButton( 7 );
    btn7->SetValue( settings->iAutostart );

    CTouchButtonOnOff* btn8 = (CTouchButtonOnOff*) GetButton( 8 );
    btn8->SetValue( settings->iCloseOnRedKeyPress );

    CTouchButtonOnOff* btn9 = (CTouchButtonOnOff*) GetButton( 9 );
    btn9->SetValue( settings->iUseTouchFeedback );

#ifdef EIGHT_PREMIUM
    CTouchButtonOnOff* btn10 = (CTouchButtonOnOff*) GetButton( KEightWindowSetupMenuButtonId );
    btn10->SetValue( settings->iCaptureMenuKey );
#endif
	}

void CEightWindowSetup::UpdateSettings()
    {
    CEightSettings* settings = CEightSettings::GetInstance();
    
    CTouchButtonOnOff* btn3 = (CTouchButtonOnOff*) GetButton( 3 );
    settings->iShowBottomButtons = btn3->IsOn();

    CTouchButtonOnOff* btn4 = (CTouchButtonOnOff*) GetButton( 4 );
    settings->iAnimateAppStartStop = btn4->IsOn();

    CTouchButtonOnOff* btn5 = (CTouchButtonOnOff*) GetButton( 5 );
    settings->iAnimateWindows = btn5->IsOn();

    CTouchButtonOnOff* btn6 = (CTouchButtonOnOff*) GetButton( 6 );
    settings->SetIncreaseTileSizeOnTouch( btn6->IsOn() );

    CTouchButtonOnOff* btn7 = (CTouchButtonOnOff*) GetButton( 7 );
    settings->iAutostart = btn7->IsOn();

    CTouchButtonOnOff* btn8 = (CTouchButtonOnOff*) GetButton( 8 );
    settings->iCloseOnRedKeyPress = btn8->IsOn();

    CTouchButtonOnOff* btn9 = (CTouchButtonOnOff*) GetButton( 9 );
    settings->iUseTouchFeedback = btn9->IsOn();

#ifdef EIGHT_PREMIUM
    CTouchButtonOnOff* btn10 = (CTouchButtonOnOff*) GetButton( KEightWindowSetupMenuButtonId );
    settings->iCaptureMenuKey = btn10->IsOn();
#else
    settings->iCaptureMenuKey = EFalse;
#endif
    
    settings->SaveToFileL();
    }

void CEightWindowSetup::TouchButtonPressed( TUint aId )
    {
    if ( aId == 2 )
        {
        CEightWindowColor* wnd = CEightWindowColor::NewL( this, iSize );
        iWindowManager->ShowWindowL( wnd );
        }
    else if ( aId == 9 )
        {
        iMsgBox->ShowL( 0x2002, CEightMsgBox::EMsgBoxButtonOk, _L("Touch feedback depends state of the 'Touch screen\nvibration' setting in the current phone profile.") );
        }
    else if ( aId == 11 )
        {
        iMsgBox->ShowL( 0x2001, CEightMsgBox::EMsgBoxButtonYesNo, _L("This will restore default tiles layout and settings.\nDo you want to continue?") );
        }
    else
        {
        if ( aId == 1 || aId == 9 || aId == KEightWindowSetupMenuButtonId ) UpdateSettings();
        CEightFormWindow::TouchButtonPressed( aId );
        }
    }

void CEightWindowSetup::FormWindowButtonAction( TUint aWindowId, TUint aButtonId )
    {
    if ( aWindowId == KEightWindowColorId && aButtonId == 1 )
        {
        iWindowManager->CloseWindow( KEightWindowColorId );
        }
    else if ( aWindowId == KEightWindowColorId )
        {
        CEightSettings* settings = CEightSettings::GetInstance();
        CTouchButtonText* btn2 = ( CTouchButtonText* ) GetButton( 2 );
        if ( btn2 ) 
            {
            btn2->SetBackgroundColor( settings->iPalette.iTileColor );
            }
        UpdateButtonsColors();
        iWindowManager->CloseWindow( KEightWindowColorId );
        }
    }

void CEightWindowSetup::MsgBoxAction( TUint aMessageId, TInt aActionId )
    {
	if ( aMessageId == 0x2001 && aActionId == KMsgBoxButtonYes )
		{
		CEightSettings* settings = CEightSettings::GetInstance();
		settings->ResetToDefaults();
		settings->SaveToFileL();
		iObserver->FormWindowButtonAction( iWindowId, KEightWindowSetupResetButtonId );
		}
    }

