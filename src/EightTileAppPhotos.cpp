/*
 ============================================================================
 Name		: EightTileAppPhotos.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileAppPhotos implementation
 ============================================================================
 */

#include "EightTileAppPhotos.h"
#include "EightTileUiDynCycle.h"
#include "EightSettings.h"
#include "EightPhotosProvider.h"
#include "TouchButtonRadioGroup.h"
#include "TouchButtonText.h"
#include "TouchControlStatic.h"


CEightTileAppPhotos::CEightTileAppPhotos( TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi ) :
    CEightTileAppShortcut( aId, aTileSize, aUi )
    {
    }

CEightTileAppPhotos::~CEightTileAppPhotos()
    {
    }

CEightTileAppPhotos* CEightTileAppPhotos::NewL( const TDesC& aExeName, TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi )
    {
    CEightTileAppPhotos* self = new ( ELeave ) CEightTileAppPhotos( aId, aTileSize, aUi );
    CleanupStack::PushL( self );
    self->ConstructL( aExeName );
    CleanupStack::Pop(); // self;
    return self;
    }

void CEightTileAppPhotos::ConstructL( const TDesC& aExeName )
    {
    CEightTileAppShortcut::ConstructL( aExeName );
    }

TInt CEightTileAppPhotos::CustomSettingsButtonsCount() const
    {
    return 3;
    }

CTouchControlBase* CEightTileAppPhotos::GetCustomSettingsButtonL( TInt aIdx )
    {
    if ( aIdx == 0 )
    	{
		CTouchButtonRadioGroup* btn = CTouchButtonRadioGroup::NewL( NULL, 23, TVgRect(), _L("Photos source"), KFontId_SegoeWP_medium, KFontId_SegoeWP_medium, CEightSettings::GetInstance()->FontManager() );
		btn->AddButtonL( 24, _L("Eight app photos") );
		btn->AddButtonL( 25, _L("Phone's DCIM folder") );
		btn->SetSelectedButtonId( ((CEightTileUiDynCycle*)iUi)->PhotosSourceId() == KPhotosSourceIdInternalFiles ? 24 : 25 );
		return btn;
    	}
    else if ( aIdx == 1 )
        {
        if ( !iDCIMRefreshButton )
            {
            iDCIMRefreshButton = CTouchButtonText::NewL( NULL, 30, TVgRect(), TVgRect(), _L("refresh DCIM"), KFontId_SegoeWP_medium, CEightSettings::GetInstance()->FontManager() );
            }
        iDCIMRefreshButton->SetEnabled( !CEightPhotosProvider::GetInstanceL()->UpdateDBPending() );

        return iDCIMRefreshButton;
        }
    else if ( aIdx == 2 )
        {
        CTouchControlStatic* txt = CTouchControlStatic::NewL( 40, TVgRect(), _L("DCIM folders which are serched for JPG files:\nc:\\DCIM, e:\\DCIM, f:\\DCIM (including subfolders).\nUsing DCIM folder may result in drop of performance\nbecause of large images which has to be resized to\nwidth of 360 pixels at runtime.\nRefreshing DCIM folder files list may take a while\ndepending on pictures count."), KFontId_SegoeWP_small, CEightSettings::GetInstance()->FontManager() );
        return txt;
        }
	return NULL;
    }

void CEightTileAppPhotos::HandleCustomSettingsButtonState( CTouchControlBase* aButton )
    {
    if ( aButton->Id() >= 23 && aButton->Id() <= 25 )
        {
        CTouchButtonRadioGroup* btn = (CTouchButtonRadioGroup*) aButton;
        TInt id = btn->SelectedButtonId();
        if ( id == 24 )
            {
            ((CEightTileUiDynCycle*)iUi)->SetPhotosSourceId( KPhotosSourceIdInternalFiles );
            }
        else if ( id == 25 )
            {
            ((CEightTileUiDynCycle*)iUi)->SetPhotosSourceId( KPhotosSourceIdDCIM );
            if ( !CEightPhotosProvider::GetInstanceL()->DBOpened() )
                {
                if ( CEightPhotosProvider::GetInstanceL()->StartDCIMUpdateL( this ) && iDCIMRefreshButton )
                    {
                    iDCIMRefreshButton->SetEnabled( EFalse );
                    }
                }
            }
        }
    else if ( aButton->Id() == 30 )
    	{
    	if ( CEightPhotosProvider::GetInstanceL()->StartDCIMUpdateL( this ) && iDCIMRefreshButton )
    	    {
    	    iDCIMRefreshButton->SetEnabled( EFalse );
    	    }
    	}
    }

void CEightTileAppPhotos::CleanupCustomSettingsButtons()
    {
    iDCIMRefreshButton = NULL; // delated by tile settings
    }

void CEightTileAppPhotos::PhotosDCIMUpdateFinished( TInt /*aError*/ )
    {
    if ( iDCIMRefreshButton )
        {
        iDCIMRefreshButton->SetEnabled( ETrue );
        }
    }

void CEightTileAppPhotos::ExternalizeL( RWriteStream& aStream ) const
	{
    aStream.WriteUint32L( sizeof( TUint32 ) ); // size of additional data
    aStream.WriteUint32L( ((CEightTileUiDynCycle*)iUi)->PhotosSourceId() );
	}

void CEightTileAppPhotos::InternalizeL( RReadStream& aStream )
	{
    TUint size = aStream.ReadUint32L();
    ((CEightTileUiDynCycle*)iUi)->SetPhotosSourceId( aStream.ReadUint32L() );
	}


