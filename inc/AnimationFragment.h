/*
 ============================================================================
 Name		: TouchMenuImageCarousele.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CTouchMenuImageCarousele declaration
 ============================================================================
 */

#ifndef ANIMATIONFRAGMENT_H
#define ANIMATIONFRAGMENT_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>


// CLASS DECLARATION

class TAnimationFragmentBase
	{
public:
	TAnimationFragmentBase() : iStarted( ETrue ) { }
	TBool iStarted;
	TBool IsStarted() const
		{
		return iStarted;
		}
	void Start()
		{
		iStarted = ETrue;
		}
	void Pause()
		{
		iStarted = EFalse;
		}
	};

class TAnimationFragment : public TAnimationFragmentBase
	{
public:
	TAnimationFragment( TInt aUpdateTime ) : iUpdateTime( aUpdateTime ), iUpdateTimeCounter( 0 ) { }
	TInt iUpdateTime;
	TInt iUpdateTimeCounter;
	TBool Update( TTimeIntervalMicroSeconds32 aTimeInterval )
		{
    	if ( !iStarted )
    		{
    		return EFalse;
    		}
		iUpdateTimeCounter += aTimeInterval.Int();
		if ( iUpdateTimeCounter >= iUpdateTime )
			{
			iUpdateTimeCounter = 0;
			return ETrue;
			}
		return EFalse;
		}
	};

template<class T> class TAnimationValueProportionalFragment : public TAnimationFragmentBase
    {
public:
    TAnimationValueProportionalFragment( TInt aBaseUpdateTime, T aValueUpdate, TInt aMaxIntervalTime = 0 )
      : iBaseUpdateTime( aBaseUpdateTime ), iMaxIntervalTime( aMaxIntervalTime ), iValue( 0 ), iValueUpdate( aValueUpdate ) { }
    TInt iBaseUpdateTime;
    TInt iMaxIntervalTime;
    T iValue;
    T iValueUpdate;
    TBool Update( TTimeIntervalMicroSeconds32 aTimeInterval )
        {
    	if ( !iStarted )
    		{
    		return EFalse;
    		}
        TReal bt = iBaseUpdateTime;
        bt /= 1000000;
        TReal t = aTimeInterval.Int();
        if ( iMaxIntervalTime != 0 && t > iMaxIntervalTime )
            {
            t = iMaxIntervalTime;
            }
        t /= 1000000;
        iValue += iValueUpdate * t / bt; 
        return ETrue;
        }
    };

template<class T> class TAnimationValueFragment : public TAnimationFragmentBase
    {
public:
    TAnimationValueFragment( TInt aUpdateTime, T aValueUpdate ) 
        : iUpdateTime( aUpdateTime ), iUpdateTimeCounter( 0 ), iValue( 0 ), iValueUpdate( aValueUpdate ), 
          iUseValueRange( EFalse ), iInfinite( ETrue ), iFinished( EFalse ) { }
    TAnimationValueFragment( TInt aUpdateTime, T aValueUpdate, T aValueMin, T aValueMax, TBool aInfinite = ETrue ) 
        : iUpdateTime( aUpdateTime ), iUpdateTimeCounter( 0 ), iValue( 0 ), iValueUpdate( aValueUpdate ),
          iUseValueRange( ETrue ), iValueMin( aValueMin ), iValueMax( aValueMax ), iInfinite( aInfinite ), iFinished( EFalse ) { }
    TInt iUpdateTime;
    TInt iUpdateTimeCounter;
    TBool Update( TTimeIntervalMicroSeconds32 aTimeInterval )
        {
    	if ( !iStarted )
    		{
    		return EFalse;
    		}
        iUpdateTimeCounter += aTimeInterval.Int();
        if ( iUpdateTimeCounter >= iUpdateTime )
            {
            iUpdateTimeCounter = 0;
            iValue += iValueUpdate;
            if ( iUseValueRange )
                {
                if ( iInfinite )
                    {
                    if ( iValue > iValueMax )
                        {
                        iValue = iValueMin;
                        }
                    else if ( iValue < iValueMin )
                        {
                        iValue = iValueMax;
                        }
                    }
                else
                    {
                    if ( iValue > iValueMax )
                        {
                        iFinished = ETrue;
                        iValue = iValueMax;
                        }
                    else if ( iValue < iValueMin )
                        {
                        iFinished = ETrue;
                        iValue = iValueMin;
                        }
                    }
                }
            return ETrue;
            }
        return EFalse;
        }
    TBool IsFinished() const
        {
        return ( !iInfinite && iFinished );
        }
    T iValue;
    T iValueUpdate;
    TBool iUseValueRange;
    T iValueMin;
    T iValueMax;
    TBool iInfinite;
    TBool iFinished;
    };

#endif // ANIMATIONFRAGMENT_H
