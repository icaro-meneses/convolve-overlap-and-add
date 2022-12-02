/*
 * ======================================================================
 *
 *       Filename:  main.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  24/11/22 22:12:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Icaro Meneses Ferreira de Santana
 *					(Icaro Meneses),
 *					icaro.santana.dev@hotmail.com
 *
 * ======================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "atv_2.h"
#include <math.h>

#define PI_CONST (3.14159265358979323846264338327950288)

int
main(void)
{
	int m = 5096, l = 2, m0 = 128;
	int n = m + l - 1;

	float *x, *h;
	float *e, *r;
	float* y;

	x		   = create_signal(m, "x(n)");
	h		   = create_signal(l, "h(n)");

	e		   = create_signal(m, "e(n)");
	r		   = create_signal(m, "r(n)");

	float freq = 1.0f / 21.0f;

	for (int i = 0; i < m; i++)
	{
		x[i] = cosf(2 * PI_CONST * freq * i);
		e[i] = 0.2f * powf(-1.0f, i);
		r[i] = x[i] + e[i];
	}
	output_file("r_n.txt", r, m);

	for (int i = 0; i < l; i++)
	{
		h[i] = 0.5f;
	}

#ifdef DEBUG_MODE
	printf("x(n) = ");
	for (int i = 0; i < m; i++)
	{
		printf("%2.2f ", x[i]);
	}
	printf("\n");

	printf("h(n) = ");
	for (int i = 0; i < l; i++)
	{
		printf("%2.2f ", h[i]);
	}
	printf("\n\n");
#endif

	y = convolve_overlap_and_add(r, h, m, l, m0);

	printf("\n\ny(n) = ");
	for (int i = 0; i < n; i++)
	{
		printf("%2.2f ", y[i]);
	}
	printf("\n\n");

	printf("Outputting the y(n) and r(n)"
		   "signals to their files...\n");
	output_file("y_n.txt", y, n);

	free(e);
	free(h);
	free(y);

	return 0;
}

