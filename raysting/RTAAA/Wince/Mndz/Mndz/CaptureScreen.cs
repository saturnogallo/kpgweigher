using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.IO;
namespace Mndz
{
    class CaptureScreen
    {
        public static void SaveScreenToFile(string fileName)
        {
            byte[] bitmapData = GetScreenBitmapArray();
            FileStream fs = new FileStream(fileName, FileMode.Create);
            fs.Write(bitmapData, 0, bitmapData.Length);
            fs.Flush();
            fs.Close();
        }
        
        #region 私有方法
        private const int PelsPerMeter = 0xb12; // 72 dpi, 96 (0xec4) also possible
        private static byte[] GetControlBitmapArray(Control control)
        {
            control.Capture = true;
            IntPtr hWnd = GetCapture();
            control.Capture = false;
            IntPtr hDC = GetDC(hWnd);
            IntPtr hMemoryDC = CreateCompatibleDC(hDC);

            BITMAPINFOHEADER bih = new BITMAPINFOHEADER();
            bih.biSize = Marshal.SizeOf(bih);
            bih.biBitCount = 24;
            bih.biClrUsed = 0;
            bih.biClrImportant = 0;
            bih.biCompression = 0;
            bih.biHeight = control.Height;
            bih.biWidth = control.Width;
            bih.biPlanes = 1;
            int cb = (int)(bih.biHeight * bih.biWidth * bih.biBitCount / 8);
            bih.biSizeImage = cb;
            bih.biXPelsPerMeter = PelsPerMeter;
            bih.biYPelsPerMeter = PelsPerMeter;

            IntPtr pBits = IntPtr.Zero;
            IntPtr pBIH = LocalAlloc(GPTR, bih.biSize);
            Marshal.StructureToPtr(bih, pBIH, false);
            IntPtr hBitmap = CreateDIBSection(hDC, pBIH, 0, ref pBits, IntPtr.Zero, 0);

            BITMAPINFOHEADER bihMem = (BITMAPINFOHEADER)Marshal.PtrToStructure(pBIH, typeof(BITMAPINFOHEADER));
            IntPtr hPreviousBitmap = SelectObject(hMemoryDC, hBitmap);
            BitBlt(hMemoryDC, 0, 0, bih.biWidth, bih.biHeight, hDC, 0, 0, SRCCOPY);
            byte[] bits = new byte[cb];
            Marshal.Copy(pBits, bits, 0, cb);

            BITMAPFILEHEADER bfh = new BITMAPFILEHEADER();
            bfh.bfSize = (uint)cb + 0x36;
            bfh.bfType = 0x4d42;
            bfh.bfOffBits = 0x36;
            int headerSize = 14;
            byte[] header = new byte[headerSize];
            BitConverter.GetBytes(bfh.bfType).CopyTo(header, 0);
            BitConverter.GetBytes(bfh.bfSize).CopyTo(header, 2);
            BitConverter.GetBytes(bfh.bfOffBits).CopyTo(header, 10);
            byte[] data = new byte[cb + bfh.bfOffBits];
            header.CopyTo(data, 0);
            header = new byte[Marshal.SizeOf(bih)];
            IntPtr pHeader = LocalAlloc(GPTR, Marshal.SizeOf(bih));
            Marshal.StructureToPtr(bihMem, pHeader, false);
            Marshal.Copy(pHeader, header, 0, Marshal.SizeOf(bih));
            LocalFree(pHeader);
            header.CopyTo(data, headerSize);
            bits.CopyTo(data, (int)bfh.bfOffBits);

            DeleteObject(SelectObject(hMemoryDC, hPreviousBitmap));
            DeleteDC(hMemoryDC);
            ReleaseDC(hDC);

            return data;
        }

