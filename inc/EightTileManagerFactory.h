/*
 ============================================================================
 Name		: EightTileManagerFactory.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileManagerFactory declaration
 ============================================================================
 */

#ifndef EIGHTTILEMANAGERFACTORY_H
#define EIGHTTILEMANAGERFACTORY_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <apgcli.h>
#include <SGEVg/VgImage.h>
#include "EightTileManager.h"
#include "EightTileDefs.h"

// CLASS DECLARATION
class CCoeControl;

/**
 *  CEightTileManagerFactory
 * 
 */
class CEightTileManagerFactory : public CBase
    {
public:

    ~CEightTileManagerFactory();

    static CEightTileManagerFactory* NewL( TSize aSize, CCoeControl* aControl );

    void CreateTilesPage( CEightTileManager* aTileManager );
    
    void CreateShortcutsPage( CEightTileManager* aTileManager );

    void ResetStoredFile();
    
    TBool AddTile( CEightTileManager* aTileManager, TUint aTileId, TBool aCustomColor, const TEightPalette& aPalette );

    void ExternalizeL( CEightTileManager* aTileManager ) const;

    void InternalizeL( CEightTileManager* aTileManager, RReadStream& aStream );
    
private:

    CEightTileManagerFactory( TSize aSize, CCoeControl* aControl );

    void ConstructL();

    void LoadPhotoImagesL();

    void LoadSvgImagesL();

    CEightTileUi* CreateTileUi( TInt idx );

    CEightTile* CreateTile( TUint aId, CEightTileUi* aUi, TBool aEmpty  );

    void AddTileTo0( CEightTileManager* aTileManager, TInt idx );

    TBool CheckAppExists( TInt idx );

    void CreateDefaultTilesL( CEightTileManager* aTileManager );

    void ExternalizeInternalL( CEightTileManager* aTileManager, RWriteStream& aStream ) const;
    
private:
    
    RApaLsSession iLs;

    TSize iSize; 
    
    typedef struct
    {
        CVgSvgImage* iImage;
        TPtrC iTitle;
        TPtrC iExeName;
        TPtrC iExeName2;
        TBool iShortcut;
        TBool iPinned;
        TBool iRunTwoTimes;
    } SIcon;
    
    TFixedArray< SIcon, KTilesDefsCount > iSvg;

    CCoeControl* iControl; // not owned
    
    };

#endif // EIGHTTILEMANAGERFACTORY_H
