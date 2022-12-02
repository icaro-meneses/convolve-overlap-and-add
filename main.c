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

int
main(void)
{
	int m = 10, l = 4, m0 = 3;
	int n = m + l - 1;

	float *x, *h;
	float* y;

	x					   = create_signal(m, "x(n)");
	h					   = create_signal(l, "h(n)");

	float sample_X_array[] = {1.07f,
							  -4.77f,
							  1.16f,
							  -3.19f,
							  -1.79f,
							  4.47f,
							  -2.99f,
							  3.52f,
							  -4.19f,
							  1.55f};
	float sample_H_array[] = {2.06f, -0.65f, -1.57f, -0.32f};

	for (int i = 0; i < m; i++)
	{
		x[i] = sample_X_array[i];
	}

	for (int i = 0; i < l; i++)
	{
		h[i] = sample_H_array[i];
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

	y = convolve_overlap_and_add(x, h, m, l, m0);

	printf("\n\ny(n) = ");
	for (int i = 0; i < n; i++)
	{
		printf("%2.2f ", y[i]);
	}
	printf("\n\n");

	printf("Outputting the y(n) signal to a file...\n");
	output_file("y_n.txt", y, n);

	free(x);
	free(h);
	free(y);

	return 0;
}

