
/*
 *  lat2eps lib 1.3
 *
 *  Coded by Andre de la Rocha. Public domain.
 */


#ifndef _LAT2EPS_H
#define _LAT2EPS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define LAT2EPS_MAXQ     16   /* Maximum number of different site colors */
#define LAT2EPS_MAXT   2048   /* Maximum number of text entries */
#define LAT2EPS_MAXL  16384   /* Maximum linear dimension of the lattice. */


/**
* Initializes the lattice resources.
* @param maxw Maximum lattice width.
* @param maxh Maximum lattice height.
* @return     Zero for failure, non-zero for success.
*/
int lat2eps_init(unsigned int maxw, unsigned int maxh);


/**
* Releases the lattice resources.
*/
void lat2eps_release();


/**
* Sets the value of a lattice site.
* @param x Horizontal coordinate of the site.
* @param y Vertical coordinate of the site.
* @param s Value to set the site to.
*/
void lat2eps_set_site(unsigned int x, unsigned int y, int s);


/**
* Sets a color index to a palette entry in the 0xRRGGBB format.
* @param index Color index.
* @param pal   Palette entry in the 0xRRGGBB format
*/
void lat2eps_set_color(unsigned int index, unsigned int pal);


/**
* Adds a text entry.
* @param x      X coordinate.
* @param y      Y coordinate.
* @param ax     Horizontal alignment. 0 for left-aligning the text relative to the X coordinate, 0.5 for centering it, 1 for right-aligning, etc.
* @param ax     Vertical alignment. 0 for placing the top of the text on the Y coordinate, 0.5 for centering it, 1 for placing the bottom of the text, etc.
* @param angle  Angle to rotate the text, in degrees (0 for horizontal, left to right text).
* @param size   Font size.
* @param color  Color index.
* @param text   Text string.
* @note         Must be called before lat2eps_gen_eps(). Parentheses must be escaped with backslashes.
*/
void lat2eps_text_out(float x, float y, float ax, float ay, float angle, unsigned int size, unsigned int color, const char *text);


/**
* Generates lattice graphic in EPS.
* @param filename  Name of the EPS file that will be created, or NULL for outputting to stdout.
* @param offx      Offset of the first lattice column that will be saved in the output.
* @param offy      Offset of the first lattice row that will be saved in the output.
* @param width     Width (in sites) of the sublattice that will be saved in the output
* @param height    Height (in sites) of the sublattice that will be saved in the output
* @param border    Width of a border that will be placed in the graphic (0 for no border).
* @param scale     Scale that will be used while generating the graphic (e.g., using 2 will create a 2x2 pixel square for each site).
* @return          Zero for failure, non-zero for success.
*/
int lat2eps_gen_eps(const char *filename, unsigned int offx, unsigned int offy, unsigned int width, unsigned int height, unsigned int border, unsigned int scale);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _LAT2EPS_H */

