/*
	File:		iTunesVisualSample.c

    Contains:   iTunes Visual Plug-ins sample code
 
    Version:    Technology: iTunes
                Release:    4.1

	Copyright: 	© Copyright 2003 Apple Computer, Inc. All rights reserved.
	
	Disclaimer:	IMPORTANT:  This Apple software is supplied to you by Apple Computer, Inc.
				("Apple") in consideration of your agreement to the following terms, and your
				use, installation, modification or redistribution of this Apple software
				constitutes acceptance of these terms.  If you do not agree with these terms,
				please do not use, install, modify or redistribute this Apple software.

				In consideration of your agreement to abide by the following terms, and subject
				to these terms, Apple grants you a personal, non-exclusive license, under Apple’s
				copyrights in this original Apple software (the "Apple Software"), to use,
				reproduce, modify and redistribute the Apple Software, with or without
				modifications, in source and/or binary forms; provided that if you redistribute
				the Apple Software in its entirety and without modifications, you must retain
				this notice and the following text and disclaimers in all such redistributions of
				the Apple Software.  Neither the name, trademarks, service marks or logos of
				Apple Computer, Inc. may be used to endorse or promote products derived from the
				Apple Software without specific prior written permission from Apple.  Except as
				expressly stated in this notice, no other rights or licenses, express or implied,
				are granted by Apple herein, including but not limited to any patent rights that
				may be infringed by your derivative works or by other works in which the Apple
				Software may be incorporated.

				The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
				WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
				WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
				PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
				COMBINATION WITH YOUR PRODUCTS.

				IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
				CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
				GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
				ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION
				OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT, TORT
				(INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN
				ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/

*/

// Windows headers
#include <windows.h>
//#include "malloc.h"
//#include <QTML.h> 
#include "shlobj.h"
//#include "iTunesCOMInterface.h"
#include "iTunesVisualAPI.h"
#include <stdio.h>
#include "SnarlInterfaceC.h"



#define GRAPHICS_DEVICE	HWND
#define	MAIN iTunesPluginMain
#define IMPEXP	__declspec(dllexport)
#define kSampleVisualPluginName		"Snarl Plugin"
#define	kSampleVisualPluginCreator	'\?\?\?\?'
#define	kSampleVisualPluginMajorVersion		1
#define	kSampleVisualPluginMinorVersion		0
#define	kSampleVisualPluginReleaseStage		0x80
#define	kSampleVisualPluginNonFinalRelease	0

struct VisualPluginData {
	void *				appCookie;
	ITAppProcPtr		appProc;
	GRAPHICS_DEVICE		destPort;
	Rect				destRect;
	OptionBits			destOptions;
	UInt32				destBitDepth;
	RenderVisualData	renderData;
	UInt32				renderTimeStampID;
	SInt8				waveformData[kVisualMaxDataChannels][kVisualNumWaveformEntries];
	UInt8				level[kVisualMaxDataChannels];		/* 0-128 */
	ITTrackInfoV1		trackInfo;
	ITStreamInfoV1		streamInfo;
	Boolean				playing;
	Boolean				padding[3];
};
enum
{
	kSettingsDialogResID	= 1000,
	kSettingsDialogOKButton	= 1,
	kSettingsDialogCancelButton,
	kSettingsDialogCheckBox1,
	kSettingsDialogCheckBox2,
	kSettingsDialogCheckBox3
};


typedef struct VisualPluginData VisualPluginData;

LONG32 SNARL_GLOBAL_MESSAGE = 0;
HWND iTunesSnarlWindow;
static char imagePath[_MAX_PATH];

// ClearMemory
//
static void ClearMemory (LogicalAddress dest, SInt32 length)
{
	register unsigned char	*ptr;

	ptr = (unsigned char *) dest;
	
	if( length > 16 )
	{
		register unsigned long	*longPtr;
		
		while( ((unsigned long) ptr & 3) != 0 )
		{
			*ptr++ = 0;
			--length;
		}
		
		longPtr = (unsigned long *) ptr;
		
		while( length >= 4 )
		{
			*longPtr++ 	= 0;
			length		-= 4;
		}
		
		ptr = (unsigned char *) longPtr;
	}
	
	while( --length >= 0 )
	{
		*ptr++ = 0;
	}
}

/*
	AllocateVisualData is where you should allocate any information that depends
	on the port or rect changing (like offscreen GWorlds).
*/
static OSStatus AllocateVisualData (VisualPluginData *visualPluginData, const Rect *destRect)
{
	OSStatus		status;

	(void) visualPluginData;
	(void) destRect;

	status = noErr;
	return status;
}


