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
	float *x, *h;
	float* y;

	x = (float*)malloc(M * sizeof(float));
	h = (float*)malloc(L * sizeof(float));

	if (x == NULL)
	{
		printf("Error in memory allocation for x(n).\n");
		exit(1);
	}

	else
	{
		float sample_X_array[M] = {1.07f,
								   -4.77f,
								   1.16f,
								   -3.19f,
								   -1.79f,
								   4.47f,
								   -2.99f,
								   3.52f,
								   -4.19f,
								   1.55f};

		for (int i = 0; i < M; i++)
		{
			x[i] = sample_X_array[i];
		}
	}

	if (h == NULL)
	{
		printf("Error in memory allocation for h(n).\n");
		exit(1);
	}

	else
	{
		float sample_H_array[L] = {2.06f, -0.65f, -1.57f, -0.32f};

		for (int i = 0; i < L; i++)
		{
			h[i] = sample_H_array[i];
		}
	}

	printf("x(n) = ");
	for (int i = 0; i < M; i++)
	{
		printf("%2.2f ", x[i]);
	}
	printf("\n");

	printf("h(n) = ");
	for (int i = 0; i < L; i++)
	{
		printf("%2.2f ", h[i]);
	}
	printf("\n\n");

	// y = convolve_overlap_and_add(x, h, M, L, M0);
	y = convolve_block(x, h, M, L);

	printf("\n\ny(n) = ");
	for (int i = 0; i < (M + L - 1); i++)
	{
		printf("%2.2f ", y[i]);
	}
	printf("\n\n");

	free(x);
	free(h);
	free(y);

	return 0;
}

