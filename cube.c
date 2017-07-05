/*
 * implement ray/cube intersection routine
 *
 *	John Amanatides, Oct 2016
 */


#include <math.h>
#include "artInternal.h"

#define EPSILON	(1e-10)

typedef struct {
        Vector normal;
        double d;
} Plane;
 
static Plane cubePlane[6]= {
	 1.0,  0.0,  0.0, -1.0,
	-1.0,  0.0,  0.0, -1.0,
	 0.0,  1.0,  0.0, -1.0,
	 0.0, -1.0,  0.0, -1.0,
	 0.0,  0.0,  1.0, -1.0,
	 0.0,  0.0, -1.0, -1.0,
};

static double IntersectPlane(Ray *ray, Plane plane)
{
	double a, b;

	a= DOT(ray->direction, plane.normal);
	b= DOT(ray->origin, plane.normal)+plane.d;

	return -b/a;
}


/*
 * compute intersection between ray and a cube (-1<= x, y, z <= 1)
 * Returns MISS if no intersection; otherwise, it returns HIT and
 * sets t to the the distance  to the intersection point and sets the
 * normal vector to the outward facing normal (unit length) at the
 * intersection point.  Note: no intersection is returned if myT >= t
 * (ie my intersection point is further than something else already hit).
 */

int IntersectCube(Ray *ray, double *t, Vector *normal)
{
	int i, result= MISS;
	double myT;
	Point hit;

	for(i=0; i < 6; i++) {
		myT= IntersectPlane(ray, cubePlane[i]);
		if(myT < *t && myT > EPSILON) {
			TIMES(hit,  ray->direction, myT);
			PLUS(hit, ray->origin, hit);
			
			if((hit.v[0] >= (-1.0-EPSILON)) &&
			   (hit.v[0] <= ( 1.0+EPSILON)) &&
			   (hit.v[1] >= (-1.0-EPSILON)) &&
			   (hit.v[1] <= ( 1.0+EPSILON)) &&
			   (hit.v[2] >= (-1.0-EPSILON)) &&
			   (hit.v[2] <= ( 1.0+EPSILON)) ) {

			*t= myT;
			*normal= cubePlane[i].normal;
			result= HIT;
			}
		}
	}

	return result;
}