/*
	DeallocateVisualData is where you should deallocate the things you have allocated
*/
static void DeallocateVisualData (VisualPluginData *visualPluginData)
{
		(void)visualPluginData;
}

static Boolean RectanglesEqual(const Rect *r1, const Rect *r2)
{
	//if (
	//	(r1->left == r2->left) &&
	//	(r1->top == r2->top) &&
	//	(r1->right == r2->right) &&
	//	(r1->bottom == r2->bottom)
	//	)
	//	return true;
	//return false;
	
}

static LRESULT CALLBACK iTunesSnarl_message_window_handler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	if(msg == SNARL_GLOBAL_MESSAGE) {
		if ((int)wparam == SNARL_LAUNCHED) {
//			char logoPath[1024] = "";

//			getLogoPath(logoPath);

		/*	snRegisterConfig2(pidginSnarlWindow,"PidginSnarl",0,logoPath);
			snRegisterAlert("PidginSnarl","Buddy logs on",TRUE);
			snRegisterAlert("PidginSnarl","Buddy logs on at startup",FALSE);
			snRegisterAlert("PidginSnarl","Buddy logs off",TRUE);
			snRegisterAlert("PidginSnarl","Incoming messages",TRUE);
			snRegisterAlert("PidginSnarl","Incoming messages on active window",FALSE);
			snRegisterAlert("PidginSnarl","Greeting",TRUE);
			snRegisterAlert("PidginSnarl","New email detailed",TRUE);
			snRegisterAlert("PidginSnarl","New email summarized if detailed not supported",TRUE);
			snRegisterAlert("PidginSnarl","New email summarized always",TRUE);
			snRegisterAlert("PidginSnarl","Connection error",TRUE);
			snRegisterAlert("PidginSnarl","Authorization requested",TRUE);

			snRegisterAlert("PidginSnarl","Buddy starts writing",FALSE);
			snRegisterAlert("PidginSnarl","Buddy changed status",FALSE);
			snRegisterAlert("PidginSnarl","Buddy changed status at startup",FALSE);
			snRegisterAlert("PidginSnarl","Buddy changed icon",FALSE);
			snRegisterAlert("PidginSnarl","Buddy changed icon at startup",FALSE);*/
		}
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

static HWND generateSnarlMessageWindow() {
	// http://msdn.microsoft.com/en-us/library/ms632599(VS.85).aspx#message_only

	HWND win_hwnd;
	WNDCLASSEX wcx;
	LPCTSTR wname;

	wname = TEXT("iTunesSnarl");

	wcx.cbSize = sizeof(wcx);
	wcx.style = 0;
	wcx.lpfnWndProc = iTunesSnarl_message_window_handler;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = 0;
	wcx.hIcon = NULL;
	wcx.hCursor = NULL;
	wcx.hbrBackground = NULL;
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = wname;
	wcx.hIconSm = NULL;

	RegisterClassEx(&wcx);

	SNARL_GLOBAL_MESSAGE = snGetGlobalMsg();

	/* Create the window */
	if(!(win_hwnd = CreateWindow("iTunesSnarl", TEXT("iTunesSnarl"), 0, 0, 0, 0, 0,
		0, NULL, 0, 0))) {
//			purple_debug_error("winpidgin", "iTunesSnarl: Unable to create message window.\n");
			return NULL;
	}
	return win_hwnd;
}

/*
VisualPluginHandler
*/
static OSStatus VisualPluginHandler (OSType message, VisualPluginMessageInfo *messageInfo, void *refCon)
{
	static char programFilesPath[50];
	char iTunesPath[33] = "\\iTunes\\Plug-Ins\\iTunesSnarl";
	char artist[255];
	char title[255];
	char length;
	OSStatus status;
	void* albumCoverArtworkImageData = NULL;
	BYTE* pDest = NULL;


	int artSize = 0;
	int i = 0;
	int temp = 0;
	
	VisualPluginData *	visualPluginData;

	visualPluginData = (VisualPluginData *)refCon;

	status = noErr;

	switch (message)
	{
		/*
		Sent when the visual plugin is registered.  The plugin should do minimal
		memory allocations here.  The resource fork of the plugin is still available.
		*/		
		case kVisualPluginInitMessage:
		{
			visualPluginData = (VisualPluginData *)malloc(sizeof(VisualPluginData));
			if (visualPluginData == nil)
			{
				status = memFullErr;
				break;
			}

			visualPluginData->appCookie	= messageInfo->u.initMessage.appCookie;
			visualPluginData->appProc	= messageInfo->u.initMessage.appProc;

			iTunesSnarlWindow = generateSnarlMessageWindow();

			SHGetSpecialFolderPath(0,programFilesPath, CSIDL_PROGRAM_FILES,false);
			sprintf(imagePath, "%s%s\\icon.png", programFilesPath, iTunesPath);
			
			snRegisterConfig2(iTunesSnarlWindow, "iTunesSnarl",0, imagePath);
			snRegisterAlert("iTunesSnarl", "Update on new song", TRUE);
		
			///* Remember the file spec of our plugin file. We need this so we can open our resource fork during */
			///* the configuration message */

			messageInfo->u.initMessage.refCon	= (void *)visualPluginData;
			break;
		}
			
		/*
			Sent when the visual plugin is unloaded
		*/		
		case kVisualPluginCleanupMessage:
			if (visualPluginData != nil)
				free(visualPluginData);

			snRevokeConfig(iTunesSnarlWindow);
			break;
			
		/*
			Sent when the visual plugin is enabled.  iTunes currently enables all
			loaded visual plugins.  The plugin should not do anything here.
		*/
		case kVisualPluginEnableMessage:
		case kVisualPluginDisableMessage:
			break;

		/*
			Sent if the plugin requests idle messages.  Do this by setting the kVisualWantsIdleMessages
			option in the PlayerRegisterVisualPluginMessage.options field.
		*/
		case kVisualPluginIdleMessage:
			break;

		/*
			Sent when iTunes is going to show the visual plugin in a port.  At
			this point, the plugin should allocate any large buffers it needs.
		*/
		case kVisualPluginShowWindowMessage:
			break;
			
		/*
			Sent when iTunes is no longer displayed.
		*/
		case kVisualPluginHideWindowMessage:
			ClearMemory(&visualPluginData->trackInfo, sizeof(visualPluginData->trackInfo));
			ClearMemory(&visualPluginData->streamInfo, sizeof(visualPluginData->streamInfo));
			break;
		
		/*
			Sent when iTunes needs to change the port or rectangle of the currently
			displayed visual.
		*/
		case kVisualPluginSetWindowMessage:
			visualPluginData->destOptions = messageInfo->u.setWindowMessage.options;
			break;
		
		/*
			Sent for the visual plugin to render a frame.
		*/
		case kVisualPluginRenderMessage:
			visualPluginData->renderTimeStampID	= messageInfo->u.renderMessage.timeStampID;
			break;
			
		/*
			Sent in response to an update event.  The visual plugin should update
			into its remembered port.  This will only be sent if the plugin has been
			previously given a ShowWindow message.
		*/	
		case kVisualPluginUpdateMessage:
			break;
		
		/*
			Sent when the player starts.
		*/
		case kVisualPluginPlayMessage:
			
			memset(artist, '\0', sizeof(artist));
			memset(title, '\0', sizeof(title));

			if (messageInfo->u.playMessage.trackInfo != nil)
			{			 
				length = messageInfo->u.playMessage.trackInfo->artist[0];
				for (i=1;i<length+1 && i<255;i++)
				{
					artist[i-1] = messageInfo->u.playMessage.trackInfo->artist[i];
				}
				temp = length;
				length = messageInfo->u.playMessage.trackInfo->album[i];
				artist[temp] = '\n';
				temp++;
				for (i=1;i<length+1 && i<255;i++)
				{
					artist[temp+i-1] = messageInfo->u.playMessage.trackInfo->album[i];
				}

				length = messageInfo->u.playMessage.trackInfo->name[0];
				for (i=1;i<length+1 && i<255;i++)
				{
					title[i-1] = messageInfo->u.playMessage.trackInfo->name[i];
				}
				

			}
			else
			{
				sprintf(artist, "%s", "no artist info");
				sprintf(title, "%s", "no title info");
			}
			snShowMessageEx("Update on new song", title, artist, 5, imagePath, hWndReply, (WPARAM)"","");


			if (messageInfo->u.playMessage.trackInfo != nil)
				visualPluginData->trackInfo = *messageInfo->u.playMessage.trackInfo;
			else
				ClearMemory(&visualPluginData->trackInfo, sizeof(visualPluginData->trackInfo));

			if (messageInfo->u.playMessage.streamInfo != nil)
				visualPluginData->streamInfo = *messageInfo->u.playMessage.streamInfo;
			else
				ClearMemory(&visualPluginData->streamInfo, sizeof(visualPluginData->streamInfo));
		
			visualPluginData->playing = true;
			break;

		/*
			Sent when the player changes the current track information.  This
			is used when the information about a track changes, or when the CD
			moves onto the next track.  The visual plugin should update any displayed
			information about the currently playing song.
		*/
		case kVisualPluginChangeTrackMessage:
			
			memset(artist, '\0', sizeof(artist));
			memset(title, '\0', sizeof(title));

			if (messageInfo->u.playMessage.trackInfo != nil)
			{			 
				length = messageInfo->u.playMessage.trackInfo->artist[0];
				for (i=1;i<length+1 && i<255;i++)
				{
					artist[i-1] = messageInfo->u.playMessage.trackInfo->artist[i];
				}
				temp = length;
				length = messageInfo->u.playMessage.trackInfo->album[i];
				artist[temp] = '\n';
				temp++;
				for (i=1;i<length+1 && i<255;i++)
				{
					artist[temp+i-1] = messageInfo->u.playMessage.trackInfo->album[i];
				}

				length = messageInfo->u.playMessage.trackInfo->name[0];
				for (i=1;i<length+1 && i<255;i++)
				{
					title[i-1] = messageInfo->u.playMessage.trackInfo->name[i];
				}

			}
			else
			{
				sprintf(artist, "%s", "no artist info");
				sprintf(title, "%s", "no title info");
			}
			snShowMessageEx("Update on new song", title, artist, 5, imagePath, hWndReply, (WPARAM)"","");

			if (messageInfo->u.changeTrackMessage.trackInfo != nil)
				visualPluginData->trackInfo = *messageInfo->u.changeTrackMessage.trackInfo;
			else
				ClearMemory(&visualPluginData->trackInfo, sizeof(visualPluginData->trackInfo));

			if (messageInfo->u.changeTrackMessage.streamInfo != nil)
				visualPluginData->streamInfo = *messageInfo->u.changeTrackMessage.streamInfo;
			else
				ClearMemory(&visualPluginData->streamInfo, sizeof(visualPluginData->streamInfo));

			break;

		/*
			Sent when the player stops.
		*/
		case kVisualPluginStopMessage:
			visualPluginData->playing = false;
			break;
		
		/*
			Sent when the player changes the track position.
		*/
		case kVisualPluginSetPositionMessage:
			break;

		/*
			Sent when the player pauses.  iTunes does not currently use pause or unpause.
			A pause in iTunes is handled by stopping and remembering the position.
		*/
		case kVisualPluginPauseMessage:
			visualPluginData->playing = false;
			break;
			
		/*
			Sent when the player unpauses.  iTunes does not currently use pause or unpause.
			A pause in iTunes is handled by stopping and remembering the position.
		*/
		case kVisualPluginUnpauseMessage:
			visualPluginData->playing = true;
			break;
		
		/*
			Sent to the plugin in response to a MacOS event.  The plugin should return noErr
			for any event it handles completely, or an error (unimpErr) if iTunes should handle it.
		*/
		case kVisualPluginEventMessage:
			break;

		default:
			status = unimpErr;
			break;
	}

	return status;	
}

//int getCurrentTrackCoverArt(void** albumCoverArtworkImageData, VisualPluginData* visualPluginData) {
//
////	PlayerGetCurrentTrackCoverArtMessage mess;
////	OSType albumCoverArtworkFileType;
////	Handle coverArtHandle = NULL;
//	int numberOfBytes = 0;
//	OSStatus			status;
//	LPVOID pointer;
//	char numberStr [ FILENAME_MAX ];
//	char formatStr [ FILENAME_MAX ];
//	OSType albumCoverArtworkFileType;
//	Handle coverArtHandle = NULL;
//	unsigned char byte0;
//	unsigned char byte1;
//	unsigned char byte2;
//	unsigned char byte3;
//
//
//	status = PlayerGetCurrentTrackCoverArt(visualPluginData->appCookie, visualPluginData->appProc, &coverArtHandle, &albumCoverArtworkFileType); 
//	if (status == noErr && coverArtHandle != NULL) {
//		HLock((Handle)coverArtHandle);
//		numberOfBytes = GetHandleSize(coverArtHandle); // win: malloc.h: _msize(aHandle) (?)
//		HUnlock((Handle)coverArtHandle);
//
////		pointer = LocalLock(coverArtHandle);
////		if (pointer != NULL) {
////			numberOfBytes = HeapSize(GetProcessHeap(), 0, coverArtHandle);
//////			numberOfBytes = _msize((Handle)coverArtHandle); // win: malloc.h: _msize(aHandle) (?)			
////		}
////		LocalUnlock(coverArtHandle);
//
//		if (numberOfBytes > 0) {
//			*albumCoverArtworkImageData = *coverArtHandle;
//			
//			byte0 = albumCoverArtworkFileType & 0xFF;
//			byte1 = (albumCoverArtworkFileType >> 8) & 0xFF;
//			byte2 = (albumCoverArtworkFileType >> 16)  & 0xFF;
//			byte3 = (albumCoverArtworkFileType >> 24) & 0xFF;
//
//			sprintf(formatStr, "%c%c%c%c", byte3,byte2,byte1,byte0);
//			sprintf(numberStr, "%i", numberOfBytes);
//			snShowMessage(formatStr, numberStr, 5, imagePath, hWndReply,(WPARAM)"");
//		}
//
//		//if (coverArtHandle) {
//		//	theVisualHostCommunication->coverArtImageDataHandle = coverArtHandle;
//		//}
//	}
//	return numberOfBytes;
//
//	
//	//if (status != noErr)
//	//{
//	//	/*char dest[_MAX_PATH];
//	//	FILE *pFile;
//	//	pFile = static_cast<FILE*>(mess.coverArt);
//	//	fread(dest, 1, _MAX_PATH,pFile);
//	//	fwrite(mess.coverArt, 1, sizeof(mess.coverArt), fp);*/
//	//	//					fputs(size, fp);
//	////	fclose(fp);
//	//}
//}

/*
	RegisterVisualPlugin
*/
static OSStatus RegisterVisualPlugin (PluginMessageInfo *messageInfo)
{
	OSStatus			status;
	PlayerMessageInfo	playerMessageInfo;
	
	ClearMemory(&playerMessageInfo.u.registerVisualPluginMessage,sizeof(playerMessageInfo.u.registerVisualPluginMessage));
	
	// copy in name length byte first
	playerMessageInfo.u.registerVisualPluginMessage.name[0] = lstrlen(kSampleVisualPluginName);
	// now copy in actual name
	memcpy(&playerMessageInfo.u.registerVisualPluginMessage.name[1], kSampleVisualPluginName, lstrlen(kSampleVisualPluginName));

	SetNumVersion(&playerMessageInfo.u.registerVisualPluginMessage.pluginVersion, kSampleVisualPluginMajorVersion, kSampleVisualPluginMinorVersion, kSampleVisualPluginReleaseStage, kSampleVisualPluginNonFinalRelease);

	playerMessageInfo.u.registerVisualPluginMessage.options					= kVisualWantsIdleMessages | kVisualWantsConfigure;
	playerMessageInfo.u.registerVisualPluginMessage.handler					= VisualPluginHandler;
	playerMessageInfo.u.registerVisualPluginMessage.registerRefCon			= 0;
	playerMessageInfo.u.registerVisualPluginMessage.creator					= kSampleVisualPluginCreator;
	
	playerMessageInfo.u.registerVisualPluginMessage.timeBetweenDataInMS		= 1000; // 16 milliseconds = 1 Tick, 0xFFFFFFFF = Often as possible.
	playerMessageInfo.u.registerVisualPluginMessage.numWaveformChannels		= 2;
	playerMessageInfo.u.registerVisualPluginMessage.numSpectrumChannels		= 2;
	
	playerMessageInfo.u.registerVisualPluginMessage.minWidth				= 64;
	playerMessageInfo.u.registerVisualPluginMessage.minHeight				= 64;
	playerMessageInfo.u.registerVisualPluginMessage.maxWidth				= 32767;
	playerMessageInfo.u.registerVisualPluginMessage.maxHeight				= 32767;
	playerMessageInfo.u.registerVisualPluginMessage.minFullScreenBitDepth	= 0;
	playerMessageInfo.u.registerVisualPluginMessage.maxFullScreenBitDepth	= 0;
	playerMessageInfo.u.registerVisualPluginMessage.windowAlignmentInBytes	= 0;
	
	status = PlayerRegisterVisualPlugin(messageInfo->u.initMessage.appCookie, messageInfo->u.initMessage.appProc,&playerMessageInfo);	

	return status;
	
}

/*
	MAIN
*/
IMPEXP OSStatus MAIN (OSType message, PluginMessageInfo *messageInfo, void *refCon)
{
	OSStatus		status;
	
	(void) refCon;

	switch (message)
	{
		case kPluginInitMessage:
			status = RegisterVisualPlugin(messageInfo);
			break;
			
		case kPluginCleanupMessage:
			status = noErr;
			break;
			
		default:
			status = unimpErr;
			break;
	}
	
	return status;
}
