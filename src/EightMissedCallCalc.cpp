/*
 ============================================================================
 Name		: EightMissedCallCalc.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightMissedCallCalc implementation
 ============================================================================
 */

#include "EightMissedCallCalc.h"
#include <logfilterandeventconstants.hrh>
 

CEightMissedCallCalc* CEightMissedCallCalc :: NewL()
{
   CEightMissedCallCalc* self = new (ELeave) CEightMissedCallCalc();
   CleanupStack :: PushL( self );
   self->ConstructL();
   CleanupStack :: Pop(self);
   return self;
}
 
 
CEightMissedCallCalc :: CEightMissedCallCalc(): CActive( CActive :: EPriorityStandard )
{
}
 
CEightMissedCallCalc :: ~CEightMissedCallCalc()
{
   Cancel();
 
   delete iRecentLogView;
   iRecentLogView = NULL;
 
   delete iDuplicateView;
   iDuplicateView = NULL;
 
   delete iLogClient;
   iLogClient = NULL;
 
   iFsSession.Close();
}
 
 
void CEightMissedCallCalc :: ConstructL()
{	
   CActiveScheduler :: Add( this );
 
   User::LeaveIfError( iFsSession.Connect() );
 
   iLogClient = CLogClient :: NewL( iFsSession );
   iRecentLogView = CLogViewRecent :: NewL( *iLogClient );
   iDuplicateView = CLogViewDuplicate::NewL(*iLogClient);
 
   GetLatestL();		
}
 
void CEightMissedCallCalc :: DoCancel()
{
   if( iRecentLogView )
      iRecentLogView->Cancel();
 
   if( iLogClient )
   {
      if( iState == EWaitingChange )
         iLogClient->NotifyChangeCancel();
      else
         iLogClient->Cancel();
   }
}
 
void CEightMissedCallCalc :: RunL()
{
   if( iStatus != KErrCancel )
      switch( iState )
      {
         case EWaitingChange: // new event
            GetLatestL();
            break;
 
         case EReadingLog: // start reading log events from last to first 
            if( iRecentLogView->CountL() > 0 )
            {	
               iCount = 0; // clear value
               iState = EReadingLogItems;
               if( iRecentLogView->LastL( iStatus ) ) // to last event
                  SetActive();
               else
                  StartL();
            }
            else
               StartL();
            break;
 
 
         case EReadingLogItems: // reading event
            if( iStatus == KErrNone && iRecentLogView )
            {
               TLogFlags iFlags = iRecentLogView->Event().Flags(); 
               if( !( iFlags & KLogEventRead ) )
               {
                    //we are going to check to see if we have a Duplicate
                    iState = EFindingDuplicates;
                    if (iRecentLogView->DuplicatesL(*iDuplicateView, iStatus))
                    {
                        SetActive();
                    }
                    else
                    {
                        //for some reason if you missed x calls from number y then you
                        //answered one we fail the call to DuplicatesL and count is then off
                        //(if there were duplicates)
                        //should still add one for the one we missed
                        iCount++;
                        //then continue going "previous" in the list
                        ReadPreviousL();
                    }
               }
               else
               {
                    ReadPreviousL();
               }
 
            }
            else
               StartL();	
            break;
 
        case EFindingDuplicates:
            iDupeCount = 0;
            if( iStatus == KErrNone && iRecentLogView
             && iDuplicateView && iDuplicateView->CountL() > 0 )
            {
                iState = EReadingDupeItems;
                if( iDuplicateView->LastL( iStatus ) ) // to last event
                {
                    SetActive();
                }
                else
                {
                    //didn't manage to go through list of dupes, but we had at least
                    //one missed call, so increment here
                    iCount++;
                    //anyway, we're done with duplicates
                    ReadPreviousL();
                }
            }
            else
            {
                //there was no duplicate but there was one, right?
                iCount++;
                //anyway, we're done with duplicates
                ReadPreviousL();
            }
            break;
 
        case EReadingDupeItems:
            if( iStatus == KErrNone && iDuplicateView )
            {
                TLogFlags iFlags = iDuplicateView->Event().Flags();
                if( !( iFlags & KLogEventRead ) )
                {
                    iDupeCount++;
                }
                else
                {
                    //since the dupe was read and is more recent than previous ones
                    //the older unread ones don't actually count but clearly the count
                    //is never going below 1
                    iDupeCount = 1;
                }
                //then continue going "previous" in the list
                if( iDuplicateView->PreviousL( iStatus ) ) // try to read prev. event
                {
                    SetActive();
                }
                else
                {
                    //at this point we add our number of dupes to our count
                    //if for some reason dupes is zero we still add one since that
                    //was the number we must have
                    if (iDupeCount == 0)
                    {
                        iCount++;
                    }
                    else
                    {
                        iCount += iDupeCount;
                    }
                    //at this point we have got all we can from the dupes
                    //return to the main list
                    ReadPreviousL();
                }
            }
            else
            {
                //at this point we add our number of dupes to our count
                //if for some reason dupes is zero we still add one since that
                //was the number we must have
                if (iDupeCount == 0)
                {
                    iCount++;
                }
                else
                {
                    iCount += iDupeCount;
                }
                //at this point we have got all we can from the dupes
                //return to the main list
                ReadPreviousL();
            }
            break;
 
         default:
            StartL();
            break;
      }	
}	
 
void CEightMissedCallCalc :: ReadPreviousL()
{
    iState = EReadingLogItems;
    if( iRecentLogView->PreviousL( iStatus ) ) // try to read prev. event
    {
        SetActive();
    }
    else
    {
        StartL();
    }
}
 
void CEightMissedCallCalc :: StartL()
{
   if( iRecentLogView )
      iRecentLogView->Cancel();
 
   iLogClient->Cancel();
 
   iState = EWaitingChange;
   iLogClient->NotifyChange( TTimeIntervalMicroSeconds32( KTimeDelay ), iStatus );
   SetActive();
}
 
void CEightMissedCallCalc :: GetLatestL()
{
   iState = EReadingLog;
   iRecentLogView->Cancel();
   if( iRecentLogView->SetRecentListL( KLogRecentMissedCalls, iStatus ) )
      SetActive();
   else
      StartL();
}