        private static byte[] GetScreenBitmapArray()
        {
            IntPtr hDC = GetDC(IntPtr.Zero);
            IntPtr hMemoryDC = CreateCompatibleDC(hDC);

            BITMAPINFOHEADER bih = new BITMAPINFOHEADER();
            bih.biSize = Marshal.SizeOf(bih);
            bih.biBitCount = 24;
            bih.biClrUsed = 0;
            bih.biClrImportant = 0;
            bih.biCompression = 0;
            bih.biHeight = Screen.PrimaryScreen.Bounds.Height;
            bih.biWidth = Screen.PrimaryScreen.Bounds.Width;
            bih.biPlanes = 1;
            int cb = (int)(bih.biHeight * bih.biWidth * bih.biBitCount / 8);
            bih.biSizeImage = cb;
            bih.biXPelsPerMeter = PelsPerMeter;
            bih.biYPelsPerMeter = PelsPerMeter;

            IntPtr pBits = IntPtr.Zero;
            IntPtr pBIH = LocalAlloc(GPTR, bih.biSize);
            Marshal.StructureToPtr(bih, pBIH, false);
            IntPtr hBitmap = CreateDIBSection(hDC, pBIH, 0, ref pBits, IntPtr.Zero, 0);

            BITMAPINFOHEADER bihMem = (BITMAPINFOHEADER)Marshal.PtrToStructure(pBIH, typeof(BITMAPINFOHEADER));
            IntPtr hPreviousBitmap = SelectObject(hMemoryDC, hBitmap);
            BitBlt(hMemoryDC, 0, 0, bih.biWidth, bih.biHeight, hDC, 0, 0, SRCCOPY);
            byte[] bits = new byte[cb];
            Marshal.Copy(pBits, bits, 0, cb);

            BITMAPFILEHEADER bfh = new BITMAPFILEHEADER();
            bfh.bfSize = (uint)cb + 0x36;
            bfh.bfType = 0x4d42;
            bfh.bfOffBits = 0x36;
            int headerSize = 14;
            byte[] header = new byte[headerSize];
            BitConverter.GetBytes(bfh.bfType).CopyTo(header, 0);
            BitConverter.GetBytes(bfh.bfSize).CopyTo(header, 2);
            BitConverter.GetBytes(bfh.bfOffBits).CopyTo(header, 10);
            byte[] data = new byte[cb + bfh.bfOffBits];
            header.CopyTo(data, 0);
            header = new byte[Marshal.SizeOf(bih)];
            IntPtr pHeader = LocalAlloc(GPTR, Marshal.SizeOf(bih));
            Marshal.StructureToPtr(bihMem, pHeader, false);
            Marshal.Copy(pHeader, header, 0, Marshal.SizeOf(bih));
            LocalFree(pHeader);
            header.CopyTo(data, headerSize);
            bits.CopyTo(data, (int)bfh.bfOffBits);

            DeleteObject(SelectObject(hMemoryDC, hPreviousBitmap));
            DeleteDC(hMemoryDC);
            ReleaseDC(hDC);

            return data;
        }
        #endregion
        #region 声明API
        private struct BITMAP
        {
            public int bmType;
            public int bmWidth;
            public int bmHeight;
            public int bmWidthBytes;
            public ushort bmPlanes;
            public ushort bmBitsPixel;
            public byte[] bmBits;
        }

        private struct BITMAPINFO
        {
            public BITMAPINFOHEADER bmiHeader;
            public RGBQUAD[] bmiColors;
        }

        private struct RGBQUAD
        {
            public byte rgbBlue;
            public byte rgbGreen;
            public byte rgbRed;
            public byte rgbReserved;
        }

        private struct BITMAPINFOHEADER
        {
            public int biSize;
            public int biWidth;
            public int biHeight;
            public ushort biPlanes;
            public ushort biBitCount;
            public int biCompression;
            public int biSizeImage;
            public int biXPelsPerMeter;
            public int biYPelsPerMeter;
            public uint biClrUsed;
            public uint biClrImportant;
        }

        private struct BITMAPFILEHEADER
        {
            public ushort bfType;
            public uint bfSize;
            public ushort bfReserved1;
            public ushort bfReserved2;
            public uint bfOffBits;
        }

        private const int GPTR = 0x40;
        private const int SRCCOPY = 0x00CC0020;

        [DllImport("coredll.dll")]
        private static extern IntPtr GetCapture();
        [DllImport("coredll.dll")]
        private static extern IntPtr LocalAlloc(uint flags, int cb);
        [DllImport("coredll.dll")]
        private static extern IntPtr LocalFree(IntPtr hMem);
        [DllImport("coredll.dll")]
        private static extern IntPtr CreateDIBSection(IntPtr hdc, IntPtr hdr, uint colors, ref IntPtr pBits, IntPtr hFile, uint offset);
        [DllImport("coredll.dll")]
        private static extern IntPtr GetDC(IntPtr hWnd);
        [DllImport("coredll.dll")]
        private static extern void ReleaseDC(IntPtr hDC);
        [DllImport("coredll.dll")]
        private static extern void DeleteDC(IntPtr hDC);
        [DllImport("coredll.dll")]
        private static extern int BitBlt(IntPtr hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, IntPtr hdcSrc, int nXSrc, int nYSrc, uint dwRop);
        [DllImport("coredll.dll")]
        private static extern IntPtr CreateCompatibleDC(IntPtr hdc);
        [DllImport("coredll.dll")]
        private static extern IntPtr SelectObject(IntPtr hdc, IntPtr hObj);
        [DllImport("coredll.dll")]
        private static extern void DeleteObject(IntPtr hObj);
        #endregion

    }


}
