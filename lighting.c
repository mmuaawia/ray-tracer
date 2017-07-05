/*
 * deals with lights/shading functions
 *
 *	John Amanatides, Oct 2016
 */


#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include "artInternal.h"

#define CHECKERBOARD    1
#define ZONE_PLATE      2


#define MAX_RECURSION	10

extern double	Normalize(Vector *);
extern Vector	ReflectRay(Vector, Vector);
extern int	IntersectScene(Ray *, double *, Vector *, Material *);
extern int	ShadowProbe(Ray *, double);
extern int TransmitRay(Vector, Vector, double, double, Vector *);
extern double frandom();

typedef struct LightNode {
	Point position;
	double intensity;
	double radius;
	struct LightNode *next;
} LightNode;

static LightNode *lights;
static Color	background;
static Material	currentMaterial;
static Color black= {0.0, 0.0, 0.0}, white= {1.0, 1.0, 1.0};


char *
art_Light(double x, double y, double z, double intensity, double radius)
{
	LightNode *newLight;

	if(intensity <= 0.0 || radius < 0.0)
		return "art_Light: domain error";
	newLight= (LightNode *) malloc(sizeof(LightNode));
	newLight->position.v[0]= x;
	newLight->position.v[1]= y;
	newLight->position.v[2]= z;
	newLight->intensity= intensity;
	newLight->radius= radius;
	newLight->next= lights;
	lights= newLight;

	return NULL;
}


char *
art_Material(Material material)
{
	currentMaterial= material; /* should really check for mistakes */
	return NULL;
}


Material
GetCurrentMaterial(void)
{
	return currentMaterial;
}


char *
art_Background(Color color)
{
	if(color.v[0] < 0.0 || color.v[0] > 1.0 || color.v[1] < 0.0 || color.v[1] > 1.0 || color.v[2] < 0.0 || color.v[2] > 1.0)
		return "art_Background: domain error";
	background= color;
	return NULL;
}

/* for A4 */
static Color
Texture(Material *material, Point position)
{               
	int funnySum;
	double EPSILON= 0.0001;
	double contribution;
	Color result;

	switch(material->texture) {

	case CHECKERBOARD: 
		funnySum= floor(position.v[0]+EPSILON)
			+ floor(position.v[1]+EPSILON)
			+ floor(position.v[2]+EPSILON);
		if(funnySum % 2)
			return white;
		else    return material->col;
	case ZONE_PLATE:
		contribution= 0.5*cos(DOT(position, position))+0.5;
		TIMES(result, material->col, contribution);
		return result;  
	default:                
	return material->col;
	}       
}       

/*
 * a simple shader
 */
static Color
ComputeRadiance(Ray *ray, double t, Vector normal, Material material, int recursionDepth)
{	
	Color currCol = white;
		
	Vector currentPosition;
	Color surfaceColor, diff, spec, reflected, transmitted;
	Ray shadowRay;
	Vector toLight, Reflected;
	double theta, alpha, distancetoLight, diffFrac, specFrac, lightFrac;
	LightNode *light;
	Point lightposs;
	Point randomp;
	(void) Normalize(&normal);
	//get intersection
	TIMES(currentPosition, ray->direction, t);
	PLUS(currentPosition, ray->origin, currentPosition);

	if(material.texture != 0)
	{
		surfaceColor = Texture(&material, currentPosition);
	}
	else
	{
		surfaceColor = material.col;
	}

	//ambient
	TIMES(currCol, surfaceColor, material.Ka);
	Reflected = ReflectRay(ray->direction, normal);
	
	for(light = lights ; light != NULL ; light = light->next){
		//printf("%f ", (light->radius));
		randomp.v[0] = (2*frandom() -1.0) * light->radius;
		randomp.v[1] = (2*frandom() -1.0) * light->radius;
		randomp.v[2] = (2*frandom() -1.0) * light->radius;
		PLUS(lightposs, light->position, randomp);
		MINUS(toLight, lightposs, currentPosition);
		distancetoLight = Normalize(&toLight);
		shadowRay.origin = currentPosition;
		shadowRay.direction = toLight;
		theta = DOT(toLight, normal);
		
		//Diffuse
		if(theta > 0.0 && ShadowProbe(&shadowRay, distancetoLight) != HIT){
			//needed for accurate specular, as intensity decreases by r^2.
			lightFrac = light -> intensity / (distancetoLight * distancetoLight);
			diffFrac = lightFrac *material.Kd*theta;
			TIMES(diff, surfaceColor, diffFrac);
			//diffuse
			PLUS( currCol, diff, currCol);
		}
		
		//Specular
		alpha = DOT(toLight, Reflected);
		if (alpha > 0.0 && ShadowProbe(&shadowRay, distancetoLight) != HIT){
			specFrac = lightFrac * material.Ks*pow(alpha, (double) material.n);
			// Assuming no metals.
			TIMES (spec, white, specFrac);
			//specular
			PLUS(currCol, spec, currCol);
		}
		
		//Reflection and refraction
		if(recursionDepth < MAX_RECURSION)
		{
			double outToIn = DOT(Reflected, normal);
			if(material.Kt > 0.0)
			{
				double transmitT;
				Vector transmitNormal;
				Material transmitMaterial;
				Ray transmitRay;
				double index1, index2;

				transmitRay.origin = currentPosition;
				if(outToIn > 0.0)
				{
					index1 = 1.0;
					index2 = material.index;
				}
				else
				{
					index1 = material.index;
					index2 = 1.0;
				}
				int tir = TransmitRay(ray->direction, normal, index1, index2, &(transmitRay.direction));
				if(tir != 0 && IntersectScene(&transmitRay, &transmitT, &transmitNormal, &transmitMaterial) == HIT)
				{
					Color transmitColor = ComputeRadiance(&transmitRay, transmitT, transmitNormal, transmitMaterial, recursionDepth + 1);
					TIMES(currCol, transmitColor, material.Kt);
				}
			}

			if(material.Kr > 0.0)
			{
				double reflectT;
				Vector reflectNormal;
				Material reflectMaterial;
				Ray reflectRay;
	
				reflectRay.origin = currentPosition;
				reflectRay.direction = ReflectRay(ray->direction, normal);
				reflectRay.direction = ReflectRay(ray->direction, normal);
				if(IntersectScene(&reflectRay, &reflectT, &reflectNormal, &reflectMaterial) == HIT)
				{
					Color reflectColor = ComputeRadiance(&reflectRay, reflectT, reflectNormal, reflectMaterial, recursionDepth + 1);
					TIMES(currCol, reflectColor, material.Kr);
				}
			} 
		}
	}
	return currCol;
}


Color
GetRadiance(Ray *ray)
{
	double t;
	Vector normal;
	Material material;

	if(IntersectScene(ray, &t, &normal, &material) == HIT)
		return ComputeRadiance(ray, t, normal, material, 0);
	else	return background;
}


void InitLighting()
{
	Material material;

	material.col= white;
	material.Ka= 0.2;
	material.Kd= 0.6;
	material.Ks= 0.7;
	material.n= 50.0;
	material.Kr= 0.0;
	material.Kt= 0.0;
	material.index= 1.0;
	(void) art_Material(material);
	(void) art_Background(black);

	lights= NULL;
}


void FinishLighting()
{
	LightNode *node;

	while(lights) {
		node= lights;
		lights= lights->next;

		free((void *) node);
	}
}

