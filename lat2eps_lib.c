
/*
 *  lat2eps 1.3
 *
 *  Coded by Andre de la Rocha. Public domain.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lat2eps.h"


#define CREATOR_STR  "lat2eps 1.3"


static int *lattice = NULL;
static unsigned int defpalette[] = { 0xFFFFFF, 0x000000, 0xBE2633, 0x44891A, 0x005784, 0xF7E26B, 0xA46422, 0xB2DCEF, 0xEB8931, 0x1B2632, 0xE06F8B, 0x493C2B, 0x2F484E, 0x9D9D9D, 0x31A2F2, 0xA3CE27 };
static unsigned int palette[LAT2EPS_MAXQ];

static unsigned int maxwidth = 0;
static unsigned int maxheight = 0;
static unsigned int txtcounter = 0;

static struct {
	float x;
	float y;
	float ax;
	float ay;
	float angle;
	unsigned int size;
	unsigned int color;
	char *text;
} textentry[LAT2EPS_MAXT];


/* Private functions */
static void release_resources();
static void gen_eps_prolog(FILE *f, unsigned int width, unsigned int height, unsigned int scale, unsigned int border);
static void gen_eps_epilog(FILE *f, unsigned int width, unsigned int height, unsigned int scale, unsigned int border);
static void gen_eps_lattice(FILE *f, unsigned int offx, unsigned int offy, unsigned int width, unsigned int height);


/* Initializes the lattice resources. */
int lat2eps_init(unsigned int maxw, unsigned int maxh)
{
	unsigned int i;

	release_resources();
	
	if ((maxw > LAT2EPS_MAXL) || (maxh > LAT2EPS_MAXL)) {
		return 0;
	}

	lattice = (int *)calloc((size_t)(maxw * maxh), sizeof(int));

	txtcounter = 0;
	maxwidth = maxw;
	maxheight = maxh;
	
	/* Initializes the initial palette by repeating the colors from the default palette table. */
	for (i = 0; i < LAT2EPS_MAXQ; ++i) {
		palette[i] = defpalette[i % (sizeof(defpalette)/sizeof(defpalette[0]))];
	}

	return 1;
}


/* Releases the lattice resources. */
void lat2eps_release()
{
	release_resources();
}


/* Sets the lattice site with coordinates x,y to value s. */
void lat2eps_set_site(unsigned int x, unsigned int y, int s)
{
	if ((x < maxwidth) && (y < maxheight)) {
		lattice[y * maxwidth + x] = s;
	}
}


/* Sets a color index to a palette entry in the 0xRRGGBB format. */
void lat2eps_set_color(unsigned int index, unsigned int pal)
{
	if (index < LAT2EPS_MAXQ) {
		palette[index] = pal;
	}
}


/* Adds a text entry */
void lat2eps_text_out(float x, float y, float ax, float ay, float angle, unsigned int size, unsigned int color, const char *text)
{
	if ((txtcounter < LAT2EPS_MAXT) && (color < LAT2EPS_MAXQ) && text) {
		textentry[txtcounter].x = x;
		textentry[txtcounter].y = y;
		textentry[txtcounter].ax = ax;
		textentry[txtcounter].ay = ay;
		textentry[txtcounter].angle = angle;
		textentry[txtcounter].size = size;
		textentry[txtcounter].color = color;
		textentry[txtcounter].text = strdup(text);
		txtcounter++;
	}
}


/* Generates lattice graphic in EPS. */
int lat2eps_gen_eps(const char *filename, unsigned int offx, unsigned int offy, unsigned int width, unsigned int height, unsigned int border, unsigned int scale)
{
	FILE *f;

	if ((width == 0) || (offx + width > maxwidth) || (height == 0) || (offy + height > maxheight) || (scale == 0)) {
		return 0;
	}

	if (filename) {
		if (!(f = fopen(filename, "wb"))) {
			return 0;
		}
	} else {
		f = stdout;
	}

	gen_eps_prolog(f, width, height, scale, border);
	gen_eps_lattice(f, offx, offy, width, height);
	gen_eps_epilog(f, width, height, scale, border);

	if (filename) {
		fclose(f);
	}

	return 1;
}


static void release_resources()
{
	unsigned int i;

	free(lattice);
	lattice = NULL;
	maxwidth = 0;
	maxheight = 0;

	for (i = 0; i < txtcounter; ++i) {
		free(textentry[i].text);
	}
	txtcounter = 0;
}


