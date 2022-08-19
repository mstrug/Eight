/*
 ============================================================================
 Name		: EightTileManager.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightTileManager implementation
 ============================================================================
 */

#include "EightTileManager.h"
#include "EightTileSpecial.h"
#include "EightTileAppWindow.h"
#include "EightTileUiStatic.h"
#include "EightPhotosProvider.h"
#include "EightSettings.h"
#include "EightWindowTileSettings.h"
#include "EightWindowAbout.h"
#include "EightWindowHelp.h"
#include "EightWindowSetup.h"
#include "EightWindowColor.h"
#include "EightWindowPremium.h"
#include "EightTileDefs.h"
//#include "logger.h"
#include <VG/vgu.h>
#include <VG/openvg.h>
#include <SGEVg/VgPrimitives.h>
#include <SGEBitmap/SGEBitmapUtils.h>
#include <HAL.H>

#include <fbs.h> //CFbsBitmap
#include <aknsskininstance.h> //MAknsSkinInstance
#include <aknsutils.h> //AknsUtils
#include <gulicon.h>


const TInt KRow[] = { -1, -1, -1, -1 };

const TInt KButtonArrow = 1;
const TInt KButtonRemove = 2;
const TInt KButtonSettings = 3;
const TInt KButtonPin = 4;
const TInt KButtonUnpin = 5;

const TInt KSelectionClosestTileAnimDiv = 6;

_LIT8( KIconArrow, "<?xml version=\"1.0\" standalone=\"no\"?><!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"><svg stroke-width=\"0.501\" stroke-linejoin=\"bevel\" fill-rule=\"evenodd\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" overflow=\"visible\" width=\"22.5pt\" height=\"22.5pt\" viewBox=\"0 0 22.5 22.5\"> <g id=\"Document\" fill=\"none\" stroke=\"black\" transform=\"scale(1 -1)\"><g id=\"Spread\" transform=\"translate(0 -45)\"><ellipse rx=\"10.125\" ry=\"10.125\" transform=\"translate(11.25 33.75) rotate(90)\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#ffffff\" stroke-width=\"2.25\" fill=\"#000000\" stroke-miterlimit=\"79.8403193612775\"/><path d=\"M 7.312,37.688 L 14.812,30.188\" fill=\"none\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#ffffff\" stroke-width=\"3\" stroke-miterlimit=\"79.8403193612775\"/><path d=\"M 8.437,29.813 L 15.187,29.813\" fill=\"none\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#ffffff\" stroke-width=\"3\" stroke-miterlimit=\"79.8403193612775\"/><path d=\"M 15.187,36.563 L 15.187,29.813\" fill=\"none\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#ffffff\" stroke-width=\"3\" stroke-miterlimit=\"79.8403193612775\"/></g></g></svg>" );
_LIT8( KIconArrowSel, "<?xml version=\"1.0\" standalone=\"no\"?><!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"><svg stroke-width=\"0.501\" stroke-linejoin=\"bevel\" fill-rule=\"evenodd\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" overflow=\"visible\" width=\"22.5pt\" height=\"22.5pt\" viewBox=\"0 0 22.5 22.5\"> <g id=\"Document\" fill=\"none\" stroke=\"black\" transform=\"scale(1 -1)\"><g id=\"Spread\" transform=\"translate(0 -45)\"><ellipse rx=\"10.125\" ry=\"10.125\" transform=\"translate(11.25 33.75) rotate(90)\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#ffffff\" stroke-width=\"2.25\" fill=\"#ffffff\" stroke-miterlimit=\"79.8403193612775\"/><path d=\"M 7.312,37.688 L 14.812,30.188\" fill=\"none\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#000000\" stroke-width=\"3\" stroke-miterlimit=\"79.8403193612775\"/><path d=\"M 8.437,29.813 L 15.187,29.813\" fill=\"none\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#000000\" stroke-width=\"3\" stroke-miterlimit=\"79.8403193612775\"/><path d=\"M 15.187,36.563 L 15.187,29.813\" fill=\"none\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#000000\" stroke-width=\"3\" stroke-miterlimit=\"79.8403193612775\"/></g></g></svg>" );
_LIT8( KIconRemove, "<?xml version=\"1.0\" standalone=\"no\"?><!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"><svg stroke-width=\"0.501\" stroke-linejoin=\"bevel\" fill-rule=\"evenodd\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" overflow=\"visible\" width=\"22.5pt\" height=\"22.5pt\" viewBox=\"0 0 22.5 22.5\"><g id=\"Document\" fill=\"none\" stroke=\"black\" transform=\"scale(1 -1)\"><g id=\"Spread\" transform=\"translate(0 -45)\"><ellipse rx=\"10.125\" ry=\"10.125\" transform=\"translate(11.25 33.75) rotate(90)\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#ffffff\" stroke-width=\"2.25\" fill=\"#000000\" stroke-miterlimit=\"79.8403193612775\"/><path d=\"M 7.5,37.5 L 15,30\" fill=\"none\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#ffffff\" stroke-width=\"3\" stroke-miterlimit=\"79.8403193612775\"/><path d=\"M 15,37.5 L 7.5,30\" fill=\"none\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#ffffff\" stroke-width=\"3\" stroke-miterlimit=\"79.8403193612775\"/></g></g></svg>" );
_LIT8( KIconRemoveSel, "<?xml version=\"1.0\" standalone=\"no\"?><!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"><svg stroke-width=\"0.501\" stroke-linejoin=\"bevel\" fill-rule=\"evenodd\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" overflow=\"visible\" width=\"22.5pt\" height=\"22.5pt\" viewBox=\"0 0 22.5 22.5\"><g id=\"Document\" fill=\"none\" stroke=\"black\" transform=\"scale(1 -1)\"><g id=\"Spread\" transform=\"translate(0 -45)\"><ellipse rx=\"10.125\" ry=\"10.125\" transform=\"translate(11.25 33.75) rotate(90)\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#ffffff\" stroke-width=\"2.25\" fill=\"#ffffff\" stroke-miterlimit=\"79.8403193612775\"/><path d=\"M 7.5,37.5 L 15,30\" fill=\"none\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#000000\" stroke-width=\"3\" stroke-miterlimit=\"79.8403193612775\"/><path d=\"M 15,37.5 L 7.5,30\" fill=\"none\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#000000\" stroke-width=\"3\" stroke-miterlimit=\"79.8403193612775\"/></g></g></svg>" );
_LIT8( KIconSettings, "<?xml version=\"1.0\" standalone=\"no\"?><!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"><svg stroke-width=\"0.501\" stroke-linejoin=\"bevel\" fill-rule=\"evenodd\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" overflow=\"visible\" width=\"22.5pt\" height=\"22.5pt\" viewBox=\"0 0 22.5 22.5\"><g id=\"Document\" fill=\"none\" stroke=\"black\" transform=\"scale(1 -1)\"><g id=\"Spread\" transform=\"translate(0 -45)\"><ellipse rx=\"10.125\" ry=\"10.125\" transform=\"translate(11.25 33.75) rotate(90)\" stroke=\"#ffffff\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke-width=\"2.25\" fill=\"#000000\" stroke-miterlimit=\"79.8403193612775\"/><path d=\"M 6.631,28.918 L 6.426,29.126 C 6.106,29.445 6.106,29.962 6.426,30.282 L 11.055,34.836 C 10.304,36.183 10.501,37.917 11.644,39.062 C 12.292,39.71 13.591,39.945 13.463,39.816 L 12.099,38.453 C 11.849,38.202 11.719,37.72 11.811,37.377 L 12.311,35.513 C 12.402,35.171 12.755,34.818 13.098,34.726 L 14.962,34.226 C 15.304,34.135 15.787,34.264 16.037,34.514 L 17.401,35.879 C 17.53,36.008 17.295,34.708 16.646,34.06 C 15.502,32.916 13.767,32.72 12.42,33.472 L 7.789,28.918 C 7.47,28.599 6.951,28.599 6.631,28.918 Z\" stroke=\"#ffffff\" fill=\"#ffffff\" stroke-linejoin=\"miter\" stroke-width=\"1.5\" marker-start=\"none\" marker-end=\"none\" stroke-miterlimit=\"79.8403193612775\"/></g></g></svg>" );
_LIT8( KIconSettingsSel, "<?xml version=\"1.0\" standalone=\"no\"?><!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"><svg stroke-width=\"0.501\" stroke-linejoin=\"bevel\" fill-rule=\"evenodd\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" overflow=\"visible\" width=\"22.5pt\" height=\"22.5pt\" viewBox=\"0 0 22.5 22.5\"><g id=\"Document\" fill=\"none\" stroke=\"black\" transform=\"scale(1 -1)\"><g id=\"Spread\" transform=\"translate(0 -45)\"><ellipse rx=\"10.125\" ry=\"10.125\" transform=\"translate(11.25 33.75) rotate(90)\" stroke=\"#ffffff\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke-width=\"2.25\" fill=\"#ffffff\" stroke-miterlimit=\"79.8403193612775\"/><path d=\"M 6.631,28.918 L 6.426,29.126 C 6.106,29.445 6.106,29.962 6.426,30.282 L 11.055,34.836 C 10.304,36.183 10.501,37.917 11.644,39.062 C 12.292,39.71 13.591,39.945 13.463,39.816 L 12.099,38.453 C 11.849,38.202 11.719,37.72 11.811,37.377 L 12.311,35.513 C 12.402,35.171 12.755,34.818 13.098,34.726 L 14.962,34.226 C 15.304,34.135 15.787,34.264 16.037,34.514 L 17.401,35.879 C 17.53,36.008 17.295,34.708 16.646,34.06 C 15.502,32.916 13.767,32.72 12.42,33.472 L 7.789,28.918 C 7.47,28.599 6.951,28.599 6.631,28.918 Z\" stroke=\"#000000\" fill=\"#000000\" stroke-linejoin=\"miter\" stroke-width=\"1.5\" marker-start=\"none\" marker-end=\"none\" stroke-miterlimit=\"79.8403193612775\"/></g></g></svg>" );
_LIT8( KIconPin, "<?xml version=\"1.0\" standalone=\"no\"?><!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"><svg stroke-width=\"0.501\" stroke-linejoin=\"bevel\" fill-rule=\"evenodd\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" overflow=\"visible\" width=\"22.5pt\" height=\"22.5pt\" viewBox=\"0 0 22.5 22.5\"> <g id=\"Document\" fill=\"none\" stroke=\"black\" transform=\"scale(1 -1)\"><g id=\"Spread\" transform=\"translate(0 -45)\"><ellipse rx=\"10.125\" ry=\"10.125\" transform=\"translate(11.25 33.75) rotate(90)\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#ffffff\" stroke-width=\"2.25\" fill=\"#000000\" stroke-miterlimit=\"79.8403193612775\"/><path d=\"M 6.879,27.213 L 9.597,32.775 L 6.315,34.67 C 6.887,35.662 8.207,35.974 9.271,35.385 L 12.094,40.271 C 11.587,40.596 11.411,41.249 11.7,41.752 C 11.702,41.756 11.706,41.761 11.707,41.766 L 17.554,38.39 C 17.55,38.384 17.549,38.38 17.547,38.378 C 17.252,37.867 16.586,37.698 16.048,37.987 L 13.228,33.103 C 14.287,32.476 14.685,31.168 14.109,30.17 L 10.827,32.065 L 7.37,26.931 L 6.879,27.213 Z\" fill=\"#ffffff\" stroke=\"none\" stroke-miterlimit=\"79.8403193612775\"/></g></g></svg>" );
_LIT8( KIconPinSel, "<?xml version=\"1.0\" standalone=\"no\"?><!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"><svg stroke-width=\"0.501\" stroke-linejoin=\"bevel\" fill-rule=\"evenodd\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" overflow=\"visible\" width=\"22.5pt\" height=\"22.5pt\" viewBox=\"0 0 22.5 22.5\"> <g id=\"Document\" fill=\"none\" stroke=\"black\" transform=\"scale(1 -1)\"><g id=\"Spread\" transform=\"translate(0 -45)\"><ellipse rx=\"10.125\" ry=\"10.125\" transform=\"translate(11.25 33.75) rotate(90)\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#ffffff\" stroke-width=\"2.25\" fill=\"#ffffff\" stroke-miterlimit=\"79.8403193612775\"/><path d=\"M 6.879,27.213 L 9.597,32.775 L 6.315,34.67 C 6.887,35.662 8.207,35.974 9.271,35.385 L 12.094,40.271 C 11.587,40.596 11.411,41.249 11.7,41.752 C 11.702,41.756 11.706,41.761 11.707,41.766 L 17.554,38.39 C 17.55,38.384 17.549,38.38 17.547,38.378 C 17.252,37.867 16.586,37.698 16.048,37.987 L 13.228,33.103 C 14.287,32.476 14.685,31.168 14.109,30.17 L 10.827,32.065 L 7.37,26.931 L 6.879,27.213 Z\" fill=\"#000000\" stroke=\"none\" stroke-miterlimit=\"79.8403193612775\"/></g></g></svg>" );
_LIT8( KIconUnpin, "<?xml version=\"1.0\" standalone=\"no\"?><!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"><svg stroke-width=\"0.501\" stroke-linejoin=\"bevel\" fill-rule=\"evenodd\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" overflow=\"visible\" width=\"22.5pt\" height=\"22.5pt\" viewBox=\"0 0 22.5 22.5\"> <g id=\"Document\" fill=\"none\" stroke=\"black\" transform=\"scale(1 -1)\"><g id=\"Spread\" transform=\"translate(0 -45)\"><ellipse rx=\"10.125\" ry=\"10.125\" transform=\"translate(11.25 33.75) rotate(90)\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#ffffff\" stroke-width=\"2.25\" fill=\"#000000\" stroke-miterlimit=\"79.8403193612775\"/><path d=\"M 14.508,22.954 L 14.499,22.977 C 14.981,23.122 15.449,23.299 15.902,23.505 L 15.911,23.482 L 14.508,22.954 Z M 14.168,23.856 L 6.926,43.114 C 7.377,43.323 7.846,43.5 8.33,43.643 L 15.571,24.384 C 15.12,24.175 14.652,23.998 14.168,23.856 Z M 7.37,26.931 L 6.879,27.213 L 9.597,32.775 L 6.315,34.67 C 6.835,35.573 7.977,35.912 8.981,35.521 L 10.476,31.544 L 7.37,26.931 Z M 14.161,30.266 L 11.067,38.494 L 12.094,40.271 C 11.587,40.596 11.411,41.249 11.7,41.752 C 11.702,41.756 11.706,41.761 11.707,41.766 L 17.554,38.39 C 17.55,38.384 17.549,38.38 17.547,38.378 C 17.252,37.867 16.586,37.698 16.048,37.987 L 13.228,33.103 C 14.253,32.496 14.658,31.251 14.161,30.266 Z M 6.596,43.994 L 6.588,44.016 L 7.992,44.543 L 7.999,44.522 C 7.517,44.377 7.048,44.2 6.596,43.994 Z\" fill=\"#ffffff\" stroke=\"none\" stroke-miterlimit=\"79.8403193612775\"/></g></g></svg>" );
_LIT8( KIconUnpinSel, "<?xml version=\"1.0\" standalone=\"no\"?><!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"><svg stroke-width=\"0.501\" stroke-linejoin=\"bevel\" fill-rule=\"evenodd\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" overflow=\"visible\" width=\"22.5pt\" height=\"22.5pt\" viewBox=\"0 0 22.5 22.5\"> <g id=\"Document\" fill=\"none\" stroke=\"black\" transform=\"scale(1 -1)\"><g id=\"Spread\" transform=\"translate(0 -45)\"><ellipse rx=\"10.125\" ry=\"10.125\" transform=\"translate(11.25 33.75) rotate(90)\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#ffffff\" stroke-width=\"2.25\" fill=\"#ffffff\" stroke-miterlimit=\"79.8403193612775\"/><path d=\"M 14.508,22.954 L 14.499,22.977 C 14.981,23.122 15.449,23.299 15.902,23.505 L 15.911,23.482 L 14.508,22.954 Z M 14.168,23.856 L 6.926,43.114 C 7.377,43.323 7.846,43.5 8.33,43.643 L 15.571,24.384 C 15.12,24.175 14.652,23.998 14.168,23.856 Z M 7.37,26.931 L 6.879,27.213 L 9.597,32.775 L 6.315,34.67 C 6.835,35.573 7.977,35.912 8.981,35.521 L 10.476,31.544 L 7.37,26.931 Z M 14.161,30.266 L 11.067,38.494 L 12.094,40.271 C 11.587,40.596 11.411,41.249 11.7,41.752 C 11.702,41.756 11.706,41.761 11.707,41.766 L 17.554,38.39 C 17.55,38.384 17.549,38.38 17.547,38.378 C 17.252,37.867 16.586,37.698 16.048,37.987 L 13.228,33.103 C 14.253,32.496 14.658,31.251 14.161,30.266 Z M 6.596,43.994 L 6.588,44.016 L 7.992,44.543 L 7.999,44.522 C 7.517,44.377 7.048,44.2 6.596,43.994 Z\" fill=\"#000000\" stroke=\"none\" stroke-miterlimit=\"79.8403193612775\"/></g></g></svg>" );
//_LIT8( KIconUnpin, "<?xml version=\"1.0\" standalone=\"no\"?><!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"><svg stroke-width=\"0.501\" stroke-linejoin=\"bevel\" fill-rule=\"evenodd\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" overflow=\"visible\" width=\"22.5pt\" height=\"22.5pt\" viewBox=\"0 0 22.5 22.5\"> <g id=\"Document\" fill=\"none\" stroke=\"black\" transform=\"scale(1 -1)\"><g id=\"Spread\" transform=\"translate(0 -45)\"><ellipse rx=\"10.125\" ry=\"10.125\" transform=\"translate(11.25 33.75) rotate(90)\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#ffffff\" stroke-width=\"2.25\" fill=\"#000000\" stroke-miterlimit=\"79.8403193612775\"/><path d=\"M 14.508,22.954 L 6.588,44.016 L 7.992,44.543 L 15.911,23.482 L 14.508,22.954 Z M 7.37,26.931 L 6.879,27.213 L 9.597,32.775 L 6.315,34.67 C 6.835,35.573 7.977,35.912 8.981,35.521 L 10.476,31.544 L 7.37,26.931 Z M 14.161,30.266 L 11.067,38.494 L 12.094,40.271 C 11.587,40.596 11.411,41.249 11.7,41.752 C 11.702,41.756 11.706,41.761 11.707,41.766 L 17.554,38.39 C 17.55,38.384 17.549,38.38 17.547,38.378 C 17.252,37.867 16.586,37.698 16.048,37.987 L 13.228,33.103 C 14.253,32.496 14.658,31.251 14.161,30.266 Z\" fill=\"#ffffff\" stroke=\"none\" stroke-miterlimit=\"79.8403193612775\"/></g></g></svg>" );
//_LIT8( KIconUnpinSel, "<?xml version=\"1.0\" standalone=\"no\"?><!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"><svg stroke-width=\"0.501\" stroke-linejoin=\"bevel\" fill-rule=\"evenodd\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" overflow=\"visible\" width=\"22.5pt\" height=\"22.5pt\" viewBox=\"0 0 22.5 22.5\"> <g id=\"Document\" fill=\"none\" stroke=\"black\" transform=\"scale(1 -1)\"><g id=\"Spread\" transform=\"translate(0 -45)\"><ellipse rx=\"10.125\" ry=\"10.125\" transform=\"translate(11.25 33.75) rotate(90)\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#ffffff\" stroke-width=\"2.25\" fill=\"#ffffff\" stroke-miterlimit=\"79.8403193612775\"/><path d=\"M 14.508,22.954 L 6.588,44.016 L 7.992,44.543 L 15.911,23.482 L 14.508,22.954 Z M 7.37,26.931 L 6.879,27.213 L 9.597,32.775 L 6.315,34.67 C 6.835,35.573 7.977,35.912 8.981,35.521 L 10.476,31.544 L 7.37,26.931 Z M 14.161,30.266 L 11.067,38.494 L 12.094,40.271 C 11.587,40.596 11.411,41.249 11.7,41.752 C 11.702,41.756 11.706,41.761 11.707,41.766 L 17.554,38.39 C 17.55,38.384 17.549,38.38 17.547,38.378 C 17.252,37.867 16.586,37.698 16.048,37.987 L 13.228,33.103 C 14.253,32.496 14.658,31.251 14.161,30.266 Z\" fill=\"#000000\" stroke=\"none\" stroke-miterlimit=\"79.8403193612775\"/></g></g></svg>" );

