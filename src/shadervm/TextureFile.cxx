#include "common.hxx"
#include "TextureFile.hxx"
#include "tiffio.h"

TextureFile::TextureFile()
{
	m_buf= NULL;
}

TextureFile::~TextureFile()
{
	delete [] m_buf;
}

int TextureFile::Load(const char *filename)
{
	TIFF *tif = TIFFOpen(filename, "r");
	if (tif==NULL) {
		return -1;
	}

	uint32 w, h;
	TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
	TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
	m_width = w; m_height = h;
	int npixels = w*h;
	uint32 *raster = (uint32 *)_TIFFmalloc(npixels*sizeof(uint32));
	if (raster!=NULL) {
		if (TIFFReadRGBAImage(tif, w, h, raster, 0)) {
			m_buf = new color3d[w*h];
			int i=0;
			for (int y=0; y<h; ++y) {
				for (int x=0; x<w; ++x) {
					uint32 clr = raster[(h-1-y)*w+x];
					m_buf[i++].set_comp(double(clr&0xff)/255.0, 
						double((clr>>8)&0xff)/255.0, double((clr>>16)&0xff)/255.0);
				}
			}
		}
		_TIFFfree(raster);
	}

	TIFFClose(tif);

	return 0;
}

color3d TextureFile::GetAt(double row, double col)
{
	row = row - int(row);
	col = col - int(col);
	int icol = int(col*m_height), irow=int(row*m_width);
	return m_buf[icol*m_width+irow];
}