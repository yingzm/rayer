#ifndef __TEXTUREFILE_HXX__
#define __TEXTUREFILE_HXX__

#include "color.hxx"

class TextureFile
{
public:
	TextureFile();
	~TextureFile();
	int Load(const char *filename);

	color3d GetAt(double row, double col);
private:
	int m_width, m_height;
	color3d *m_buf;
};

#endif /* __TEXTUREFILE_HXX__ */