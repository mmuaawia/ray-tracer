/*
 * does the actual tracing of rays and does anti-aliasing
 * (not fully written)
 *
 *	John Amanatides, Oct 2016
 */


#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "artInternal.h"

extern	Ray ShootRay(double, double);
extern	Color GetRadiance(Ray *);
extern FILE *OpenTIFF(int, int, char *);
extern void CloseTIFF(FILE *);
extern void WritePixelTIFF(FILE *, int, int, int);
extern void set_frandom(long);
extern double frandom();
double checkEdges(double , int , int );

#define INVERSE_GAMMA	(1.0/2.2)

char *
art_Trace(int xRes, int yRes, int numSamples, char *filename)
{
	FILE *fp;
	Ray ray;
	int x, y, red, green, blue;
	double u, v, randomX, randomY;
	Color sample, sampleTemp;
	int i, sampleCount;

	if(xRes < 1 || yRes < 1 || numSamples < 1 )
		return "art_Trace: domain error";
	if ((fp = OpenTIFF(xRes, yRes, filename)) == NULL)
		return "art_Trace: couldn't open output file";

	set_frandom(time(NULL));

	printf("%d ", numSamples);
	/* compute image */
  for(y=0; y < yRes; y++) {
          for(x= 0; x < xRes; x++) {
			sample.v[0] = sample.v[1] = sample.v[2] = 0.0;
			for(int q = 0; q < numSamples; q++)
			{
				for(int w = 0; w < numSamples; w++)
				{
											u= ((double) x + ((double) q/numSamples))/xRes;
											v= 1.0 - ((double) y + ((double) w/numSamples))/yRes;
											ray= ShootRay(u, v);
											PLUS(sample, sample, GetRadiance(&ray));
				}
			}
			TIMES(sample, sample, 1.0/(numSamples*numSamples));

			/* convert to bytes and write out */
			red= 255.0*pow(sample.v[0], INVERSE_GAMMA);
			if(red > 255)
				red= 255;
			else if(red < 0)
				red= 0;
			green= 255.0*pow(sample.v[1], INVERSE_GAMMA);
			if(green > 255)
				green= 255;
			else if(green < 0)
				green= 0;
			blue= 255.0*pow(sample.v[2], INVERSE_GAMMA);
			if(blue > 255)
				blue= 255;
			else if(blue < 0)
				blue= 0;
			WritePixelTIFF(fp, red, green, blue);
			printf("\rPercent Completed: %f%%", ((double) y / yRes * 100));
                }
        }

        CloseTIFF(fp);
	return NULL;
}
//
// double checkEdges(double point, int fmax, int fmin)
// {
// 	if((int)point >= fmax)
// 	{
// 		return fmax;
// 	}
// 	else if((int)point <= fmin)
// 	{
// 		return fmin;
// 	}
// 	else
// 	{
// 		return point;
// 	}
// }
