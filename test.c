
/*
 *  Sample program using the lat2eps lib functions.
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <lat2eps.h>

#define L   512
#define NCHARS  250

int main(int argc, char *argv[])
{
	int x, y, i;

	/* Initializes liblat2eps for a lattice of width L and height L. */
	if (!lat2eps_init(L, L)) return -1;

	lat2eps_set_color(9, 0x6070a0);  /* Sets color number 9 to red:60 green:70 blue:a0 */
	lat2eps_set_color(10, 0x00aa00);

	/* Fills the lattice with a nice pattern using simple math. */
	for (y = 0; y < L; ++y) {
		for (x = 0; x < L; ++x) {
			float xx = x * 6.0 / L;
			float yy = y * 6.0 / L;
			lat2eps_set_site(x, y, (int)((sin(xx) + sin(yy/2) + sin((xx+yy)/2) + sin(sqrt(xx*xx + yy*yy))) * 5));
		}
	}

	/* Generates an spiral of text. */
	for (i = 0; i < NCHARS; ++i) {
		char s[] = "A";
		float ratio = (NCHARS - i) / (float)NCHARS;
		float radius = 5 + 200 * pow(ratio, 0.8);
		float txtang = pow(ratio, 0.6) * 360 * 8;
		float spang = (txtang + 90) / 180.0 * M_PI;
		int fontsize = (int)(15.5 + 25 * ratio);
		s[0] += i % 6;
		lat2eps_text_out(L/2 + radius * cos(spang), L/2 - radius * sin(spang), 0.5, 0.5, txtang, fontsize, i % LAT2EPS_MAXQ, s);
	}

	/* Puts some text tags that can be replaced with LaTex text using PSfrag. */
	lat2eps_text_out(10, 10, 0, 0, 0, 25, 1, "TAG1");
	lat2eps_text_out(L-10, 10, 1, 1, 90, 25, 0, "TAG2");  /* Places text at x:L-10 y:10, anchored to bottom-right (ax:1.0, ay:1.0), rotated 90 degrees CCW, with font size 25, in color number 0 */
	lat2eps_text_out(90, L-10, 0, 0, 55, 25, 1, "TAG3");

	/* Generates an EPS file with the entire lattice (beginning at site 0,0 with width L and height L), with border width 1 and scale 1. */
	lat2eps_gen_eps("test.eps", 0, 0, L, L, 1, 1); 
	lat2eps_release();  /* Releases resources allocated in lat2eps_init. */

	return 0;	
}

