/*
 * ======================================================================
 *
 *       Filename:  atv_2.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  25/11/22 00:24:23
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Icaro Meneses Ferreira de Santana
 *					(Icaro Meneses),
 *					icaro.santana.dev@hotmail.com
 *
 * ======================================================================
 */

#include "atv_2.h"
#include <stdio.h>
#include <stdlib.h>

float*
convolve_block(float* x_block, float* h, int x_size, int h_size)
{
	static float* y_block;
	
	y_block = (float*) malloc(N_SIZE * sizeof(float));
	
	if (y_block == NULL)
	{
		printf("A error ocurred in memory allocation for y block.\n");
		exit(1);
	}

	for (int i = 0; i < N_SIZE; i++)
	{
		y_block[i] = 0.0f;
	}

	int operation_counter = 0;
	int h_index			  = 0;
	int h_initial		  = 0;

	/* Process of convolution */
	for (int y_index = 0; y_index < N_SIZE; y_index++)
	{
		operation_counter = y_index;

		h_index			  = h_initial;
		while (h_index < h_size)
		{
			if (operation_counter > (x_size - 1))
			{
				operation_counter = x_size - 1;
				h_initial++;
				h_index = h_initial;

				continue;
			}

			y_block[y_index] +=
				(x_block[operation_counter] * h[h_index]);

			printf("y_block[%d] += ", y_index);
			printf("x[%d] * h[%d] = ", operation_counter, h_index);
			printf("%.2f * %.2f = %2.2f\n",
				   x_block[operation_counter],
				   h[h_index],
				   y_block[y_index]);

			operation_counter--;
			if (operation_counter < 0)
			{
				break;
			}

			h_index++;
		}
	}
	printf("\n\n");

	return y_block;
}

void
overlap_and_add(float** blocks_of_y, float* y)
{
	int block_number = 0;
	int block_index	 = 1;

	printf("\nStarting the process of Overlapping and Adding...\n\n");

	y[0] = blocks_of_y[0][0];
	printf("y(%d) = y_blocks[%d][%d] = %2.2f\n", 0, 0, 0, y[0]);

	for (int final_index = 1; final_index < (X_PADDED_SIZE - 1);
		 final_index++)
	{
		if ((block_number + 1) > (NUM_BLOCKS - 1))
		{
			break;
		}

		if (final_index % (N_SIZE - 1))
		{
			y[final_index] = blocks_of_y[block_number][block_index];
			printf("y(%d) = y_blocks[%d][%d] = %2.2f\n",
				   final_index,
				   block_number,
				   block_index,
				   y[final_index]);

			block_index++;
		}

		else
		{
			y[final_index] = blocks_of_y[block_number][block_index] +
							 blocks_of_y[block_number + 1][0];
			printf("Overlaping and Adding:\n");
			printf("y(%d) = y_blocks[%d][%d] + y_blocks[%d][%d] = "
				   "%2.2f\n",
				   final_index,
				   block_number,
				   block_index,
				   block_number + 1,
				   0,
				   y[final_index]);

			block_number++;
			block_index = 1;
		}
	}

	y[X_PADDED_SIZE - 1] = blocks_of_y[NUM_BLOCKS - 1][N_SIZE - 1];
}

float*
convolve_overlap_and_add(float* x_n,
						 float* h_n,
						 int x_size,
						 int h_size,
						 int block_size)
{
	float **y_blocks, **x_blocks;
	float* y_n;

	/* Zero Padding for x(n) */
	x_n = realloc(x_n, X_PADDED_SIZE * sizeof(float));

	for (int i = x_size; i < X_PADDED_SIZE; i++)
	{
		x_n[i] = 0.0f;
	}

	/* Allocation for y(n) */
	y_n = (float*)malloc(X_PADDED_SIZE * sizeof(float));

	if (y_n == NULL)
	{
		/* If is'nt possible to allocate memory */
		printf("Error in memory allocation for h(n).\n");
		exit(1);
	}

	else
	{
		/* Initialize with zeros */
		for (int i = 0; i < X_PADDED_SIZE; i++)
		{
			y_n[i] = 0.0f;
		}
	}

	/* Division of x(n) in N blocks */
	x_blocks = (float**)malloc(N_SIZE * sizeof(float*));

	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		x_blocks[i] = (float*)malloc(block_size * sizeof(float));
	}

	/* Separate each M0 blocks in a 2d array */
	for (int block = 0; block < NUM_BLOCKS; block++)
	{
		for (int index = 0; index < block_size; index++)
		{
			x_blocks[block][index] =
				x_n[index + (block_size * block)];
		}
	}

	printf("x_blocks:\n");
	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		for (int j = 0; j < M0; j++)
		{
			printf("%6.2f ", x_blocks[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");

	/* Create a block for each
	 * convolution of x_blocks * h_n -> y_blocks */
	y_blocks = (float**)malloc(NUM_BLOCKS * sizeof(float*));

	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		y_blocks[i] = (float*)malloc(N_SIZE * sizeof(float));
	}

	/* Initialize with zeros */
	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		for (int j = 0; j < N_SIZE; j++)
		{
			y_blocks[i][j] = 0.0f;
		}
	}

	/* Process of convolution of each block */
	for (int block = 0; block < NUM_BLOCKS; block++)
	{
		y_blocks[block] =
			convolve_block(x_blocks[block], h_n, block_size, h_size);
	}

	printf("\n\ny_blocks:\n");
	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		for (int j = 0; j < N_SIZE; j++)
		{
			printf("%6.2f ", y_blocks[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");

	overlap_and_add(y_blocks, y_n);

	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		free(x_blocks[i]);
		free(y_blocks[i]);
	}

	free(y_blocks);

	return y_n;
}
