/*
 ============================================================================
 Name		: EightSettings.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightSettings declaration
 ============================================================================
 */

#ifndef EIGHTSETTINGS_H
#define EIGHTSETTINGS_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGEVg/VgPrimitives.h>
#include <SGEVg/VgFontManager.h>
#include <SGEVg/VgImageManager.h>
#include <SGEFile/SGEFileProvider.h>
#include <SGEFile/SGEFile.h>
#include "EightPalette.h"
#include "EightTile.h"




const TInt KFontId_SGE = 1;
const TInt KFontId_SegoeWP_small = 2;   //14x19
const TInt KFontId_SegoeWP_medium = 3;  // 17x23
const TInt KFontId_SegoeWP_large = 5;   // 26x36
const TInt KFontId_SegoeWP_largeNum = 4;
const TInt KFontId_SegoeWP_hugeNum = 6;


const TInt KButtonHeight = 35;
const TInt KButtonMargin = 25;
const TInt KButtonWidth = 142; // 142 ~ ( 360 - 3 * KButtonMargin ) / 2; //150;


// CLASS DECLARATION

/**
 *  CEightSettings
 * 
 */
class CEightSettings : public CBase
	{
public:
    
    static CEightSettings* GetInstance();

    void Destroy();
    
    void ResetToDefaults(); // except color
    
    void SetThemeColorIndex( TInt aIdx );

    TInt ThemeColorIndex() const;
    
    CVgFontManager& FontManager();
    
    CSGEFileProvider& FileProvider();

	void ResetSettingsFile();
	
	void SaveToFileL();

    void SetIncreaseTileSizeOnTouch( TBool aEnabled );
    
    TBool IncreaseTileSizeOnTouch() const;
    
    void SetImeiMd5( const TDesC8& aImeiMd5 );
    
    const TDesC& ImeiMd5() const;
    
private:

	CEightSettings();

	~CEightSettings();

	void ConstructL();

	void UpdateAutostartL();
	
	void LoadFromFileL();

    void InternalizeL( RReadStream& aStream );
    
    void ExternalizeL( RWriteStream& aStream );
    
private:

    TInt iThemeColorIdx; // 0 .. KEightColorsCount
	
	TBool iIncreaseTileSizeOnTouch;

public:
	
	TEightPalette iPalette;
	
	TVgSize iScale[ KEightTileStateCount ][ KEightTileSizeCount ];
	
	TBool iShowBottomButtons;
	
    TBool iAnimateAppStartStop;

    TBool iAnimateWindows;

	TBool iAutostart;

    TBool iCloseOnRedKeyPress;

    TBool iUseTouchFeedback;
    
    TBool iCaptureMenuKey;
    
    TUint32 iAdClientId;
    
    TUint32 iIap;

    TUint32 iAdQueryShowed;

private:
	
    CVgFontManager* iFontManager; // owned
    
    CSGEFileProvider* iSGEFileProvider; // owned
    
    RBuf iImeiMd5;

	};

#endif // EIGHTSETTINGS_H
