/*
 ============================================================================
 Name        : Eight.cpp
 Author      : Michal Strug
 Company     : 
 Version     : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : 
 ============================================================================
 */

// INCLUDE FILES
#include "EightApplication.h"

// CCalSession, CContactDatabase CContactItemField 
// http://www.developer.nokia.com/Community/Wiki/Fetch_Image_from_contacts
// http://www.developer.nokia.com/Community/Wiki/Archived:Updating_contact%27s_images_in_S60_3rd_Edition_FP2
// http://docs.huihoo.com/symbian/nokia-symbian3-developers-library-v0.8/GUID-92156269-0645-54AB-9075-EE7708D333F4.html
// KUidContactFieldVCardMapPHOTO KUidContactFieldVCardMapBMP CPbkContactItem CPbkFieldInfo 

// -----------------------------------------------------------------------------
// E32Main()
// -----------------------------------------------------------------------------
//
GLDEF_C TInt E32Main()
    {
    return CEightApplication::Start( CEightApplication::NewApplication );
    }

// End of file
