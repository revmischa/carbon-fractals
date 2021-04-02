//#include <Carbon/Carbon.h>
#include <QuicktimeComponents.h>
#include <math.h>

#include "Imagering.h"
#include "FractalNumbers.h"
#include "Complex.h"
#include "EventStuff.h"
#include "SetupAndKill.h"
#include "ColorFractal.h"
//#include "Profiler.h"

WindowPtr	gMainWindow;	//The main window, duh
GWorldPtr	offscreen;
PixMapHandle	offscreenPM;

ControlRef	zoomIn, zoomOut, pb, gEditTextReal, stopBtn;

void SaveAsPicture()
{
    OSErr err;

    FSSpec documentFSSpec;
    NavReplyRecord reply;
    NavDialogOptions dialogO;


    err=NavGetDefaultDialogOptions( &dialogO );
    if( err != noErr )
        DieError( "\pCould not load Nav Services Dialog Options!", err );

    GetIndString( dialogO.clientName, 129, 1 );
    GetIndString( dialogO.windowTitle, 129, 2 );
    GetIndString( dialogO.actionButtonLabel, 129, 3 );
    GetIndString( dialogO.cancelButtonLabel, 129, 4 );
    GetIndString( dialogO.savedFileName, 129, 5 );
    GetIndString( dialogO.message, 129, 6 );
    dialogO.preferenceKey=NULL;
    dialogO.popupExtension=NULL;

    err = NavPutFile(NULL, &reply, &dialogO, NULL, kQTFileTypeTIFF, 'ogle', NULL );

    if ( reply.validRecord && err == noErr )
    {
        AEKeyword 	keyWord;
        DescType 	typeCode;
        Size 		actualSize = 0;

        err = AEGetNthPtr( &(reply.selection), 1, typeFSS, &keyWord, &typeCode, &documentFSSpec, sizeof( documentFSSpec ), &actualSize );
        if( err != noErr )
            DieError( "\pAEGetNthPtr error", err );

        if ( !reply.replacing )
        {
            err=FSpCreate( &documentFSSpec, 'ogle', kQTFileTypeTIFF, reply.keyScript );
            if( err != noErr )
                DieError( "\pFSpCreate error", err );
        }

        if( err != noErr )
            DieError( "\pCreateJPEGFile error", err );
    }
    else
    {
        return;
    }

    unsigned long sizeWritten;
    ComponentResult cr;

    GraphicsExportComponent ci;
    //ci = OpenDefaultComponent(GraphicsExporterComponentType, kBaseGraphicsExporterSubType);
    OpenADefaultComponent( GraphicsExporterComponentType, kQTFileTypeJPEG, &ci );
    if (ci)
    {
        if ( CallComponentCanDo( ci , kGraphicsExportRequestSettingsSelect ) )
        {
            cr=GraphicsExportRequestSettings( ci, nil, nil );

            cr=GraphicsExportSetInputGWorld( ci, offscreen );

            cr=GraphicsExportSetOutputFile( ci, &documentFSSpec );

            //SCRequestImageSettings(ci);
            //cr=GraphicsExportSetOutputFileTypeAndCreator( ci, kQTFileTypeJPEG, 'ogle' ); // 'ogle' = PictureViewer
            //cr=GraphicsExportSetCompressionMethod( ci, kJPEGCodecType );
            //cr=GraphicsExportSetCompressionQuality( ci, codecNormalQuality );
            cr=GraphicsExportDoExport( ci, &sizeWritten );
        }
        CloseComponent( ci );
    }

    NavCompleteSave(&reply, NULL);
}