_LIT8( KIconExit, "<?xml version=\"1.0\" standalone=\"no\"?><!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"><svg stroke-width=\"0.501\" stroke-linejoin=\"bevel\" fill-rule=\"evenodd\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" overflow=\"visible\" width=\"22.5pt\" height=\"22.5pt\" viewBox=\"0 0 22.5 22.5\"><g id=\"Document\" fill=\"none\" stroke=\"black\" transform=\"scale(1 -1)\"><g id=\"Spread\" transform=\"translate(0 -45)\"><ellipse rx=\"10.125\" ry=\"10.125\" transform=\"translate(11.25 33.75) rotate(90)\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#ffffff\" stroke-width=\"1\" fill=\"#000000\" stroke-miterlimit=\"79.8403193612775\"/><path d=\"M 7.5,37.5 L 15,30\" fill=\"none\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#ffffff\" stroke-width=\"1.5\" stroke-miterlimit=\"79.8403193612775\"/><path d=\"M 15,37.5 L 7.5,30\" fill=\"none\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#ffffff\" stroke-width=\"1.5\" stroke-miterlimit=\"79.8403193612775\"/></g></g></svg>" );
_LIT8( KIconExitSel, "<?xml version=\"1.0\" standalone=\"no\"?><!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"><svg stroke-width=\"0.501\" stroke-linejoin=\"bevel\" fill-rule=\"evenodd\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" overflow=\"visible\" width=\"22.5pt\" height=\"22.5pt\" viewBox=\"0 0 22.5 22.5\"><g id=\"Document\" fill=\"none\" stroke=\"black\" transform=\"scale(1 -1)\"><g id=\"Spread\" transform=\"translate(0 -45)\"><ellipse rx=\"10.125\" ry=\"10.125\" transform=\"translate(11.25 33.75) rotate(90)\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#ffffff\" stroke-width=\"1\" fill=\"#ffffff\" stroke-miterlimit=\"79.8403193612775\"/><path d=\"M 7.5,37.5 L 15,30\" fill=\"none\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#000000\" stroke-width=\"1.5\" stroke-miterlimit=\"79.8403193612775\"/><path d=\"M 15,37.5 L 7.5,30\" fill=\"none\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#000000\" stroke-width=\"1.5\" stroke-miterlimit=\"79.8403193612775\"/></g></g></svg>" );
_LIT8( KIconArrowRight, "<?xml version=\"1.0\" standalone=\"no\"?><!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"><svg stroke-width=\"0.501\" stroke-linejoin=\"bevel\" fill-rule=\"evenodd\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" overflow=\"visible\" width=\"22.5pt\" height=\"22.5pt\" viewBox=\"0 0 22.5 22.5\"><g id=\"Document\" fill=\"none\" stroke=\"black\" transform=\"scale(1 -1)\"><g id=\"Spread\" transform=\"translate(0 -45)\"><ellipse rx=\"10.125\" ry=\"10.125\" transform=\"translate(11.25 33.75) rotate(90)\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#ffffff\" stroke-width=\"1\" fill=\"#000000\" stroke-miterlimit=\"79.8403193612775\"/><path d=\"M 11.84,28.941 L 11.769,29.012 L 16.506,33.749 L 11.769,38.487 L 11.84,38.558 L 16.613,33.785 L 16.577,33.749 L 16.613,33.714 L 11.84,28.941 Z M 5.441,33.7 L 5.441,33.8 L 16.047,33.8 L 16.047,33.7 L 5.441,33.7 Z\" fill=\"none\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#ffffff\" stroke-width=\"1.5\" stroke-miterlimit=\"79.8403193612775\"/></g></g></svg>" );
_LIT8( KIconArrowRightSel, "<?xml version=\"1.0\" standalone=\"no\"?><!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"><svg stroke-width=\"0.501\" stroke-linejoin=\"bevel\" fill-rule=\"evenodd\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" overflow=\"visible\" width=\"22.5pt\" height=\"22.5pt\" viewBox=\"0 0 22.5 22.5\"><g id=\"Document\" fill=\"none\" stroke=\"black\" transform=\"scale(1 -1)\"><g id=\"Spread\" transform=\"translate(0 -45)\"><ellipse rx=\"10.125\" ry=\"10.125\" transform=\"translate(11.25 33.75) rotate(90)\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#ffffff\" stroke-width=\"1\" fill=\"#ffffff\" stroke-miterlimit=\"79.8403193612775\"/><path d=\"M 11.84,28.941 L 11.769,29.012 L 16.506,33.749 L 11.769,38.487 L 11.84,38.558 L 16.613,33.785 L 16.577,33.749 L 16.613,33.714 L 11.84,28.941 Z M 5.441,33.7 L 5.441,33.8 L 16.047,33.8 L 16.047,33.7 L 5.441,33.7 Z\" fill=\"none\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#000000\" stroke-width=\"1.5\" stroke-miterlimit=\"79.8403193612775\"/></g></g></svg>" );
_LIT8( KIconArrowLeft, "<?xml version=\"1.0\" standalone=\"no\"?><!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"><svg stroke-width=\"0.501\" stroke-linejoin=\"bevel\" fill-rule=\"evenodd\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" overflow=\"visible\" width=\"22.5pt\" height=\"22.5pt\" viewBox=\"0 0 22.5 22.5\"><g id=\"Document\" fill=\"none\" stroke=\"black\" transform=\"scale(1 -1)\"><g id=\"Spread\" transform=\"translate(0 -45)\"><ellipse rx=\"10.125\" ry=\"10.125\" transform=\"translate(11.25 33.75) rotate(90)\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#ffffff\" stroke-width=\"1\" fill=\"#000000\" stroke-miterlimit=\"79.8403193612775\"/><path d=\"M 10.214,28.941 L 10.285,29.012 L 5.548,33.749 L 10.285,38.487 L 10.214,38.558 L 5.441,33.785 L 5.477,33.749 L 5.441,33.714 L 10.214,28.941 Z M 16.613,33.7 L 16.613,33.8 L 6.007,33.8 L 6.007,33.7 L 16.613,33.7 Z\" fill=\"none\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#ffffff\" stroke-width=\"1.5\" stroke-miterlimit=\"79.8403193612775\"/></g></g></svg>" );
_LIT8( KIconArrowLeftSel, "<?xml version=\"1.0\" standalone=\"no\"?><!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"><svg stroke-width=\"0.501\" stroke-linejoin=\"bevel\" fill-rule=\"evenodd\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" overflow=\"visible\" width=\"22.5pt\" height=\"22.5pt\" viewBox=\"0 0 22.5 22.5\"><g id=\"Document\" fill=\"none\" stroke=\"black\" transform=\"scale(1 -1)\"><g id=\"Spread\" transform=\"translate(0 -45)\"><ellipse rx=\"10.125\" ry=\"10.125\" transform=\"translate(11.25 33.75) rotate(90)\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#ffffff\" stroke-width=\"1\" fill=\"#ffffff\" stroke-miterlimit=\"79.8403193612775\"/><path d=\"M 10.214,28.941 L 10.285,29.012 L 5.548,33.749 L 10.285,38.487 L 10.214,38.558 L 5.441,33.785 L 5.477,33.749 L 5.441,33.714 L 10.214,28.941 Z M 16.613,33.7 L 16.613,33.8 L 6.007,33.8 L 6.007,33.7 L 16.613,33.7 Z\" fill=\"none\" stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke=\"#000000\" stroke-width=\"1.5\" stroke-miterlimit=\"79.8403193612775\"/></g></g></svg>" );


CEightTileManager::CEightTileManager( MEightTileManagerObserver& aObserver, TInt aMargin, TInt aTileSize, TSize aSize ) : iObserver( aObserver ), iTilesWidth( 4 ), iMargin( aMargin ), 
        iTileSize( aTileSize ), iScreenSize( aSize ), iSelectedTile( -1, -1 )
    {
    }

CEightTileManager::~CEightTileManager()
    {
    iTiles.ResetAndDestroy();
    iBoard.Close();
    
    delete iButtonArrow;
    delete iButtonRemove;
    delete iButtonSettings;
    delete iButtonPin;
    delete iButtonUnpin;
    
    for ( TInt i = 0; i < iAppsImgs.Count(); i++ )
        {
        iAppsImgs[ i ]->DeleteImage();
        }
    iAppsImgs.ResetAndDestroy();
    }

CEightTileManager* CEightTileManager::NewL( MEightTileManagerObserver& aObserver, TInt aMargin, TInt aTileSize, TSize aSize )
    {
    CEightTileManager* self = new ( ELeave ) CEightTileManager( aObserver, aMargin, aTileSize, aSize );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self;
    return self;
    }

MVgImage* CEightTileManager::ConstructImageL( CVgImageManager& aImageManager, TUint aImageId, const TDesC8& aImageData )
	{
	MVgImage* img = aImageManager.Get( aImageId );
    if ( !img )
    	{
    	CVgSvgImage* simg = CVgSvgImage::NewL( aImageData );
    	aImageManager.AddImageL( aImageId, simg, ETrue );
    	return simg;
    	}
    return img;
	}

void CEightTileManager::ConstructL()
    {
    User::LeaveIfError( HAL::Get( HAL::ENanoTickPeriod, iNanokernelTickPeriod ) );
    
    CVgImageManager& iman = CEightPhotosProvider::GetInstanceL()->ImageManager();
    iIconArrow = ConstructImageL( iman, KTileManagerImageArrow, KIconArrow() );
    iIconArrowSel = ConstructImageL( iman, KTileManagerImageArrowSel, KIconArrowSel() );
    iIconRemove = ConstructImageL( iman, KTileManagerImageRemove, KIconRemove() );
    iIconRemoveSel = ConstructImageL( iman, KTileManagerImageRemoveSel, KIconRemoveSel() );
    iIconSettings = ConstructImageL( iman, KTileManagerImageSettings, KIconSettings() );
    iIconSettingsSel = ConstructImageL( iman, KTileManagerImageSettingsSel, KIconSettingsSel() );
    iIconPin = ConstructImageL( iman, KTileManagerImagePin, KIconPin() );
    iIconPinSel = ConstructImageL( iman, KTileManagerImagePinSel, KIconPinSel() );
    iIconUnpin = ConstructImageL( iman, KTileManagerImageUnpin, KIconUnpin() );
    iIconUnpinSel = ConstructImageL( iman, KTileManagerImageUnpinSel, KIconUnpinSel() );
    
    iButtonArrow = CTouchButtonPath::NewL( this, KButtonArrow, TVgPoint(), TVgRect(), (CVgSvgImage*)iIconArrow, (CVgSvgImage*)iIconArrowSel );
    iButtonRemove = CTouchButtonPath::NewL( this, KButtonRemove, TVgPoint(), TVgRect(), (CVgSvgImage*)iIconRemove, (CVgSvgImage*)iIconRemoveSel );
    iButtonSettings = CTouchButtonPath::NewL( this, KButtonSettings, TVgPoint(), TVgRect(), (CVgSvgImage*)iIconSettings, (CVgSvgImage*)iIconSettingsSel );
    iButtonPin = CTouchButtonPath::NewL( this, KButtonPin, TVgPoint(), TVgRect(), (CVgSvgImage*)iIconPin, (CVgSvgImage*)iIconPinSel );
    iButtonUnpin = CTouchButtonPath::NewL( this, KButtonUnpin, TVgPoint(), TVgRect(), (CVgSvgImage*)iIconUnpin, (CVgSvgImage*)iIconUnpinSel );

#ifndef __WINS__  
    iTouchFeedback = MTouchFeedback::Instance();
#endif
    }

void CEightTileManager::SetState( TState aNewState )
    {
    iOldState = iState;
    iState = aNewState;
    if ( iDrawingState == ENormalDrawing )
        {
        CEightPhotosProvider::GetInstanceL()->SetHighFpsIdle( iState == ENormal );
        }
    //iStateChangeTimestamp.HomeTime();
    }

void CEightTileManager::SetMsgBox( CEightMsgBox* aMsgBox )
    {
    iMsgBox = aMsgBox;
    for ( TInt i = 0; i < iTiles.Count(); i++ )
        {
        iTiles[ i ]->SetMsgBox( aMsgBox );
        }
    }

void CEightTileManager::SetFormWindowManager( CEightFormWindowManager* aWndMan )
    {
    iWndMan = aWndMan;
    }

