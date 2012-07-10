#include "common.hxx"
#include "parsefunc.hxx"

int skip_whitespace(const char *&p)
{
	while (is_whitespace(*p)) {
		if (*p=='\0')
			return -1;
		++p;
	}

	if (*p=='\0')
		return -1;
	else
		return 0;
}

const char *get_int(const char *p, int *i)
{
	char buf[256];
	char *dp = buf;

	if (skip_whitespace(p)<0)
		return NULL;

	while (!is_whitespace(*p) && *p!='\0') {
		*dp++ = *p++;
	}
	*dp = '\0';

	*i = atoi(buf);

	return p;
}

const char *get_double(const char *p, double *d)
{
	char buf[256];
	char *dp = buf;

	if (skip_whitespace(p)<0)
		return NULL;

	while (!is_whitespace(*p) && *p!='\0') {
		*dp++ = *p++;
	}
	*dp = '\0';

	*d = atof(buf);

	return p;
}