void MakeMenu()
{
    Handle		menuBar;
    MenuRef		menu;
    long		response;
    OSErr		err;

    menuBar = GetNewMBar(129);
    if (menuBar != nil)
    {
        SetMenuBar(menuBar);
        err = Gestalt( gestaltSystemVersion, &response );
        if ( (err == noErr) && (response >= 0x00001000) )
        {
            menu = GetMenuHandle( 129 );
            DeleteMenuItem( menu, 2 );
        }
        DrawMenuBar();
    }
    else
    {
        ParamText( "\pCould not create menu bar!", nil, nil, nil);
        StopAlert(128,nil);
        ExitToShell();
    }
}

void DrawWindo( GWorldPtr off )
{
    RGBColor coler;
    double y,x;
    Rect tempRect;
    GDHandle oldGDeviceH;

    GrafPtr savePort2;
    GetGWorld (&savePort2, &oldGDeviceH);

    GetPortBounds(off, &tempRect);

    SetGWorld(off, NULL);
    OSErr err;
    err=LockPixels( GetGWorldPixMap(off) );
    if( !err )
        DieError( "\pCould not lock pixels!" );

    EraseRect( &tempRect );

/*#if __profile__ && DO_PROFILE
	ProfilerSetStatus(true);
#endif*/

    DrawSelectedFractal();
    
/*#if __profile__ && DO_PROFILE
	ProfilerSetStatus(false);
#endif*/
    
    SetControlValue( pb, 100 );

    UnlockPixels( GetGWorldPixMap(off) );

    DrawIntoMain( off );

    SetGWorld (savePort2, oldGDeviceH);
}

void DrawIntoMain(GWorldPtr off)
{
    Rect FDrawRect, eraseRec;
    GrafPtr savePort2;
    RGBColor oldColor;
    
    GetPort (&savePort2);

    SetPortWindowPort(gMainWindow);

    GetWindowPortBounds( gMainWindow, &eraseRec );
    EraseRect( &eraseRec ); //erase gMainWindow

    GetForeColor(&oldColor);
    ForeColor(greenColor);

    MoveTo( 0, SCREENYMAX+1 );
    LineTo( SCREENXMAX+1, SCREENYMAX+1 );
    MoveTo( SCREENXMAX+1, 0 );
    LineTo( SCREENXMAX+1, SCREENYMAX+1 );

    MoveTo( 10, eraseRec.top+SCREENYMAX+20 );
    Str255 tempStr;
    NumToString( complexConstant.getR()*10000, tempStr );
    //float real=complexConstant.getR();
    //ExtendedToString( &real, NULL, NULL, tempStr ); //GetIntlResourceTable
    //SetControlTitle( gEditTextReal, tempStr );
    DrawString( tempStr );

    MoveTo( 10, eraseRec.top+SCREENYMAX+40 );
    NumToString( complexConstant.getI()*10000, tempStr );
    DrawString( tempStr );

    MoveTo( 200, eraseRec.top+SCREENYMAX+20 );
    DrawString( "\pFrame:" );
    MoveTo( 200, eraseRec.top+SCREENYMAX+40 );
    NumToString( frameCount, tempStr );
    DrawString( tempStr );
    
    //Draw Crosshairs:
    /*MoveTo(0, SCREENYMAX/2);
    LineTo(SCREENXMAX, SCREENYMAX/2);
    MoveTo(SCREENXMAX/2, 0);
    LineTo(SCREENXMAX/2, SCREENYMAX);*/

    RGBForeColor(&oldColor);;
    GetPortBounds( off, &FDrawRect );
    CopyBits( GetPortBitMapForCopyBits(off), GetPortBitMapForCopyBits(GetWindowPort(gMainWindow)), &FDrawRect, &FDrawRect, srcCopy, NULL );


    DrawControls(gMainWindow);
    QDFlushPortBuffer( GetWindowPort(gMainWindow), NULL );

    SetPort(savePort2);
}

void DieError( const unsigned char *errString )
{

    ParamText( errString, nil, nil, nil);
    StopAlert(128,nil);
    Cleanup();
}
void DieError( const unsigned char *errString, OSErr err )
{

    Str255 errStr;
    NumToString( err, errStr );
    ParamText( errString, errStr, nil, nil);
    DebugStr(errStr);
    StopAlert(128,nil);
    Cleanup();
}

