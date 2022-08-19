/*
 ============================================================================
 Name       : EightTileDefs.h
 Author   : Michal Strug
 Version     : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : 
 ============================================================================
 */

#ifndef EIGHTTILEDEFS_H
#define EIGHTTILEDEFS_H

#include "EightCompilationSettings.h"


enum
{
    // must start from 0
    KIdxTileAbout, 
    KIdxTileAds,    
    KIdxTileCalc,
    KIdxTileCalendar,
    KIdxTileCamera,
    KIdxTileClock,
    KIdxTileEmail,
#ifndef EIGHT_PREMIUM
    KIdxTilePremium,
#endif
    KIdxTileFacebook,
    KIdxTileFolder,
    KIdxTileHelp,
    KIdxTileRadio,
    KIdxTileMaps,
    KIdxTileMenu,
    KIdxTileMessaging,
    KIdxTileHeadphones,
    KIdxTileMusic,
    KIdxTileNotes,
    KIdxTilePeople,
    KIdxTilePeoples,
    KIdxTilePhone,
    KIdxTilePhotos,
    KIdxTilePlay,
    KIdxTilePoint,
    KIdxTileRecorder,
    KIdxTileSearch,
    KIdxTileSettings,
    KIdxTileSetup,
    KIdxTileSkype,
    KIdxTileSocial,
    KIdxTileStore,
    KIdxTileThemeColor,
    KIdxTileTwitter,
    KIdxTileVideos,
    KIdxTileWeb,
    
    // table size:
    KTilesDefsCount,
    
    // others:
    
};


#endif
