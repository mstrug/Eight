/*
 ============================================================================
 Name		: EightFormWindowManager.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightFormWindowManager implementation
 ============================================================================
 */

#include "EightFormWindowManager.h"


CEightFormWindowManager::CEightFormWindowManager() : iWindowIdxToClose( KErrNotFound )
    {
    }

CEightFormWindowManager::~CEightFormWindowManager()
    {
    iWindows.ResetAndDestroy();
    }

CEightFormWindowManager* CEightFormWindowManager::NewL()
    {
    CEightFormWindowManager* self = new ( ELeave ) CEightFormWindowManager();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self;
    return self;
    }

void CEightFormWindowManager::ConstructL()
    {

    }

void CEightFormWindowManager::ShowWindowL( CEightFormWindow* aWindow, TBool aTakeWindowOwnership )
    {
    CItem* item = new (ELeave) CItem();
    item->iWindow = aWindow;
    item->iOwned = aTakeWindowOwnership;
    aWindow->SetWindowManager( this );
    iWindows.AppendL( item );
    iVisible = ETrue;
    aWindow->Activated();
    }

void CEightFormWindowManager::SetNextWindowActivateOnClose()
    {
    iNextWindowActivateOnClose = ETrue;
    }

TBool CEightFormWindowManager::CloseWindow( TUint aWindowId )
    {
    if ( iWindowIdxToClose != KErrNotFound )
        {
        return EFalse;
        }
    for ( TInt i = 0; i < iWindows.Count(); i++ )
        {
        if ( iWindows[ i ]->iWindow->Id() == aWindowId )
            {
            iWindows[ i ]->iWindow->Deactivated();
            iWindowIdxToClose = i;
            break;
            }
        }
    return ETrue;
    }

void CEightFormWindowManager::CloseAllWindows()
    {
    if ( iWindows.Count() > 0 )
        {
        iClosingAllWindows = ETrue;
        CloseWindow( iWindows[ iWindows.Count() - 1 ]->iWindow->Id() );
        }
    }

void CEightFormWindowManager::CloseWindowInternal()
    {
    if ( iWindowIdxToClose >= 0 )
        {
        delete iWindows[ iWindowIdxToClose ];
        iWindows.Remove( iWindowIdxToClose );
        iWindowIdxToClose = KErrNotFound;
        
        iVisible = ( iWindows.Count() > 0 );
        
        if ( iNextWindowActivateOnClose && iWindows.Count() > 0 )
        	{
        	iWindows[ iWindows.Count() - 1 ]->iWindow->Activated();
        	}
        if ( iVisible && iClosingAllWindows )
            {
            CloseAllWindows();
            }
        else
            {
            iClosingAllWindows = EFalse;
            }
        }
    }

TBool CEightFormWindowManager::IsVisible() const
    {
    return iVisible;
    }

TBool CEightFormWindowManager::IsShowinOrHidingWindow()
    {
    if ( iWindows.Count() > 0 )
        {
        return iWindows[ iWindows.Count() - 1 ]->iWindow->IsShowinOrHiding();
        }
    return EFalse;
    }

TInt CEightFormWindowManager::WindowsCount() const
    {
    return iWindows.Count();
    }

TBool CEightFormWindowManager::HandlePointerEventL( const TPointerEvent& aPointerEvent )
    {
    if ( iWindows.Count() > 0 )
        {
        return iWindows[ iWindows.Count() - 1 ]->iWindow->HandlePointerEventL( aPointerEvent );
        }
    return EFalse;
    }

void CEightFormWindowManager::HandleTouchGestureL( AknTouchGestureFw::MAknTouchGestureFwEvent& aEvent )
    {
    if ( iWindows.Count() > 0 )
        {
        iWindows[ iWindows.Count() - 1 ]->iWindow->HandleTouchGestureL( aEvent );
        }
    }

void CEightFormWindowManager::Draw()
    {
    if ( iWindows.Count() > 0 )
        {
        if ( iWindowIdxToClose > 0 && iWindows.Count() > 1 &&
        	 !iWindows[ iWindows.Count() - 2 ]->iWindow->IsShowinOrHiding() )
            {
            iWindows[ iWindows.Count() - 2 ]->iWindow->Draw();
            }
        else if ( iWindows[ iWindows.Count() - 1 ]->iWindow->IsShowinOrHiding() && iWindows.Count() > 1 &&
				  !iWindows[ iWindows.Count() - 2 ]->iWindow->IsShowinOrHiding() )
            {
            iWindows[ iWindows.Count() - 2 ]->iWindow->Draw();
            }
        iWindows[ iWindows.Count() - 1 ]->iWindow->Draw();
        }
    }

void CEightFormWindowManager::Update( TTimeIntervalMicroSeconds32 aTimeInterval )
    {
    if ( iWindows.Count() > 0 )
        {
        if ( iWindowIdxToClose >= 0 )
            {
            if ( iWindows[ iWindowIdxToClose ]->iWindow->IsShowinOrHiding() )
                {
                iWindows[ iWindowIdxToClose ]->iWindow->Update( aTimeInterval );
                }
            if ( !iWindows[ iWindowIdxToClose ]->iWindow->IsShowinOrHiding() )
                {
                CloseWindowInternal();
                }
            return;
            }
            
        iWindows[ iWindows.Count() - 1 ]->iWindow->Update( aTimeInterval );
        }
    }

