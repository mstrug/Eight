/*
 ============================================================================
 Name		: EightMsgBox.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightMsgBox declaration
 ============================================================================
 */

#ifndef EIGHTMSGBOX_H
#define EIGHTMSGBOX_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <w32std.h>
#include <touchfeedback.h>
#include <SGEVg/VgFontManager.h>
#include "TouchButtonText.h"
#include "TouchButtonObserver.h"



const TInt KMsgBoxButtonYes = 1;
const TInt KMsgBoxButtonNo = 2;
const TInt KMsgBoxButtonOk = 1;

const TInt KMsgBoxMessageExit = 1;
const TInt KMsgBoxMessageRemoveTile = 2;
const TInt KMsgBoxMessagePinAds = 3;



// CLASS DECLARATION

class MEightMsgBoxObserver
    {
public:
    virtual void MsgBoxAction( TUint aMessageId, TInt aActionId ) = 0;

    virtual void MsgBoxWillShow() { };
    };

/**
 *  CEightMsgBox
 * 
 */
class CEightMsgBox : public CBase, MTouchButtonObserver
    {
public:
    
    typedef enum
    {
        EMsgBoxButtonOk,
        EMsgBoxButtonYesNo
    } TMsgBoxButton;
    
public:

    CEightMsgBox( TSize aScreenSize, CVgFontManager& aFontManager, TInt aFontTitleId, TInt aFontTextId, TInt aFontButtonId );

    ~CEightMsgBox();
    
    void RegisterObserver( MEightMsgBoxObserver* aObserver );
    
    void UnregisterObserver( MEightMsgBoxObserver* aObserver );

    void SetBlock();
    
    TBool IsVisible() const;

    TUint MessageId() const;
    
    TBool ShowL( TUint aMessageId, TMsgBoxButton aButtons, const TDesC& aMessage );
    
    void Hide();
    
    TBool HandlePointerEventL( const TPointerEvent& aPointerEvent );
    
    void Draw();
    
    void Update( TTimeIntervalMicroSeconds32 aTimeInterval );
    
private: // from MTouchButtonObserver
    
    void TouchButtonPressed( TUint aId );
    
private:

    TSize iScreenSize;
        
    CVgFontManager& iFontManager;
    
    RBuf iMessage;
    
    VGPath iBackgroundPath;
    VGPaint iBackgroundPaint;

    VGPath iFadePath;
    VGPath iFadePaint;

    RPointerArray< CTouchButtonText > iButtons;
    
    RArray< MEightMsgBoxObserver* > iObservers;

    TInt iFontTitleId;

    TInt iFontTextId;

    TInt iFontButtonId;
    
    TBool iVisible;
    
    TInt iHeight;

    TUint iMessageId;
    
    VGfloat iOffsetY;
    
    TUint iStoredTouchButtonId;

    MTouchFeedback* iTouchFeedback;
    
    TBool iBlock;
    
    };

#endif // EIGHTMSGBOX_H
