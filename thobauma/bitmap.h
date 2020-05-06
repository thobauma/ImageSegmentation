// Simple BMP graphics using direct raw binary BMP format
//
// Copyright (c) 2014, Felix Friedrich, Florian Negele, Department of Computer Science, ETH Zurich
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
//
// *	Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
// *	Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
// *	Neither the name of the ETH Zurich nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef BITMAP_H
#define BITMAP_H

#include <vector>
#include <cassert>
#include <fstream>
#include <algorithm>
#include <boost/unordered_map.hpp>

struct Color
{
public:
	Color ();
	Color (float, float, float);

	float GetR() const { return r; }
	float GetG() const { return g; }
	float GetB() const { return b; }
	unsigned int rgb() const { return toInt(r) << 16 | toInt(g) << 8 | toInt(b); }

	Color& operator += (Color);
	Color& operator -= (Color);
	Color& operator *= (float);
	Color& operator *= (Color);

private:
	float r, g, b;
	
    unsigned int toInt(float value) const {
        return 255 * (value < 0 ? 0 : value > 1 ? 1 : value);
    }
};

Color operator + (Color a, Color b);
Color operator - (Color a, Color b);
Color operator * (Color a, Color b);
Color operator * (Color a, float b);

const Color Red (1, 0, 0);
const Color Green (0, 1, 0);
const Color Blue (0, 0, 1);
const Color Black (0, 0, 0);
const Color White (1, 1, 1);

class Bitmap
{
public:
	Bitmap (unsigned, unsigned);

	Color& operator () (unsigned, unsigned);
	Color operator () (unsigned, unsigned) const;

	unsigned Width () const;
	unsigned Height () const;

	void Fill (Color);
	void Save (const std::string&) const;
	
private:
	unsigned width, height;
	std::vector<Color> pixels;
		    	    
	bool MakeColorTable (boost::unordered_map<unsigned int, unsigned int>& used_colors, unsigned int* color_table) const;
	unsigned int WriteUncompressedData (std::ostream& os) const;
	unsigned int WriteCompressedData (std::ostream& os, boost::unordered_map<unsigned int, unsigned int>& used_colors) const;
	void WriteUncompressedChunk (std::ostream& os, unsigned char* bytes, unsigned int size) const;
	    
	static void Write (std::ostream&, float);
	static void Write (std::ostream&, unsigned, int);
};

Bitmap readpgm(const std::string& filename);

#endif

