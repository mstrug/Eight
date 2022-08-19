/*
 ============================================================================
 Name		: EightTileAppPhotos.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileAppPhotos declaration
 ============================================================================
 */

#ifndef EIGHTTILEAPPPHOTOS_H
#define EIGHTTILEAPPPHOTOS_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGEVg/VgSvgImage.h>
#include <SGEVg/VgPrimitives.h>
#include <SGEUtils/SGEUtilsGPTimer.h>
#include <SGEUtils/SGEUtilsGPTimerObserver.h>
#include "EightTileAppShortcut.h"
#include "EightPhotosDCIM.h"
#include "TouchButtonText.h"

// CLASS DECLARATION

/**
 *  CEightTileAppPhotos
 * 
 */
class CEightTileAppPhotos : public CEightTileAppShortcut, MEightPhotosDCIMObserver
    {
public:

    ~CEightTileAppPhotos();

    static CEightTileAppPhotos* NewL( const TDesC& aExeName, TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi = NULL );

    TInt CustomSettingsButtonsCount() const;

    CTouchControlBase* GetCustomSettingsButtonL( TInt aIdx );

    void HandleCustomSettingsButtonState( CTouchControlBase* aButton );

    void CleanupCustomSettingsButtons();

    void ExternalizeL( RWriteStream& aStream ) const;

    void InternalizeL( RReadStream& aStream );
    
private:

    CEightTileAppPhotos( TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi );

    void ConstructL( const TDesC& aExeName );

private: // from MEightPhotosDCIMObserver
    
    void PhotosDCIMUpdateFinished( TInt aError );

private: // data
    
    CTouchButtonText* iDCIMRefreshButton; // not owned
    
    };

#endif // EIGHTTILEAPPPHOTOS_H
