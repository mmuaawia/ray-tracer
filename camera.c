/*
 *
 * implements art's camera model (lens not implemented)
 *
 *	John Amanatides, Oct 2016
 */


#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "artInternal.h"

extern Vector	Cross(Vector, Vector);
extern double	Normalize(Vector *);
extern double frandom();

static Point camera;
static Vector vx, vy, vz;
static double scaleX, scaleY, focusDistance, lensRadius;



char *
art_Camera(Point lookFrom, Point lookAt, Vector lookUp)
{
	Vector x, z;

	MINUS(z, lookAt, lookFrom);
	if(Normalize(&z) == 0.0)
		return "art_Camera: domain error";
	x= Cross(z, lookUp);
	if(Normalize(&x) == 0.0)
		return "art_Camera: domain error";

	camera= lookFrom;
	vx= x;
	vy= Cross(x, z);
	vz= z;
	return NULL;
}

/* for A4 */
char *
art_Lens(double focus, double radius)
{
	if(focus <= 0.0 || radius < 0.0)
		return "art_Lens: domain error";
	focusDistance= focus;
	lensRadius= radius;
	return NULL;
}


char *
art_Perspective(double width, double height, double distance)
{
	if(distance <= 0.0 || width <= 0.0 || height <= 0.0)
		return "art_Perspective: domain error";
	scaleX= width/distance;
	scaleY= height/distance;
	return NULL;
}


void InitCamera(void)
{
	Vector from, to, up;

	from.v[0]= 0.0;
	from.v[1]= 0.0;
	from.v[2]= 10.0;

	to.v[0]= 0.0;
	to.v[1]= 0.0;
	to.v[2]= 0.0;

	up.v[0]= 0.0;
	up.v[1]= 1.0;
	up.v[2]= 0.0;

	(void) art_Camera(from, to, up);
	(void) art_Lens(1.0, 0.0);
	(void) art_Perspective(1.0, 1.0, 1.0);
}



/*
 * given sample point on unit viewing square (0.0 <= u, v <= 1.0)
 * return ray that travels in that direction from the eye
 *	Note: this code assumes the pinhole camera
 */


Ray
ShootRay(double u, double v)
{
	Ray ray;
	Vector tmp;

	ray.origin= camera;
	ray.direction= vz;
	ray.generation= 1;

	if(u < 0.0 || u > 1.0 || v < 0.0 || v > 1.0) {
		(void) fprintf(stderr, "ShootRay: domain error!!\n");
		exit(1);
	} else {
		TIMES(tmp, vx, (u-0.5)*scaleX);
		PLUS(ray.direction, tmp, ray.direction);
		TIMES(tmp, vy, (v-0.5)*scaleY);
		PLUS(ray.direction, tmp, ray.direction);
		(void) Normalize(&ray.direction);

		//Approximation for lens.
		if(focusDistance > 0.0 && lensRadius > 0.0)
		{
			double cosTheta = DOT(vz, ray.direction);
			Point focusPoint;
			TIMES(focusPoint, ray.direction, (focusDistance / cosTheta));
			PLUS(focusPoint, focusPoint, camera);

			Point randomOrigin;
			randomOrigin.v[0] = (2.0*frandom() - 1.0) * lensRadius;
			randomOrigin.v[1] = (2.0*frandom() - 1.0) * lensRadius;
			randomOrigin.v[2] = 0.0;
			PLUS(ray.origin, randomOrigin, camera);

			MINUS(ray.direction, focusPoint, ray.origin);
			(void) Normalize(&ray.direction);
		}
	}

	return ray;
}