TVgImage* CEightTileManager::GetAppIconL( TApaAppInfo appInfo )
    {
    CFbsBitmap* AppIcon(NULL);
    CFbsBitmap* AppIconMsk(NULL);
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    AknsUtils::CreateAppIconLC( skin, appInfo.iUid, EAknsAppIconTypeList, AppIcon, AppIconMsk );
    AknIconUtils::DisableCompression( AppIcon );
    AknIconUtils::DisableCompression( AppIconMsk );
    CleanupStack::Pop(2);
    if ( AknIconUtils::SetSize( AppIcon, TSize( 60, 60 ), EAspectRatioNotPreserved ) != KErrNone ) 
        {
        delete AppIcon;
        delete AppIconMsk;
        return NULL;
        }
    if ( AknIconUtils::SetSize( AppIconMsk, TSize( 60, 60 ), EAspectRatioNotPreserved ) != KErrNone )
        {
        delete AppIcon;
        delete AppIconMsk;
        return NULL;
        }
    

    TUid extendedBitmapTypem = AppIconMsk->ExtendedBitmapType();
    if (extendedBitmapTypem != KNullUid)
        {
        CFbsBitmap *rasterBitmap =  new (ELeave) CFbsBitmap();
        rasterBitmap->Create(AppIconMsk->SizeInPixels(), EGray256);

        CFbsBitmapDevice *rasterBitmapDev = 0;
        TRAP_IGNORE(rasterBitmapDev = CFbsBitmapDevice::NewL(rasterBitmap));
            CFbsBitGc *rasterBitmapGc = 0;
        TInt err = rasterBitmapDev->CreateContext(rasterBitmapGc);
        if (err != KErrNone) {
            delete rasterBitmap;
            delete rasterBitmapDev;
            rasterBitmapDev = 0;
            delete AppIcon;
            delete AppIconMsk;
            return NULL;
        }

        rasterBitmapGc->BitBlt(TPoint( 0, 0), AppIconMsk);
        //rasterBitmapGc->BitBltMasked(TPoint( 0, 0), AppIcon, AppIcon->SizeInPixels(), AppIconMsk, ETrue );

        delete AppIconMsk;
        AppIconMsk = rasterBitmap;

        delete rasterBitmapDev;
        delete rasterBitmapGc;

        rasterBitmapDev = 0;
        rasterBitmapGc = 0;

        //deleteSourceBitmap = true;
    }
        
    TUid extendedBitmapType = AppIcon->ExtendedBitmapType();
    if (extendedBitmapType != KNullUid)
        {
        CFbsBitmap *rasterBitmap =  new (ELeave) CFbsBitmap();
        rasterBitmap->Create(AppIcon->SizeInPixels(), EColor16M);

        CFbsBitmapDevice *rasterBitmapDev = 0;
        TRAP_IGNORE(rasterBitmapDev = CFbsBitmapDevice::NewL(rasterBitmap));
            CFbsBitGc *rasterBitmapGc = 0;
        TInt err = rasterBitmapDev->CreateContext(rasterBitmapGc);
        if (err != KErrNone) {
            delete rasterBitmap;
            delete rasterBitmapDev;
            rasterBitmapDev = 0;
            delete AppIcon;
            delete AppIconMsk;
            return NULL;
        }

        rasterBitmapGc->BitBlt(TPoint( 0, 0), AppIcon);
        //rasterBitmapGc->BitBltMasked(TPoint( 0, 0), AppIcon, AppIcon->SizeInPixels(), AppIconMsk, ETrue );

        delete AppIcon;
        AppIcon = rasterBitmap;

        delete rasterBitmapDev;
        delete rasterBitmapGc;

        rasterBitmapDev = 0;
        rasterBitmapGc = 0;

        //deleteSourceBitmap = true;
    }
    //CGulIcon* icon = CGulIcon::NewL( AppIcon,AppIconMsk );
    
    TDisplayMode m = AppIcon->DisplayMode();
    TDisplayMode mm = AppIconMsk->DisplayMode();
    TInt ds = AppIcon->DataSize();
    TInt dss = AppIcon->DataStride();
    
    CFbsBitmap* bmp = NULL;
    TRAPD( err, bmp = SGEBitmapUtils::CombineBitmapsL( *AppIcon, *AppIconMsk, EFalse, ETrue ) );
    if ( err == KErrNone && bmp )
        {
        TVgImage *img = new TVgImage();
        if ( img->CreateImage( bmp, VG_IMAGE_QUALITY_BETTER ) == KErrNone )
            {
            delete bmp;
            delete AppIcon;
            delete AppIconMsk;
            return img;
            }
        delete img;
        }
    delete bmp;
    
    delete AppIcon;
    delete AppIconMsk;
    return NULL;
    }

TBool CEightTileManager::IsHeightSmallerThanScreen()
    { // todo: cache this value
    //TVgRect r = GetTileRect( 0, iBoard.Count() - 1, CEightTile::ETileSize_1x1 );
    //return r.iBr.iY <= iScreenSize.iHeight;
    return ( iBoard.Count() * ( iTileSize + iMargin ) ) <= iScreenSize.iHeight;
    }

void CEightTileManager::AddSpecialTiles( TInt aSpecialTilesType )
    {
    CVgImageManager& iman = CEightPhotosProvider::GetInstanceL()->ImageManager();
    iSpecialTilesType = aSpecialTilesType;
    if ( aSpecialTilesType == 1 )
        {
        //vgSeti( VG_STROKE_CAP_STYLE, VG_CAP_ROUND );
        
        // add tile
    //    CEightTile* t1 = CEightTileSpecial::NewL( CEightTile::ETileSize_1x1, *iIconRemove );
    //    AddTileL( t1 );
    //    iBoard[ iBoard.Count() - 1 ][ 0 ] = iTiles.Count() - 1;
    
        // switch to apps list
    //    CEightTile* t2 = CEightTileSpecial::NewL( CEightTile::ETileSize_1x1, *iIconArrow, *iIconArrowSel );
    //    AddTileL( t2 );
    //    iBoard[ iBoard.Count() - 1 ][ 3 ] = iTiles.Count() - 1;
    
    	iIconExit = ConstructImageL( iman, KTileManagerImageExit, KIconExit() );
    	iIconExitSel = ConstructImageL( iman, KTileManagerImageExitSel, KIconExitSel() );
    	iIconArrowRight = ConstructImageL( iman, KTileManagerImageArrowRight, KIconArrowRight() );
    	iIconArrowRightSel = ConstructImageL( iman, KTileManagerImageArrowRightSel, KIconArrowRightSel() );

        CEightTile* t2 = CEightTileSpecial::NewL( ETileSize_1x1, MEightTileManagerObserver::ESwapRight, *(CVgSvgImage*)iIconArrowRight, *(CVgSvgImage*)iIconArrowRightSel );
        TInt idx2 = FindTile( t2->Id() );
        if ( idx2 == KErrNotFound )
            {
            AddTileL( t2 );
            }
        else
            {
            delete iTiles[ idx2 ];
            iTiles[ idx2 ] = t2;
            }

        // exit
        CEightTile* t3 = CEightTileSpecial::NewL( ETileSize_1x1, MEightTileManagerObserver::EExit, *(CVgSvgImage*)iIconExit, *(CVgSvgImage*)iIconExitSel );
        TInt idx3 = FindTile( t3->Id() );
        if ( idx3 == KErrNotFound )
            {
            AddTileL( t3 );
            }
        else
            {
            delete iTiles[ idx3 ];
            iTiles[ idx3 ] = t3;
            }

        if ( idx2 == KErrNotFound && idx3 == KErrNotFound )
            {
            iBoard[ iBoard.Count() - 1 ][ 0 ] = iTiles.Count() - 1; // exit
            iBoard[ iBoard.Count() - 1 ][ 1 ] = KTileSpecialTile;
            iBoard[ iBoard.Count() - 1 ][ 2 ] = KTileSpecialTile;
            iBoard[ iBoard.Count() - 1 ][ 3 ] = iTiles.Count() - 2; // arrow
            }
        
        iSpecialRow = ETrue;
        }
    else if ( aSpecialTilesType == 2 )
        {
    	iIconArrowLeft = ConstructImageL( iman, KTileManagerImageArrowLeft, KIconArrowLeft() );
    	iIconArrowLeftSel = ConstructImageL( iman, KTileManagerImageArrowLeftSel, KIconArrowLeftSel() );

        CEightTile* t2 = CEightTileSpecial::NewL( ETileSize_1x1, MEightTileManagerObserver::ESwapLeft, *(CVgSvgImage*)iIconArrowLeft, *(CVgSvgImage*)iIconArrowLeftSel );
        TInt idx2 = FindTile( t2->Id() );
        if ( idx2 == KErrNotFound )
            {
            AddTileL( t2 );
            iBoard[ 0 ][ 0 ] = iTiles.Count() - 1;
            }
        else
            {
            delete iTiles[ idx2 ];
            iTiles[ idx2 ] = t2;
            }
        }
    }

void CEightTileManager::CompressBoard()
    {
    // discard empty tiles
    for ( TInt i = iBoard.Count() - 1; i >= 0; i-- )
        {
        for ( TInt j = 0; j < iBoard[ i ].Count(); j++ )
            {
            TFixedArray< TInt, 4 >& row = iBoard[ i ];
            if ( iBoard[ i ][ j ] != KErrNotFound && iBoard[ i ][ j ] != KTileSpecialTile )
                {
                TInt idx = iBoard[ i ][ j ] & KTilePlaceholderMask;
                if ( idx >= iTiles.Count() || iTiles[ idx ]->IsEmpty() )
                    {
                    iBoard[ i ][ j ] = KErrNotFound;
                    }
                }
            }
        }

    // remove empty rows
    for ( TInt i = iBoard.Count() - 1; i >= 0; i-- )
        {
        TBool remove = ETrue;
        for ( TInt j = 0; j < iBoard[ i ].Count(); j++ )
            {
            if ( iBoard[ i ][ j ] == KTileSpecialTile )
                {
                remove = EFalse;
                break;
                }
            if ( iBoard[ i ][ j ] != KErrNotFound )
                {
                TInt idx = iBoard[ i ][ j ] & KTilePlaceholderMask;
                if ( idx < iTiles.Count() && !iTiles[ idx ]->IsEmpty() )
                    {
                    remove = EFalse;
                    break;
                    }
                }
            }
        if ( remove )
            {
            iBoard.Remove( i );
            if ( iSelectedTile.iY != -1 && iSelectedTile.iY > i )
                {
                iSelectedTile.iY--;
                }
            }
        }
    }

void CEightTileManager::UpdateTilesRects()
    {
    for ( TInt i = 0; i < iBoard.Count(); i++ )
        {
        for ( TInt j = 0; j < iBoard[ i ].Count(); j++ )
            {
            TInt idx = iBoard[ i ][ j ];
            if ( idx >= 0 && idx < iTiles.Count() )
                {
                iTiles[ idx ]->SetRect( GetTileRect( j, i, iTiles[ idx ]->TileSize() ) );
                }
            }
        }
    }

TInt CEightTileManager::FindTile( TPoint aScreenCoords )
    {
    for ( TInt i = 0; i < iBoard.Count(); i++ )
        {
        for ( TInt j = 0; j < iBoard[ i ].Count(); j++ )
            {
            if ( iBoard[ i ][ j ] >= 0 && iBoard[ i ][ j ] < iTiles.Count() && iTiles[ iBoard[ i ][ j ] ]->Rect().Contains( aScreenCoords ) )
                {
                return iBoard[ i ][ j ];
                }
            }
        }
    return KErrNotFound;
    }

TInt CEightTileManager::FindTile( TUint aId )
    {
    for ( TInt i = 0; i < iTiles.Count(); i++ )
        {
        if ( iTiles[ i ]->Id() == aId )
            {
            return i;
            }
        }
    return KErrNotFound;
    }

TPoint CEightTileManager::FindTileInBoard( CEightTile* aPtr )
    {
    for ( TInt i = 0; i < iBoard.Count(); i++ )
        {
        for ( TInt j = 0; j < iBoard[ i ].Count(); j++ )
            {
            if ( iBoard[ i ][ j ] >= 0 && iBoard[ i ][ j ] < iTiles.Count() && iTiles[ iBoard[ i ][ j ] ] == aPtr )
                {
                return TPoint( j, i );
                }
            }
        }
    return TPoint( -1, -1 );
    }

void CEightTileManager::InsertNewTileL( CEightTile* aTile )
    {
    TInt bidx = KErrNotFound;
    if ( iSpecialRow && iBoard.Count() > 1 && aTile->Size().iHeight == 1 )
        {
        if ( iBoard[ iBoard.Count() - 2 ][ 0 ] == KErrNotFound ) bidx = 0;
        else if ( iBoard[ iBoard.Count() - 2 ][ 1 ] == KErrNotFound ) bidx = 1;
        else if ( iBoard[ iBoard.Count() - 2 ][ 2 ] == KErrNotFound ) bidx = 2;
        else if ( iBoard[ iBoard.Count() - 2 ][ 3 ] == KErrNotFound ) bidx = 3;
        }

    aTile->SetMsgBox( iMsgBox );
    
    if ( bidx == KErrNotFound )
        {    
        AddTileL( aTile );
        bidx = 0;
        }
    else
        {
        iTiles.AppendL( aTile );
        }
    
    TInt idx = iTiles.Count() - 1;
    if ( iSpecialRow )
        {
        if ( aTile->Size().iHeight > 1 )
            {
            SetupTile( TPoint( bidx, iBoard.Count() - 3 ), idx );
            }
        else
            {
            SetupTile( TPoint( bidx, iBoard.Count() - 2 ), idx );
            }
        }
    else
        {
        if ( aTile->Size().iHeight > 1 )
            {
            SetupTile( TPoint( bidx, iBoard.Count() - 2 ), idx );
            }
        else
            {
            SetupTile( TPoint( bidx, iBoard.Count() - 1 ), idx );
            }
        }
    
    CompressBoard();
    UpdateTilesRects();
    
    SelectTile( aTile->Rect().Center() );
    
    iTilesOffset.iY = BottomBorder();
    }

void CEightTileManager::AddTileL( CEightTile* aTile )
    {
    iTiles.AppendL( aTile );
    
    TFixedArray< TInt, 4 > tab( KRow, 4 );
    if ( iSpecialRow && iBoard.Count() > 1 )
        {
        if ( aTile->Size().iHeight == 1 )
            {
            iBoard.Insert( tab, iBoard.Count() - 1 );
            }
        else
            {
            iBoard.Insert( tab, iBoard.Count() - 1 );
            iBoard.Insert( tab, iBoard.Count() - 1 );
            }
        }
    else
        {
        if ( aTile->Size().iHeight == 1 )
            {
            iBoard.AppendL( tab );
            }
        else
            {
            iBoard.AppendL( tab );
            iBoard.AppendL( tab );
            }
        }
    }

void CEightTileManager::AddIconTileL( TUint aId, CVgSvgImage* aImage, const TDesC& aTitle, TBool aPinned )
    {
    CEightTileUiStatic* uiStatI = new (ELeave) CEightTileUiStatic();
    uiStatI->AddSvgImageL( TVgRect( TVgPoint( 0, iTileSize ), TVgSize( iTileSize, iTileSize ) ), aImage, EFalse );
    uiStatI->AddTextL( TVgRect( TVgPoint( iTileSize + iMargin, iTileSize ), TVgSize( 2 * iTileSize + iMargin, iTileSize - 8 ) ), KFontId_SegoeWP_medium, aTitle, EFalse, ESGEFontAlignHorizontalLeft );
    CEightTile* tile = CEightTile::NewL( aId, ETileSize_3x1, uiStatI );
    tile->SetPinned( aPinned );
    AddTileL( tile );
    }

void CEightTileManager::AddIconTileL( CEightTile* aTile, CVgSvgImage* aImage, const TDesC& aTitle, TBool aPinned )
    {
    CEightTileUiStatic* uiStatI = new (ELeave) CEightTileUiStatic();
    uiStatI->AddSvgImageL( TVgRect( TVgPoint( 0, iTileSize ), TVgSize( iTileSize, iTileSize ) ), aImage, EFalse );
    uiStatI->AddTextL( TVgRect( TVgPoint( iTileSize + iMargin, iTileSize ), TVgSize( 2 * iTileSize + iMargin, iTileSize - 8 ) ), KFontId_SegoeWP_medium, aTitle, EFalse, ESGEFontAlignHorizontalLeft );
    aTile->SetUi( uiStatI );
    aTile->SetTileSize( ETileSize_3x1 );
    aTile->SetPinned( aPinned );
    AddTileL( aTile );
    }

void CEightTileManager::AddAppTileL( TApaAppInfo appInfo )
    {
    TVgImage* img = GetAppIconL( appInfo );
    if ( img )
        {
        iAppsImgs.Append( img );
        CEightTileUiStatic* uiStatI = new (ELeave) CEightTileUiStatic();
        uiStatI->AddImageL( TVgRect( TVgPoint( 0, iTileSize ), TVgSize( iTileSize, iTileSize ) ), img, EFalse );
        //uiStatI->AddImageL( TVgRect( TVgPoint( 0, iTileSize ), TVgSize( iTileSize, iTileSize ) ), img );
        uiStatI->AddTextL( TVgRect( TVgPoint( iTileSize + iMargin, iTileSize ), TVgSize( 2 * iTileSize + iMargin, iTileSize - 8 ) ), KFontId_SegoeWP_medium, appInfo.iCaption, EFalse, ESGEFontAlignHorizontalLeft );
        AddTileL( CEightTile::NewL( appInfo.iUid.iUid, ETileSize_3x1, uiStatI ) );
        //iBoard[ iBoard.Count() - 1 ][ 0 ] = iTiles.Count() - 1;
        
        TInt idx = iTiles.Count() - 1;
        TPoint place = FindPlace3x1();
        iBoard[ place.iY ][ place.iX ] = idx;
        iBoard[ place.iY ][ place.iX + 1 ] = KTilePlaceholder | idx;
        iBoard[ place.iY ][ place.iX + 2 ] = KTilePlaceholder | idx;
        
        CompressBoard();
        UpdateTilesRects();
        }
    }

CEightTile* CEightTileManager::GetAppSetupTile()
	{
    for ( TInt i = 0; i < iTiles.Count(); i++ )
        {
        if ( iTiles[ i ]->Id() == KIdxTileSetup )
            {
        	CEightTile* tile = iTiles[ i ];
        	CEightTile* tileE = CEightTile::NewL( 0x113a, tile->TileSize(), NULL, ETrue );
        	iTiles[ i ] = tileE;
            return tile;
            }
        }
    return NULL;
	}

TBool CEightTileManager::SetAppSetupTile( CEightTile* aTile )
	{
    for ( TInt i = 0; i < iTiles.Count(); i++ )
        {
        if ( iTiles[ i ]->Id() == KIdxTileSetup )
            {
        	delete iTiles[ i ];
        	iTiles[ i ] = aTile;
        	UpdateTilesRects();
            return ETrue;
            }
        }
    return EFalse;
	}

