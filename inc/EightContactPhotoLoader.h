/*
 ============================================================================
 Name		: EightContactPhotoLoader.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightContactPhotoLoader declaration
 ============================================================================
 */

#ifndef EIGHTCONTACTPHOTOLOADER_H
#define EIGHTCONTACTPHOTOLOADER_H

#include <e32base.h>	// For CActive, link against: euser.lib
#include <e32std.h>		// For RTimer, link against: euser.lib
#include <imageconversion.h>
#include <SGEBitmap/SGEBitmapItem.h>


class MEightContactPhotoLoader
    {
public:
    virtual void ContactPhotoLoaded( TInt aError, CSGEBitmapItem* aBitmapItem ) = 0;
    };


class CEightContactPhotoLoader : public CActive
    {
public:

    ~CEightContactPhotoLoader();

    static CEightContactPhotoLoader* NewL( MEightContactPhotoLoader* aObserver );

    TInt LoadContactL( HBufC8* aBuffer );
    
private:
    
    CEightContactPhotoLoader( MEightContactPhotoLoader* aObserver );

    void ConstructL();

private:

    void RunL();

    void DoCancel();

    TInt RunError( TInt aError );

private:
    
    RFs iFs;
    
    MEightContactPhotoLoader* iObserver;
    
    CImageDecoder* iImageDecoder; // owned
    
    CSGEBitmapItem *iBitmapItem; // owned
    
    RBuf8 iBuf;
    };

#endif // EIGHTCONTACTPHOTOLOADER_H
