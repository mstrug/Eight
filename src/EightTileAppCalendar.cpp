/*
 ============================================================================
 Name		: EightTileAppCalendar.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileAppCalendar implementation
 ============================================================================
 */

#include "EightTileAppCalendar.h"
#include "EightSettings.h"
#include "EightTileUiStatic.h"
//#include "logger.h"
#include <aknappui.h> 
#include <APGCLI.H> 
#include <APACMDLN.H> 
#include <APGWGNAM.H> 
#include <e32std.h> 

const TPtrC KDayNamesAbr[] = { _L("Mon"), _L("Tue"), _L("Wed"), _L("Thu"), _L("Fri"), _L("Sat"), _L("Sun") };
const TPtrC KDayNames[] = { _L("Monday"), _L("Tuesday"), _L("Wednesday"), _L("Thursday"), _L("Friday"), _L("Saturday"), _L("Sunday") };


CEightTileAppCalendar::CEightTileAppCalendar( TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi )
    : CEightTile( aId, aTileSize, aUi, EFalse ), iNameIdx( KErrNotFound ), iDayIdx( KErrNotFound ), iDayNameIdx( KErrNotFound ), 
      iEventTitleIdx( KErrNotFound ), iEventLine2Idx( KErrNotFound ), iEventLine3Idx( KErrNotFound )
    {
    }

CEightTileAppCalendar::~CEightTileAppCalendar()
    {
    CEightPhotosProvider::GetInstanceL()->UnregisterFontLoadClient( this );
	
    delete iTimer;
    delete iCalSession;
    delete iCalFilter;
    delete iChangeNotifier;
    }

