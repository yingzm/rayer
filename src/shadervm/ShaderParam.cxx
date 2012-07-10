#include "common.hxx"
#include "ShaderParam.hxx"
#include "parsefunc.hxx"

ShaderParam::ShaderParam()
{
	m_type = variable_t(-1);
	m_sz = 0;
	m_value = NULL;
	m_offset = 0;
}

ShaderParam::ShaderParam(const ShaderParam &sp)
{
	if (this==&sp)
		return;

	m_value = NULL;
	m_type = variable_t(-1);
	m_sz = 0;
	m_offset = 0;
	if (sp.m_value==NULL) 
		m_value = sp.m_value;
	else {
		//if (m_type!=sp.m_type)
		//	DestroyValue();

		switch(sp.m_type) {
		case VAR_FLOAT:
			Setd(sp.Getd());
			break;
		case VAR_POINT:
			Setp(sp.Getp());
			break;
		case VAR_VECTOR:
			Setv(sp.Getv());
			break;
		case VAR_NORMAL:
			Setn(sp.Getn());
			break;
		case VAR_COLOR:
			Setc(sp.Getc());
			break;
		case VAR_MATRIX:
			Setm(sp.Getm());
			break;
		case VAR_STRING:
			Sets(sp.Gets());
			break;
		case (VAR_FLOAT|VAR_ARRAY):
			m_value = new double[sp.GetItemNum()];
			memcpy(m_value, sp.m_value, sizeof(double)*sp.GetItemNum());
			break;
		case (VAR_POINT|VAR_ARRAY):
			m_value = new point3d[sp.GetItemNum()];
			memcpy(m_value, sp.m_value, sizeof(point3d)*sp.GetItemNum());
			break;
		case (VAR_VECTOR|VAR_ARRAY):
			m_value = new vector3d[sp.GetItemNum()];
			memcpy(m_value, sp.m_value, sizeof(vector3d)*sp.GetItemNum());
			break;
		case (VAR_NORMAL|VAR_ARRAY):
			m_value = new vector3d[sp.GetItemNum()];
			memcpy(m_value, sp.m_value, sizeof(vector3d)*sp.GetItemNum());
			break;
		case (VAR_COLOR|VAR_ARRAY):
			m_value = new color3d[sp.GetItemNum()];
			memcpy(m_value, sp.m_value, sizeof(color3d)*sp.GetItemNum());
			break;
		case (VAR_MATRIX|VAR_ARRAY):
			m_value = new matrix4d[sp.GetItemNum()];
			memcpy(m_value, sp.m_value, sizeof(matrix4d)*sp.GetItemNum());
			break;
		case (VAR_STRING|VAR_ARRAY):
			m_value = new string[sp.GetItemNum()];
			for (int i=0; i<sp.GetItemNum(); ++i) {
				((string *)m_value)[i] = sp.Getsa(i);
			}
			break;
		}
	}

	m_name = sp.m_name;
	m_sz = sp.m_sz;
	m_type = sp.m_type;
	m_offset = sp.m_offset;
}

ShaderParam::~ShaderParam()
{
	if (m_value!=NULL)
		DestroyValue();
}

void ShaderParam::DestroyValue()
{
	switch (m_type) {
		case VAR_FLOAT:
			delete (double *)m_value;
			break;
		case VAR_POINT:
			delete (point3d *)m_value;
			break;
		case VAR_VECTOR:
		case VAR_NORMAL:
			delete (vector3d *)m_value;
			break;
		case VAR_COLOR:
			delete (color3d *)m_value;
			break;
		case VAR_STRING:
			delete (string *)m_value;
			break;
		case (VAR_FLOAT|VAR_ARRAY):
			delete [] (double *)m_value;
			break;
		case (VAR_POINT|VAR_ARRAY):
			delete [] (point3d *)m_value;
			break;
		case (VAR_VECTOR|VAR_ARRAY):
			delete [] (vector3d *)m_value;
			break;
		case (VAR_NORMAL|VAR_ARRAY):
			delete [] (vector3d *)m_value;
			break;
		case (VAR_COLOR|VAR_ARRAY):
			delete [] (color3d *)m_value;
			break;
		case (VAR_MATRIX|VAR_ARRAY):
			delete [] (matrix4d *)m_value;
			break;
		case (VAR_STRING|VAR_ARRAY):
			delete [] (string *)m_value;
			break;
		default:
			break;
	}

	m_value = NULL;
	m_type = variable_t(-1);
}

