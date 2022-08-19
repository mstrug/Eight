/*
 ============================================================================
 Name		: InnerActiveMobileAd.h
 Author	  : inneractive
 Copyright   : inneractive
 Description : CInnerActiveMobileAd declaration
 ============================================================================
 */

#ifndef INNERACTIVEMOBILEAD_H
#define INNERACTIVEMOBILEAD_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "InnerActiveAdObserver.h"
#include "AdControl.h"
#include "iaProtocolParams.h"
// CLASS DECLARATION
class CiaKernel;
/**
 *  CInnerActiveMobileAd
 * 
 */
class CInnerActiveMobileAd: public CBase, public MIaaObserver
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CInnerActiveMobileAd();

	/**
	 * Two-phased constructor.
	 */
	static CInnerActiveMobileAd* NewL(CCoeControl* aRoot, MIaaObserver& aObserver, TDesC& aInnerActiveAppUid, TUint32 aApnUid = 0, TInt aAdReloadPeriodSec = 120);

	/**
	 * Two-phased constructor.
	 */
	static CInnerActiveMobileAd* NewLC(CCoeControl* aRoot, MIaaObserver& aObserver, TDesC& aInnerActiveAppUid, TUint32 aApnUid = 0, TInt aAdReloadPeriodSec = 120);

public:
	
	// Initializes ad module infrastructure.
	//
	// Can leave with following:
	// 1. KErrBadName - InnerActiveAppUid is empty.
	// 2. KErrInUse - Initializing or Ad Reload process is already started.
	void 			InitializeL();
	
	// Reconnects Ad module to network and performs infrastructure initialization.
	// @apnMode - APN selection mode 
	//		EIAAPNMODE_APN_SELECTION_DIALOG - reconnects using APN selected from system APN selection dialog.
	//		EIAAPNMODE_CURRENT_APN - reconnects using current (previously provided) APN. If APN wasn't provided system APN dialog pops up.
	//		EIAAPNMODE_NEW_APN - reconnects using APN UID passed in second function parameter.
	// @aNewApnUid - client side originated APN UID. 
	//
	// Can leave with following:
	// 1. KErrBadName - InnerActiveAppUid is empty.
	// 2. KErrInUse - Initializing or Ad Reload process is already started.
	void			ReconnectL(TIaAPNMode apnMode, TUint32 aNewApnUid = 0);
	
	// Open Ad dialog
	//
	// @aDialogID - ID from resource (RSS/RSG) file 
	// @aSoftkeysSkipId - ID from resource (RSS/RSG) file 
	// @aDlgSkipBtnId -  ID from HRH file 
	// @aDlgOpenAdBtnId -  ID from HRH file 
	// @aDlgContainerType -  ID from HRH file 
	// @aDlgContainerId -  ID from HRH file
	// @aTimeoutSeconds - after this time out the dialog is closed 
	//
	// Can leave with following:
	// 1. KErrNotReady - Ad still not ready
	// 2. KErrCorrupt - Ad images are corrupted
	// 3. System wide error codes
	void			OpenAdDialogL(TUint aDialogID, 
									TInt aSoftkeysSkipId,
									TInt aDlgSkipBtnId,
									TInt aDlgOpenAdBtnId,
									TInt aDlgContainerType,
									TInt aDlgContainerId,
									TInt aTimeoutSeconds = 0);
	
	// Initiate Ad reload mechanism
	// Can leave with following:
	// 1. KErrBadName - InnerActiveAppUid is empty
	// 2. KErrInUse - Initializing or Ad Reload process is started
	void			ReloadAdL();
	
	
	// Get Ad control
	//
	// @aRect - initial rect for the control
	// @apParent - parent control (container)
	//
	// Can leave with following:
	// 1. KErrNotReady - Ad still not ready
	// 2. KErrCorrupt - Ad images are corrupted
	// 3. System wide error codes
	CAdControl* 	GetAdControlL(const TRect& aRect, const CCoeControl* apParent);
	
	// Get current Ad bitmap.
	//
	// Can leave with following:
	// 1. KErrNotReady - Ad still not ready
	// 2. KErrCorrupt - Ad images are corrupted
	// 3. System wide error codes
	CFbsBitmap*		GetCurrentAdBitmapL();
	
	// Update Ad control
	//
	// @apAdControl - control that should be updated
	//
	// Can leave with following:
	// 1. KErrNotReady - Ad still not ready
	// 2. KErrCorrupt - Ad images are corrupted
	// 3. KerrNotFound - control parameter is NULL
	// 4. System wide error codes
	void			UpdateAdControlL(CAdControl* apAdControl);

	// Open current Ad url in browser and start Ad reload
	void ActivateCurrentAd();
	
	// Set Specific inneractive protocol param
	//
	// @param aParamType - see TIaProtocolParams enum
	// @param aParamValue - see TIaProtocolParams enum for data format/restrictions
	//
	// Can leave with following:
	// KErrInUse - Initializing or Ad Reload process is started.
	// KErrOverflow - buffer length > 256 characters
	void SetProtocolParamL(TIaProtocolParams aParamType, const TDesC& aParamValue);


	
private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CInnerActiveMobileAd(MIaaObserver& aObserver);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL(CCoeControl* aRoot, TDesC& aInnerActiveAppUid, TUint32 aApnUid, TInt aAdReloadPeriodSec);

	void InnerActiveAdStateChanged(	TInnerActiveAdObserverEvent aEvent, 
									TInt aParam = -1,
									const TDesC8& aData = _L8(""));
	
private:
	MIaaObserver&		iClientObserver;
	CiaKernel*			ipKernel;

};

#endif // INNERACTIVEMOBILEAD_H