CEightTileAppCalendar* CEightTileAppCalendar::NewL( TUint aId, TEightTileSize aTileSize, CEightTileUi* aUi )
    {
    CEightTileAppCalendar* self = new ( ELeave ) CEightTileAppCalendar( aId, aTileSize, aUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self;
    return self;
    }

void CEightTileAppCalendar::ConstructL()
    {
    BaseConstructL();
    
    iTimer = CSGEUtilsGPTimer::NewL( *this );
    
    iCalSession = CCalSession::NewL();
    iCalSession->OpenL( KNullDesC() );

    CEightTileUiStatic* ui = ((CEightTileUiStatic*)iUi);
    if ( ui )
        {
        iNameIdx = ui->AddTextL( TVgRect( TVgPoint( 6, 70 ), TVgPoint( 70, 6 ) ), KFontId_SegoeWP_medium, _L("Calendar"), EFalse, ESGEFontAlignHorizontalLeft, ESGEFontAlignVerticalBottom );
        
        iDayIdx = ui->AddTextL( TVgRect( TVgPoint( 6, 70 ), TVgPoint( 170, 6 ) ), KFontId_SegoeWP_largeNum, _L("01"), EFalse, ESGEFontAlignHorizontalRight, ESGEFontAlignVerticalBottom );

        iDayNameIdx = ui->AddTextL( TVgRect( TVgPoint( 6, 70 ), TVgPoint( 150, 6 ) ), KFontId_SegoeWP_large, _L("Mon"), EFalse, ESGEFontAlignHorizontalRight, ESGEFontAlignVerticalBottom );

        iEventTitleIdx = ui->AddTextL( TVgRect( TVgPoint( 6, 70 ), TVgPoint( 150, 6 ) ), KFontId_SegoeWP_small, _L("Title"), EFalse, ESGEFontAlignHorizontalLeft, ESGEFontAlignVerticalBottom );
        iEventLine2Idx = ui->AddTextL( TVgRect( TVgPoint( 6, 70 ), TVgPoint( 150, 6 ) ), KFontId_SegoeWP_small, _L("Place"), EFalse, ESGEFontAlignHorizontalLeft, ESGEFontAlignVerticalBottom );
        iEventLine3Idx = ui->AddTextL( TVgRect( TVgPoint( 6, 70 ), TVgPoint( 150, 6 ) ), KFontId_SegoeWP_small, _L("Duration"), EFalse, ESGEFontAlignHorizontalLeft, ESGEFontAlignVerticalBottom );
        
        UpdateEvents();

        
        TTime time;
        time.HomeTime();
        TTime timeS;
        timeS.HomeTime();
        timeS += TTimeIntervalDays( 7 );
        TCalTime t1, t2;
        t1.SetTimeLocalL( time );
        t2.SetTimeLocalL( timeS );
        CalCommon::TCalTimeRange tr( t1, t2 );
        iCalFilter = CCalChangeNotificationFilter::NewL( EChangeEntryEvent, EFalse, tr );
        iCalSession->StartChangeNotification( *this, *iCalFilter );
        
        
        iChangeNotifier = CEnvironmentChangeNotifier::NewL( CActive::EPriorityHigh, TCallBack( CallbackNotifier, this ) );
        iChangeNotifier->Start();
        
        CEightPhotosProvider::GetInstanceL()->RegisterFontLoadClientL( this );
        }
    }

TEightTileType CEightTileAppCalendar::Type() const
    {
    return ETileTypeAppShortcut;
    }

void CEightTileAppCalendar::UpdateDate()
    {
    CEightTileUiStatic* ui = ((CEightTileUiStatic*)iUi);
    if ( ui ) 
        {
        TTime time;
        time.HomeTime();
        TInt textW = 30;
        
        TBuf<26> dayBuf;
        dayBuf.Num( time.DayNoInMonth() + 1 );
        if ( time.DayNoInMonth() + 1 < 10 ) dayBuf.Insert( 0, _L("0") );
        CVgFont* font = CEightSettings::GetInstance()->FontManager().Get( KFontId_SegoeWP_largeNum );
        if ( font ) 
        	{
        	textW = font->GetTextWidth( dayBuf );
            ui->SetItemHide( iDayIdx, ( iTileSize == ETileSize_1x1 ) );
            ui->SetItemHide( iDayNameIdx, ( iTileSize == ETileSize_1x1 ) );
        	}
        else
        	{
            ui->SetItemHide( iDayIdx, ETrue );
            ui->SetItemHide( iDayNameIdx, ETrue );
        	}
//        ui->UpdateRect( iDayIdx, TVgRect( TVgPoint( iRect.Width() - 45, 70 ), TVgPoint( iRect.Width(), 8 ) ) );
        ui->UpdateRect( iDayIdx, TVgRect( TVgPoint( 0, 70 ), TVgPoint( iRect.Width() - 10, 8 ) ) );
        ui->UpdateText( iDayIdx, dayBuf );
        
        TDay d = time.DayNoInWeek();
        ui->UpdateRect( iDayNameIdx, TVgRect( TVgPoint( 0, 70 ), TVgPoint( iRect.Width() - 10 - textW - 5, 10 ) ) );
        ui->UpdateText( iDayNameIdx, KDayNamesAbr[ d ] );

        TInt h = iRect.Height();
        TInt w = iRect.Width();
        TInt a = 20; //24;
        TInt s = 21; //26;
        ui->UpdateRect( iEventTitleIdx, TVgRect( TVgPoint( 6, h ), TVgPoint( w - 10, h - a ) ) );
        ui->UpdateRect( iEventLine2Idx, TVgRect( TVgPoint( 6, h - 30 ), TVgPoint( w - 10, h - a - s ) ) );
        ui->UpdateRect( iEventLine3Idx, TVgRect( TVgPoint( 6, h - 60 ), TVgPoint( w - 10, h - a - 2 * s ) ) );
        }
    }

void CEightTileAppCalendar::UpdateEvents()
    {
    TTime time;
    time.HomeTime();
    TTime timeS;
    timeS.HomeTime();
    timeS += TTimeIntervalDays( 7 );
    CCalInstanceView* view = CCalInstanceView::NewL( *iCalSession );
    RPointerArray< CCalInstance > tab;
    CalCommon::TCalViewFilter filter = CalCommon::EIncludeAppts | CalCommon::EIncludeEvents | CalCommon::EIncludeAnnivs;
    TCalTime t1, t2;
    t1.SetTimeLocalL( time );
    t2.SetTimeLocalL( timeS );
    CalCommon::TCalTimeRange tr( t1, t2 );
    view->FindInstanceL( tab, filter, tr );
    if ( tab.Count() > 0 )
        {
        CEightTileUiStatic* ui = ((CEightTileUiStatic*)iUi);
        if ( ui ) 
            {
            ui->UpdateText( iEventTitleIdx, tab[ 0 ]->Entry().SummaryL() );
            TPtrC loc( tab[ 0 ]->Entry().LocationL() );
            if ( loc.Length() != 0 )
                {
                ui->UpdateText( iEventLine2Idx, tab[ 0 ]->Entry().LocationL() );
                }

            //TBuf<64> tb1, tb2;
            TBuf<128> tb;
            TTime s1 = tab[ 0 ]->StartTimeL().TimeLocalL();
            TTime s2 = tab[ 0 ]->EndTimeL().TimeLocalL();
            
            if ( s2.DaysFrom( s1 ).Int() == 1 && s1.DateTime().Hour() == 0 && s1.DateTime().Minute() == 0 &&
                 s2.DateTime().Hour() == 0 && s2.DateTime().Minute() == 0 )
                {
                tb.Append( KDayNames[ s1.DayNoInWeek() ] );
                tb.Append( _L(" - all day") );                
                }
            else
                {
                tb.Append( KDayNames[ s1.DayNoInWeek() ] );
                tb.Append( _L(" ") );
                tb.AppendNum( s1.DateTime().Hour() );
                tb.Append( _L(":") );
                if ( s1.DateTime().Minute() < 10 ) tb.AppendNum( 0 );
                tb.AppendNum( s1.DateTime().Minute() );
                tb.Append( _L(" - ") );
                if ( s1.DateTime().Day() != s2.DateTime().Day() )
                    {
                    tb.Append( KDayNames[ s2.DayNoInWeek() ] );
                    tb.Append( _L(" ") );
                    }
                tb.AppendNum( s2.DateTime().Hour() );
                tb.Append( _L(":") );
                if ( s2.DateTime().Minute() < 10 ) tb.AppendNum( 0 );
                tb.AppendNum( s2.DateTime().Minute() );
                }
            
//            TLocale locale;
//            locale.SetCountryCode( 44 );
//            s1.FormatL( tb1, _L("%E %J:%T"), locale );
//            if ( s1.DateTime().Day() == s2.DateTime().Day() )
//                {
//                s2.FormatL( tb2, _L("%J:%T"), locale );
//                }
//            else
//                {
//                s2.FormatL( tb2, _L("%E %J:%T"), locale );
//                }
//            RBuf bf;
//            bf.Create( tb1.Length() + tb2.Length() + 3 );
//            bf.Append( tb1 );
//            bf.Append( _L(" - ") );
//            bf.Append( tb2 );
            if ( loc.Length() != 0 )
                {
                ui->UpdateText( iEventLine3Idx, tb );
                }
            else
                {
                ui->UpdateText( iEventLine2Idx, tb );
                ui->UpdateText( iEventLine3Idx, KNullDesC() );
                }
            //bf.Close();
            }
        }
    else
        {
        CEightTileUiStatic* ui = ((CEightTileUiStatic*)iUi);
        if ( ui ) 
            {
            if ( iTileSize == ETileSize_4x2 )
                {
                ui->UpdateText( iEventTitleIdx, _L("No calendar events") );
                }
            else
                {
                ui->UpdateText( iEventTitleIdx, _L("No events") );
                }
            ui->UpdateText( iEventLine2Idx, KNullDesC() );
            ui->UpdateText( iEventLine3Idx, KNullDesC() );
            }
        }
    /*for ( TInt i = 0; i < tab.Count(); i++ )
        {
        Logger::LogStr( _L("c:\\data\\cal.txt"), tab[ i ]->Entry().SummaryL() );
        }*/
    tab.ResetAndDestroy();
    delete view;
    }

TInt CEightTileAppCalendar::CallbackNotifier( TAny* aPtr )
    {
    TInt val = ((CEightTileAppCalendar*)aPtr)->iChangeNotifier->Change();
    if ( ( val & EChangesMidnightCrossover ) == EChangesMidnightCrossover ||
         ( val & EChangesSystemTime ) == EChangesSystemTime )
        {
        ((CEightTileAppCalendar*)aPtr)->UpdateDate();
        }
    return KErrNone;
    }

void CEightTileAppCalendar::SetRect( const TVgRect& aRect )
    {
    CEightTile::SetRect( aRect );
    
    CEightTileUiStatic* ui = ((CEightTileUiStatic*)iUi);
    if ( ui ) 
        {
        ui->SetItemHide( 0, ( iTileSize == ETileSize_4x2 || iTileSize == ETileSize_2x2 ) );
        
        ui->SetItemHide( iNameIdx, ( iTileSize == ETileSize_1x1 || iTileSize == ETileSize_2x2 ) );
        ui->UpdateRect( iNameIdx, TVgRect( TVgPoint( 6, 70 ), TVgPoint( iRect.Width() - 6, 6 ) ) );

        //ui->SetItemHide( iDayIdx, ( iTileSize == ETileSize_1x1 ) );
        //ui->SetItemHide( iDayNameIdx, ( iTileSize == ETileSize_1x1 ) );

        ui->SetItemHide( iEventTitleIdx, ( iTileSize == ETileSize_1x1 ) );
        ui->SetItemHide( iEventLine2Idx, ( iTileSize == ETileSize_1x1 ) );
        ui->SetItemHide( iEventLine3Idx, ( iTileSize == ETileSize_1x1 ) );

        UpdateDate();
        }
    }

TBool CEightTileAppCalendar::RunInternal()
    {
    if ( !iTimer->IsTimerStarted() )
        {
        iTimer->Start( 50000 );
        }
    return ETrue;
    }

void CEightTileAppCalendar::GPTimerEvent( CSGEUtilsGPTimer* /*aTimer*/ )
    {
    CApaCommandLine* cl = CApaCommandLine::NewLC();
    cl->SetExecutableNameL( _L("calendar.exe") );

    RApaLsSession ls;
    ls.Connect();
    
    TInt err = ls.StartApp( *cl );
    
    if ( err == KErrNone )
        {
        ls.GetAllApps();
        TApaAppInfo info;
        TUid uid = { 0 };
        while ( ls.GetNextApp( info ) == KErrNone )
            {
            if ( info.iFullName.FindF( _L("calendar.exe") ) != KErrNotFound )
                {
                uid = info.iUid;
                break;
                }
            }
        
        if ( uid.iUid )
            {
            RWsSession& ws( CCoeEnv::Static()->WsSession() );
            TInt id = 0;
            CApaWindowGroupName::FindByAppUid( uid, ws, id );
            if ( id > 0 )
                {
                TApaTask task( ws );
                task.SetWgId( id );
                task.BringToForeground();

                TWsEvent ev;
                ev.SetTimeNow();
                ev.SetType( EEventFocusGained );
                ws.SendEventToWindowGroup( id, ev );
                }
            }
        }
    
    ls.Close();
    CleanupStack::PopAndDestroy();
    }

void CEightTileAppCalendar::CalChangeNotification( RArray<TCalChangeEntry>& /*aChangeItems*/ )
    {
    UpdateEvents();
    }

void CEightTileAppCalendar::FontsLoaded()
	{
	UpdateDate();
	}