void CheckMovieError(  )
{
    OSErr err;
    err=GetMoviesError();
    if( err != noErr) DieError( "\pGeneral movies error", err );
}

void StartMakingMovie()
{
    OSErr err;
    ThreadID Tid;
    
    //MaxApplZone();
    
   /* err=NewThread( kCooperativeThread, MovieThread, NULL, 0, kCreateIfNeeded, NULL, &Tid );
    if( err != noErr )
        DieError( "\pCould not create thread!", err );
    err=SetThreadState( Tid, kRunningThreadState, Tid );
    if( err != noErr )
        DieError( "\pCould not run thread!", err );*/
    MovieThread();
    
    //DisposeThread( Tid, NULL, false );
}


voidPtr MovieThread( void )
{
    OSErr err;
    Movie mov;
    Track trk;
    Media videoMedia;
    Rect movieBounds;
    GWorldPtr GW;
    GrafPtr oldPort;
    GDHandle oldGDevice;
    long maxCompressionSize;
    Handle compressedData;
    ImageDescriptionHandle imageDescH;
    TimeValue sampleDuration;

    
    FSSpec documentFSSpec;
    short resRefNum = 0;
    short resId = movieInDataForkResID;
    NavReplyRecord reply;
    NavDialogOptions dialogO;

    err=NavGetDefaultDialogOptions( &dialogO );
    if( err != noErr )
        DieError( "\pCould not load Nav Services Dialog Options!", err );

    GetIndString( dialogO.clientName, 128, 1 );
    GetIndString( dialogO.windowTitle, 128, 2 );
    GetIndString( dialogO.actionButtonLabel, 128, 3 );
    GetIndString( dialogO.cancelButtonLabel, 128, 4 );
    GetIndString( dialogO.savedFileName, 128, 5 );
    GetIndString( dialogO.message, 128, 6 );
    dialogO.preferenceKey=NULL;
    dialogO.popupExtension=NULL;

    err = NavPutFile(NULL, &reply, &dialogO, NULL, kQTFileTypeMovie, 'TVOD', NULL );

    if ( reply.validRecord && err == noErr )
    {
        AEKeyword 	keyWord;
        DescType 	typeCode;
        Size 		actualSize = 0;

        err = AEGetNthPtr( &(reply.selection), 1, typeFSS, &keyWord, &typeCode, &documentFSSpec, sizeof( documentFSSpec ), &actualSize );
        if( err != noErr )
            DieError( "\pAEGetNthPtr error", err );

        if ( !reply.replacing )
        {
            err=FSpCreate( &documentFSSpec, 'TVOD', kQTFileTypeMovie, reply.keyScript );
            if( err != noErr )
                DieError( "\pFSpCreate error", err );
        }

        if( err != noErr )
            DieError( "\pCreateMovieFile error", err );
    }
    else
    {
        return 0;
    }

    
    err=EnterMovies();
    if( err != noErr )
        DieError( "\pCould not load quicktime!", err );

    err = CreateMovieFile (&documentFSSpec, kQTFileTypeMovie, smCurrentScript, createMovieFileDeleteCurFile | createMovieFileDontCreateResFile, &resRefNum, &mov );
    if( err != noErr )
        DieError( "\pCreateMovieFile error", err );

    CheckMovieError();

    NavCompleteSave(&reply, NULL);

    trk=NewMovieTrack (mov, FixRatio(SCREENXMAX,1), FixRatio(SCREENYMAX,1), kNoVolume);
    CheckMovieError();

    videoMedia = NewTrackMedia (trk, VideoMediaType, FPS, nil, 0);
    CheckMovieError();

    BeginMediaEdits(videoMedia);
    CheckMovieError();

    SetRect( &movieBounds, 0, 0, SCREENXMAX, SCREENYMAX );

    short wPixDepth = (**(GetPortPixMap(GetWindowPort(gMainWindow)))).pixelSize;
    err = NewGWorld(&GW, wPixDepth, &movieBounds, nil, nil, 0);
    if( err != noErr )
        DieError( "\pCould not create movie GWorld!", err );

    GetGWorld(&oldPort, &oldGDevice);
    SetGWorld(GW, nil);

    PixMapHandle PM=GetGWorldPixMap(GW);

    CopyBits( GetPortBitMapForCopyBits(offscreen), GetPortBitMapForCopyBits(GW), &movieBounds, &movieBounds, srcCopy, NULL );

    err = GetMaxCompressionSize(PM, &movieBounds, wPixDepth, codecNormalQuality, kAnimationCodecType, anyCodec, &maxCompressionSize);
    if( err != noErr )
        DieError( "\pGetMaxCompressionSize error!", err );
    CheckMovieError();

    compressedData = NewHandle(maxCompressionSize);
    if( MemError() )
        DieError ( "\pMemory error" );
    CheckMovieError();

    if (compressedData == nil)
        DieError ( "\pMemory error" );

    MoveHHi(compressedData);
    CheckMovieError();

    HLock(compressedData);

    imageDescH = (ImageDescriptionHandle)NewHandle(4);

    RGBColor coler;

    double x,y;

    SetPort( GW );
    frameCount=1;
    bool movieDone=false;

    EventRecord evt;
    Point p;
    ControlHandle		control;
    short 			ControlPart;

    SetControlVisibility( stopBtn, true, true );


    while( !movieDone )
    {
        //SystemTask();
        //GetNextEvent( mDownMask, &evt );


        ///Do drawing here!

        EraseRect(&movieBounds);

        complexConstant.setMe( complexConstant.getR() - 0.001 , complexConstant.getI() - 0.001 );

	 if( Button() )
          {
                 movieDone=true;
          }
          
        DrawSelectedFractal();
        
        DrawIntoMain(GW);

        CheckMovieError();

        ///End drawing

        err = CompressImage( PM, &movieBounds, codecNormalQuality,
                             kAnimationCodecType, imageDescH, *compressedData );
        if( err != noErr )
            DieError( "\pCompressImage error!", err );
        CheckMovieError();

        err = AddMediaSample(videoMedia, compressedData, 0, (**imageDescH).dataSize,
                             1, (SampleDescriptionHandle)imageDescH,
                             1, 0, &sampleDuration);
        CheckMovieError();


        // Need to make sure that it's not just spun out of control, filling up the hard disk, and is just generating black frames:
        if( frameCount > 10000 ) break; // I think ten thousand frames... yah.
        if( abs(complexConstant) > 8 ) break; // Just black, just black.

        frameCount++;
        QDFlushPortBuffer( GetWindowPort(gMainWindow), NULL );

    }

    SetControlVisibility( stopBtn, false, true );

    err = InsertMediaIntoTrack(trk, 0, 0,
                               GetMediaDuration(videoMedia), fixed1);

    if(GetMoviesError() != noErr) DieError( "\pInsertMediaIntoTrack error" );

    err = EndMediaEdits(videoMedia);
    if(GetMoviesError() != noErr) DieError( "\pEndMediaEdits error" );

    SetGWorld(oldPort, oldGDevice);

    HUnlock(compressedData);

    if (GW)
    {
        DisposeGWorld(GW);
    }
    if (compressedData)
    {
        DisposeHandle(compressedData);
    }
    if (imageDescH)
    {
        DisposeHandle((Handle)imageDescH);
    }

    //save movie:


    err = AddMovieResource(mov, resRefNum, &resId, "\pFractal Movie");
    if( err != noErr )
        DieError( "\pCould not create movie resource!", err );

    if (resRefNum)
    {
        CloseMovieFile (resRefNum);
    }

    DisposeMovie( mov );
    return 0;
}
