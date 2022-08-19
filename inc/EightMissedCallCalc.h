/*
 ============================================================================
 Name		: EightMissedCallCalc.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightMissedCallCalc declaration
 ============================================================================
 */

#ifndef EIGHTMISSEDCALLCALC_H
#define EIGHTMISSEDCALLCALC_H

#include <e32base.h>
#include <logcli.h>
#include <logview.h>
 
const TInt KTimeDelay = 1000000; // minimum timeout for notification request
 
class CEightMissedCallCalc : public CActive
{
   enum TState
   {
      EWaitingChange = 1,
      EReadingLog,      
      EReadingLogItems,
      EFindingDuplicates,
      EReadingDupeItems
   };
 
   public:
      static CEightMissedCallCalc* NewL(); // factory
      ~CEightMissedCallCalc();
 
      inline TInt Count() { return iCount; } // missed call count
      inline TBool IsCalcCompleted() { return iState == EWaitingChange; } // if true - then Count() returns final value
 
   protected:
      CEightMissedCallCalc();
      void ConstructL();
 
      void GetLatestL();
      void StartL();
      void DoCancel();
      void RunL();
 
      void ReadPreviousL();
 
   private:
      TInt iCount;   // current count of the missed calls
      TInt iDupeCount; //used for interim count of duplicates
      TState iState; // current state
 
      CLogClient* iLogClient;
      CLogViewRecent* iRecentLogView;
      RFs iFsSession;
 
      //to detect duplicates
      CLogViewDuplicate* iDuplicateView;
};


#endif