void CEightTileManager::SetupTile( TPoint place, TInt aIdx )
    {
    if ( iTiles[ aIdx ]->TileSize() == ETileSize_1x1 )
        {
        iBoard[ place.iY ][ place.iX ] = aIdx;
        }
    else if ( iTiles[ aIdx ]->TileSize() == ETileSize_2x2 )
        {
        iBoard[ place.iY ][ place.iX ] = aIdx;
        iBoard[ place.iY + 1 ][ place.iX ] = KTilePlaceholder | aIdx;
        iBoard[ place.iY ][ place.iX + 1 ] = KTilePlaceholder | aIdx;
        iBoard[ place.iY + 1 ][ place.iX + 1 ] = KTilePlaceholder | aIdx;
        }
    else if ( iTiles[ aIdx ]->TileSize() == ETileSize_4x2 )
        {
        iBoard[ place.iY ][ place.iX ] = aIdx;
        iBoard[ place.iY + 1 ][ place.iX ] = KTilePlaceholder | aIdx;
        iBoard[ place.iY ][ place.iX + 1 ] = KTilePlaceholder | aIdx;
        iBoard[ place.iY + 1 ][ place.iX + 1 ] = KTilePlaceholder | aIdx;
        iBoard[ place.iY ][ place.iX + 2 ] = KTilePlaceholder | aIdx;
        iBoard[ place.iY + 1 ][ place.iX + 2 ] = KTilePlaceholder | aIdx;
        iBoard[ place.iY ][ place.iX + 3 ] = KTilePlaceholder | aIdx;
        iBoard[ place.iY + 1 ][ place.iX + 3 ] = KTilePlaceholder | aIdx;
        }        
    else if ( iTiles[ aIdx ]->TileSize() == ETileSize_3x1 )
        {
        iBoard[ place.iY ][ place.iX ] = aIdx;
        iBoard[ place.iY ][ place.iX + 1 ] = KTilePlaceholder | aIdx;
        iBoard[ place.iY ][ place.iX + 2 ] = KTilePlaceholder | aIdx;
        }        
    }

void CEightTileManager::SetupBoard( TInt aSpecialTilesType )
    {
    for ( TInt i = 0; i < iTiles.Count(); i++ )
        {
        TPoint place;
        if ( iTiles[ i ]->TileSize() == ETileSize_1x1 )
            {
            place = FindPlace1x1();
            }
        else if ( iTiles[ i ]->TileSize() == ETileSize_2x2 )
            {
            place = FindPlace2x2();
            }
        else if ( iTiles[ i ]->TileSize() == ETileSize_4x2 )
            {
            place = FindPlace4x2();
            }        
        else if ( iTiles[ i ]->TileSize() == ETileSize_3x1 )
            {
            place = FindPlace3x1();
            }        
        SetupTile( place, i );
        }
    
    if ( aSpecialTilesType > KErrNotFound )
        {
        AddSpecialTiles( aSpecialTilesType );
        }
    CompressBoard();
    UpdateTilesRects();
    
    //TouchGestureDrag( TSize( 0, 200 ), ETrue );
    }

void CEightTileManager::SetupBoardAfterInternalize( TInt aSpecialTilesType )
    {
	iSpecialTilesType = aSpecialTilesType;
    if ( aSpecialTilesType > KErrNotFound && CEightSettings::GetInstance()->iShowBottomButtons )
        {
        AddSpecialTiles( aSpecialTilesType );
        }
    
    CompressBoard();
    UpdateTilesRects();
    }

TVgRect CEightTileManager::GetTileRect( TInt aX, TInt aY, TEightTileSize aSize )
    {
    if (aSize == ETileSize_1x1 )
        {
        TInt s = iTileSize;
        return TVgRect( TVgPoint( iMargin + aX * ( iMargin + iTileSize ), 
                                  iScreenSize.iHeight - ( iMargin + aY * ( iMargin + iTileSize ) ) ), 
                                  TVgSize( s, s ) );
        }
    else if (aSize == ETileSize_2x2 )
        {
        TInt s = 2 * iTileSize + iMargin;
        return TVgRect( TVgPoint( iMargin + aX * ( iMargin + iTileSize ), 
                                   iScreenSize.iHeight - ( iMargin + aY * ( iMargin + iTileSize ) ) ), 
                                       TVgSize( s, s ) );
        }
    else if (aSize == ETileSize_4x2 )
        {
        TInt s = 2 * iTileSize + iMargin;
        return TVgRect( TVgPoint( iMargin + aX * ( iMargin + iTileSize ), 
                                  iScreenSize.iHeight - ( iMargin + aY * ( iMargin + iTileSize ) ) ), 
                                  TVgSize( 4 * iTileSize + 3 * iMargin, s ) );
        }
    else if (aSize == ETileSize_3x1 )
        {
        TInt s = iTileSize;
        return TVgRect( TVgPoint( iMargin + aX * ( iMargin + iTileSize ), 
                                  iScreenSize.iHeight - ( iMargin + aY * ( iMargin + iTileSize ) ) ), 
                                  TVgSize( 3 * iTileSize + 2 * iMargin, s ) );
        }
    return TVgRect();
    }

TPoint CEightTileManager::FindPlace1x1()
    {
    for ( TInt j = 0; j < iBoard.Count(); j++ )
        {
        for ( TInt k = 0; k < iBoard[ j ].Count(); k++ )
            {
            if ( iBoard[ j ][ k ] == KErrNotFound )
                {
                return TPoint( k, j );
                }
            }
        }

    TFixedArray< TInt, 4 > tab( KRow, 4 );
    iBoard.AppendL( tab );
 
    return TPoint( 0, iBoard.Count() - 1 );
    }

TPoint CEightTileManager::FindPlace2x2()
    {
    for ( TInt j = 0; j < iBoard.Count() - 1; j++ )
        {
        for ( TInt k = 0; k < iBoard[ j ].Count() - 1; k++ )
            {
            if ( iBoard[ j ][ k ] == KErrNotFound && iBoard[ j + 1 ][ k ] == KErrNotFound && 
                 iBoard[ j ][ k + 1 ] == KErrNotFound && iBoard[ j + 1 ][ k + 1 ] == KErrNotFound )
                {
                return TPoint( k, j );
                }
            }
        }

    TFixedArray< TInt, 4 > tab( KRow, 4 );
    iBoard.AppendL( tab );
    iBoard.AppendL( tab );
 
    return TPoint( 0, iBoard.Count() - 2 );
    }

TPoint CEightTileManager::FindPlace4x2()
    {
    for ( TInt j = 0; j < iBoard.Count() - 1; j++ )
        {
        if ( iBoard[ j ][ 0 ] == KErrNotFound && iBoard[ j + 1 ][ 0 ] == KErrNotFound && 
             iBoard[ j ][ 1 ] == KErrNotFound && iBoard[ j + 1 ][ 1 ] == KErrNotFound && 
             iBoard[ j ][ 2 ] == KErrNotFound && iBoard[ j + 1 ][ 2 ] == KErrNotFound && 
             iBoard[ j ][ 3 ] == KErrNotFound && iBoard[ j + 1 ][ 3 ] == KErrNotFound ) 
            {
            return TPoint( 0, j );
            }
        }

    TFixedArray< TInt, 4 > tab( KRow, 4 );
    iBoard.AppendL( tab );
    iBoard.AppendL( tab );
 
    return TPoint( 0, iBoard.Count() - 2 );
    }

TPoint CEightTileManager::FindPlace3x1()
    {
    for ( TInt j = 0; j < iBoard.Count() - 1; j++ )
        {
        if ( iBoard[ j ][ 1 ] == KErrNotFound && 
             iBoard[ j ][ 2 ] == KErrNotFound && 
             iBoard[ j ][ 3 ] == KErrNotFound ) 
            {
            return TPoint( 1, j );
            }
        }

    TFixedArray< TInt, 4 > tab( KRow, 4 );
    iBoard.AppendL( tab );
 
    return TPoint( 1, iBoard.Count() - 1 );
    }

TBool CEightTileManager::SelectTile( TPoint aPosition )
    {
    for ( TInt y = 0; y < iBoard.Count(); y++ )
        {
        for ( TInt x = 0; x < iBoard[ y ].Count(); x++ )
            {
            TInt idx = iBoard[ y ][ x ];
            if ( idx != KErrNotFound && idx != KTileSpecialTile && ( idx & KTilePlaceholder ) == 0 && !iTiles[ idx ]->IsEmpty() &&
                 iTiles[ idx ]->Rect().Contains( aPosition ) /*&& iTiles[ idx ]->CanBeMoved()*/ && iTiles[ idx ]->Type() != ETileTypeSpecial )
                {
                SelectionModeStop();
                iTiles[ idx ]->SetSelected( ETrue );
                iSelectedTile.SetXY( x, y );
                SetButtonsCoords();
                FadeAllTiles( ETrue );
                //iBoard.Remove( iBoard.Count() - 1 );
                iSelectionClosestTileAnim = iTiles[ idx ]->Rect().iTl;
                iSelectionClosestTile = iTiles[ idx ]->Rect();
                return ETrue;
                }
            }
        }
    return EFalse;
    }

void CEightTileManager::SelectionModeStop()
    {
    if ( iSelectedTile.iX > -1 && iSelectedTile.iY > -1 )
        {
        TInt idx = iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ];
        if ( idx >= 0 && idx < iTiles.Count() )
            {
            iTiles[ idx ]->SetSelected( EFalse );
            }
        FadeAllTiles( EFalse );
        }
    iSelectedTile.SetXY( -1, -1 );
    }

TBool CEightTileManager::IsSelectionMode() const
	{
	return !( iSelectedTile.iX == -1 && iSelectedTile.iY == -1 );
	}

void CEightTileManager::HandleForegroundEventL( TBool aForeground )
    {
    for ( TInt i = 0; i < iTiles.Count(); i++ )
        {
        if ( iTiles[ i ]->Id() == KIdxTileAds )
            {
            iTiles[ i ]->HandleForegroundEventL( aForeground );
            return;
            }
        }
    }

void CEightTileManager::ResetPointerEvents()
    {
    if ( iPressedDownTileIdx > KErrNotFound && iPressedDownTileIdx < iTiles.Count() && iTiles[ iPressedDownTileIdx ]->TileState() == EPressed )
        {
        iTiles[ iPressedDownTileIdx ]->SetPressed( EFalse );
        iPressedDownTileIdx = KErrNotFound;
        }
    if ( iState == ETileMoving )
        {
        TouchGestureDragStop();
        }
    }

TBool CEightTileManager::HandlePointerEventL( const TPointerEvent& aPointerEvent )
	{ // function not used
//	if ( aPointerEvent.iType == TPointerEvent::EButton1Down )
//		{
//		if ( IsSelectionMode() )
//			{
//			iSelectedTile.SetXY( -1, -1 );
//			}
//		else
//			{
//			return SelectTile( aPointerEvent.iPosition );
//			}
//		}
    if ( iDrawingState != ENormalDrawing )
        {
        return EFalse;
        }

    if ( IsSelectionMode() )
        {
        iButtonArrow->HandlePointerEventL( aPointerEvent );
        iButtonRemove->HandlePointerEventL( aPointerEvent );
        iButtonSettings->HandlePointerEventL( aPointerEvent );
        iButtonPin->HandlePointerEventL( aPointerEvent );
        iButtonUnpin->HandlePointerEventL( aPointerEvent );
        }
    else
        {
        if ( aPointerEvent.iType == TPointerEvent::EButton1Down )
            {
            TPoint p( aPointerEvent.iPosition );
            p.iX -= iTilesOffset.iX;
            p.iY -= iTilesOffset.iY;
            iPressedDownTileIdx = FindTile( p );
            if ( iPressedDownTileIdx > KErrNotFound && iPressedDownTileIdx < iTiles.Count() )
                {
                iTiles[ iPressedDownTileIdx ]->SetPressed( ETrue );
                }
            }
        else if ( aPointerEvent.iType == TPointerEvent::EButton1Up && iPressedDownTileIdx > KErrNotFound && iPressedDownTileIdx < iTiles.Count() )
            {
            iTiles[ iPressedDownTileIdx ]->SetPressed( EFalse );
            }
        }
    
	return EFalse;
	}

TBool CEightTileManager::ButtonsContainsPoint( TPoint aPosition )
    {
    return iButtonArrow->RectWithOffset().Contains( aPosition ) || iButtonRemove->RectWithOffset().Contains( aPosition ) || 
            iButtonSettings->RectWithOffset().Contains( aPosition ) || iButtonPin->RectWithOffset().Contains( aPosition ) ||
            iButtonUnpin->RectWithOffset().Contains( aPosition );
    }

void CEightTileManager::RunTimeout()
    {
    for ( TInt i = 0; i < iTiles.Count(); i++ )
        {
        iTiles[ i ]->ResetRunState();
        }
    }

TBool CEightTileManager::RunTile( TUint aId )
    {
    CEightSettings* settings = CEightSettings::GetInstance();
    for ( TInt i = 0; i < iTiles.Count(); i++ )
        {
        if ( iTiles[ i ]->Id() == aId && !iTiles[ i ]->IsEmpty() )
            {            
            return iTiles[ i ]->RunInternal();
            }
        }
    return EFalse;
    }

void CEightTileManager::TouchGestureTap( TPoint aPosition )
	{
    if ( iDrawingState != ENormalDrawing )
        {
        return;
        }

    if ( IsSelectionMode() )
        {
        if ( ButtonsContainsPoint( aPosition ) )
            {
            return;
            }
    	aPosition.iX += iTilesOffset.iX;
    	aPosition.iY -= iTilesOffset.iY;
        TPoint selTile = iSelectedTile;
        if ( !SelectTile( TPoint( aPosition.iX, aPosition.iY ) ) )
            {
            SelectionModeStop();
            }
        if ( selTile == iSelectedTile )
            {
            SelectionModeStop();
            }
        }
    else
    	{ // do the tile action
        CEightSettings* settings = CEightSettings::GetInstance();
    	aPosition.iX += iTilesOffset.iX;
    	aPosition.iY -= iTilesOffset.iY;
    	TInt idx = FindTile( aPosition );
    	if ( idx > KErrNotFound )
    		{
            iTiles[ idx ]->SetPressed( EFalse );
    	    if ( iTiles[ idx ]->Type() == ETileTypeSpecial )
    	        {
    	        CEightTileSpecial* sp = (CEightTileSpecial*)iTiles[ idx ];
                iObserver.TileManagerAction( *this, (MEightTileManagerObserver::TTileManagerAction)sp->ActionId() );
    	        }
            else if ( iTiles[ idx ]->Type() == ETileTypeAppWindow )
                {
                CEightTileAppWindow* tile = (CEightTileAppWindow*) iTiles[ idx ];
                CEightFormWindow* wnd = tile->Window();
                if ( tile->Id() == KIdxTileThemeColor )
                    {
                    CEightWindowColor* wndc = (CEightWindowColor*) wnd;
                    wndc->SelectColor( settings->iPalette.iTileColor );
                    }
                wnd->SetObserver( this );
                wnd->SetMsgBox( iMsgBox );
                iWndMan->ShowWindowL( wnd, EFalse );
                }
            else if ( iTiles[ idx ]->Type() == ETileTypeAppShortcut )
                {
                if ( settings->iAnimateAppStartStop )
                    {
                    iObserver.TileManagerAction( *this, MEightTileManagerObserver::EStartingApp, iTiles[ idx ]->Id() );
                    iTiles[ idx ]->Run();
                    }
                else
                    {
                    iTiles[ idx ]->RunInternal();
                    }
                }
            else
            	{
                if ( settings->iAnimateAppStartStop )
                    {
                    iTiles[ idx ]->Run();
                    }
                else
                    {
                    iTiles[ idx ]->RunInternal();
                    }
            	}
    		//TInt idx1 = iBoard[ iBoard.Count() - 1 ][ 3 ];
    		//if ( idx1 == idx )
//    			{
//                iTiles[ iPressedDownTileIdx ]->SetPressed( EFalse );
//    			iObserver.TileManagerAction( *this, MEightTileManagerObserver::EExit );
//    			}
    		}
    	}
	}

void CEightTileManager::TouchGestureLongTap( TPoint aPosition )
	{
    if ( iDrawingState != ENormalDrawing )
        {
        return;
        }
	if ( !IsSelectionMode() )
		{
		SelectTile( TPoint( aPosition.iX, aPosition.iY - iTilesOffset.iY ) );
		}
	else
	    {
        if ( ButtonsContainsPoint( aPosition ) )
            {
            return;
            }
        SelectionModeStop();
	    }
	}

const TInt KMaxFlickSpeed = 1200;
const TReal KFlickAccelerationMod[ 3 ] = { 1, 1, 1 };
void CEightTileManager::TouchGestureFlickUp( TInt aValue )
    {
    TUint32 tick = User::NTickCount();
    if ( ( tick - iNanokernelTickCountDragStop ) * iNanokernelTickPeriod < 500000 )
        {
        return;
        }
    if ( iState == EScreenBouncingUp || iTilesOffset.iY >= BottomBorder() || iState == ETileMoving )
        {
        return;
        }
    if ( IsHeightSmallerThanScreen() )
        {
        return;
        }
    //TouchGestureDrag( TSize( 0, 50 ) );
    SetState( EScrollingUp );
    aValue = Abs( aValue );
    if ( aValue > KMaxFlickSpeed )
        {
        aValue = KMaxFlickSpeed;
        }
    iScrollSpeed = aValue; //100; //aValue;
    if ( aValue < KMaxFlickSpeed / 3 ) iScrollAcceleration = KFlickAccelerationMod[ 0 ] * aValue;
    else if ( aValue < 2 * KMaxFlickSpeed / 3 ) iScrollAcceleration = KFlickAccelerationMod[ 1 ] * aValue;
    else iScrollAcceleration = KFlickAccelerationMod[ 2 ] * aValue;    
    iScrollTime = 0;
    iScrollOffset = 0;
//    Logger::LogStrNum( _L("c:\\data\\akntouch.txt"), _L( "FlickUp speed: "), aValue );
    
    if ( CEightSettings::GetInstance()->iUseTouchFeedback && iTouchFeedback )
        {
        iTouchFeedback->InstantFeedback( ETouchFeedbackList );
        }
    }