void ShaderParam::ConvertTo(int type)
{
	if (m_type!=VAR_STRING) {
		printf("!<ShaderParam::ConverTo> original type must be string.\n");
		return;
	}

	string s = Gets();

	double d0, d1, d2;
	double m[16];
	double *darray;
	int size;

	switch(type) {
	case VAR_FLOAT:
		Setd(atof(s.c_str()));
		break;
	case VAR_POINT:
		sscanf(s.c_str(), "%lf%lf%lf", &d0, &d1, &d2);
		Setp(point3d(d0, d1, d2));
		break;
	case VAR_VECTOR:
		sscanf(s.c_str(), "%lf%lf%lf", &d0, &d1, &d2);
		Setv(vector3d(d0, d1, d2));
		break;
	case VAR_NORMAL:
		sscanf(s.c_str(), "%lf%lf%lf", &d0, &d1, &d2);
		Setn(vector3d(d0, d1, d2));
		break;
	case VAR_COLOR:
		sscanf(s.c_str(), "%lf%lf%lf", &d0, &d1, &d2);
		Setc(color3d(d0, d1, d2));
		break;
	case VAR_MATRIX:
		sscanf(s.c_str(), "%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf", 
			m, m+1, m+2, m+3, m+4, m+5, m+6, m+7, 
			m+8, m+9, m+10, m+11, m+12, m+13, m+14, m+15);
		Setm(matrix4d(m));
		break;
	case VAR_STRING:
		break;
	case (VAR_FLOAT|VAR_ARRAY):
		size = GetDoubleArray(s.c_str(), darray);
		Setda(size, darray);
		delete [] darray;
		break;
	case (VAR_POINT|VAR_ARRAY):
		size = GetDoubleArray(s.c_str(), darray);
		if ( (size%3)!=0 )
			printf("?<ShaderParam::ConvertTo> number of values is not multiple of 3\n");
		Setpa(size/3, (point3d *)darray);
		delete [] darray;
		break;
	case (VAR_VECTOR|VAR_ARRAY):
		size = GetDoubleArray(s.c_str(), darray);
		if ( (size%3)!=0 )
			printf("?<ShaderParam::ConvertTo> number of values is not multiple of 3\n");
		Setva(size/3, (vector3d *)darray);
		delete [] darray;
		break;
	case (VAR_NORMAL|VAR_ARRAY):
		size = GetDoubleArray(s.c_str(), darray);
		if ( (size%3)!=0) 
			printf("?<ShaderParam::ConverTo> number of values is not multiple of 3\n");
		Setna(size/3, (vector3d *)darray);
		delete [] darray;
		break;
	case (VAR_COLOR|VAR_ARRAY):
		size = GetDoubleArray(s.c_str(), darray);
		if ( (size%3)!=0 )
			printf("?<ShaderParam::ConvertTo> number of values is not multiple of 3\n");
		Setca(size/3, (color3d *)darray);
		delete [] darray;
		break;
	case (VAR_MATRIX|VAR_ARRAY):
		size = GetDoubleArray(s.c_str(), darray);
		if ( (size%16)!=0 )
			printf("?<ShaderParam::ConvertTo> number of values is not multiple of 3\n");
		Setma(size/16, (matrix4d *)darray);
		delete [] darray;
		break;
	case (VAR_STRING|VAR_ARRAY):
		printf("!<ShaderParam::Convert> Hey, are you sure you are using string array? Not implemented yet\n");
		break;

	default:
		break;
	}
}


int ShaderParam::GetDoubleArray(const char *s, double *&arr)
{
	double d;
	const char *p = s;
	vector<double> darray;

	while ( (p = get_double(p, &d)) != NULL ) {
		darray.push_back(d);
	}

	if (darray.size()>0) {
		arr = new double[darray.size()];
		for (int i=0; i<darray.size(); ++i) {
			arr[i] = darray[i];
		}
	}

	return darray.size();
}
