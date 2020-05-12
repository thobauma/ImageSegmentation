// Simple BMP graphics using direct raw binary BMP format
//
// Copyright (c) 2014, Felix Friedrich, Florian Negele, Department of Computer Science, ETH Zurich
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
//
// *    Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
// *    Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
// *    Neither the name of the ETH Zurich nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#include <vector>
#include <cassert>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <unordered_map>

#include "bitmap.hpp"


Color::Color () :
    r (1), g (1), b (1)
{
}

Color::Color (float R, float G, float B) :
    r (R), g (G), b (B)
{
}

Color& Color::operator += (Color c)
{
    r += c.r; g += c.g; b += c.b; return *this;
}

Color& Color::operator -= (Color c)
{
    r -= c.r; g -= c.g; b -= c.b; return *this;
}

Color& Color::operator *= (float s)
{
    r *= s; g *= s; b *= s; return *this;
}

Color& Color::operator *= (Color s)
{
    r *= s.r; g *= s.g; b *= s.b; return *this;
}


Color operator + (Color a, Color b)
{
    return a += b;
}

Color operator - (Color a, Color b)
{
    return a -= b;
}

Color operator * (Color a, Color b)
{
    return a *= b;
}


Color operator * (Color a, float b)
{
    return a *= b;
}


Bitmap::Bitmap (unsigned w, unsigned h) :
    width (w), height (h), pixels (width * height)
{
    assert (width < 32767);
    assert (height < 32767);
}

Color& Bitmap::operator () (unsigned x, unsigned y)
{
    assert (x < width);
    assert (y < height);
    return pixels[y * width + x];
}

Color Bitmap::operator () (unsigned x, unsigned y) const
{
    assert (x < width);
    assert (y < height);
    return pixels[y * width + x];
}

unsigned Bitmap::Width () const
{
    return width;
}

unsigned Bitmap::Height () const
{
    return height;
}

void Bitmap::Fill (Color color)
{
    std::fill (pixels.begin (), pixels.end (), color);
}

enum {BYTE = 1, WORD = 2, DWORD = 4, LONG = 4};

void Bitmap::Save (const std::string& filename) const
{
    std::ofstream file (filename, std::ios::binary);

    // use RLE compression if less than 256 colors used
    std::unordered_map<unsigned int, unsigned int> used_colors;
    unsigned int color_table[256] = {0};
    unsigned int biBitCount;

    bool do_compress = MakeColorTable(used_colors, color_table);
    if (!do_compress) {
        std::cout << "compression disabled (more than 256 colors used)\n";
    }

    // skip header, written after file size is known
    for(int i = 0; i < 14 + 40; ++i) {
         Write(file, 1, 0);
    }

    // write data
    unsigned int colorSize;
    unsigned int dataSize;
    if (do_compress) {
        biBitCount = 8;
        // write color table
        for (unsigned int i = 0 ; i < 256; ++i) {
            Write (file, DWORD, color_table[i]);
        }
        colorSize = 4 * 256;
        dataSize  = WriteCompressedData(file, used_colors);

    } else {
        biBitCount = 24;
        colorSize = 0;
        dataSize  = WriteUncompressedData(file);
    }

    // write header (reset file to begin)
    file.seekp(0);

    // BITMAPFILEHEADER
    Write (file, WORD, 0x4d42);                          // bfType
    Write (file, DWORD, 14 + 40 + colorSize + dataSize); // bfSize
    Write (file, WORD, 0);                               // bfReserved1
    Write (file, WORD, 0);                               // bfReserved2
    Write (file, DWORD, 14 + 40 + colorSize);            // bfOffBits

    // BITMAPINFOHEADER
    Write (file, DWORD, 40);          // biSize
    Write (file, LONG, width);        // biWidth
    Write (file, LONG, height);       // biHeight
    Write (file, WORD, 1);            // biPlanes
    Write (file, WORD, biBitCount);   // biBitCount
    Write (file, DWORD, do_compress); // biCompression
    Write (file, DWORD, dataSize);    // biSizeImage
    Write (file, LONG, 0);            // biXPelsPerMeter
    Write (file, LONG, 0);            // biYPelsPerMeter
    Write (file, DWORD, 0);           // biClrUsed
    Write (file, DWORD, 0);           // biClrImportant

}

unsigned int Bitmap::WriteUncompressedData (std::ostream& os) const {
    const unsigned padding = (4 - width * 3 % 4) % 4;

    for (unsigned y = height-1; y < height; --y)
    {
        for (unsigned x = 0; x != width; ++x)
        {
            Write (os, 3, pixels[y * width + x].rgb());
        }

        for (unsigned x = 0; x != padding; ++x)
            Write (os, 0);
    }

    return (width * 3 + padding) * height;
}

