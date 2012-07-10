#ifndef __SHADERINST_HXX__
#define __SHADERINST_HXX__

#include "ShaderParam.hxx"


class ShaderFile;
/*
 * ShaderInst
 * A Shader instance
 */
class ShaderInst
{
public:
	ShaderInst()	{ file=NULL; }
	ShaderFile *file;
	string name;
	vector<ShaderParam> params;
	matrix4d mat;
};

#endif 
