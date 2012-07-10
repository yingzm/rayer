#ifndef __COMMON_HXX__
#define __COMMON_HXX__

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

#include <assert.h>

#ifndef ASSERT
#define ASSERT assert
#endif

#define NULLREF(a) ( *(a *)(NULL) )

#define BEGIN_NAMESPACE
#define END_NAMESPACE

/*
 * include stl
 */
#include <fstream>
#include <vector>
#include <list>
#include <climits>
#include <cmath>
#include <stack>
#include <map>
using namespace std;


#endif /* __COMMON_HXX__ */
