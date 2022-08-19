/*
 ============================================================================
 Name		: AdControl.h
 Author	  : inneractive
 Copyright   : inneractive
 Description : CAdControl declaration
 ============================================================================
 */

#ifndef ADCONTROL_H
#define ADCONTROL_H

// INCLUDES
#include <coecntrl.h>
#include <eiklabel.h>
#include "iaAdControlObserver.h"

// CLASS DECLARATION

/**
 *  CAdControl
 * 
 */
class CAdControl: public CCoeControl
{
public:
	CAdControl(	const TRect& aRect, 
				TBool abPenEnbaled,
				TBool aFullScreen,
				CFbsBitmap* apFbsBitmap,
				TDesC& aText, 
				TInt aControlType = -1,
				TBool abLargeFont = EFalse,
				MiaAdControlObserver* apAdControlObserver = NULL);
	~CAdControl();
	
	static CAdControl* NewL(const TRect& aRect, 
							TBool abPenEnbaled,
							TBool aFullScreen,
							CFbsBitmap* apFbsBitmap,
							TDesC& aText, 
							TInt aControlType = -1,
							TBool abLargeFont = EFalse, 
							const CCoeControl* aParent = NULL,
							MiaAdControlObserver* apAdControlObserver = NULL);
	
	static CAdControl* NewLC(const TRect& aRect, 
							TBool abPenEnbaled,
							TBool aFullScreen,
							CFbsBitmap* apFbsBitmap,
							TDesC& aText, 
							TInt aControlType = -1,
							TBool abLargeFont = EFalse, 
							const CCoeControl* aParent = NULL,
							MiaAdControlObserver* apAdControlObserver = NULL);
	
    void ConstructFromResourceL(TResourceReader& aReader);
    void UpdateData(CFbsBitmap* apBitmap, TDesC& aText);
    //void UpdateData(CWsBitmap* apBitmap, TDesC& aText){};
    void UpdateText(TDesC& aText);
    
public: // from CCoeControl
    TSize MinimumSize();
    void HandlePointerEventL(const TPointerEvent& aPointerEvent);
    
private: // from CCoeControl
    void Draw(const TRect& aRect) const;
    void SizeChanged();
    TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
    
private: // own methods
    void ConstructL(const CCoeControl* aParent = NULL);
    void DrawFocusFrame(const TRect& aRect) const;
    TSize GetAppropriateBitmapSize();

private: // data
    CEikLabel* 		ipLabel;
    CFbsBitmap*		ipBitmap;
    TBuf<150>		iText;
    TBool			ibLargeFont;
    TRect			iInitialRect;
    
    MiaAdControlObserver* ipAdControlObserver;

    TBool 			ibPenEnabled;
    TBool 			iFullScreen;
    
    TInt			iControlType;
    
};

#endif // ADCONTROL_H
