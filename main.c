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
	int m = 25, l = 2, m0 = 7;
	int n = m + l - 1;

	float *x, *h;
	float* y;

	x					   = create_signal(m, "x(n)");
	h					   = create_signal(l, "h(n)");

	float sample_X_array[] = {-1.86f, -1.62f, 4.41f, 1.40f,	 -3.40f,
							  1.47f,  0.64f,  0.59f, 4.70f,	 3.57f,
							  -0.43f, 4.20f,  2.04f, -2.83f, -4.65f,
							  0.99f,  -1.98f, 1.67f, -4.67f, 4.18f,
							  -4.37f, 0.68f,  1.79f, 0.63f,	 -3.85f};
	float sample_H_array[] = {3.83f, 0.01f};

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

