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

#include <vector>
#include <atlbase.h>
#include <initguid.h>
#include <Wincodec.h>

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Include header:      Wincodec.h
// Add to linker input: Windowscodecs.lib
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// GUID of the Lisa image format (this should be known by applications that want to write this format).
// {91DFBD70-3D2C-440F-B297-1E2097D4A833}
DEFINE_GUID(GUID_LisaContainerFormat, 0x91dfbd70, 0x3d2c, 0x440f, 0xb2, 0x97, 0x1e, 0x20, 0x97, 0xd4, 0xa8, 0x33);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SaveAsLisa( const wstring& filename )
{
    CoInitialize( nullptr );
    {
        CComPtr<IWICImagingFactory> pFactory;
        CComPtr<IWICBitmapDecoder> pDecoder;
        CComPtr<IWICBitmapEncoder> pEncoder;
        CComPtr<IWICBitmapFrameDecode> pFrameDecode;
        CComPtr<IWICBitmapFrameEncode> pFrameEncode;
        CComPtr<IWICFormatConverter> pFormatConverter;
        CComPtr<IWICStream> pFileStream;
        CComPtr<IPropertyBag2> pPropertyBag;

        CoCreateInstance( CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&pFactory );

        // Read the specified file by using an installed WIC codec.

        pFactory->CreateDecoderFromFilename( filename.c_str( ), NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder );
        // Check the return value to see if:
        //  - The specified file exists and can be read.
        //  - A decoder is found for this file format.

        UINT frameCount = 0;
        pDecoder->GetFrameCount( &frameCount );

        pDecoder->GetFrame( 0, &pFrameDecode );

        UINT width = 0;
        UINT height = 0;
        pFrameDecode->GetSize( &width, &height );

        WICPixelFormatGUID pixelFormatSource;
        pFrameDecode->GetPixelFormat( &pixelFormatSource );

        WICPixelFormatGUID pixelFormatDestination = GUID_WICPixelFormat24bppRGB;

        // The frame can use many different pixel formats.
        // You can copy the raw pixel values by calling "pFrame->CopyPixels( )".
        // This method needs a buffer that can hold all bytes.
        // The total number of bytes is: width x height x bytes per pixel

        // The disadvantage of this solution is that you have to deal with all possible pixel formats.

        // You can make your life easy by converting the frame to a pixel format of
        // your choice. The code below shows how to convert the pixel format to 24-bit RGB.

        pFactory->CreateFormatConverter( &pFormatConverter );

        pFormatConverter->Initialize( pFrameDecode,                 // Input bitmap to convert
                                      pixelFormatDestination,       // Destination pixel format
                                      WICBitmapDitherTypeNone,      // Specified dither pattern
                                      nullptr,                      // Specify a particular palette
                                      0.f,                          // Alpha threshold
                                      WICBitmapPaletteTypeCustom ); // Palette translation type

        UINT bytesPerPixel = 3; // Because we have converted the frame to 24-bit RGB
        UINT stride = width * bytesPerPixel;
        UINT size = width * height * bytesPerPixel; // The size of the required memory buffer for
                                                    // holding all the bytes of the frame.

        vector<BYTE> bytes( size ); // The buffer to hold all the bytes of the frame.
        pFormatConverter->CopyPixels( NULL, stride, size, bytes.data( ) );

        // Save the image in the Lisa format.
        // The Lisa WIC codec should be installed (see the link below).
        // https://github.com/ReneSlijkhuis/example-wic-codec

        pFactory->CreateEncoder( GUID_LisaContainerFormat, NULL, &pEncoder );
        // Check the return value to see if an encoder is found for the specified file format.

        pFactory->CreateStream( &pFileStream );

        wstring output = filename + L".lisa";
        pFileStream->InitializeFromFilename( output.c_str( ), GENERIC_WRITE );

        pEncoder->Initialize( pFileStream, WICBitmapEncoderNoCache );

        pEncoder->CreateNewFrame( &pFrameEncode, &pPropertyBag );

        pFrameEncode->SetPixelFormat( &pixelFormatDestination );

        pFrameEncode->SetSize( width, height );

        pFrameEncode->WritePixels( height, stride, size, bytes.data( ) );

        pFrameEncode->Commit( );
        pEncoder->Commit( );

        // Note: the WIC COM pointers should be released before 'CoUninitialize( )' is called.
    }
    CoUninitialize( );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main( )
{
    SaveAsLisa( L"<filename>" );

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////