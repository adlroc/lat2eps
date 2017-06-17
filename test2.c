
/*
 *  Sample program that generates a simple lattice data file that can be used as input data to the lat2eps command.
 */


#include <stdio.h>
#include <stdlib.h>


#define L   256


int main(int argc, char *argv[])
{
	int x, y;
	FILE *f;
	
	if ((f = fopen("grid.dat", "w"))) {

		/* Generates embedded commands in the lattice data file that will output text and redefine colors. */
		fprintf(f, "#TXT %d, %d, 0.5, 1, 0, 20, 1, X\n", L/2, L-6);
		fprintf(f, "#TXT 6, %d, 0.5, 0, 90, 20, 6, Y\n", L/2);
		fprintf(f, "#TXT %d, %d, 0.5, 1, 45, 20, 5, My 2D Lattice\n", L/2, L/2);
		fprintf(f, "#PAL 555577, FFFFFF, 334455, FF1122\n");
		fprintf(f, "#COL 2, 00ffff\n");

		/* Makes a square within the lattice with color 2, and sets its diagonal elements to color 3.  */
		/* Sites that are not defined in the data file are output in the EPS with the default color 0. */
		for (y = 0; y < L; ++y) {
			for (x = 0; x < L; ++x) {
				if ((y == 0) || (y == L-1) || (x == 0) || (x == L-1) ) {
					fprintf(f, "%d %d 2\n", x, y);
				} else if ((x == y) || (x == L-1-y)) {
					fprintf(f, "%d %d 3\n", x, y);
				}
			}
		}

		fclose(f);
	}	

	return 0;	
}

