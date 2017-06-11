#include <stdio.h>


#define N 256


int main(void)
{
	for (int y = 0; y < N; ++y)
		for (int x = 0; x < N; ++x)
			printf("%d %d %d\n", x, y, y==0||y==N-1||x==0||x==N-1||x==y||x==N-y-1?2:0);

	return 0;
}

