/*
	File:		artwork.cpp

    Contains:   Code to retrieve current played song artwork from iTunes via COM interface
 
    Technology: iTunes

	Author:		Jernej Virag,
				27. 6. 2009

*/

#include "artwork.h"

inline BSTR ConvertStringToBSTR(const char* pSrc)
{
    if(!pSrc) return NULL;

    DWORD cwch;

    BSTR wsOut(NULL);

    if(cwch = ::MultiByteToWideChar(CP_ACP, 0, pSrc, 
         -1, NULL, 0))//get size minus NULL terminator
    {
                cwch--;
            wsOut = ::SysAllocStringLen(NULL, cwch);

        if(wsOut)
        {
            if(!::MultiByteToWideChar(CP_ACP, 
                     0, pSrc, -1, wsOut, cwch))
            {
                if(ERROR_INSUFFICIENT_BUFFER == ::GetLastError())
                    return wsOut;
                ::SysFreeString(wsOut);//must clean up
                wsOut = NULL;
            }
		}
 
    };

    return wsOut;
};

extern "C" boolean SaveCurrentArtwork(char* outputFileName)
{
	BOOL artworkRetrieved = false;

	const CLSID CLSID_iTunesApp = {0xDC0C2640,0x1415,0x4644,{0x87,0x5C,0x6F,0x4D,0x76,0x98,0x39,0xBA}};
	const IID IID_IiTunes = {0x9DD6680B,0x3EDC,0x40db,{0xA7,0x71,0xE6,0xFE,0x48,0x32,0xE3,0x4A}};

	// iTunes COM interface
	IiTunes *iITunes = NULL;
	IITTrack *iITrack = NULL;
	IITArtworkCollection *iArtworkCollection = NULL;
	IITArtwork *iArtwork = NULL; 
	
	// Number of artworks for track
	long artworkCount = 0;
	
	// Output filename in special format
	BSTR fileName = 0;

	CoInitialize(0);

	HRESULT hRes;
	hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID *) &iITunes);

	if (hRes == S_OK && iITunes)
	{
		// Get current track
		iITunes->get_CurrentTrack(&iITrack);

		if (iITrack)
		{
			// Get current track artwork
			iITrack->get_Artwork(&iArtworkCollection);
			
			if (iArtworkCollection)
			{
				iArtworkCollection->get_Count(&artworkCount);
					
				// Does track have artwork?
				if (artworkCount > 0)
				{
					// Grab first
					iArtworkCollection->get_Item(1, &iArtwork);

					if (iArtwork)
					{
						fileName = ConvertStringToBSTR(outputFileName);

						if (fileName != NULL)
						{
							hRes = iArtwork->SaveArtworkToFile(fileName);

							if (hRes == S_OK)
							{
								artworkRetrieved = true;
							}
						}
						
						::SysFreeString(fileName);
						iArtwork->Release();
					}

				}

				iArtworkCollection->Release();
				
			}
			
			iITrack->Release();
		} 

		iITunes->Release();
	} 

	iITunes = NULL;

	CoUninitialize();

	return artworkRetrieved;
}