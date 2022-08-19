/*
 * InnerActiveAdObserver.h
 *
 *  Created on: Jun 10, 2010
 *      Author: inneractive
 *		Copyright: inneractive
 */

#ifndef INNERACTIVEADOBSERVER_H_
#define INNERACTIVEADOBSERVER_H_


typedef enum
{
	EIaaObserver_Undefined				= -1,
	EIaaObserver_AdReady				= 0,
	EIaaObserver_InitApnInProcess		= 1,
	EIaaObserver_InitAdInProcess		= 2,
	EIaaObserver_ReloadAdInProcess		= 3,		
	EIaaObserver_AdImageCorrupted		= 4,
	EIaaObserver_LogoImageCorrupted		= 5,
	EIaaObserver_BadAdXmlResponse		= 6,
	EIaaObserver_ApnSelectionCanceled	= 7,
	EIaaObserver_ApnSelectionError		= 8,
	EIaaObserver_ServerTimeOut			= 9,
	EIaaObserver_ServerError			= 10,
	EIaaObserver_GeneralError			= 11,
	EIaaObserver_SessionFailed			= 12,
	EIaaObserver_GetButtonInProcess		= 13
	
} TInnerActiveAdObserverEvent;

class MIaaObserver
{
public:
	virtual void InnerActiveAdStateChanged(	TInnerActiveAdObserverEvent aEvent, 
											TInt aParam = KErrNone,
											const TDesC8& aData = _L8("")) = 0;
};

#endif /* INNERACTIVEADOBSERVER_H_ */
