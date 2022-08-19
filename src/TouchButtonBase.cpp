/*
 ============================================================================
 Name		: TouchButtonBase.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CTouchButtonBase implementation
 ============================================================================
 */

#include "TouchButtonBase.h"
#include "EightSettings.h"


CTouchButtonBase::CTouchButtonBase( MTouchButtonObserver* aObserver, TUint aId, TVgRect aRect )
    : iObserver( aObserver ), iId( aId ), iRect( aRect ), iVisible( ETrue ), iEnable( ETrue )
    {
	iDisabledPath = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 8, 4, VG_PATH_CAPABILITY_ALL );
    TVgRect r( aRect );
    r.Grow( 1, 1 );
	r.AddVguRect( iDisabledPath );
    iDisabledPaint = CEightSettings::GetInstance()->iPalette.iTilesFadeColor.CreateVgPaint();
    }

CTouchButtonBase::~CTouchButtonBase()
    {
    vgDestroyPath( iDisabledPath );
    vgDestroyPaint( iDisabledPaint );
    }

TUint CTouchButtonBase::Id() const
    {
    return iId;
    }

TVgRect CTouchButtonBase::Rect() const
    {
    return iRect;
    }

void CTouchButtonBase::SetRect( const TVgRect& aRect )
    {
    iRect = aRect;
    vgClearPath( iDisabledPath, VG_PATH_CAPABILITY_ALL );
    TVgRect r( aRect );
    r.Grow( 1, 1 );
	r.AddVguRect( iDisabledPath );
    }

void CTouchButtonBase::SetObserver( MTouchButtonObserver* aObserver )
    {
    iObserver = aObserver;
    }

TBool CTouchButtonBase::IsPressed() const
    {
    return iPressed;
    }

void CTouchButtonBase::Unpress()
    {
    iPressed = EFalse;
    }

void CTouchButtonBase::SetVisible( TBool aVisible )
    {
    iVisible = aVisible;
    }

void CTouchButtonBase::SetEnabled( TBool aEnable )
	{
	iEnable = aEnable;
	}

void CTouchButtonBase::UpdateColorsFromTheme()
	{
	
	}

void CTouchButtonBase::UpdateColorsFromIndex( TInt /*aColorIndex*/ )
	{
	
	}

void CTouchButtonBase::DrawDisabledRect()
	{
    vgSetPaint( iDisabledPaint, VG_FILL_PATH ); //| VG_STROKE_PATH );
    vgDrawPath( iDisabledPath, VG_FILL_PATH ); //| VG_STROKE_PATH );
	}

void CTouchButtonBase::Draw()
    {
    
    }

void CTouchButtonBase::Update( TTimeIntervalMicroSeconds32 /*aTimeInterval*/ )
    {
    
    }

TBool CTouchButtonBase::HandlePointerEventL( const TPointerEvent& /*aPointerEvent*/ )
    {
    return EFalse;
    }

