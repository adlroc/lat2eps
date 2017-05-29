
/*
 *  lat2eps 1.2
 *
 *  Coded by Andre de la Rocha. Public domain.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAXL   4096   // Maximum linear dimension of the lattice.
#define MAXQ   16     // Maximum number of different site colors.


static int lattice[MAXL][MAXL];
static int palette[MAXQ] = { 0xFFFFFF, 0x000000, 0xBE2633, 0x44891A, 0x005784, 0xF7E26B, 0xA46422, 0xB2DCEF, 0xEB8931, 0x1B2632, 0xE06F8B, 0x493C2B, 0x2F484E, 0x9D9D9D, 0x31A2F2, 0xA3CE27 };


void print_usage()
{
	fprintf(stderr, "Usage: lat2eps <off x> <off y> <width> <height> <border> <scale> [hex palette]\n");
}


// Reads a single line of text data and extracts x, y, s. Returns 1 if data ok, 0 for EOF.
int get_data(FILE *f, int *x, int *y, int *s)
{
	char buffer[100];
	
	while (fgets(buffer, sizeof(buffer), f)) {

		char *p = buffer, *tok;
		while (isblank(*p)) ++p;

		// ignores lines beginning with #
		if (*p && *p != '#') {

			int c = 0;
			
			while ((tok = strsep(&p, " \t\n\r"))) {
				if (strlen(tok) > 0) {
					switch (c++) {
						case 0:
							*x = atoi(tok);
							break;
						case 1:
							*y = atoi(tok);
							break;
						case 2:
							*s = atoi(tok);
							return 1;
					}
				}
			}
		}
	}
	
	return 0;
}


// Reads the lattice data.
void read_lattice(FILE *f, int offx, int offy, int width, int height)
{
	int x, y, s;
	
	while (get_data(f, &x, &y, &s)) {
		if ((x >= offx) && (x < offx + width) && (y >= offy) && (y < offy + height)) {
			lattice[y-offy][x-offx] = s;
		}
	}
}


// Generates EPS prolog, including L/P procedures and palette definition.
void gen_eps_prolog(FILE *f, int width, int height, int scale, int border)
{
	fprintf(f, "%%!PS-Adobe-2.0 EPSF-2.0\n");
	fprintf(f, "%%%%Creator: lat2eps 1.2\n");
	fprintf(f, "%%%%BoundingBox: %d %d %d %d\n", -border, -border, width * scale + border, height * scale + border);
	fprintf(f, "%%%%EndComments\n");
	fprintf(f, "%%%%BeginProlog\n");
	fprintf(f, "/L { 2 rectfill } def\n");    // Line procedure. The extra row is for adding overlap between lines, to work around an anti-aliasing bug in several ps/pdf viewers that would show glitches otherwise.
	fprintf(f, "/P { 1 2 rectfill } def\n");  // Pixel procedure.
	for (int i = 0; i < MAXQ; ++i)
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
	for (int y = 0; y < height; ++y) {

		for (int x = 0; x < width;) {

			int s = lattice[y][x];
			int cnt = 1;

			// Counts the length of a sequence of sites of the same type.
			while ((x + cnt < width) && (lattice[y][x + cnt] == s)) ++cnt;
			
			// Maps any positive or negative site value to one of the available colors.
			int col = (s >= 0) ? (s % MAXQ) : (MAXQ - 1 - (-s % MAXQ));
			
			if (cnt > 1)
				fprintf(f, "C%X %d %d %d L\n", col, x, y, cnt);
			else
				fprintf(f, "C%X %d %d P\n", col, x, y);

			x += cnt;
		}
	}
}


int main(int argc, char *argv[])
{
	if (argc < 7) {
		print_usage();
		return -1;
	}
	
	int arg = 1;
	
	int offx = atoi(argv[arg++]);
	int offy = atoi(argv[arg++]);
	int width = atoi(argv[arg++]);
	int height = atoi(argv[arg++]);
	int border = atoi(argv[arg++]);
	int scale = atoi(argv[arg++]);
	
	if ((width <= 0) || (width > MAXL) || (height <= 0) || (height > MAXL) || (border < 0) || (scale <= 0)) {
		print_usage();
		return -1;
	}

	for (int i = 0; i < MAXQ; ++i) {
		if (arg < argc) {
			palette[i] = (int)strtol(argv[arg++], NULL, 16);
		}
	}

	read_lattice(stdin, offx, offy, width, height);
	gen_eps_prolog(stdout, width, height, scale, border);
	gen_eps_lattice(stdout, width, height);
	gen_eps_epilog(stdout, width, height, scale, border);

	return 0;
}

