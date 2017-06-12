
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


/* Shows usage info. */
void print_usage()
{
	fprintf(stderr, "Usage: lat2eps <off x> <off y> <width> <height> <border> <scale> [hex palette]\n");
}


/* Reads a single line of text data and extracts x, y, s. Returns 1 if data ok, 0 for EOF. */
int get_data(FILE *f, int *x, int *y, int *s)
{
	char buffer[1024];
	
	while (fgets(buffer, sizeof(buffer), f)) {

		char *p = buffer, *tok;
		while (isblank(*p)) ++p;

		/* ignores lines beginning with # */
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


/* Reads the lattice data. */
void read_lattice(FILE *f, int offx, int offy, int width, int height)
{
	int x, y, s;
	
	while (get_data(f, &x, &y, &s)) {
		if ((x >= offx) && (x < offx + width) && (y >= offy) && (y < offy + height)) {
			lat2eps_set_site(x-offx, y-offy, s);
		}
	}
}


int main(int argc, char *argv[])
{
	int arg, offx, offy, width, height, border, scale, i;

	if (argc < 7) {
		print_usage();
		return -1;
	}
	
	arg = 1;
	
	offx = atoi(argv[arg++]);
	offy = atoi(argv[arg++]);
	width = atoi(argv[arg++]);
	height = atoi(argv[arg++]);
	border = atoi(argv[arg++]);
	scale = atoi(argv[arg++]);
	
	if ((width <= 0) || (width > LAT2EPS_MAXL) || (height <= 0) || (height > LAT2EPS_MAXL) || (border < 0) || (scale <= 0)) {
		print_usage();
		return -1;
	}

	if (!lat2eps_init(width, height)) {
		fprintf(stderr, "Initialization error.\n");
		return -1;
	}

	for (i = 0; i < LAT2EPS_MAXQ; ++i) {
		if (arg < argc) {
			lat2eps_set_color(i, (unsigned int)strtol(argv[arg++], NULL, 16));
		}
	}

	read_lattice(stdin, offx, offy, width, height);

	if (!lat2eps_gen_eps(NULL, 0, 0, width, height, border, scale)) {
		lat2eps_release();
		fprintf(stderr, "EPS generation error.\n");
		return -1;
	}

	lat2eps_release();

	return 0;
}

