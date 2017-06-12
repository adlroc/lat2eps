
/*
 *  Sample program using the lat2eps lib functions.
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lat2eps.h"

#define L   512
#define NC  250

int main(int argc, char *argv[])
{
	int x, y, i;

	if (!lat2eps_init(L, L)) return -1;

	lat2eps_set_color(0, 0x606090);
	lat2eps_set_color(1, 0x00ff00);

	for (y = 0; y < L; ++y) {
		for (x = 0; x < L; ++x) {
			float xx = x * 6.0 / L;
			float yy = y * 6.0 / L;
			lat2eps_set_site(x, y, (int)((sin(xx) + sin(yy/2) + sin((xx+yy)/2) + sin(sqrt(xx*xx+yy*yy))) * 5));
		}
	}

	for (i = 0; i < NC; ++i) {
		char s[] = "A";
		float rt = (NC-i) / (float)NC;
		float r = 5 + 200 * pow(rt, 0.8);
		float ag = pow(rt, 0.6) * 360*8;
		float ar = (ag+90)/180.0*M_PI;
		float sz = 15 + 25 * rt;
		s[0] += i % 6;
		lat2eps_text_out(L/2 + r * cos(ar), L/2 - r * sin(ar), 0.5, 0.5, ag, (int)(sz+0.5), 1 + i % 15, s);
	}

	lat2eps_gen_eps("test.eps", 0, 0, L, L, 1, 1);
	lat2eps_release();

	return 0;	
}

