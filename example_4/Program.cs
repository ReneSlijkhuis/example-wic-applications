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
using Microsoft.Test.Tools.WicCop.InteropServices;
using Microsoft.Test.Tools.WicCop.InteropServices.ComTypes;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace WicClient
{
    class Program
    {
        static void Main(string[] args)
        {
            IWICImagingFactory factory = (IWICImagingFactory)new WICImagingFactory();
            IWICBitmapDecoder decoder = null;
            IWICBitmapFrameDecode frame = null;
            IWICFormatConverter formatConverter = null;

            try
            {
                const string filename = @"<filename>";

                decoder = factory.CreateDecoderFromFilename(filename, null, NativeMethods.GenericAccessRights.GENERIC_READ,
                    WICDecodeOptions.WICDecodeMetadataCacheOnDemand);

                uint count = decoder.GetFrameCount();

                frame = decoder.GetFrame(0);

                uint width = 0;
                uint height = 0;
                frame.GetSize(out width, out height);

                Guid pixelFormat;
                frame.GetPixelFormat(out pixelFormat);

                // The frame can use many different pixel formats.
                // You can copy the raw pixel values by calling "frame.CopyPixels( )".
                // This method needs a buffer that can hold all bytes.
                // The total number of bytes is: width x height x bytes per pixel

                // The disadvantage of this solution is that you have to deal with all possible pixel formats.

                // You can make your life easy by converting the frame to a pixel format of 
                // your choice. The code below shows how to convert the pixel format to 24-bit RGB.

                formatConverter = factory.CreateFormatConverter();

                formatConverter.Initialize(frame,
                                           Consts.GUID_WICPixelFormat24bppRGB,
                                           WICBitmapDitherType.WICBitmapDitherTypeNone,
                                           null,
                                           0.0,
                                           WICBitmapPaletteType.WICBitmapPaletteTypeCustom);

                uint bytesPerPixel = 3; // Because we have converted the frame to 24-bit RGB
                uint stride = ((Utilities.GetBitPerPixel(pixelFormat) * width + 7) / 8) * bytesPerPixel;
                byte[] bytes = new byte[stride * height];

                formatConverter.CopyPixels(null, stride, stride * height, bytes);
            }
            finally
            {
                frame.ReleaseComObject();
                decoder.ReleaseComObject();
                factory.ReleaseComObject();
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////