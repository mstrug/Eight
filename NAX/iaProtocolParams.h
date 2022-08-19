/*
 * iaProtocolParams.h
 *
 *  Created on: Jun 30, 2010
 *      Author: inneractive
 */

#ifndef IAPROTOCOLPARAMS_H_
#define IAPROTOCOLPARAMS_H_


typedef enum
{
	EIAAPNMODE_APN_SELECTION_DIALOG	= 0,
	EIAAPNMODE_CURRENT_APN			= 1,
	EIAAPNMODE_NEW_APN				= 2
	
}TIaAPNMode;

typedef enum
{
	
	// Must be one of pre-defined channels in the system, possible values provided by innerActive.
	// For example, for the Ovi Store the value should be 551.
	EIaProtocolParams_DistributionChannelPortal = 0,
	
	// An application specific id – the id of the requesting device in the partner’s domain.
	EIaProtocolParams_ExternalId				= 1,
	
	// User’s age (value between 1 and 99)
	EIaProtocolParams_Age						= 2,
	
	// User’s gender. May be oneof: "M, F, male, female". Case insensitive.
	EIaProtocolParams_Gender					= 3,
	
	// User's mobile number. MSISDN format, with international prefix  
	EIaProtocolParams_MobileNumber				= 4,
	
	// Keywords relevant to this user’s specific session. Comma separated list.
	EIaProtocolParams_Keywords					= 5,
	
	// Category – a single word description of the application.
	EIaProtocolParams_Category					= 6,
	
	// Location string – comma separated list of country, state/province, city. For example: US, NY, NY
	EIaProtocolParams_LocationString			= 7,
	
	// GPS location – ISO code location data. In latitude, longitude format.
	EIaProtocolParams_LocationGps				= 8,
	
	// Test mode. Will always return a test campaign. Values for activating test mode – case insensitive "true", "1".
	EIaProtocolParams_TestMode					= 9,
	
} TIaProtocolParams;

#endif /* IAPROTOCOLPARAMS_H_ */
