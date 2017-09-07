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

using System;
using stakx.WIC;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace WicClient
{
    class Program
    {
        // The GUID definition below should be part of the 'stakx.WIC' package.
        private static readonly Guid GUID_WICPixelFormat24bppRGB = new Guid(0x6fddc324, 0x4e03, 0x4bfe, 0xb1, 0x85, 0x3d, 0x77, 0x76, 0x8d, 0xc9, 0x0d);

        static void Main(string[] args)
        {
            // This example requires the NuGet package 'stakx.WIC'.
            // https://www.nuget.org/packages/stakx.WIC/
            // https://github.com/stakx/WIC

            const string filename = @"<filename>";

            IWICImagingFactory factory = new WICImagingFactory();
            IWICBitmapDecoder decoder = null;
            IWICBitmapFrameDecode frame = null;
            IWICFormatConverter formatConverter = null;

            decoder = factory.CreateDecoderFromFilename(filename, IntPtr.Zero, StreamAccessMode.GENERIC_READ,
                WICDecodeOptions.WICDecodeMetadataCacheOnDemand);

            int count = decoder.GetFrameCount();

            frame = decoder.GetFrame(0);

            int width = 0;
            int height = 0;
            frame.GetSize(out width, out height);

            Guid pixelFormat = frame.GetPixelFormat();

            // The frame can use many different pixel formats.
            // You can copy the raw pixel values by calling "frame.CopyPixels( )".
            // This method needs a buffer that can hold all bytes.
            // The total number of bytes is: width x height x bytes per pixel

            // The disadvantage of this solution is that you have to deal with all possible pixel formats.

            // You can make your life easy by converting the frame to a pixel format of
            // your choice. The code below shows how to convert the pixel format to 24-bit RGB.

            formatConverter = factory.CreateFormatConverter();

            formatConverter.Initialize(frame,
                                        GUID_WICPixelFormat24bppRGB,
                                        WICBitmapDitherType.WICBitmapDitherTypeNone,
                                        null,
                                        0.0,
                                        WICBitmapPaletteType.WICBitmapPaletteTypeCustom);

            int bytesPerPixel = 3; // Because we have converted the frame to 24-bit RGB
            int stride = width * bytesPerPixel;
            byte[] bytes = new byte[stride * height];

            formatConverter.CopyPixels(IntPtr.Zero, stride, stride * height, bytes);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////