void CEightTileManager::TouchGestureFlickDown( TInt aValue )
    {
    TUint32 tick = User::NTickCount();
    if ( ( tick - iNanokernelTickCountDragStop ) * iNanokernelTickPeriod < 500000 )
        {
        return;
        }
    if ( iState == EScreenBouncingDown || iTilesOffset.iY <= 0 || iState == ETileMoving )
        {
        return;
        }
    if ( IsHeightSmallerThanScreen() )
        {
        return;
        }
    //TouchGestureDrag( TSize( 0, -50 ) );
    SetState( EScrollingDown );
    aValue = Abs( aValue );
    if ( aValue > KMaxFlickSpeed )
        {
        aValue = KMaxFlickSpeed;
        }
    iScrollSpeed = -aValue; //-100; //
    if ( aValue < KMaxFlickSpeed / 3 ) iScrollAcceleration = -KFlickAccelerationMod[ 0 ] * aValue;
    else if ( aValue < 2 * KMaxFlickSpeed / 3 ) iScrollAcceleration = -KFlickAccelerationMod[ 1 ] * aValue;
    else iScrollAcceleration = -KFlickAccelerationMod[ 2 ] * aValue;    
    iScrollTime = 0;
    iScrollOffset = 0;
//    Logger::LogStrNum( _L("c:\\data\\akntouch.txt"), _L( "FlickDown speed: "), aValue );
    
    if ( CEightSettings::GetInstance()->iUseTouchFeedback && iTouchFeedback )
        {
        iTouchFeedback->InstantFeedback( ETouchFeedbackList );
        }
    }

void CEightTileManager::TouchGestureDragStart( TPoint aStartPoint, TPoint aCurrentPoint )
    {
    if ( IsSelectionMode() )
        {
        if ( ButtonsContainsPoint( aStartPoint ) )
            {
            return;
            }

        TInt idx = iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ];
        if ( !iTiles[ idx ]->CanBeMoved() )
            {
            return;
            }
        
        TVgRect r = iTiles[ idx ]->Rect();
        r.Move( iTilesOffset.iX, iTilesOffset.iY );
        if ( r.Contains( aStartPoint ) )
            {
            iSelectionTileOffset.SetXY( aCurrentPoint.iX - aStartPoint.iX, aCurrentPoint.iY - aStartPoint.iY );
            SetState( ETileMoving );
            
            if ( !iSpecialRow )
                {
                TFixedArray< TInt, 4 > tab( KRow, 4 );
                iBoard.AppendL( tab );
                }
            }
        }
    }
const TInt KMaxDragSize = 100;
TBool CEightTileManager::TouchGestureDrag( TSize aOffset, TBool anyState )
    {
    if ( !IsSelectionMode() && IsHeightSmallerThanScreen() )
        {
        return EFalse;
        }
    if ( iState == ETileMoving )
        {
        iSelectionTileOffset.iX -= aOffset.iWidth;
        iSelectionTileOffset.iY += aOffset.iHeight;
        return EFalse;
        }
    if ( !anyState && iState != ENormal )
        {
        return EFalse;
        }
    if ( iButtonArrow->IsPressed() || iButtonRemove->IsPressed() || iButtonSettings->IsPressed() || iButtonPin->IsPressed() || iButtonUnpin->IsPressed() )
        {
        return EFalse;
        }
    if ( Abs( aOffset.iHeight ) > KMaxDragSize )
        {
        aOffset.iHeight = ( aOffset.iHeight > 0 ? KMaxDragSize : -KMaxDragSize );
        }
    iTilesOffset.iY += aOffset.iHeight;
//    if ( iState == ENormal )
//        {
//        CheckScreenBounce();
//        }
    if ( iState == ENormal && iTilesOffset.iY < 0 )
        {
        iTilesOffset.iY = 0;
        }
    else if ( iState == ENormal && iTilesOffset.iY > BottomBorder() )
        {
        if ( IsHeightSmallerThanScreen() )
            {
            iTilesOffset.iY = 0;
            }
        else
            {
            iTilesOffset.iY = BottomBorder();
            }
        }
    return ETrue;
    }

void CEightTileManager::TouchGestureDragStop()
    {
    if ( iState == ETileMoving )
        {
        if ( iSelectedTile != iSelectionClosestTileIndexes )
            {
            if ( MoveTile( iSelectedTile, iSelectionClosestTileIndexes ) )
                {
                iSelectedTile = iSelectionClosestTileIndexes;
                }
            }
        iSelectionTileOffset.SetXY( 0, 0 );
        SetState( ENormal );
        CEightTile* ptr = iTiles[ iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ] ];
        CompressBoard();
        UpdateTilesRects();
        iSelectedTile = FindTileInBoard( ptr );
//        if ( iTilesOffset.iY < 0 )
//            {
//            iTilesOffset.iY = 0;
//            }
//        else if ( iTilesOffset.iY > BottomBorder() )
//            {
//            if ( !IsHeightSmallerThanScreen() )
//                {
//                iTilesOffset.iY = BottomBorder();
//                }
//            }
        CheckScreenBounce();
        SetButtonsCoords();
        
        UpdateClosestTileCoords();
        
        iNanokernelTickCountDragStop = User::NTickCount();
        }
    else
        {
        //CheckScreenBounce();
        }
    }

void CEightTileManager::CheckScreenBounce()
    {
    if ( IsHeightSmallerThanScreen() )
        {
        iTilesOffset.iY = 0;
        }
    else if ( iTilesOffset.iY < 0 )
        {
//        iTilesOffset.iY = 0;
        SetState( EScreenBouncingDown );
        iBouncingValue = 10;
        if ( CEightSettings::GetInstance()->iUseTouchFeedback && iTouchFeedback )
            {
            iTouchFeedback->InstantFeedback( ETouchFeedbackBoundaryList );
            }
        }
    else if ( /*iTilesOffset.iY != 0 &&*/ iTilesOffset.iY > BottomBorder() )
        {
//        iTilesOffset.iY = BottomBorder();
        SetState( EScreenBouncingUp );
        iBouncingValue = -10;
        if ( CEightSettings::GetInstance()->iUseTouchFeedback && iTouchFeedback )
            {
            iTouchFeedback->InstantFeedback( ETouchFeedbackBoundaryList );
            }
        }
    }

TInt CEightTileManager::BottomBorder( TBool aWithSpecialRow ) const
    {
    return ( iBoard.Count() - ( aWithSpecialRow ? 0 : 1 ) ) * ( iMargin + iTileSize ) - iScreenSize.iHeight + iMargin;
    }

TVgRect CEightTileManager::FindNearestMovingTile()
    {
    if ( IsSelectionMode() )
        {
        TInt idx = iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ];
        TVgRect r = iTiles[ idx ]->Rect();
        //r.Move( iTilesOffset.iX, iTilesOffset.iY );
        r.Move( iSelectionTileOffset.iX, iSelectionTileOffset.iY );
        TVgPoint pc = r.Center();
        TReal min = 9999999;
        TInt minX = iSelectedTile.iX;
        TInt minY = iSelectedTile.iY;
        TVgRect minR;
        TInt ymaxcnt = ( iSpecialRow ? iBoard.Count() - 1 : iBoard.Count() );
        for ( TInt y = 0; y < ymaxcnt; y++ )
//        for ( TInt y = ( iSelectedTile.iY == 0 ? 0 : iSelectedTile.iY - 1 ); y <= ( iSelectedTile.iY + 1 < iBoard.Count() ? iSelectedTile.iY + 1 : iSelectedTile.iY ); y++ )
            {
            TInt cnt = iBoard[ iSelectedTile.iY ].Count();
            if ( iTiles[ idx ]->TileSize() == ETileSize_2x2 ) cnt = 3;
            else if ( iTiles[ idx ]->TileSize() == ETileSize_4x2 ) cnt = 1;
                
            for ( TInt x = 0; x < cnt; x++ )
//            for ( TInt x = 0; x <= iBoard[ iSelectedTile.iY ].Count(); x++ )
                {
                if ( x == iSelectedTile.iX && y == iSelectedTile.iY )
                    {
                    //continue;
                    }
                TVgRect r1 = GetTileRect( x, y, iTiles[ idx ]->TileSize() );
                TVgPoint pc1 = r1.Center();
                TReal len1 = ( pc1.iX - pc.iX ) * ( pc1.iX - pc.iX ) +( pc1.iY - pc.iY ) * ( pc1.iY - pc.iY );
                if ( len1 < min )
                    {
                    min = len1;
                    minX = x;
                    minY = y;
                    minR = r1;
                    }
                }            
            }
        iSelectionClosestTileIndexes.SetXY( minX, minY );
        return minR;
        }
    return TVgRect( TVgPoint( 10, 100 ), TVgSize( 40, 60 ) );
    }

void CEightTileManager::InsertRow( TInt aOffsetY )
    {
    TFixedArray< TInt, 4 > tab( KRow, 4 );
    iBoard.Insert( tab, aOffsetY );
    if ( aOffsetY > 0 )
        {
        for ( TInt i = 0; i < 4; i++ )
            {
            if ( iBoard[ aOffsetY - 1 ][ i ] != KErrNotFound && ( iBoard[ aOffsetY - 1 ][ i ] & KTilePlaceholderMask ) == ( iBoard[ aOffsetY + 1 ][ i ] & KTilePlaceholderMask ) )
                {
                iBoard[ aOffsetY ][ i ] = iBoard[ aOffsetY + 1 ][ i ];
                iBoard[ aOffsetY + 1 ][ i ] = KErrNotFound;
                }
            }
        }
    }

void CEightTileManager::RemoveLastRow()
    {
    for ( TInt i = 0; i < 4; i++ )
        {
        TInt idx = iBoard[ iBoard.Count() - 1 ][ i ];
        if ( idx >= 0 && idx < iTiles.Count() )
            {
            iBoard[ iBoard.Count() - 1 ][ i ] = KErrNotFound;
            delete iTiles[ idx ];
            iTiles[ idx ] = CEightTile::NewL( 0xf10e, ETileSize_1x1, NULL, ETrue );
            //iTiles.Remove( idx );
            }
        }
    iBoard.Remove( iBoard.Count() - 1 );
    }

void CEightTileManager::FadeAllTiles( TBool aFade )
	{
    for ( TInt j = 0; j < iBoard.Count(); j++ )
        {
		for ( TInt i = 0; i < iBoard[ j ].Count(); i++ )
			{
			if ( iBoard[ j ][ i ] != KErrNotFound && iBoard[ j ][ i ] < iTiles.Count() )
				{
				TInt idx = iBoard[ j ][ i ];
				if ( aFade && !( i == iSelectedTile.iX && j == iSelectedTile.iY ) )
					{
					iTiles[ idx ]->SetFaded( ETrue );
					}
				else if ( !aFade )
					{
					iTiles[ idx ]->SetFaded( EFalse );
					}
				}
			}
        }
	}

void CEightTileManager::SetButtonsCoords()
    {
    if ( IsSelectionMode() )
        {
        TInt idx = iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ];
        if ( idx != KErrNotFound )
            {
            TVgRect r = iTiles[ idx ]->Rect();
            
            TVgSize s( 40, 40 );
            
            TVgPoint pa( r.iBr.iX - 20, r.iBr.iY - 10 );
            TVgRect ra = TVgRect::RectFromBottomLeft( pa, s );
            ra.Move( -5, -5 );
            iButtonArrow->SetRect( ra );
            iButtonArrow->SetImagePosition( pa );
            iButtonArrow->SetVisible( ETrue );
            iButtonArrow->Unpress();
            
            TVgPoint pr( r.iBr.iX - 20, r.iTl.iY - 20 );
            TVgRect rr = TVgRect::RectFromBottomLeft( pr, s );
            rr.Move( -5, -5 );
            iButtonRemove->SetRect( rr );
            iButtonRemove->SetImagePosition( pr );
            iButtonRemove->SetVisible( iTiles[ idx ]->CanBeRemoved() );
            iButtonRemove->Unpress();

            TVgPoint ps( r.iTl.iX - 10, r.iTl.iY - 20 );
            TVgRect rs = TVgRect::RectFromBottomLeft( ps, s );
            rs.Move( -5, -5 );
            iButtonSettings->SetRect( rs );
            iButtonSettings->SetImagePosition( ps );
            iButtonSettings->SetVisible( iTiles[ idx ]->CanOpenSettings() );
            iButtonSettings->Unpress();
            
            iButtonUnpin->SetVisible( EFalse );
            iButtonPin->SetVisible( EFalse );
            if ( iTiles[ idx ]->TileSize() == ETileSize_3x1 && iTiles[ idx ]->SupportsRemoving() )
                {
                TVgPoint pp( r.iTl.iX + iTileSize - 20, r.iTl.iY - 20 );
                TVgRect rp = TVgRect::RectFromBottomLeft( pp, s );
                rp.Move( -5, -5 );
                if ( iTiles[ idx ]->IsPinned() )
                    {
                    iButtonUnpin->SetRect( rp );
                    iButtonUnpin->SetImagePosition( pp );
                    iButtonUnpin->SetVisible( ETrue );
                    iButtonUnpin->Unpress();
                    }
                else
                    {
                    iButtonPin->SetRect( rp );
                    iButtonPin->SetImagePosition( pp );
                    iButtonPin->SetVisible( ETrue );
                    iButtonPin->Unpress();
                    }
                }
            
            if ( iTiles[ idx ]->CanIncreaseSize() )
                {
                if ( iTiles[ idx ]->TileSize() == ETileSize_1x1 )
                    {
                    iButtonArrow->SetImageAngle( 0 );
                    }
                else if ( iTiles[ idx ]->TileSize() == ETileSize_2x2 )
                    {
                    iButtonArrow->SetImageAngle( 45 );
                    }
                }
            else if ( iTiles[ idx ]->CanDecreaseSize() )
                {
                if ( iTiles[ idx ]->TileSize() == ETileSize_2x2 )
                    {
                    iButtonArrow->SetImageAngle( 180 );
                    }
                else if ( iTiles[ idx ]->TileSize() == ETileSize_4x2 )
                    {
                    iButtonArrow->SetImageAngle( 180 ); // 225
                    }
                }
            else
                {
                iButtonArrow->SetVisible( EFalse );
                }
            }
        }
    }

