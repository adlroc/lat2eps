
/*
 *  lat2eps lib 1.2
 *
 *  Coded by Andre de la Rocha. Public domain.
 */


int lat2eps_init(int w, int h);
void lat2eps_set_site(int x, int y, int s);
void lat2eps_set_color(int index, int pal);
int lat2eps_gen_eps(const char *filename, int border, int scale);

