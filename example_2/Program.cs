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
using System.Windows.Media.Imaging;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace WicClient
{
    class Program
    {
        static void Main(string[] args)
        {
            ConvertToPng(@"<filename>");
        }

        private static void ConvertToPng(string filename)
        {
            using (var inFile = File.OpenRead(filename))
            {
                var decoder = BitmapDecoder.Create(inFile, BitmapCreateOptions.None, BitmapCacheOption.None);
                var encoder = new PngBitmapEncoder();

                var frame = decoder.Frames[0];
                encoder.Frames.Add(frame);

                using (var outFile = File.OpenWrite(filename + ".png"))
                {
                    encoder.Save(outFile);
                }
            }
        }

        private static void ConvertToGif(string filename)
        {
            using (var inFile = File.OpenRead(filename))
            {
                var decoder = BitmapDecoder.Create(inFile, BitmapCreateOptions.None, BitmapCacheOption.None);
                var encoder = new GifBitmapEncoder();

                for (int i = 0; i < decoder.Frames.Count; i++)
                {
                    var frame = decoder.Frames[i];
                    encoder.Frames.Add(frame);
                }

                using (var outFile = File.OpenWrite(filename + ".gif"))
                {
                    encoder.Save(outFile);
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////