void CEightTileManager::IncreaseSelectedTileSize()
    {
    TInt idx = iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ];
    if ( iTiles[ idx ]->TileSize() == ETileSize_1x1 )
        {
        if ( iSelectedTile.iX == 0 )
            {
            TInt i1 = iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 1 ];
            TInt i2 = iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 2 ];
            TInt i3 = iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 3 ];
            if ( i1 == KErrNotFound || ( i1 & KTilePlaceholderMask ) != ( i2 & KTilePlaceholderMask ) )
                {
                if ( i1 != KErrNotFound || !( iSelectedTile.iY < iBoard.Count() - 1 && iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX ] == KErrNotFound && iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 1 ] == KErrNotFound ) )
                    {
                    InsertRow( iSelectedTile.iY + 1 );
                    if ( iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 2 ] == KErrNotFound ) iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 2 ] = i1;
                    else iBoard[ iSelectedTile.iY + 2 ][ iSelectedTile.iX + 2 ] = i1;
                    }
                iTiles[ idx ]->IncreaseTileSize();
                iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ] = idx;
                iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 1 ] = idx | KTilePlaceholder;
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX ] = idx | KTilePlaceholder;
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 1 ] = idx | KTilePlaceholder;
                }
            else if ( i1 == ( i2 & KTilePlaceholderMask ) )
                {
                InsertRow( iSelectedTile.iY + 1 );
                iBoard[ iSelectedTile.iY + 2 ][ iSelectedTile.iX + 2 ] = i3;
                iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 2 ] = i1;
                iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 3 ] = i1 | KTilePlaceholder;
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 2 ] = i1 | KTilePlaceholder;
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 3 ] = i1 | KTilePlaceholder;
                iTiles[ idx ]->IncreaseTileSize();
                iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ] = idx;
                iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 1 ] = idx | KTilePlaceholder;
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX ] = idx | KTilePlaceholder;
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 1 ] = idx | KTilePlaceholder;
                }
            else if ( ( i1 & KTilePlaceholder ) == ( i2 & KTilePlaceholder ) )
                {
                InsertRow( iSelectedTile.iY + 1 );
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 3 ] = i3;
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 2 ] = iBoard[ iSelectedTile.iY - 1 ][ iSelectedTile.iX + 3 ];
                i1 = i1 & KTilePlaceholderMask;
                iBoard[ iSelectedTile.iY - 1 ][ iSelectedTile.iX + 2 ] = i1;
                iBoard[ iSelectedTile.iY - 1 ][ iSelectedTile.iX + 3 ] = i1 | KTilePlaceholder;
                iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 2 ] = i1 | KTilePlaceholder;
                iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 3 ] = i1 | KTilePlaceholder;
                iBoard[ iSelectedTile.iY - 1 ][ iSelectedTile.iX + 1 ] = KErrNotFound;
                iTiles[ idx ]->IncreaseTileSize();
                iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ] = idx;
                iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 1 ] = idx | KTilePlaceholder;
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX ] = idx | KTilePlaceholder;
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 1 ] = idx | KTilePlaceholder;
                }
            }
        else if ( iSelectedTile.iX == 1 )
            {
            TInt i0 = iBoard[ iSelectedTile.iY ][ iSelectedTile.iX - 1 ];
            TInt i2 = iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 1 ];
            TInt i3 = iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 2 ];
            if ( i2 == KErrNotFound )
                {
                if ( !( iSelectedTile.iY < iBoard.Count() - 1 && iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX ] == KErrNotFound && iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 1 ] == KErrNotFound ) )
                    {
                    InsertRow( iSelectedTile.iY + 1 );
                    }
                iTiles[ idx ]->IncreaseTileSize();
                iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 1 ] = idx | KTilePlaceholder;
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX ] = idx | KTilePlaceholder;
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 1 ] = idx | KTilePlaceholder;
                }
            else if ( i0 != i2 && i0 != i3 && ( ( i0 & KTilePlaceholder ) == 0 || i0 == KErrNotFound ) && ( ( i2 & KTilePlaceholder ) == 0 || i2 == KErrNotFound ) && ( ( i3 & KTilePlaceholder ) == 0 || i3 == KErrNotFound ) )
                {
                InsertRow( iSelectedTile.iY + 1 );
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 2 ] = i2;
                iTiles[ idx ]->IncreaseTileSize();
                iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 1 ] = idx | KTilePlaceholder;
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX ] = idx | KTilePlaceholder;
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 1 ] = idx | KTilePlaceholder;
                }
            else if ( ( i2 & KTilePlaceholderMask ) == ( i3 & KTilePlaceholderMask ) || i0 == KErrNotFound )
                {
                InsertRow( iSelectedTile.iY + 1 );
                if ( iBoard[ iSelectedTile.iY + 2 ][ iSelectedTile.iX + 1 ] == KErrNotFound ) iBoard[ iSelectedTile.iY + 2 ][ iSelectedTile.iX + 1 ] = i0;
                else iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 1 ] = i0;
                iTiles[ idx ]->IncreaseTileSize();
                iSelectedTile.iX--;
                iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ] = idx;
                iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 1 ] = idx | KTilePlaceholder;
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX ] = idx | KTilePlaceholder;
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 1 ] = idx | KTilePlaceholder;                
                }            
            }
        else if ( iSelectedTile.iX == 2 )
            {
            TInt i0 = iBoard[ iSelectedTile.iY ][ iSelectedTile.iX - 2 ];
            TInt i1 = iBoard[ iSelectedTile.iY ][ iSelectedTile.iX - 1 ];
            TInt i3 = iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 1 ];
            if ( i3 == KErrNotFound )
                {
                if ( !( iSelectedTile.iY < iBoard.Count() - 1 && iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX ] == KErrNotFound && iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 1 ] == KErrNotFound ) )
                    {
                    InsertRow( iSelectedTile.iY + 1 );
                    }
                iTiles[ idx ]->IncreaseTileSize();
                iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 1 ] = idx | KTilePlaceholder;
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX ] = idx | KTilePlaceholder;
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 1 ] = idx | KTilePlaceholder;
                }
            else if ( i0 != i1 && i0 != i3 && ( ( i0 & KTilePlaceholder ) == 0 || i0 == KErrNotFound ) && ( ( i1 & KTilePlaceholder ) == 0 || i1 == KErrNotFound ) && ( ( i3 & KTilePlaceholder ) == 0 || i3 == KErrNotFound ) )
                {
                InsertRow( iSelectedTile.iY + 1 );
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX - 1 ] = i3;
                iTiles[ idx ]->IncreaseTileSize();
                iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 1 ] = idx | KTilePlaceholder;
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX ] = idx | KTilePlaceholder;
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 1 ] = idx | KTilePlaceholder;
                }
            else if ( ( i0 & KTilePlaceholderMask ) == ( i1 & KTilePlaceholderMask ) )
                {
                InsertRow( iSelectedTile.iY + 1 );
                if ( iBoard[ iSelectedTile.iY + 2 ][ iSelectedTile.iX - 1 ] == KErrNotFound ) iBoard[ iSelectedTile.iY + 2 ][ iSelectedTile.iX - 1 ] = i3;
                else iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX - 1 ] = i3;
                iTiles[ idx ]->IncreaseTileSize();
                iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ] = idx;
                iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 1 ] = idx | KTilePlaceholder;
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX ] = idx | KTilePlaceholder;
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 1 ] = idx | KTilePlaceholder;                
                }  
            }
        else if ( iSelectedTile.iX == 3 )
            {
            TInt i0 = iBoard[ iSelectedTile.iY ][ iSelectedTile.iX - 3 ];
            TInt i1 = iBoard[ iSelectedTile.iY ][ iSelectedTile.iX - 2 ];
            TInt i2 = iBoard[ iSelectedTile.iY ][ iSelectedTile.iX - 1 ];
            if ( i2 == KErrNotFound || ( i2 & KTilePlaceholderMask ) != ( i1 & KTilePlaceholderMask ) )
                {
                if ( i2 != KErrNotFound || !( iSelectedTile.iY < iBoard.Count() - 1 && iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX ] == KErrNotFound && iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX - 1 ] == KErrNotFound ) )
                    {
                    InsertRow( iSelectedTile.iY + 1 );
                    if ( iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX - 2 ] == KErrNotFound ) iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX - 2 ] = i2;
                    else iBoard[ iSelectedTile.iY + 2 ][ iSelectedTile.iX - 2 ] = i2;
                    }
                iTiles[ idx ]->IncreaseTileSize();
                iSelectedTile.iX--;
                iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ] = idx;
                iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 1 ] = idx | KTilePlaceholder;
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX ] = idx | KTilePlaceholder;
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 1 ] = idx | KTilePlaceholder;
                }
            else if ( i1 == ( i2 & KTilePlaceholderMask ) )
                {
                InsertRow( iSelectedTile.iY + 1 );
                iBoard[ iSelectedTile.iY + 2 ][ iSelectedTile.iX - 2 ] = i0;
                iBoard[ iSelectedTile.iY ][ iSelectedTile.iX - 3 ] = i1;
                iBoard[ iSelectedTile.iY ][ iSelectedTile.iX - 2 ] = i1 | KTilePlaceholder;
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX - 3 ] = i1 | KTilePlaceholder;
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX - 2 ] = i1 | KTilePlaceholder;
                iTiles[ idx ]->IncreaseTileSize();
                iSelectedTile.iX--;
                iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ] = idx;
                iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 1 ] = idx | KTilePlaceholder;
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX ] = idx | KTilePlaceholder;
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 1 ] = idx | KTilePlaceholder;
                }
            else if ( ( i1 & KTilePlaceholder ) == ( i2 & KTilePlaceholder ) )
                {
                InsertRow( iSelectedTile.iY + 1 );
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX - 3 ] = i0;
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX - 2 ] = iBoard[ iSelectedTile.iY - 1 ][ iSelectedTile.iX - 3 ];
                i1 = i1 & KTilePlaceholderMask;
                iBoard[ iSelectedTile.iY - 1 ][ iSelectedTile.iX - 3 ] = i1;
                iBoard[ iSelectedTile.iY - 1 ][ iSelectedTile.iX - 2 ] = i1 | KTilePlaceholder;
                iBoard[ iSelectedTile.iY ][ iSelectedTile.iX - 3 ] = i1 | KTilePlaceholder;
                iBoard[ iSelectedTile.iY ][ iSelectedTile.iX - 2 ] = i1 | KTilePlaceholder;
                iBoard[ iSelectedTile.iY - 1 ][ iSelectedTile.iX - 1 ] = KErrNotFound;
                iTiles[ idx ]->IncreaseTileSize();
                iSelectedTile.iX--;
                iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ] = idx;
                iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 1 ] = idx | KTilePlaceholder;
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX ] = idx | KTilePlaceholder;
                iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 1 ] = idx | KTilePlaceholder;
                }
            }
        }
    else if ( iTiles[ idx ]->TileSize() == ETileSize_2x2 )
        {
        InsertRow( iSelectedTile.iY );
        InsertRow( iSelectedTile.iY );
        iBoard[ iSelectedTile.iY + 2 ][ iSelectedTile.iX ] = KErrNotFound;
        iBoard[ iSelectedTile.iY + 2 ][ iSelectedTile.iX + 1 ] = KErrNotFound;
        iBoard[ iSelectedTile.iY + 3 ][ iSelectedTile.iX ] = KErrNotFound;
        iBoard[ iSelectedTile.iY + 3 ][ iSelectedTile.iX + 1 ] = KErrNotFound;
        if ( ( iSelectedTile.iX == 0 && iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 2 ] != KErrNotFound && ( iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 2 ] & KTilePlaceholder ) == KTilePlaceholder ) ||
             ( iSelectedTile.iX == 2 && iBoard[ iSelectedTile.iY ][ iSelectedTile.iX - 2 ] != KErrNotFound && ( iBoard[ iSelectedTile.iY ][ iSelectedTile.iX - 2 ] & KTilePlaceholder ) == KTilePlaceholder ) )
            {
            iSelectedTile.iY++;
            }
        else if ( iSelectedTile.iX == 1 )
            {
            iBoard[ iSelectedTile.iY + 2 ][ 1 ] = iBoard[ iSelectedTile.iY + 3 ][ 0 ];
            iBoard[ iSelectedTile.iY + 2 ][ 2 ] = iBoard[ iSelectedTile.iY + 3 ][ 3 ];
            iBoard[ iSelectedTile.iY + 3 ][ 0 ] = KErrNotFound;
            iBoard[ iSelectedTile.iY + 3 ][ 3 ] = KErrNotFound;
            }
        iTiles[ idx ]->IncreaseTileSize();
        iSelectedTile.iX = 0;
        iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ] = idx;
        iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 1 ] = idx | KTilePlaceholder;
        iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 2 ] = idx | KTilePlaceholder;
        iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 3 ] = idx | KTilePlaceholder;
        iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX ] = idx | KTilePlaceholder;
        iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 1 ] = idx | KTilePlaceholder;
        iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 2 ] = idx | KTilePlaceholder;
        iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 3 ] = idx | KTilePlaceholder;
        }

    CompressBoard();
    UpdateTilesRects();
    SetButtonsCoords();
    UpdateClosestTileCoords();
    }

void CEightTileManager::DecreaseSelectedTileSize()
    {
    TInt idx = iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ];
    if ( iTiles[ idx ]->TileSize() == ETileSize_2x2 )
        {
        iTiles[ idx ]->DecreaseTileSize();
        iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 1 ] = KErrNotFound;
        iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX ] = KErrNotFound;
        iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 1 ] = KErrNotFound;
        }
    else if ( iTiles[ idx ]->TileSize() == ETileSize_4x2 )
        {
        iTiles[ idx ]->DecreaseTileSize();
        if ( iTiles[ idx ]->TileSize() == ETileSize_2x2 )
            {
            iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 2 ] = KErrNotFound;
            iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 3 ] = KErrNotFound;
            iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 2 ] = KErrNotFound;
            iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 3 ] = KErrNotFound;
            }
        else if ( iTiles[ idx ]->TileSize() == ETileSize_1x1 )
            {
            iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 1 ] = KErrNotFound;
            iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 2 ] = KErrNotFound;
            iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 3 ] = KErrNotFound;
            iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX ] = KErrNotFound;
            iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 1 ] = KErrNotFound;
            iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 2 ] = KErrNotFound;
            iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 3 ] = KErrNotFound;
            }
        }
    // flow after decrease
    if ( iTiles[ idx ]->TileSize() == ETileSize_1x1 )
        {
    	TBool updated = EFalse;
        for ( TInt i = 0; i < 4; i++ )
            {
            if ( iSelectedTile.iY + 2 < iBoard.Count() && iBoard[ iSelectedTile.iY + 2 ][ iSelectedTile.iX + i ] == KErrNotFound )
                {
                iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ] = KErrNotFound;
                iSelectedTile.iY += 2;
                iSelectedTile.iX += i;
                iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ] = idx;
                updated = ETrue;
                break;
                }
            }
        if ( !updated && iSelectedTile.iY > 0 )
        	{
            for ( TInt i = 0; i < 4; i++ )
                {
                if ( iBoard[ iSelectedTile.iY - 1 ][ iSelectedTile.iX + i ] == KErrNotFound )
                    {
                    iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ] = KErrNotFound;
                    iSelectedTile.iY--;
                    iSelectedTile.iX += i;
                    iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ] = idx;
                    break;
                    }
                }
        	}
        }
    else if ( iTiles[ idx ]->TileSize() == ETileSize_2x2 )
        {
        if ( iSelectedTile.iY + 3 < iBoard.Count() && 
             iBoard[ iSelectedTile.iY + 2 ][ iSelectedTile.iX ] == KErrNotFound && iBoard[ iSelectedTile.iY + 2 ][ iSelectedTile.iX + 1 ] == KErrNotFound &&
             iBoard[ iSelectedTile.iY + 3 ][ iSelectedTile.iX ] == KErrNotFound && iBoard[ iSelectedTile.iY + 3 ][ iSelectedTile.iX + 1 ] == KErrNotFound )
            {
            iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ] = KErrNotFound;
            iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 1 ] = KErrNotFound;
            iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX ] = KErrNotFound;
            iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 1 ] = KErrNotFound;
            iSelectedTile.iY += 2;
            iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ] = idx;
            iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 1 ] = idx | KTilePlaceholder;
            iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX ] = idx | KTilePlaceholder;
            iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 1 ] = idx | KTilePlaceholder;
            }
        else if ( iSelectedTile.iY + 2 < iBoard.Count() && iBoard[ iSelectedTile.iY + 2 ][ iSelectedTile.iX ] == KErrNotFound && iBoard[ iSelectedTile.iY + 2 ][ iSelectedTile.iX + 1 ] == KErrNotFound )
            {
            iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ] = KErrNotFound;
            iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 1 ] = KErrNotFound;
            iSelectedTile.iY++;
            iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ] = idx;
            iBoard[ iSelectedTile.iY ][ iSelectedTile.iX + 1 ] = idx | KTilePlaceholder;
            iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX ] = idx | KTilePlaceholder;
            iBoard[ iSelectedTile.iY + 1 ][ iSelectedTile.iX + 1 ] = idx | KTilePlaceholder;
            }
        }

    CompressBoard();
    UpdateTilesRects();
    SetButtonsCoords();
    CheckScreenBounce();
    UpdateClosestTileCoords();
    }

void CEightTileManager::RemoveSelectedTile()
    {
    TInt idx = iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ];
    if ( iTiles[ idx ]->Id() == KIdxTileAds )
        {
        CEightSettings* settings = CEightSettings::GetInstance();
        settings->iAdQueryShowed = EFalse;
        settings->SaveToFileL();
        }
    TInt id = iTiles[ idx ]->Id();
    CEightTile* tile = CEightTile::NewL( 0xffed, iTiles[ idx ]->TileSize(), NULL, ETrue );
    delete iTiles[ idx ];
    iTiles[ idx ] = tile;
    SelectionModeStop();
    //iSelectedTile.SetXY( -1, -1 );
    CompressBoard();
    UpdateTilesRects();
    CheckScreenBounce();
    iObserver.TileManagerRemovedTile( *this, id );
    }

void CEightTileManager::RemoveTile( TUint aTileId )
    {
    for ( TInt i = 0; i < iTiles.Count(); i++ )
        {
        if ( iTiles[ i ]->Id() == aTileId && !iTiles[ i ]->IsEmpty() )
            {
            TInt idx = i;
            if ( iSelectedTile.iY > -1 && iSelectedTile.iX > -1 ) idx = iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ];
            if ( i == idx )
                {
                CEightTile* tile = CEightTile::NewL( 0xffed, iTiles[ i ]->TileSize(), NULL, ETrue );
                delete iTiles[ i ];
                iTiles[ i ] = tile;
                SelectionModeStop();
                CompressBoard();
                UpdateTilesRects();
                CheckScreenBounce();
                }
            break;
            }
        }
    }

TBool CEightTileManager::CanSwapLeftRight()
    {
    TUint32 tick = User::NTickCount();
    return ( ( tick - iNanokernelTickCountDragStop ) * iNanokernelTickPeriod > 500000 );
//    if ( iOldState == ETileMoving && iState != ETileMoving )
//        {
//        TTime time;
//        time.HomeTime();
//        return ( time.MicroSecondsFrom( iStateChangeTimestamp ).Int64() > 700000 );
//        }
//    return iState != ETileMoving;
    }

void CEightTileManager::SetXOffset( VGfloat aOffset )
    {
    iTilesOffset.iX = aOffset;
    }

void CEightTileManager::SetPinTile( TUint aTIleId, TBool aPinned )
    {
    for ( TInt i = 0; i < iTiles.Count(); i++ )
        {
        if ( iTiles[ i ]->Id() == aTIleId )
            {
            iTiles[ i ]->SetPinned( aPinned );
            SetButtonsCoords();
            break;
            }
        }
    }

void CEightTileManager::UpdatePinTiles( CEightTileManager* aManager )
	{
	for ( TInt i = 0; i < iTiles.Count(); i++ )
		{
		iTiles[ i ]->SetPinned( aManager->Tile( iTiles[ i ]->Id() ) != NULL );
		}
	}

