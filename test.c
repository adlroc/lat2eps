
/*
 *  Sample program using the lat2eps lib functions.
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lat2eps_lib.h"


#define L   256
#define NC  250
#define NL  6


int main(void)
{
	int x, y, i;
	
	const char *s[NL] = { "A", "B", "C", "D", "E", "F" };

	lat2eps_init(L, L);

	lat2eps_set_color(0, 0x606090);
	lat2eps_set_color(1, 0x00ff00);

	for (y = 0; y < L; ++y) {
		for (x = 0; x < L; ++x) {
			lat2eps_set_site(x, y, y==0||y==L-1||x==0||x==L-1||x==y||x==L-1-y?5:pow(sin(y/80.0), 2)*pow(cos(x/80.0), 2)*15);
		}
	}

	for (i = 0; i < NC; ++i) {
		float rt = (NC-i) / (float)NC;
		float r = 5 + 150 * pow(rt, 0.8);
		float ag = pow(rt, 0.6) * 360*8;
		float ar = (ag+90)/180.0*M_PI;
		float sz = 12 + 20 * rt;
		lat2eps_text_out(L/2 + r * cos(ar), L/2 - r * sin(ar), 0.5, 0.5, ag, (int)(sz+0.5), 1 + i % 15, s[i % NL]);
	}

	lat2eps_gen_eps("test.eps", 0, 0, L, L, 2, 2);
	lat2eps_release();

	return 0;	
}

