/*
 * Functions to write RGB image file in uncompressed tiff format.
 * Only works on little-endian computer.
 *
 * John Amanatides, June 2007
 */
#include <stdio.h>

#define LITTLE_ENDIAN		0x4949
#define MAGIC_CONSTANT		42
#define	ZERO_UPPER		0
#define	SHORT_TYPE		3
#define RATIONAL_TYPE		5
#define LONG_TYPE		4

#define IMAGE_WIDTH		256
#define IMAGE_LENGTH		257
#define BITS_PER_SAMPLE		258
#define COMPRESSION		259
#define NO_COMPRESSION		1
#define PHOTOMETRIC_INTERP	262
#define RGB			2
#define STRIP_OFFSETS		273
#define SAMPLES_PER_PIXEL	277
#define ROWS_PER_STRIP		278
#define STRIP_BYTE_COUNTS	279
#define X_RESOLUTION		282
#define Y_RESOLUTION		283
#define RESOLUTION_UNIT		296
#define	INCH			2

FILE *OpenTIFF(int xRes, int yRes, char *fileName)
{
	FILE *fp;
	short int header[100];
	int byteCount;

	if((fp= fopen(fileName, "wb")) == NULL) {
		fprintf(stderr, "sorry, could not open '%s'\n", fileName);
		return NULL;
	}

	/* determine TIFF header */
	header[0] = LITTLE_ENDIAN;
	header[1] = MAGIC_CONSTANT;
	header[2] = 8;	/* byte offset of start of IFD */
	header[3] = ZERO_UPPER;
	
	header[4] = 12;     /* number of required fields */
	
	/* each field has:
		2-byte tag
		2-byte type
		4-byte count of the number of types
		4-byte value or value offset (depending on type & count)
	*/

	header[5] = IMAGE_WIDTH;
	header[6] = SHORT_TYPE;
	header[7] = 1;
	header[8] = ZERO_UPPER;
	header[9] = xRes;
	header[10] = ZERO_UPPER;

	header[11] = IMAGE_LENGTH;
	header[12] = SHORT_TYPE;
	header[13] = 1;
	header[14] = ZERO_UPPER;
	header[15] = yRes;
	header[16] = ZERO_UPPER;

	header[17] = BITS_PER_SAMPLE;
	header[18] = SHORT_TYPE;
	header[19] = 3;
	header[20] = ZERO_UPPER;
	header[21] = 79*2;
	header[22] = ZERO_UPPER;

	header[23] = COMPRESSION;
	header[24] = SHORT_TYPE;
	header[25] = 1;
	header[26] = ZERO_UPPER;
	header[27] = NO_COMPRESSION;
	header[28] = ZERO_UPPER;

	header[29] = PHOTOMETRIC_INTERP;
	header[30] = SHORT_TYPE;
	header[31] = 1;
	header[32] = ZERO_UPPER;
	header[33] = RGB;
	header[34] = ZERO_UPPER;

	header[35] = STRIP_OFFSETS;
	header[36] = LONG_TYPE;
	header[37] = 1;
	header[38] = ZERO_UPPER;
	header[39] = 90*2;
	header[40] = ZERO_UPPER;

	header[41] = SAMPLES_PER_PIXEL;
	header[42] = SHORT_TYPE;
	header[43] = 1;
	header[44] = ZERO_UPPER;
	header[45] = 3;
	header[46] = ZERO_UPPER;

	header[47] = ROWS_PER_STRIP;
	header[48] = LONG_TYPE;
	header[49] = 1;
	header[50] = ZERO_UPPER;
	header[51] = 1;
	header[52] = yRes;

	byteCount= xRes*yRes*3;
	header[53] = STRIP_BYTE_COUNTS;
	header[54] = LONG_TYPE;
	header[55] = 1;
	header[56] = ZERO_UPPER;
	header[57] = byteCount & 0xffff;
	header[58] = byteCount >> 16;

	header[59] = X_RESOLUTION;
	header[60] = RATIONAL_TYPE;
	header[61] = 1;
	header[62] = ZERO_UPPER;
	header[63] = 82*2;
	header[64] = ZERO_UPPER;

	header[65] = Y_RESOLUTION;
	header[66] = RATIONAL_TYPE;
	header[67] = 1;
	header[68] = ZERO_UPPER;
	header[69] = 86*2;
	header[70] = ZERO_UPPER;

	header[71] = RESOLUTION_UNIT;
	header[72] = SHORT_TYPE;
	header[73] = 1;
	header[74] = ZERO_UPPER;
	header[75] = INCH;
	header[76] = ZERO_UPPER;

	header[77] = 0;      /* Next IFD offset */
	header[78] = 0;

	header[79] = 8;      /* Bits Per Sample */
	header[80] = 8;
	header[81] = 8;

	header[82] = 100;      /* X Resolution */
	header[83] = ZERO_UPPER;
	header[84] = 1;
	header[85] = ZERO_UPPER;

	header[86] = 100;      /* Y Resolution */
	header[97] = ZERO_UPPER;
	header[88] = 1;
	header[89] = ZERO_UPPER;

	fwrite(header, sizeof(short int), 90, fp);
	return fp;
}

void
WritePixelTIFF(FILE *fp, int r, int g, int b)
{
	fputc(r, fp);
	fputc(g, fp);
	fputc(b, fp);
}

void
CloseTIFF(FILE *fp)
{
	(void) fclose(fp);
}
