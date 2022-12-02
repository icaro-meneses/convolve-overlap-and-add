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
	int m = 12, l = 2, m0 = 4;
	int n = m + l - 1;

	float *x, *h;
	float* y;

	x					   = create_signal(m, "x(n)");
	h					   = create_signal(l, "h(n)");

	float sample_X_array[] = {-0.27f,
							  1.06f,
							  1.98f,
							  4.41f,
							  4.01f,
							  3.57f,
							  2.25f,
							  -4.96f,
							  4.39f,
							  -0.40f,
							  2.08f,
							  3.66f};
	float sample_H_array[] = {-1.27f, 1.76f};

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

	free(h);
	free(y);

	return 0;
}

