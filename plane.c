/*
 * implement ray/plane intersection routine
 *
 *	John Amanatides, Nov 2016
 */


#include "artInternal.h"

#define EPSILON	(1e-10)

/*
 * compute intersection between ray the y=0 plane.
 * Returns MISS if no intersection; otherwise, it returns HIT and
 * sets t to the the distance  to the intersection point and sets the
 * normal vector to the outward facing normal (unit length) at the
 * intersection point.  Note: no intersection is returned if myT >= t
 * (ie my intersection point is further than something else already hit).
 */

int IntersectPlane(Ray *ray, double *t, Vector *normal)
{
	double myT;

	/* make sure that there is at least an intersection */
	if(ray->origin.v[1] * ray->direction.v[1] >= 0.0)
		return MISS;

	myT= -(ray->origin.v[1]/ray->direction.v[1]);
	if((myT <= EPSILON) || (myT >= *t))
		return MISS;

	*t= myT;
	normal->v[0]= 0.0;
	normal->v[1]= 1.0;
	normal->v[2]= 0.0;
	return HIT;
}
