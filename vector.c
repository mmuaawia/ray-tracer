/*
 * Misc. vector/affine functions (including reflection/refraction)
 *
 *	John Amanatides, Oct 2016
 */


#include <math.h>
#include "artInternal.h"


/*
 * result= m1*m2
 */


Matrix
MultMatrix(Matrix *m1, Matrix *m2)
{
	Matrix result;
	int r, c, i;

	for(r=0; r < 4; r++)
		for(c=0; c < 4; c++) {
			result.m[r][c]= 0.0;
			for(i=0; i < 4; i++)
				result.m[r][c] += m1->m[r][i]*m2->m[i][c];
		}

	return result;
}


/*
 * compute cross product
 */

Vector
Cross(Vector a, Vector b)
{
	Vector result;

	result.v[0]= a.v[1]*b.v[2] - a.v[2]*b.v[1];
	result.v[1]= a.v[2]*b.v[0] - a.v[0]*b.v[2];
	result.v[2]= a.v[0]*b.v[1] - a.v[1]*b.v[0];
	return result;
}

/*
 * make vector unit length;  also, return length of original vector.
 */

double
Normalize(Vector *a)
{
	double length, scale;

	length= DOT(*a,*a);
	length= sqrt(length);
	if(length != 0.0) {
		scale= 1.0/length;
		a->v[0] *= scale;
		a->v[1] *= scale;
		a->v[2] *= scale;
	}
	return length;
}

/*
 * Transform point from world space to model space
 */

Point
InvTransPoint(Point p, Affine *a)
{
	int r, c;
	double sum;
	Point result;

	for(r=0; r < 3; r++) {
		sum= a->inverseTM.m[r][3];
		for(c=0; c < 3; c++)
			sum += a->inverseTM.m[r][c]*p.v[c];
		result.v[r]= sum;
	}
	return result;
}

/*
 * transform vector from world space to model space
 */

Vector
InvTransVector(Vector v, Affine *a)
{
	int r, c;
	double sum;
	Vector result;

	for(r=0; r < 3; r++) {
		sum= 0.0;
		for(c=0; c < 3; c++)
			sum += a->inverseTM.m[r][c]*v.v[c];
		result.v[r]= sum;
	}
	return result;
}

/*
 * transform point from model space to world space
 */

Point
TransPoint(Point p, Affine *a)
{
	int r, c;
	double sum;
	Point result;

	for(r=0; r < 3; r++) {
		sum= a->TM.m[r][3];
		for(c=0; c < 3; c++)
			sum += a->TM.m[r][c]*p.v[c];
		result.v[r]= sum;
	}
	return result;
}

/*
 * transform normal from model space to world space
 */

Vector
TransNormal(Vector n, Affine *a)
{
	int r, c;
	double sum;
	Vector result;

	for(r=0; r < 3; r++) {
		sum= 0.0;
		for(c=0; c < 3; c++)
			sum += a->inverseTM.m[c][r]*n.v[c];
		result.v[r]= sum;
	}
	return result;
}

/*
 * given incident ray and surface normal, compute reflected direction
 * Assumes unit length for incident, normal vectors
 */

Vector
ReflectRay(Vector incident, Vector normal)
{
	Vector reflect;
	double dotIN;

	dotIN= DOT(incident, normal);
	TIMES(reflect, normal, -2.0*dotIN);
	PLUS(reflect, incident, reflect);

	return reflect;
}

/*
 * Compute transmitted direction vector, given incident, normal vectors and IRs.
 * assume incident, normal normalized
 *
 * return 0 if total internal reflection
 *
 * 	assume incident & normal vector are of unit length
 *	index1 and index2 are indicies of refraction on both sides of boundary
 */
int
TransmitRay(Vector incident, Vector normal, double index1, double index2, Vector * transmit)
{
	double dotIN, index, dotTN;
	Vector vtmp;

	dotIN= DOT(incident, normal);
	TIMES(vtmp, normal, dotIN);
	MINUS(vtmp, incident, vtmp);
	index= index1/index2;
	TIMES(vtmp, vtmp, index);
	dotTN= 1.0 - DOT(vtmp, vtmp);
	if(dotTN <= 0.0)
		return 0;	/* total internal reflection */
	
	dotTN= sqrt(dotTN);
	TIMES(*transmit, normal, dotTN);
	if(dotIN < 0.0)
		MINUS(*transmit, vtmp, *transmit)
	else
		PLUS(*transmit, vtmp, *transmit)
	return 1;
}
