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
        CComPtr<IWICFormatConverter> pFormatConverter;

        CoCreateInstance( CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&pFactory );

        pFactory->CreateDecoderFromFilename( filename.c_str( ), NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder );
        // Check the return value to see if:
        //  - The specified file exists and can be read.
        //  - A decoder is found for this file format. 

        UINT frameCount = 0;
        pDecoder->GetFrameCount( &frameCount );
 
        pDecoder->GetFrame( 0, &pFrame );
        // The zero-based index should be smaller than the frame count.

        UINT width = 0;
        UINT height = 0;
        pFrame->GetSize( &width, &height );

        WICPixelFormatGUID pixelFormatGUID;
        pFrame->GetPixelFormat( &pixelFormatGUID );
        
        // The frame can use many different pixel formats.
        // You can copy the raw pixel values by calling "pFrame->CopyPixels( )".
        // This method needs a buffer that can hold all bytes.
        // The total number of bytes is: width x height x bytes per pixel

        // The disadvantage of this solution is that you have to deal with all possible pixel formats.

        // You can make your life easy by converting the frame to a pixel format of 
        // your choice. The code below shows how to convert the pixel format to 24-bit RGB.
        
        pFactory->CreateFormatConverter( &pFormatConverter );

        pFormatConverter->Initialize( pFrame,                       // Input bitmap to convert
                                      GUID_WICPixelFormat24bppRGB,  // Destination pixel format
                                      WICBitmapDitherTypeNone,      // Specified dither pattern
                                      nullptr,                      // Specify a particular palette 
                                      0.f,                          // Alpha threshold
                                      WICBitmapPaletteTypeCustom ); // Palette translation type

        UINT bytesPerPixel = 3; // Because we have converted the frame to 24-bit RGB
        UINT stride = width * bytesPerPixel;
        UINT size = width * height * bytesPerPixel; // The size of the required memory buffer for 
                                                    // holding all the bytes of the frame.

        vector<BYTE> bitmap( size ); // The buffer to hold all the bytes of the frame.
        pFormatConverter->CopyPixels( NULL, stride, size, bitmap.data( ) );

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