#include "lat2eps_lib.h"

#define L 256

int main(void)
{
	int x, y;
	
	lat2eps_init(L, L);

	lat2eps_set_color(0, 0x606090);
	lat2eps_set_color(1, 0x00ff00);

	for (y = 0; y < L; ++y)
		for (x = 0; x < L; ++x)
			lat2eps_set_site(x, y, y==0||y==L-1||x==0||x==L-1||x==y||x==L-1-y?1:0);

	lat2eps_gen_eps("test.eps", 2, 2);

	return 0;	
}

