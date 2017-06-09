
/*
 *  lat2eps lib 1.2
 *
 *  Coded by Andre de la Rocha. Public domain.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lat2eps_lib.h"


#define MAXL   4096   // Maximum linear dimension of the lattice.
#define MAXQ   16     // Maximum number of different site colors.


static int lattice[MAXL][MAXL];
static int palette[MAXQ] = { 0xFFFFFF, 0x000000, 0xBE2633, 0x44891A, 0x005784, 0xF7E26B, 0xA46422, 0xB2DCEF, 0xEB8931, 0x1B2632, 0xE06F8B, 0x493C2B, 0x2F484E, 0x9D9D9D, 0x31A2F2, 0xA3CE27 };

static int width;
static int height;


static void gen_eps_prolog(FILE *f, int width, int height, int scale, int border);
static void gen_eps_epilog(FILE *f, int width, int height, int scale, int border);
static void gen_eps_lattice(FILE *f, int width, int height);


// Initializes the lattice to width w and height h.
int lat2eps_init(int w, int h)
{
	int x, y;

	if ((w <= 0) || (w > MAXL) || (h <= 0) || (h > MAXL)) {
		return 0;
	}

	width = w;
	height = h;

	for (y = 0; y < height; ++y)
		for (x = 0; x < width; ++x)
			lattice[y][x] = 0;

	return 1;
}


// Sets the lattice site with coordinates x,y to value s.
void lat2eps_set_site(int x, int y, int s)
{
	if ((x >= 0) && (x < width) && (y >= 0) && (y < height)) {
		lattice[y][x] = s;
	}
}


// Sets a color index to a palette entry in the 0xRRGGBB format.
void lat2eps_set_color(int index, int pal)
{
	if ((index >= 0) && (index < MAXQ)) {
		palette[index] = pal;
	}
}


// Generates lattice graphic in EPS.
int lat2eps_gen_eps(const char *filename, int border, int scale)
{
	FILE *f;

	if ((width <= 0) || (width > MAXL) || (height <= 0) || (height > MAXL) || (border < 0) || (scale <= 0)) {
		return 0;
	}

	if (!(f = fopen(filename, "wb"))) {
		return 0;
	}

	gen_eps_prolog(f, width, height, scale, border);
	gen_eps_lattice(f, width, height);
	gen_eps_epilog(f, width, height, scale, border);

	fclose(f);

	return 1;
}


// Generates EPS prolog, including L/P procedures and palette definition.
void gen_eps_prolog(FILE *f, int width, int height, int scale, int border)
{
	int i;
	fprintf(f, "%%!PS-Adobe-2.0 EPSF-2.0\n");
	fprintf(f, "%%%%Creator: lat2eps 1.2\n");
	fprintf(f, "%%%%BoundingBox: %d %d %d %d\n", -border, -border, width * scale + border, height * scale + border);
	fprintf(f, "%%%%EndComments\n");
	fprintf(f, "%%%%BeginProlog\n");
	fprintf(f, "/L { 2 rectfill } def\n");    // Line procedure. The extra row is for adding overlap between lines, to work around an anti-aliasing bug in several ps/pdf viewers that would show glitches otherwise.
	fprintf(f, "/P { 1 2 rectfill } def\n");  // Pixel procedure.
	for (i = 0; i < MAXQ; ++i)
		fprintf(f, "/C%X { %f %f %f setrgbcolor } def\n", i, ((palette[i] >> 16) & 255)/255.0, ((palette[i] >> 8) & 255)/255.0, (palette[i] & 255)/255.0);
	fprintf(f, "%%%%EndProlog\n");
	fprintf(f, "%%%%Page: 1 1\n");
	fprintf(f, "%d %d scale\n", scale, -scale);  // Inverts y axis to be top to bottom.
	fprintf(f, "0 %d translate\n", -height);
}


// Generates EPS epilog.
void gen_eps_epilog(FILE *f, int width, int height, int scale, int border)
{
	if (border > 0) {
		fprintf(f, "0 0 0 setrgbcolor\n");
		fprintf(f, "%f setlinewidth\n", (float)border / scale);
		fprintf(f, "%f %f %f %f rectstroke\n", -0.5 * border / scale, -0.5 * border / scale, width + (float)border / scale, height + (float)border / scale);
	}
}


// Generates lattice graphic in EPS. Each lattice line is run-length encoded, generating a single "line" call for a sequence of adjacent sites of the same type.
void gen_eps_lattice(FILE *f, int width, int height)
{
	int x, y, col;

	for (y = 0; y < height; ++y) {

		for (x = 0; x < width;) {

			int s = lattice[y][x];
			int cnt = 1;

			// Counts the length of a sequence of sites of the same type.
			while ((x + cnt < width) && (lattice[y][x + cnt] == s)) ++cnt;
			
			// Maps any positive or negative site value to one of the available colors.
			col = (s >= 0) ? (s % MAXQ) : (MAXQ - 1 - (-s % MAXQ));
			
			if (cnt > 1)
				fprintf(f, "C%X %d %d %d L\n", col, x, y, cnt);
			else
				fprintf(f, "C%X %d %d P\n", col, x, y);

			x += cnt;
		}
	}
}
