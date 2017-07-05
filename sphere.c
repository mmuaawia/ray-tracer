/*
 * implement ray/sphere intersection routine
 *
 *	John Amanatides, Oct 2016
 */


#include <math.h>
#include "artInternal.h"

#define EPSILON (1.0e-5)

/*
 * compute intersection between ray and unit (radius) sphere at origin.
 * Returns MISS if no intersection; otherwise, it returns HIT and
 * sets t to the the distance  to the intersection point and sets the
 * normal vector to the outward facing normal at the intersection point.
 * Note: no intersection is returned if myT >= t
 * (ie my intersection point is further than something else already hit).
 */

int IntersectSphere(Ray *ray, double *t, Vector *normal)
{
	double a, b, c;
	double d, myT;
	double r1, r2;
	Point hit;

	a= DOT(ray->direction, ray->direction);
	b= 2.0*DOT(ray->direction, ray->origin);
	c= DOT(ray->origin, ray->origin) -1.0;

	d= b*b-4.0*a*c;
	if(d <= 0.0)
		return MISS;
	
	if(b < 0.0) {
		r2= (-b+sqrt(d))/(2.0*a);
		r1= c/(a*r2);
	} else {
		r1= (-b-sqrt(d))/(2.0*a);
		r2= c/(a*r1);
	}
	if(r1 < EPSILON)
		if(r2 < EPSILON)
			return MISS;
		else	myT= r2;
	else	myT= r1;

	if(myT >= *t)
		return MISS;
	TIMES(hit,  ray->direction, myT);
	PLUS(hit, ray->origin, hit);
	*t= myT;
	*normal= hit;

	return HIT;
}
