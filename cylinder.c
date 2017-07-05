/*
 * implement ray/cylinder intersection routine
 *
 *	John Amanatides, Oct 2016
 */


#include <math.h>
#include "artInternal.h"

#define EPSILON	(1e-5)

/*
 * compute intersection between ray and a cylinder (-1<= y <= 1, x^2 + z^2 <= 1)
 * Returns MISS if no intersection; otherwise, it returns HIT and
 * sets t to the the distance  to the intersection point and sets the
 * normal vector to the outward facing normal (unit length) at the
 * intersection point.  Note: no intersection is returned if myT >= t
 * (ie my intersection point is further than something else already hit).
 */

int IntersectCylinder(Ray *ray, double *t, Vector *normal)

	
{

	double a, b, c, x, y, z;
	double d, myT,t0,t1;
	double r1, r2;
	int result = MISS;
 	Point start = ray->origin;
 	Vector Direction = ray->direction;
 	Point hit;

// a=xD2+yD2, b=2xExD+2yEyD, and c=xE2+yE2-1. 
 a = ray->direction.v[0] * ray->direction.v[0]
	+ ray->direction.v[2] * ray->direction.v[2];

 b = 2.0 * ray->origin.v[0] * ray->direction.v[0]
	+ 2.0 * ray->origin.v[2] * ray->direction.v[2];

 c = ray->origin.v[0] * ray->origin.v[0]
	+ ray->origin.v[2] * ray->origin.v[2]
	- 1.0;

double b24ac = b*b - 4.0*a*c;

	
if (b24ac > 0.0){
	double sqb24ac = sqrt(b24ac);
	t0 = (-b + sqb24ac) / (2.0 * a);
	t1 = (-b - sqb24ac) / (2.0 * a);

	if(b < 0.0) {
		r2= t0;
		r1= c/(a*r2);
	} else {
		r1= t1;
		r2= c/(a*r1);
	}
	if(r1 < EPSILON)
			myT= r2;
	else	myT= r1;

if (myT >= EPSILON && myT <= *t) {
	y = ray->origin.v[1] + myT*ray->direction.v[1];
	//if (abs(y) < 1.0 + EPSILON )
	if (y < 1.0 + EPSILON && y > -1.0 - EPSILON)	
	{
		//TIMES(hit,  ray->direction, myT);
		//PLUS(hit, ray->origin, hit);
		//hit.v[2] = 0;
		*t= myT;
		//*normal = hit;
		normal->v[1] = 0.0;
		normal->v[0] = ray->origin.v[0] + myT*ray->direction.v[0];
		normal->v[2] = ray->origin.v[2] + myT*ray->direction.v[2];
		result = HIT;

	}

}
}
	myT = (1.0 - ray->origin.v[1])/ray->direction.v[1];
	if (myT >= EPSILON && myT < *t)
	{
		x = ray->origin.v[0] + myT*ray->direction.v[0];
		z = ray->origin.v[2] + myT*ray->direction.v[2];
		if(x*x + z*z <= 1.0 + EPSILON)
		{
			*t = myT;
			normal->v[0] = 0.0;
			normal->v[1] = 1.0;
			normal->v[2] = 0.0;
			result = HIT;
		}
	}


	myT = (-1.0 - ray->origin.v[1])/ray->direction.v[1];
	if (myT >= EPSILON && myT < *t)
	{
		x = ray->origin.v[0] + myT*ray->direction.v[0];
		z = ray->origin.v[2] + myT*ray->direction.v[2];
		if(x*x + z*z <= 1.0 + EPSILON)
		{
			*t = myT;
			normal->v[0] = 0.0;
			normal->v[1] = -1.0;
			normal->v[2] = 0.0;
			result = HIT;
		}
	}

	return result;
}