/* Generates EPS prolog, including Line/Pixel/Text procedures and palette definition. */
static void gen_eps_prolog(FILE *f, unsigned int width, unsigned int height, unsigned int scale, unsigned int border)
{
	unsigned int i;

	fprintf(f, "%%!PS-Adobe-2.0 EPSF-2.0\n");
	fprintf(f, "%%%%Creator: %s\n", CREATOR_STR);
	fprintf(f, "%%%%BoundingBox: %d %d %d %d\n", -border, -border, width * scale + border, height * scale + border);
	fprintf(f, "%%%%EndComments\n");
	fprintf(f, "%%%%BeginProlog\n");

	/* Line procedure. The extra row is for adding overlap between lines, to work around an anti-aliasing bug in several ps/pdf viewers that would show glitches otherwise. */
	fprintf(f, "/L { 2 rectfill } def\n");
	/* Pixel procedure. */
	fprintf(f, "/P { 1 2 rectfill } def\n");

	/* Text procedure */
	fprintf(f, "/T { /SS exch def /SZ exch def /RR exch def /AY exch def /AX exch def /YY exch def /XX exch def\n");
	fprintf(f, "/Times-Roman findfont SZ scalefont setfont gsave 1 -1 scale newpath XX YY neg moveto SS true charpath pathbbox exch 4 1 roll sub neg 3 1 roll sub\n");
	fprintf(f, "/WW exch def /HH exch def XX WW RR cos mul AX mul sub HH RR sin mul 1 AY sub mul add\n");
	fprintf(f, "YY neg WW RR sin mul AX mul sub HH RR cos mul 1 AY sub mul sub newpath moveto RR rotate SS show grestore } def\n");

	/* Palette */
	for (i = 0; i < LAT2EPS_MAXQ; ++i) {
		fprintf(f, "/C%X { %f %f %f setrgbcolor } def\n", i, ((palette[i] >> 16) & 255)/255.0, ((palette[i] >> 8) & 255)/255.0, (palette[i] & 255)/255.0);
	}

	fprintf(f, "%%%%EndProlog\n");
	fprintf(f, "%%%%Page: 1 1\n");
	fprintf(f, "%d %d scale\n", scale, -scale);  /* Inverts y axis to be top to bottom. */
	fprintf(f, "0 %d translate\n", -height);
}


/* Generates EPS epilog. */
static void gen_eps_epilog(FILE *f, unsigned int width, unsigned int height, unsigned int scale, unsigned int border)
{
	unsigned int i;

	/* Outputs text entries */
	for (i = 0; i < txtcounter; ++i) {
		fprintf(f, "C%X %f %f %f %f %f %u (%s) T\n", textentry[i].color, textentry[i].x, textentry[i].y, textentry[i].ax, textentry[i].ay, textentry[i].angle, textentry[i].size, textentry[i].text);
	}

	/* Outputs border */
	if (border > 0) {
		fprintf(f, "0 0 0 setrgbcolor\n");
		fprintf(f, "%f setlinewidth\n", (float)border / scale);
		fprintf(f, "%f %f %f %f rectstroke\n", -0.5 * border / scale, -0.5 * border / scale, width + (float)border / scale, height + (float)border / scale);
	}
}


/* Generates lattice graphic in EPS. Each lattice line is run-length encoded, generating a single "line" call for a sequence of adjacent sites of the same type. */
static void gen_eps_lattice(FILE *f, unsigned int offx, unsigned int offy, unsigned int width, unsigned int height)
{
	unsigned int x, y, col;

	for (y = 0; y < height; ++y) {

		for (x = 0; x < width;) {

			int s = lattice[(offy + y) * maxwidth + offx + x];
			unsigned int cnt = 1;

			/* Counts the length of a sequence of sites of the same type. */
			while ((x + cnt < width) && (lattice[(offy + y) * maxwidth + offx + x + cnt] == s)) ++cnt;
			
			/* Maps any positive or negative site value to one of the available colors. */
			col = (s >= 0) ? (s % LAT2EPS_MAXQ) : (LAT2EPS_MAXQ - 1 - (-s % LAT2EPS_MAXQ));
			
			if (cnt > 1)
				fprintf(f, "C%X %u %u %u L\n", col, x, y, cnt);
			else
				fprintf(f, "C%X %u %u P\n", col, x, y);

			x += cnt;
		}
	}
}

