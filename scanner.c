/*
 * textual front end for art; scan input and call appropriate functions
 *
 *	John Amanatides, Nov 2010
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "scanner.h"
#include "art.h"


#define MAX_IN_DEPTH	8
#define LINE_LENGTH	256
#define OUTPUT(string) if(input == stdin) ((void) printf(string))

/*
 * decode first word of string
 */
static int
DecodeWord(Words *list, char *string)
{
	int i, result;
	char command[LINE_LENGTH];

	if(string == NULL || *string == '\n')
		return COMMENT;
	(void) sscanf(string, "%s", command);
	
	result= ERROR;
	for(i=0;list[i].word != NULL && result == ERROR; i++) {
		if(strcmp(command,list[i].word) == 0)
			result= list[i].code;
	}
	return result;
}


/*
 * perform scanning of input
 */
static void
Dialogue(FILE *input, int indirectDepth)
{
	FILE *newFile;
	char line[LINE_LENGTH], stringArg[LINE_LENGTH];
	char *result;
	double x, y, z;
	double f, t, r;
	int ix, iy, n;
	char axis1[2], axis2[2];
	Point from, to;
	Vector up;
	Color color;
	Material material;

	while(1) {
		result = NULL;
		OUTPUT("? ");
		if(fgets(line, LINE_LENGTH, input) == NULL)
			return;	/* End of File */

		switch(DecodeWord(Commands, line)) {

		case ERROR:
		default:
			(void) printf("Eh?\n");
			break;
		case QUIT:
			return;
		case COMMENT:
			break;
		case INCLUDE:
			if(indirectDepth >= MAX_IN_DEPTH) {
				(void) printf("Sorry, the depth of indirection is too great.\n");
				break;
			}
			(void) sscanf(line,"%*s %s", stringArg);
			if((newFile= fopen(stringArg,"r")) == NULL)
				(void) printf("Sorry, cannot open file '%s' for input.\n",stringArg);
			else	{
				Dialogue(newFile, indirectDepth+1);
				(void) fclose(newFile);
			}
			break;

		case SCALE:
			(void) sscanf(line, "%*s %lf %lf %lf", &x, &y, &z);
			result= art_Scale(x, y, z);
			break;
		case ROTATE:
			(void) sscanf(line, "%*s %1s %lf", axis1, &x);
			result= art_Rotate(axis1[0], x);
			break;
		case TRANSLATE:
			(void) sscanf(line, "%*s %lf %lf %lf", &x, &y, &z);
			result= art_Translate(x, y, z);
			break;
		case SHEAR:
			(void) sscanf(line,"%*s %1s %1s %lf", axis1, axis2, &x);
			result= art_Shear(axis1[0], axis2[0], x);
			break;
		case INIT_TM:
			result= art_InitTM();
			break;
		case PUSH_TM:
			result= art_PushTM();
			break;
		case POP_TM:
			result= art_PopTM();
			break;
		case CAMERA:
			up.v[0]= 0.0;
			up.v[1]= 1.0;
			up.v[2]= 0.0;
			(void) sscanf(line, "%*s %lf %lf %lf %lf %lf %lf %lf %lf %lf",
				&from.v[0], &from.v[1], &from.v[2],
				&to.v[0], &to.v[1], &to.v[2],
				&up.v[0], &up.v[1], &up.v[2]);
			result= art_Camera(from, to, up);
			break;
		case LENS:
			(void) sscanf(line, "%*s %lf %lf", &f, &r);
			result= art_Lens(f, r);
			break;
		case PERSPECTIVE:
			(void) sscanf(line, "%*s %lf %lf %lf", &x, &y, &z);
			result= art_Perspective(x, y, z);
			break;
		case SPHERE:
			result= art_Sphere();
			break;
		case PLANE:
			result= art_Plane();
			break;
		case CUBE:
			result= art_Cube();
			break;
		case CYLINDER:
			result= art_Cylinder();
			break;
		case LIGHT:
			r= 0.0;
			(void) sscanf(line, "%*s %lf %lf %lf %lf %lf", &x, &y, &z, &f, &r);
			result= art_Light(x, y, z, f, r);
			break;
		case BACKGROUND:
			(void) sscanf(line, "%*s %lf %lf %lf",
					&color.v[0], &color.v[1], &color.v[2]);
			result= art_Background(color);
			break;
		case MATERIAL:
			material.Kr= 0.0;
			material.Kt= 0.0;
			material.index= 1.0;
			material.texture= 0;
			(void) sscanf(line, "%*s %lf %lf %lf  %lf %lf %lf %lf  %lf %lf %lf %d",
					&material.col.v[0], &material.col.v[1], &material.col.v[2], 
					&material.Ka, &material.Kd, &material.Ks, &material.n,
					&material.Kr, &material.Kt, &material.index,
					&material.texture);
			result= art_Material(material);
			break;
		case TRACE:
			n= 1;
			(void) sscanf(line, "%*s %d %d %s %d", &ix, &iy, stringArg, &n);
			result= art_Trace(ix, iy, n, stringArg);
			break;
		}
		if (result != NULL)
			(void) fprintf(stderr, "Error in art command-  %s\n", result);
	}
}

int
main()
{
	(void) art_Start();
	Dialogue(stdin, 1);
	(void) art_End();
	return 0;
}