TBool CEightTileManager::MoveTile( TPoint& aStartIdx, TPoint& aEndIdx )
    {
    TInt idx = iBoard[ aStartIdx.iY ][ aStartIdx.iX ];
    TInt idxEorg = iBoard[ aEndIdx.iY ][ aEndIdx.iX ];
    TInt idxE = idxEorg;
    if( idxE > 0 ) idxE &= KTilePlaceholderMask;
    if ( iTiles[ idx ]->TileSize() == ETileSize_1x1 )
        {
        if ( idxE == KErrNotFound )
            {
            iBoard[ aStartIdx.iY ][ aStartIdx.iX ] = KErrNotFound;
            iBoard[ aEndIdx.iY ][ aEndIdx.iX ] = idx;
            return ETrue;
            }
        else if ( idxE >= 0 && idxE < iTiles.Count() && iTiles[ idxE ] )
            {
            if ( aEndIdx.iY - 1 >= 0 && idxE == ( iBoard[ aEndIdx.iY - 1 ][ aEndIdx.iX ] & KTilePlaceholderMask ) )
                {
                aEndIdx.iY++;
                }
            InsertRow( aEndIdx.iY );
            if ( aStartIdx.iY >= aEndIdx.iY )
                {
                aStartIdx.iY++;
                }
            iBoard[ aStartIdx.iY ][ aStartIdx.iX ] = KErrNotFound;
            iBoard[ aEndIdx.iY ][ aEndIdx.iX ] = idx;
            return ETrue;
            }
        }
    else if ( iTiles[ idx ]->TileSize() == ETileSize_2x2 )
        {
        if ( aEndIdx.iX + 1 >= 4 || aEndIdx.iY + 1 >= iBoard.Count() )
            {
            return EFalse;
            }
        TInt idxE11 = idxE;
        TInt idxE21 = iBoard[ aEndIdx.iY ][ aEndIdx.iX + 1 ];
        TInt idxE12 = iBoard[ aEndIdx.iY + 1 ][ aEndIdx.iX ];
        TInt idxE22 = iBoard[ aEndIdx.iY + 1 ][ aEndIdx.iX + 1 ];
        TBool emp11 = ( idxE11 == KErrNotFound || ( idxE11 & KTilePlaceholderMask ) == idx );
        TBool emp21 = ( idxE21 == KErrNotFound || ( idxE21 & KTilePlaceholderMask ) == idx );
        TBool emp12 = ( idxE12 == KErrNotFound || ( idxE12 & KTilePlaceholderMask ) == idx );
        TBool emp22 = ( idxE22 == KErrNotFound || ( idxE22 & KTilePlaceholderMask ) == idx );
        if ( emp11 && emp21 && emp12 && emp22 )
            {
            iBoard[ aStartIdx.iY ][ aStartIdx.iX ] = KErrNotFound;
            iBoard[ aStartIdx.iY ][ aStartIdx.iX + 1 ] = KErrNotFound;
            iBoard[ aStartIdx.iY + 1 ][ aStartIdx.iX ] = KErrNotFound;
            iBoard[ aStartIdx.iY + 1 ][ aStartIdx.iX + 1 ] = KErrNotFound;
            iBoard[ aEndIdx.iY ][ aEndIdx.iX ] = idx;
            iBoard[ aEndIdx.iY ][ aEndIdx.iX + 1 ] = idx | KTilePlaceholder;
            iBoard[ aEndIdx.iY + 1 ][ aEndIdx.iX ] = idx | KTilePlaceholder;
            iBoard[ aEndIdx.iY + 1 ][ aEndIdx.iX + 1 ] = idx | KTilePlaceholder;
            return ETrue;
            }
        else 
            {
            TInt insertCnt = 0;
            if ( emp11 && emp21 && ( !emp12 || !emp22 ) )
                {
                InsertRow( aEndIdx.iY );
                insertCnt = 1;
                }
            else if ( emp12 && emp22 && ( !emp11 || !emp21 ) && aEndIdx.iY > 0 )
                {
                aEndIdx.iY++;
                InsertRow( aEndIdx.iY );
                insertCnt = 1;
                }
            else
                {
                if ( aEndIdx.iY > 0 && aEndIdx.iY < iBoard.Count() - 1 && iBoard[ aEndIdx.iY ][ 1 ] > KErrNotFound && iBoard[ aEndIdx.iY ][ 2 ] > KErrNotFound &&
                     aStartIdx.iY != aEndIdx.iY )
                    {
                    if ( ( ( iBoard[ aEndIdx.iY ][ 1 ] & KTilePlaceholderMask ) == ( iBoard[ aEndIdx.iY + 1 ][ 1 ] & KTilePlaceholderMask ) && 
                           ( iBoard[ aEndIdx.iY ][ 2 ] & KTilePlaceholderMask ) == ( iBoard[ aEndIdx.iY - 1 ][ 2 ] & KTilePlaceholderMask ) ) ||
                         ( ( iBoard[ aEndIdx.iY ][ 1 ] & KTilePlaceholderMask ) == ( iBoard[ aEndIdx.iY - 1 ][ 1 ] & KTilePlaceholderMask ) && 
                           ( iBoard[ aEndIdx.iY ][ 2 ] & KTilePlaceholderMask ) == ( iBoard[ aEndIdx.iY + 1 ][ 2 ] & KTilePlaceholderMask ) ) )
                        {
                        insertCnt++;
                        InsertRow( aEndIdx.iY );
                        aEndIdx.iY++;
                        }
                    }
                if ( aEndIdx.iY > 0 )
                    {
                    aEndIdx.iY++;
                    }
                if ( ( idxE11 > KErrNotFound && idxE12 > KErrNotFound && ( idxE11 & KTilePlaceholderMask ) == ( idxE12 & KTilePlaceholderMask ) && idx != ( idxE11 & KTilePlaceholderMask ) ) ||
                     ( idxE22 > KErrNotFound && idxE21 > KErrNotFound && ( idxE22 & KTilePlaceholderMask ) == ( idxE21 & KTilePlaceholderMask ) && idx != ( idxE22 & KTilePlaceholderMask ) ) )
                    {
                    if ( aEndIdx.iY > 0 )
                        {
                        aEndIdx.iY--;
                        }
                    }

                insertCnt += 2;
                InsertRow( aEndIdx.iY );
                InsertRow( aEndIdx.iY );
                }
            if ( aStartIdx.iY >= aEndIdx.iY )
                {
                aStartIdx.iY += insertCnt;
                }
            iBoard[ aStartIdx.iY ][ aStartIdx.iX ] = KErrNotFound;
            iBoard[ aStartIdx.iY ][ aStartIdx.iX + 1 ] = KErrNotFound;
            iBoard[ aStartIdx.iY + 1 ][ aStartIdx.iX ] = KErrNotFound;
            iBoard[ aStartIdx.iY + 1 ][ aStartIdx.iX + 1 ] = KErrNotFound;
            iBoard[ aEndIdx.iY ][ aEndIdx.iX ] = idx;
            iBoard[ aEndIdx.iY ][ aEndIdx.iX + 1 ] = idx | KTilePlaceholder;
            iBoard[ aEndIdx.iY + 1 ][ aEndIdx.iX ] = idx | KTilePlaceholder;
            iBoard[ aEndIdx.iY + 1 ][ aEndIdx.iX + 1 ] = idx | KTilePlaceholder;
            return ETrue;
            }
        }
    else if ( iTiles[ idx ]->TileSize() == ETileSize_4x2 )
        {
        if ( aEndIdx.iX > 0 || aEndIdx.iY + 1 >= iBoard.Count() )
            {
            return EFalse;
            }
        if ( aEndIdx.iY > 0 )
            {
            if ( aEndIdx.iY < iBoard.Count() - 1 && iBoard[ aEndIdx.iY ][ 1 ] > KErrNotFound && iBoard[ aEndIdx.iY ][ 2 ] > KErrNotFound && aStartIdx.iY != aEndIdx.iY )
                {
                if ( ( ( iBoard[ aEndIdx.iY ][ 1 ] & KTilePlaceholderMask ) == ( iBoard[ aEndIdx.iY + 1 ][ 1 ] & KTilePlaceholderMask ) && 
                       ( iBoard[ aEndIdx.iY ][ 2 ] & KTilePlaceholderMask ) == ( iBoard[ aEndIdx.iY - 1 ][ 2 ] & KTilePlaceholderMask ) ) ||
                     ( ( iBoard[ aEndIdx.iY ][ 1 ] & KTilePlaceholderMask ) == ( iBoard[ aEndIdx.iY - 1 ][ 1 ] & KTilePlaceholderMask ) && 
                       ( iBoard[ aEndIdx.iY ][ 2 ] & KTilePlaceholderMask ) == ( iBoard[ aEndIdx.iY + 1 ][ 2 ] & KTilePlaceholderMask ) ) )
                    {
                    InsertRow( aEndIdx.iY );
                    aEndIdx.iY++;
                    if ( aStartIdx.iY >= aEndIdx.iY )
                        {
                        aStartIdx.iY++;
                        }
                    }
                }
            if ( ( iBoard[ aEndIdx.iY ][ 0 ] > KErrNotFound && ( iBoard[ aEndIdx.iY ][ 0 ] & KTilePlaceholderMask ) == ( iBoard[ aEndIdx.iY - 1 ][ 0 ] & KTilePlaceholderMask ) ) || 
                 ( iBoard[ aEndIdx.iY ][ 2 ] > KErrNotFound && ( iBoard[ aEndIdx.iY ][ 2 ] & KTilePlaceholderMask ) == ( iBoard[ aEndIdx.iY - 1 ][ 2 ] & KTilePlaceholderMask ) ) )
                {
                aEndIdx.iY++;
                }
            }
        InsertRow( aEndIdx.iY );
        InsertRow( aEndIdx.iY );    
        if ( aStartIdx.iY >= aEndIdx.iY )
            {
            aStartIdx.iY += 2;
            }
        for ( TInt i = 0; i < 4; i++ )
            {
            iBoard[ aStartIdx.iY ][ i ] = KErrNotFound;
            iBoard[ aStartIdx.iY + 1 ][ i ] = KErrNotFound;
            iBoard[ aEndIdx.iY ][ i ] = idx | KTilePlaceholder;
            iBoard[ aEndIdx.iY + 1 ][ i ] = idx | KTilePlaceholder;
            }
        iBoard[ aEndIdx.iY ][ aEndIdx.iX ] = idx;
        return ETrue;
        }

    return EFalse;
    }

void CEightTileManager::StartReady()
    {
    TInt v = -360;
    for ( TInt i = 0; i < iTiles.Count(); i++ )
        {
        iTiles[ i ]->iDrawingOffset = v;
        iTiles[ i ]->iDrawingScale = 0.5;
        v -= 80;
        }
    }

void CEightTileManager::NormalReady()
    {
    iDrawingState = ENormalDrawing;
    }

void CEightTileManager::EndReady()
    {
    SelectionModeStop();
    iDrawingState = EEnd;
	iDrawingEndIdx = ( CEightSettings::GetInstance()->iShowBottomButtons ? 2 : 0 );
    }

void CEightTileManager::Update( TTimeIntervalMicroSeconds32 aTimeInterval )
    {
    if ( iDrawingState == EStart )
        {
        TBool swichState = ETrue;
        for ( TInt i = 0; i < iBoard.Count(); i++ )
            {
            for ( TInt j = 0; j < iBoard[ i ].Count(); j++ )
                {
                TInt idx = iBoard[ i ][ j ];
                if ( idx >= 0 && idx < iTiles.Count() )
                    {
                    if ( iTiles[ idx ]->iDrawingOffset < 0 )
                        {
                        iTiles[ idx ]->iDrawingOffset /= 1.2;
                        if ( Abs( iTiles[ idx ]->iDrawingOffset ) < 1 )
                            {
                            iTiles[ idx ]->iDrawingOffset = 0;
                            }
//                            iTiles[ idx ]->iDrawingScale *= 1.01;
//                            if ( iTiles[ idx ]->iDrawingScale > 1 )
//                                {
//                                iTiles[ idx ]->iDrawingScale = 1;
//                                }
                        swichState = EFalse;
                        }
                    }
                }
            }
        if ( swichState )
            {
            CEightPhotosProvider::GetInstanceL()->SetHighFpsIdle( ETrue );
            iDrawingState = ENormalDrawing;
            }
        }
    else if ( iDrawingState == EEnd )
        {
        TBool finished = ETrue;
        for ( TInt i = iBoard.Count() - 1, k = 0; i >= 0 ; i-- )
            {
            TBool br = EFalse;
            for ( TInt j = 0; j < iBoard[ i ].Count(); j++ )
                {
                TInt idx = iBoard[ i ][ j ];
                if ( idx >= 0 && idx < iTiles.Count() )
                    {
                    if ( iTiles[ idx ]->iDrawingOffset > -360 )
                        {
                        finished = EFalse;
						if ( iTiles[ idx ]->iDrawingOffset == 0 )
							{
							iTiles[ idx ]->iDrawingOffset = -1;
							}
						else
							{
							iTiles[ idx ]->iDrawingOffset *= 1.6;
							}
                        iDrawingEndIdx += 0.1;
						
                        if ( (TInt)iDrawingEndIdx <= k )
                            {
                            br = ETrue;
                            break;
                            }
                        }
                    k++;
                    }
                }                
            if ( br ) break;
            }
        if ( finished )
            {
            iObserver.TileManagerAction( *this, MEightTileManagerObserver::EExitAnimFinished );
            }
        }
    
    if ( iState == ENormal )
        {
        
        }
    else if ( iState == EScrollingDown || iState == EScrollingUp )
        {
        TReal t = aTimeInterval.Int();
        t /= 1000000; // s
        iScrollTime += t;
        iScrollOffsetOld = iScrollOffset;
        iScrollOffset = iScrollSpeed * iScrollTime - iScrollAcceleration * iScrollTime * iScrollTime / 2;
        if ( ( iState == EScrollingUp && iScrollSpeed - iScrollAcceleration * iScrollTime < 0 ) ||
             ( iState == EScrollingDown && iScrollSpeed - iScrollAcceleration * iScrollTime > 0 ) )
            {
            //TouchGestureDrag( TSize( 0, iScrollOffset - iScrollOffsetOld ), ETrue );
            SetState( ENormal );
            iScrollOffset = 0;
            iScrollOffsetOld = 0;
            CheckScreenBounce();
            }
        else
            {
            TouchGestureDrag( TSize( 0, iScrollOffset - iScrollOffsetOld ), ETrue );
            TInt b = BottomBorder();
            if ( iTilesOffset.iY < 0 )
                {
                iScrollAcceleration += iTilesOffset.iY / 2;
                }
            else if ( iTilesOffset.iY > b )
                {
                iScrollAcceleration += ( iTilesOffset.iY - b ) / 2;
                }
            }
//        if ( ! )
//            {
//            SetState( ENormal );
//            iScrollOffset = 0;
//            iScrollOffsetOld = 0;
//            }
        
//        TBuf<64> buf;
//        buf.AppendFormat( _L("scrool offset: %.4f   time %.4f"), iScrollOffset, t );

        //Logger::LogStr( _L("c:\\data\\akntouch.txt"), buf );
        }
    else if ( iState == EScreenBouncingDown || iState == EScreenBouncingUp )
        {
        if ( iState == EScreenBouncingDown && iTilesOffset.iY < 0 )
            {
            iTilesOffset.iY += iBouncingValue;
            iBouncingValue += 5;
            if ( iTilesOffset.iY > 0 )
                {
                iTilesOffset.iY = 0;
                }
            }
        else if ( iState == EScreenBouncingUp && iTilesOffset.iY > BottomBorder() )
            {
            iTilesOffset.iY += iBouncingValue;
            iBouncingValue -= 5;
            if ( iTilesOffset.iY < BottomBorder() )
                {
                iTilesOffset.iY = BottomBorder();
                }
            }
        else
            {
            SetState( ENormal );
            }
        }
    else if ( iState == ETileMoving )
        {
        iSelectionClosestTile = FindNearestMovingTile();
        
        TInt idx = iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ];
        TVgRect r = iTiles[ idx ]->Rect();
        r.Move( iTilesOffset.iX, iTilesOffset.iY );
        r.Move( iSelectionTileOffset.iX, iSelectionTileOffset.iY );
        TVgPoint pc = r.Center();
        
        iSelectionClosestTileAnim.iX += ( iSelectionClosestTile.iTl.iX - iSelectionClosestTileAnim.iX ) / KSelectionClosestTileAnimDiv;
        iSelectionClosestTileAnim.iY += ( iSelectionClosestTile.iTl.iY - iSelectionClosestTileAnim.iY ) / KSelectionClosestTileAnimDiv;
        
        if ( pc.iY < 3 * iTileSize / 2 )
            {
            TInt inc = 1;
            if ( pc.iY < iTileSize / 2 )
                {
                inc = 7;
                }
            else if ( pc.iY < iTileSize )
                {
                inc = 3;
                }
            iTilesOffset.iY += inc;
            if ( iTilesOffset.iY > BottomBorder() )
                {
                if ( !IsHeightSmallerThanScreen() )
                    {
                    iTilesOffset.iY = BottomBorder();
                    }
                }
            else
                {
                iSelectionTileOffset.iY -= inc;
                }
            }
        else if ( pc.iY > iScreenSize.iHeight - 3 * iTileSize / 2 )
            {
            TInt inc = 1;
            if ( pc.iY > iScreenSize.iHeight - iTileSize / 2 )
                {
                inc = 7;
                }
            else if ( pc.iY > iScreenSize.iHeight - iTileSize )
                {
                inc = 3;
                }
            iTilesOffset.iY -= inc;
            if ( iTilesOffset.iY < 0 )
                {
                if ( !IsHeightSmallerThanScreen() )
                    {
                    iTilesOffset.iY = 0;
                    }
                }
            else
                {
                iSelectionTileOffset.iY += inc;
                }
            }
        }
    
    if ( IsSelectionMode() )
        {
        iButtonArrow->SetButtonRectOffset( iTilesOffset + iSelectionTileOffset );
        iButtonRemove->SetButtonRectOffset( iTilesOffset + iSelectionTileOffset );
        iButtonSettings->SetButtonRectOffset( iTilesOffset + iSelectionTileOffset );
        iButtonPin->SetButtonRectOffset( iTilesOffset + iSelectionTileOffset );
        iButtonUnpin->SetButtonRectOffset( iTilesOffset + iSelectionTileOffset );
        }
    
    for ( TInt i = 0; i < iTiles.Count(); i++ )
        {
        iTiles[ i ]->Update( aTimeInterval );
        }
    }

