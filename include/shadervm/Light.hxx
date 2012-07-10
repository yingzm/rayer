#ifndef __LIGHT_HXX__
#define __LIGHT_HXX__

class Light
{
public:
	bool on;
	bool shadowon;
	enum {
		AMBIENT,
		NONAMBIENT
	};
	unsigned int type;

	ShaderInst *m_shader;
};

#endif /* __LIGHT_HXX__ */