bool Bitmap::MakeColorTable (std::unordered_map<unsigned int, unsigned int>& used_colors, unsigned int* color_table) const {
    for (unsigned y = 0; y != height; ++y)
    {
        for (unsigned x = 0; x != width; ++x)
        {
            unsigned int color = pixels[y * width + x].rgb();
            if (used_colors.find(color) == used_colors.end()) {
                if (used_colors.size() == 256) {
                    return false;
                }
                unsigned int index = used_colors.size();
                color_table[index] = color;
                used_colors[color] = index;
            }
        }
    }
    return true;
}

unsigned int Bitmap::WriteCompressedData (std::ostream& os, std::unordered_map<unsigned int, unsigned int>& used_colors) const {
    unsigned int dataSize = 0;
    for (unsigned y = height-1; y < height; --y)
    {
        unsigned int pos = y * width;
        unsigned int end = pos + width;

        bool compress = true;
        unsigned char uc_bytes[256]; // max + padding
        unsigned int uc_size;

        while (pos < end) {
            unsigned int color = pixels[pos].rgb();
            unsigned int runlength = 1;

            // find next tuple
            while((pos + runlength) < end && runlength < 255 && pixels[pos + runlength].rgb() == color) {
               runlength++;
            }

            if (compress) {
                // three consequentive tuples all having run length 1, switch to uncompressed mode
                if (runlength == 1 && pos + 2 < end && pixels[pos + 1].rgb() != pixels[pos + 2].rgb() && (pos + 2 == end || pixels[pos + 2].rgb() != pixels[pos + 3].rgb())) {
                    compress = false;
                    uc_size = 0;

                } else {
                    Write (os, BYTE, runlength);
                    Write (os, BYTE, used_colors[color]);
                    dataSize += 2;
                    pos += runlength;
                }
            } else {

                // found tuple with length > 2 or exceed chunk length, switch to compressed mode
                if (runlength > 2 || (uc_size + runlength) > 255) {
                    WriteUncompressedChunk (os, uc_bytes, uc_size);
                    dataSize += 2 + uc_size + uc_size % 2;
                    compress = true;

                } else if (runlength == 2) {
                    uc_bytes[uc_size++] = used_colors[color];
                    uc_bytes[uc_size++] = used_colors[color];
                    pos += runlength;

                } else if (runlength == 1) {
                    uc_bytes[uc_size++] = used_colors[color];
                    pos += runlength;
                }
            }
        }

        // finish uncompressed run at end of line
        if (!compress) {
            WriteUncompressedChunk (os, uc_bytes, uc_size);
            dataSize += 2 + uc_size + uc_size % 2;
            compress = true;
        }

        Write (os, WORD, 0x0000); // end of scan line marker
        dataSize += 2;
    }
    Write (os, WORD, 0x0100); // end of data marker
    dataSize += 2;

    return dataSize;
}

void Bitmap::WriteUncompressedChunk(std::ostream& os, unsigned char* bytes, unsigned int size) const {
    assert(size > 2);

    Write (os, BYTE, 0);
    Write (os, BYTE, size); // size must be >=3

    // pad with zero if unequal
    if (size % 2 == 1) {
        bytes[size++] = 0;
    }

    for (unsigned int i = 0; i < size; ++i) {
        Write (os, BYTE, bytes[i]);
    }
}

void Bitmap::Write (std::ostream& os, float value)
{
    int intValue = 256 * (value < 0 ? 0 : value > 1 ? 1 : value);
    os.put (intValue < 256 ? intValue : 255);
}

void Bitmap::Write (std::ostream& os, unsigned length, int value)
{
    while (length--) os.put (value & 0xff), value >>= 8;
}

void skipcomment(std::istream& file){
    char c;
    file >> c;
    while (c=='#'){
        file.ignore(128,'\n');
        file >> c;
    }
    file.putback(c);
}

// read portable graymap picture
Bitmap readpgm(const std::string& filename){
    std::ifstream file(filename);
    file >> std::skipws;
    std::string id;
    file >> id;
    assert(id=="P2"); // uncompresed raw textual data 
    unsigned int width;
    unsigned int height;
    unsigned int depth;
    skipcomment(file);
    file >> width;
    skipcomment(file);
    file >> height;
    skipcomment(file);
    file >> depth; // maximal value
    //std::cout << width << "x" << height << "x" << depth << "\n";
    depth++; // cap
    Bitmap bitmap(width,height);
    bitmap.Fill(Black);
    for (unsigned int y = 0; y< height; ++y){
        for (unsigned int x = 0; x<width;++x){
            float value;
            file >> value;
            value /= depth;
            bitmap(x,y) = Color(value,value,value);
        }
    }
    return bitmap;
}


/*
int main ()
{
    Bitmap bitmap (512, 512);

    Color top = Red;
    Color bottom = Green;

    for (unsigned y = 0; y != bitmap.GetHeight (); ++y)
    {
        Color color = top + (bottom - top) * (float (y) / bitmap.GetHeight ());

        for (unsigned x = 0; x != bitmap.GetWidth (); ++x)
            bitmap (x, y) = color;
    }

    bitmap.Save ("test.bmp");
}
*/