RPointerArray< CEightTile >& CEightTileManager::Tiles()
    {
    return iTiles;
    }

RArray< TFixedArray< TInt, 4 > >& CEightTileManager::Board()
    {
    return iBoard;
    }

CEightTile* CEightTileManager::Tile( TUint aId )
    {
    for ( TInt i = 0; i < iTiles.Count(); i++ )
        {
        if ( iTiles[ i ]->Id() == aId )
            {
            return iTiles[ i ];
            }
        }
    return NULL;
    }

void CEightTileManager::Draw()
    {
    DrawTiles();
    }

void CEightTileManager::DrawTiles()
    {
    vgSeti( VG_MATRIX_MODE, VG_MATRIX_GLYPH_USER_TO_SURFACE );
    vgLoadIdentity();
    vgTranslate( iTilesOffset.iX, iTilesOffset.iY );// + iScrollOffset );
    
    vgSeti( VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE );
    vgLoadIdentity();
    vgTranslate( iTilesOffset.iX, iTilesOffset.iY );// + iScrollOffset );
    
    vgSeti( VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE );
    vgLoadIdentity();
    vgTranslate( iTilesOffset.iX, iTilesOffset.iY );// + iScrollOffset );
    
    for ( TInt y = 0; y < iBoard.Count(); y++ )
        {
        for ( TInt x = 0; x < iBoard[ y ].Count(); x++ )
            {
            TInt idx = iBoard[ y ][ x ];
            if ( idx < iTiles.Count() && idx != KErrNotFound && idx != KTileSpecialTile && ( idx & KTilePlaceholder ) == 0 && !iTiles[ idx ]->IsEmpty() )
                {
                if ( !( iSelectedTile.iX == x && iSelectedTile.iY == y ) )
                    {
                    TVgRect r = iTiles[ idx ]->Rect();
                    r.Move( iTilesOffset );
                    if ( r.iBr.iY < iScreenSize.iHeight && r.iTl.iY > 0 )
                        {
                        //if ( iDrawingState != ENormalDrawing && iTiles[ idx ]->iDrawingOffset + r.Width() < 0 ) continue;
                        VGfloat m[3][9];
                        vgSeti( VG_MATRIX_MODE, VG_MATRIX_GLYPH_USER_TO_SURFACE ); vgGetMatrix( m[ 0 ] );
                        if ( iDrawingState != ENormalDrawing ) vgTranslate( iTiles[ idx ]->iDrawingOffset, 0 );
                        //if ( iDrawingState != ENormalDrawing ) vgScale( iTiles[ idx ]->iDrawingScale, 1 );
                        vgSeti( VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE ); vgGetMatrix( m[ 1 ] );
                        if ( iDrawingState != ENormalDrawing ) vgTranslate( iTiles[ idx ]->iDrawingOffset, 0 );
                        //if ( iDrawingState != ENormalDrawing ) vgScale( iTiles[ idx ]->iDrawingScale, 1 );
                        vgSeti( VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE ); vgGetMatrix( m[ 2 ] );
                        if ( iDrawingState != ENormalDrawing ) vgTranslate( iTiles[ idx ]->iDrawingOffset, 0 );
                        //if ( iDrawingState != ENormalDrawing ) vgScale( iTiles[ idx ]->iDrawingScale, 1 );
                        
                        iTiles[ idx ]->Draw();
                        
                        vgSeti( VG_MATRIX_MODE, VG_MATRIX_GLYPH_USER_TO_SURFACE ); vgLoadMatrix( m[ 0 ] );
                        vgSeti( VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE ); vgLoadMatrix( m[ 1 ] );
                        vgSeti( VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE ); vgLoadMatrix( m[ 2 ] );
                        }
                    }
                }
            }
        }
    
	if ( IsSelectionMode() )
		{
	    VGPath path1 = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 10, 20, VG_PATH_CAPABILITY_ALL );
	    
		
	    VGfloat m[ 9 ];
	    vgSeti( VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE );
	    vgGetMatrix( m );
		TInt idx = iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ];
		TVgRect r = iTiles[ idx ]->Rect();
		
		// draw all tiles fade 
        VGPaint paint = CEightSettings::GetInstance()->iPalette.iTilesFadeColor.CreateVgPaint();
        vgSetPaint( paint, VG_FILL_PATH );
        vgClearPath( path1, VG_PATH_CAPABILITY_ALL );
        vguRect( path1, 0, -iTilesOffset.iY, iScreenSize.iWidth, iScreenSize.iHeight ); //iBoard.Count() * ( iMargin + iTileSize ) );
        vgDrawPath( path1, VG_FILL_PATH );
        vgDestroyPaint( paint );
        
        if ( iState == ETileMoving )
            {
    		// draw neareset possible tile placeholder 
            //TVgRect r = iSelectionClosestTile;
            TVgRect r( iSelectionClosestTileAnim, iSelectionClosestTile.Size() );
            VGPaint paint = CEightSettings::GetInstance()->iPalette.iTileMovePlaceholderColor.CreateVgPaint();
            vgSetPaint( paint, VG_FILL_PATH );
            vgClearPath( path1, VG_PATH_CAPABILITY_ALL );
            VGfloat n = iMargin / 2;
            vguRect( path1, r.iTl.iX - n, r.iBr.iY - n, r.Width() + 2 * n, r.Height() + 2 * n );
            vgDrawPath( path1, VG_FILL_PATH );
            vgDestroyPaint( paint );
            }
        
		// draw selected tile 
	    vgSeti( VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE );
        vgTranslate( iSelectionTileOffset.iX, iSelectionTileOffset.iY );
	    vgSeti( VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE );
        vgTranslate( iSelectionTileOffset.iX, iSelectionTileOffset.iY );
	    vgSeti( VG_MATRIX_MODE, VG_MATRIX_GLYPH_USER_TO_SURFACE );
        vgTranslate( iSelectionTileOffset.iX, iSelectionTileOffset.iY );
        iTiles[ idx ]->Draw();
        
        vgSeti( VG_MATRIX_MODE, VG_MATRIX_GLYPH_USER_TO_SURFACE );
        vgLoadIdentity();
        //vgTranslate( iSelectionTileOffset.iX, iSelectionTileOffset.iY );
        vgSeti( VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE );
        
        vgLoadIdentity();
#ifdef DRAW_DEBUG
        iButtonArrow->DrawDebug();
#endif
        iButtonArrow->Draw();
        vgLoadIdentity();
#ifdef DRAW_DEBUG
        iButtonRemove->DrawDebug();
#endif
        iButtonRemove->Draw();
        vgLoadIdentity();
#ifdef DRAW_DEBUG
        iButtonSettings->DrawDebug();
#endif
        iButtonSettings->Draw();
        vgLoadIdentity();
#ifdef DRAW_DEBUG
        iButtonPin>DrawDebug();
#endif
        iButtonPin->Draw();
        vgLoadIdentity();
#ifdef DRAW_DEBUG
        iButtonUnpin->DrawDebug();
#endif
        iButtonUnpin->Draw();
        
        vgLoadMatrix( m );
        
        vgDestroyPath( path1 );    
        }
	
#ifdef DRAW_DEBUG
    vgSeti( VG_MATRIX_MODE, VG_MATRIX_GLYPH_USER_TO_SURFACE );
    vgLoadIdentity();
    vgTranslate( iTilesOffset.iX, iTilesOffset.iY );
    for ( TInt y = 0; y < iBoard.Count(); y++ )
        {
        for ( TInt x = 0; x < iBoard[ y ].Count(); x++ )
            {
        	if ( iSelectedTile.iX == x && iSelectedTile.iY == y )
        		{
                vgSeti( VG_MATRIX_MODE, VG_MATRIX_GLYPH_USER_TO_SURFACE );
                vgLoadIdentity();
                vgTranslate( iTilesOffset.iX, iTilesOffset.iY );
                vgTranslate( iSelectionTileOffset.iX, iSelectionTileOffset.iY );
        		}
            DrawTileDebug( x, y );
        	if ( iSelectedTile.iX == x && iSelectedTile.iY == y )
        		{
                vgSeti( VG_MATRIX_MODE, VG_MATRIX_GLYPH_USER_TO_SURFACE );
                vgLoadIdentity();
                vgTranslate( iTilesOffset.iX, iTilesOffset.iY );
        		}
            }
        }
#endif
//    vgClearPath( path1, VG_PATH_CAPABILITY_ALL );
//    for ( TInt y = 0; y < iBoard.Count(); y++ )
//        {
//        for ( TInt x = 0; x < iBoard[ y ].Count(); x++ )
//            {
//            vguRect( path1, iMargin + x * ( iMargin + iTileSize ), iScreenSize.iHeight - ( iMargin + iTileSize + y * ( iMargin + iTileSize ) ), iTileSize, iTileSize );
//            }
//        }
//
//    paint = KVgColorWhite.CreateVgPaint();
//    
//    vgSetPaint( paint, VG_STROKE_PATH );
//    vgDrawPath( path1, VG_STROKE_PATH );
//    vgSeti( VG_STROKE_LINE_WIDTH, 1 );
//    
//    vgDestroyPaint( paint );
    }

void CEightTileManager::DrawTileDebug( TInt aX, TInt aY )
	{
    TInt idx = iBoard[ aY ][ aX ];

    CEightSettings* settings = CEightSettings::GetInstance();
    CVgFont* font = settings->FontManager().Get( 1 );
    if ( font )
		{
		TBuf<64> buf;
		TVgRect r;
		if ( idx > -1 && ( idx & KTilePlaceholder ) )
			{
			r = GetTileRect( aX, aY, ETileSize_1x1 );
			buf.AppendNum( idx & KTilePlaceholderMask ); buf.Append( _L("p") ); 
			} 
		else
			{
			if ( idx == KErrNotFound )
				{
				r = GetTileRect( aX, aY, ETileSize_1x1 );
				}
			else
				{
				if ( iTiles[ idx ]->Size().iWidth > 1 )
					{
					r = GetTileRect( aX, aY, ETileSize_1x1 );
					}
				else
					{
					r = iTiles[ idx ]->Rect();
					}
				}
			buf.AppendNum( idx );
			}
		font->DrawText( r.BottomLeft(), buf );
		}
    //iTiles[ idx ]->DrawDebug();
	}

void CEightTileManager::TouchButtonPressed( TUint aId )
    {
    if ( aId == (TUint) KButtonArrow )
        {
        if ( iSelectedTile.iY >= 0 && iSelectedTile.iX >= 0 )
            {
            TInt idx = iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ];
            if ( iTiles[ idx ]->CanIncreaseSize() )
                {
                IncreaseSelectedTileSize();
                }
            else if ( iTiles[ idx ]->CanDecreaseSize() )
                {
                DecreaseSelectedTileSize();
                }
            }
        }
    else if ( aId == (TUint) KButtonRemove )
        {
        if ( iSelectedTile.iY >= 0 && iSelectedTile.iX >= 0 )
            {
            if ( iMsgBox )
                {
                iMsgBox->ShowL( KMsgBoxMessageRemoveTile, CEightMsgBox::EMsgBoxButtonYesNo, _L("Remove tile?") );
                }
            else
                {
                RemoveSelectedTile();
                }
            }        
        }
    else if ( aId == (TUint) KButtonSettings )
        {
        if ( iSelectedTile.iY >= 0 && iSelectedTile.iX >= 0 )
            {
            if ( iWndMan )
                {
                TInt idx = iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ];

                CEightWindowTileSettings* wnd = CEightWindowTileSettings::NewL( this, iScreenSize, *iTiles[ idx ] );
                iWndMan->ShowWindowL( wnd );
                }
            }  
        }
    else if ( aId == (TUint) KButtonPin )
        {
        if ( iSelectedTile.iY >= 0 && iSelectedTile.iX >= 0 )
            {
            TInt idx = iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ];
            CEightTile* tile = iTiles[ idx ];
            iObserver.TileManagerAddTile( *this, tile->Id() );
            SelectionModeStop();
            }
        }
    else if ( aId == (TUint) KButtonUnpin )
        {
        if ( iSelectedTile.iY >= 0 && iSelectedTile.iX >= 0 )
            {
            TInt idx = iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ];
            CEightTile* tile = iTiles[ idx ];
            iObserver.TileManagerRemovedTile( *this, tile->Id() );
            }
        }
    }

void CEightTileManager::FormWindowButtonAction( TUint aWindowId, TUint aButtonId )
    {
    if ( aWindowId == (TUint)KEightWindowTileSettingsId && aButtonId == 1 )
        {
        if ( iSelectedTile.iY >= 0 && iSelectedTile.iX >= 0 )
            {
            TInt idx = iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ];
            CEightTile* tile = iTiles[ idx ];
            //if ( tile->CustomColor() ) // always update app list tile color
                {
                iObserver.TileManagerAction( *this, MEightTileManagerObserver::ETileColorChanged, tile->Id() );
                }
            }
        iWndMan->CloseWindow( KEightWindowTileSettingsId );
        }
    else if ( aWindowId == (TUint)KEightWindowAboutId && aButtonId == 1 )
        {
        iWndMan->CloseWindow( KEightWindowAboutId );
        }
    else if ( aWindowId == (TUint)KEightWindowHelpId && aButtonId == 1 )
        {
        iWndMan->CloseWindow( KEightWindowHelpId );
        }
    else if ( aWindowId == (TUint)KEightWindowHelpId && aButtonId == 2 )
        { // start first time run wizzard
    	iObserver.TileManagerAction( *this, MEightTileManagerObserver::EActivateFRWizzard );
        }
    else if ( aWindowId == (TUint)KEightWindowSetupId && aButtonId == 1 )
        {
        iWndMan->CloseWindow( KEightWindowSetupId );
        UpdateTilesColor();
        iObserver.TileManagerAction( *this, MEightTileManagerObserver::ESettigsUpdated );
        }
    else if ( aWindowId == (TUint)KEightWindowSetupId && aButtonId == KEightWindowSetupResetButtonId )
        { // reset
        iWndMan->CloseWindow( KEightWindowSetupId );
        iObserver.TileManagerAction( *this, MEightTileManagerObserver::EReset );
        }
    else if ( aWindowId == (TUint)KEightWindowSetupId && aButtonId == KEightWindowSetupMenuButtonId )
        { // capture menu key
        iObserver.TileManagerAction( *this, MEightTileManagerObserver::ESettigsCaptureMenuKey );
        }
    else if ( aWindowId == (TUint)KEightWindowColorId )
        {
        iWndMan->CloseWindow( KEightWindowColorId );
        UpdateTilesColor();
        iObserver.TileManagerAction( *this, MEightTileManagerObserver::ESettigsUpdated );
        }
    else if ( aWindowId == (TUint)KEightWindowPremiumId && aButtonId == 1  )
        {
        iWndMan->CloseWindow( KEightWindowPremiumId );
        }
    }

void CEightTileManager::ResetTilesColor()
	{
    for ( TInt i = 0; i < iTiles.Count(); i++ )
        {
        if ( iTiles[ i ]->Id() == KIdxTileFacebook ) continue;
        iTiles[ i ]->SetCustomColor( EFalse );
        iTiles[ i ]->UpdateColorsFromTheme();
        }
	}

void CEightTileManager::UpdateTilesColor()
    {
    for ( TInt i = 0; i < iTiles.Count(); i++ )
        {
        iTiles[ i ]->UpdateColorsFromTheme();
        }
    }

void CEightTileManager::UpdateTileColor( TUint aTileId, TBool aCustomColor, const TEightPalette& aPalette )
    {
    CEightTile* tile = Tile( aTileId );
    if ( tile )
        {
        tile->SetCustomColor( aCustomColor );
        tile->UpdateColorsFromPalette( aPalette );
        }
    }

void CEightTileManager::UpdateTilesColors( CEightTileManager* aManager )
	{
    for ( TInt i = 0; i < iTiles.Count(); i++ )
        {
        CEightTile* tile = aManager->Tile( iTiles[ i ]->Id() );
        if ( tile )
            {
        	aManager->UpdateTileColor( iTiles[ i ]->Id(), tile->CustomColor(), iTiles[ i ]->Palette() );
            }
        }
	}

void CEightTileManager::UpdateSpcialRow()
	{
	if ( !CEightSettings::GetInstance()->iShowBottomButtons )
		{
		if ( iSpecialRow )
			{
			iSpecialRow = EFalse;
			RemoveLastRow();
			CheckScreenBounce();
			}
		}
	else
		{
		if ( !iSpecialRow )
			{
			AddSpecialTiles( iSpecialTilesType );
			CompressBoard();
			UpdateTilesRects();
			CheckScreenBounce();
			}
		}
	}

void CEightTileManager::UpdateClosestTileCoords()
    {
    if ( IsSelectionMode() )
        {
        TInt idx = iBoard[ iSelectedTile.iY ][ iSelectedTile.iX ];
        iSelectionClosestTileAnim = iTiles[ idx ]->Rect().iTl;
        iSelectionClosestTile = iTiles[ idx ]->Rect();
        }
    }

void CEightTileManager::ExternalizeL( RWriteStream &aStream ) const
    {
    const TUint32 KStreamId = ( '8' | ( 'S' << 8 ) | ( 'T' << 16 ) | ( 'M' << 24 ) );
    const TUint16 KStreamVersion = 0x0001;
    
    aStream.WriteUint32L( KStreamId );
    aStream.WriteUint16L( KStreamVersion );
    }

void CEightTileManager::InternalizeL( RReadStream& /*aStream*/ )
    {

    }

void CEightTileManager::PrepareForExternalize()
    {
    CompressBoard();
    }

