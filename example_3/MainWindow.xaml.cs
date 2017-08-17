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

using System.IO;
using System.Windows;
using System.Windows.Media.Imaging;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace WicClient
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();

            image.Source = LoadImage(@"<filename>");
        }

        private BitmapSource LoadImage(string filename, int frameIndex = 0)
        {
            using (var inFile = File.OpenRead(filename))
            {
                var decoder = BitmapDecoder.Create(inFile, BitmapCreateOptions.None, BitmapCacheOption.None);
                return Convert(decoder.Frames[frameIndex]);
            }
        }

        private BitmapSource Convert(BitmapFrame frame)
        {
            int stride = frame.PixelWidth * (frame.Format.BitsPerPixel / 8);
            byte[] pixels = new byte[frame.PixelHeight * stride];

            frame.CopyPixels(pixels, stride, 0);

            var bmpSource = BitmapSource.Create(frame.PixelWidth, frame.PixelHeight,
                frame.DpiX, frame.DpiY, frame.Format, frame.Palette, pixels, stride);

            return bmpSource;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
