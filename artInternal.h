#include "art.h"

#define	HIT		1
#define MISS		0
#define UNIVERSE	(1e10)

typedef struct {
	Point	origin;
	Vector	direction;
	int 	generation;
} Ray;

typedef struct {
	double	m[4][4];
} Matrix;

typedef struct {
	Matrix	TM;
	Matrix	inverseTM;
} Affine;

#define DOT(a,b) ((a).v[0]*(b).v[0] + (a).v[1]*(b).v[1] + (a).v[2]*(b).v[2])
#define PLUS(r,a,b)	{ \
			(r).v[0] = (a).v[0] + (b).v[0]; \
			(r).v[1] = (a).v[1] + (b).v[1]; \
			(r).v[2] = (a).v[2] + (b).v[2]; \
			}

#define MINUS(r,a,b)	{ \
			(r).v[0] = (a).v[0] - (b).v[0]; \
			(r).v[1] = (a).v[1] - (b).v[1]; \
			(r).v[2] = (a).v[2] - (b).v[2]; \
			}

#define TIMES(r,a,s)	{ \
			(r).v[0] = (s)*(a).v[0]; \
			(r).v[1] = (s)*(a).v[1]; \
			(r).v[2] = (s)*(a).v[2]; \
			}
