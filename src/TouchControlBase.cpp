/*
 ============================================================================
 Name		: TouchButtonBase.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CTouchControlBase implementation
 ============================================================================
 */

#include "TouchControlBase.h"
#include "EightSettings.h"


CTouchControlBase::CTouchControlBase( MTouchButtonObserver* aObserver, TUint aId, TVgRect aRect )
    : iObserver( aObserver ), iId( aId ), iRect( aRect ), iVisible( ETrue ), iEnable( ETrue )
    {
	iDisabledPath = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 8, 4, VG_PATH_CAPABILITY_ALL );
    TVgRect r( aRect );
    r.Grow( 1, 1 );
	r.AddVguRect( iDisabledPath );
    iDisabledPaint = CEightSettings::GetInstance()->iPalette.iTilesFadeColor.CreateVgPaint();
    }

CTouchControlBase::~CTouchControlBase()
    {
    vgDestroyPath( iDisabledPath );
    vgDestroyPaint( iDisabledPaint );
    }

TUint CTouchControlBase::Id() const
    {
    return iId;
    }

TVgRect CTouchControlBase::Rect() const
    {
    return iRect;
    }

void CTouchControlBase::SetRect( const TVgRect& aRect )
    {
    iRect = aRect;
    vgClearPath( iDisabledPath, VG_PATH_CAPABILITY_ALL );
    TVgRect r( aRect );
    r.Grow( 1, 1 );
	r.AddVguRect( iDisabledPath );
    }

void CTouchControlBase::SetObserver( MTouchButtonObserver* aObserver )
    {
    iObserver = aObserver;
    }

TBool CTouchControlBase::IsPressed() const
    {
    return iPressed;
    }

void CTouchControlBase::Unpress()
    {
    iPressed = EFalse;
    }

void CTouchControlBase::SetVisible( TBool aVisible )
    {
    iVisible = aVisible;
    }

void CTouchControlBase::SetEnabled( TBool aEnable )
	{
	iEnable = aEnable;
	}

void CTouchControlBase::UpdateColorsFromTheme()
	{
	
	}

void CTouchControlBase::UpdateColorsFromIndex( TInt /*aColorIndex*/ )
	{
	
	}

void CTouchControlBase::DrawDisabledRect()
	{
    vgSetPaint( iDisabledPaint, VG_FILL_PATH ); //| VG_STROKE_PATH );
    vgDrawPath( iDisabledPath, VG_FILL_PATH ); //| VG_STROKE_PATH );
	}

void CTouchControlBase::Draw()
    {
    
    }

void CTouchControlBase::Update( TTimeIntervalMicroSeconds32 /*aTimeInterval*/ )
    {
    
    }

TBool CTouchControlBase::HandlePointerEventL( const TPointerEvent& /*aPointerEvent*/ )
    {
    return EFalse;
    }

