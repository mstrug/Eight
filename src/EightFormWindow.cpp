/*
 ============================================================================
 Name		: EightFormWindow.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightFormWindow implementation
 ============================================================================
 */

#include "EightFormWindow.h"
#include "EightFormWindowManager.h"
#include "EightSettings.h"
#include <vg/vgu.h>

CEightFormWindow::CEightFormWindow( MEightFormWindowObserver* aObserver, TUint aWindowId, TSize aSize ) 
    : iObserver( aObserver ), iWindowId( aWindowId ), iSize( aSize ), iTitleOffset( KButtonMargin )
    {
    }

CEightFormWindow::~CEightFormWindow()
    {
    vgDestroyPath( iBackgroundPath );
    vgDestroyPaint( iBackgroundPaint );
    
    iButtons.ResetAndDestroy();
    iTitle.Close();
    }

CEightFormWindow* CEightFormWindow::NewL( MEightFormWindowObserver* aObserver, TUint aWindowId, TSize aSize, const TDesC& aTitle )
    {
    CEightFormWindow* self = new ( ELeave ) CEightFormWindow( aObserver, aWindowId, aSize );
    CleanupStack::PushL( self );
    self->ConstructL( aTitle );
    CleanupStack::Pop(); // self;
    return self;
    }

void CEightFormWindow::ConstructL( const TDesC& aTitle )
    {
    iBackgroundPath = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 8, 4, VG_PATH_CAPABILITY_ALL );
    vguRect( iBackgroundPath, 0, 0, iSize.iWidth, iSize.iHeight );

    iBackgroundPaint = vgCreatePaint();
    vgSetParameteri( iBackgroundPaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR );
    vgSetParameterfv( iBackgroundPaint, VG_PAINT_COLOR, 4, CEightSettings::GetInstance()->iPalette.iFormWindowBackgroundColor.iColor.Begin() );
    
    iTitle.Create( aTitle );
    }

void CEightFormWindow::AddButtonL( CTouchControlBase* aButton )
    {
    iButtons.AppendL( aButton );
    }

CTouchControlBase* CEightFormWindow::GetButton( TUint aButtonId )
    {
    for ( TInt i = 0; i < iButtons.Count(); i++ )
         {
         if ( iButtons[ i ]->Id() == aButtonId )
             {
             return iButtons[ i ];
             }
         }
    return NULL;
    }

TUint CEightFormWindow::Id() const
    {
    return iWindowId;
    }

void CEightFormWindow::Activated()
    {
    if ( CEightSettings::GetInstance()->iAnimateWindows )
        {
        iOffsetY = iSize.iWidth;
        }
    }

void CEightFormWindow::Deactivated()
    {
    if ( CEightSettings::GetInstance()->iAnimateWindows )
        {
        iOffsetY = -iSize.iWidth;
        }
    }

void CEightFormWindow::SetObserver( MEightFormWindowObserver* aObserver )
    {
    iObserver = aObserver;
    }

void CEightFormWindow::SetWindowManager( CEightFormWindowManager* aManager )
    {
    iWindowManager = aManager;
    }

void CEightFormWindow::SetMsgBox( CEightMsgBox* aMsgBox )
    {
    iMsgBox = aMsgBox;
    }

TBool CEightFormWindow::IsShowinOrHiding()
    {
    return iOffsetY != 0;
    }

void CEightFormWindow::TouchButtonPressed( TUint aId )
    {
    if ( iObserver ) iObserver->FormWindowButtonAction( iWindowId, aId );
    }

TBool CEightFormWindow::HandlePointerEventL( const TPointerEvent& aPointerEvent )
    {
    if ( iOffsetY != 0 )
        {
        return EFalse;
        }
    
    for ( TInt i = 0; i < iButtons.Count(); i++ )
         {
         if ( iButtons[ i ]->HandlePointerEventL( aPointerEvent ) )
             {
             return ETrue;
             }
         }

    return EFalse;
    }

void CEightFormWindow::HandleTouchGestureL( AknTouchGestureFw::MAknTouchGestureFwEvent& aEvent )
    {
    if ( aEvent.Group() == AknTouchGestureFw::EAknTouchGestureFwGroupFlick && 
         aEvent.Type() == AknTouchGestureFw::EAknTouchGestureFwFlickLeft )
        {
    	TPointerEvent pe;
    	pe.iType = TPointerEvent::EButton1Up;
    	pe.iPosition.SetXY( -100, -100 );
    	HandlePointerEventL( pe ); // cancel all button presses
    	
        WindowClosedByGesture();
        }
    }

TInt CEightFormWindow::WindowContentHeight()
    {
    return iSize.iHeight - 2 * KButtonMargin - 20 - 15;
    }

void CEightFormWindow::WindowClosedByGesture()
    {
    }

void CEightFormWindow::Draw()
    {    
    if ( iOffsetY > 0 )
        {
        vgSeti( VG_MATRIX_MODE, VG_MATRIX_GLYPH_USER_TO_SURFACE );
        vgTranslate( -iOffsetY, 0 );
        vgSeti( VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE );
        vgTranslate( -iOffsetY, 0 );
        vgSeti( VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE );
        vgTranslate( -iOffsetY, 0 );
        }
    else if ( iOffsetY < 0 )
        {
        vgSeti( VG_MATRIX_MODE, VG_MATRIX_GLYPH_USER_TO_SURFACE );
        vgTranslate( -( iSize.iWidth + iOffsetY ), 0 );
        vgSeti( VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE );
        vgTranslate( -( iSize.iWidth + iOffsetY ), 0 );
        vgSeti( VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE );
        vgTranslate( -( iSize.iWidth + iOffsetY ), 0 );
        }
    
    vgSetPaint( iBackgroundPaint, VG_FILL_PATH );
    vgDrawPath( iBackgroundPath, VG_FILL_PATH );
    
    CVgFont* fnt = CEightSettings::GetInstance()->FontManager().Get( KFontId_SegoeWP_large );
    if ( fnt )
        {
        fnt->DrawText( TVgPoint( iTitleOffset, iSize.iHeight - KButtonMargin - 20 ), iTitle );
        }
    
    for ( TInt i = 0; i < iButtons.Count(); i++ )
        {
        iButtons[ i ]->Draw();
        }
    }

void CEightFormWindow::Update( TTimeIntervalMicroSeconds32 aTimeInterval )
    {
    if ( iOffsetY != 0 )
        {
        iOffsetY /= 1.5;
        if ( Abs( iOffsetY ) < 1 )
            {
            iOffsetY = 0;
            }
        }
    
    for ( TInt i = 0; i < iButtons.Count(); i++ )
        {
        iButtons[ i ]->Update( aTimeInterval );
        }
    }


