#ifndef __PARSEFUNC_HXX__
#define __PARSEFUNC_HXX__

bool is_whitespace(char c);
// skip whitespace in p, however, if no valid chars encounter until reaches end of p
// -1 is returned
int skip_whitespace(const char *&p);

const char *get_double(const char *p, double *d);
const char *get_int(const char *p, int *i);

inline bool is_whitespace(char c)
{
	if (c==' ' || c=='\n' || c=='\t')
		return true;
	else
		return false;
}


#endif