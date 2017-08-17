//----------------------------------------------------------------------------------------
// THIS CODE AND INFORMATION IS PROVIDED "AS-IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//----------------------------------------------------------------------------------------

using System;
using System.Runtime.InteropServices;

using Microsoft.Test.Tools.WicCop.InteropServices.ComTypes;

namespace Microsoft.Test.Tools.WicCop.InteropServices
{
    static class Utilities
    {
        public static void ReleaseComObject(this object o)
        {
            if (o == null) return;
            if (o.GetType().IsArray) { foreach (object i in o as Array) i.ReleaseComObject(); }
            else Marshal.ReleaseComObject(o);
        }

        public static uint GetBitPerPixel(Guid pixelFormat)
        {
            IWICImagingFactory factory = (IWICImagingFactory)new WICImagingFactory();
            IWICPixelFormatInfo info = null;
            try
            {
                info = (IWICPixelFormatInfo)factory.CreateComponentInfo(pixelFormat);

                return info.GetBitsPerPixel();
            }
            finally
            {
                factory.ReleaseComObject();
                info.ReleaseComObject();
            }
        }
    }
}
