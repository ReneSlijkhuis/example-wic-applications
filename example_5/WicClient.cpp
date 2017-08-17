///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    MIT License
//
//    Copyright(c) 2017 René Slijkhuis
//
//    Permission is hereby granted, free of charge, to any person obtaining a copy
//    of this software and associated documentation files (the "Software"), to deal
//    in the Software without restriction, including without limitation the rights
//    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//    copies of the Software, and to permit persons to whom the Software is
//    furnished to do so, subject to the following conditions:
//
//    The above copyright notice and this permission notice shall be included in all
//    copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//    SOFTWARE.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <atlbase.h>
#include <vector>
#include <Wincodec.h>

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Include header:      Wincodec.h
// Add to linker input: Windowscodecs.lib
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WicReadFrame( const wstring& filename )
{
    CoInitialize( nullptr );
    {
        CComPtr<IWICImagingFactory> pFactory;
        CComPtr<IWICBitmapDecoder> pDecoder;
        CComPtr<IWICBitmapFrameDecode> pFrame;
        CComPtr<IWICMetadataQueryReader> pMetadataReader;

        CoCreateInstance( CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&pFactory );

        pFactory->CreateDecoderFromFilename( filename.c_str( ), NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder );
        // Check the return value to see if:
        //  - The specified file exists and can be read.
        //  - A decoder is found for this file format. 

        pDecoder->GetFrame( 0, &pFrame );

        // Get the WIC metadata reader.
        pFrame->GetMetadataQueryReader( &pMetadataReader );

        // Overview of Reading and Writing Image Metadata:
        // https://msdn.microsoft.com/en-us/library/windows/desktop/ee719799(v=vs.85).aspx

        // Native Image Format Metadata Queries:
        // https://msdn.microsoft.com/en-us/library/windows/desktop/ee719904(v=vs.85).aspx

        PROPVARIANT variant;
        PropVariantInit( &variant );
        
        // IFD Metadata - Image width
        pMetadataReader->GetMetadataByName( L"/ifd/{ushort=256}", &variant );
        PropVariantClear( &variant );

        // IFD Metadata - Camera model
        pMetadataReader->GetMetadataByName( L"/ifd/{ushort=272}", &variant );
        PropVariantClear( &variant );

        // IFD Metadata - Date and time
        pMetadataReader->GetMetadataByName( L"/ifd/{ushort=306}", &variant );
        PropVariantClear( &variant );

        // EXIF Metadata - Date and time original
        pMetadataReader->GetMetadataByName( L"/app1/ifd/exif/{ushort=36867}", &variant );
        PropVariantClear( &variant );

        // Note: the WIC COM pointers should be released before 'CoUninitialize( )' is called.
    }    
    CoUninitialize( );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main( )
{
    WicReadFrame( L"<filename>" );
    
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////