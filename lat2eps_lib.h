
/*
 *  lat2eps lib 1.2
 *
 *  Coded by Andre de la Rocha. Public domain.
 */


#ifndef _LAT2EPS_H
#define _LAT2EPS_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


#define LAT2EPS_MAXW  4096   // Maximum lattice width
#define LAT2EPS_MAXH  4096   // Maximum lattice height
#define LAT2EPS_MAXQ    16   // Maximum number of different site colors
#define LAT2EPS_MAXT  1024   // Maximum number of text entries


int lat2eps_init(int w, int h);
void lat2eps_set_site(int x, int y, int s);
void lat2eps_set_color(int index, int pal);
void lat2eps_text_out(float x, float y, float ax, float ay, float angle, int size, int color, const char *text);
int lat2eps_gen_eps(const char *filename, int border, int scale);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _LAT2EPS_H

