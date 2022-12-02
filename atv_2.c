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
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

float*
create_signal(int signal_size, char* signal_name)
{
	float* signal_pointer;

	signal_pointer = (float*)malloc(signal_size * sizeof(float));

	if (signal_pointer == NULL)
	{
		printf("Error in memory allocation for %s array.\n",
			   signal_name);
		exit(1);
	}

	else
	{
		for (int i = 0; i < signal_size; i++)
		{
			signal_pointer[i] = 0.0f;
		}
	}

	return signal_pointer;
}

void
sum_signals(float* signal_A,
			float* signal_B,
			float* signal_result,
			int size_result)
{
	for (int i = 0; i < size_result; i++)
	{
		signal_result[i] = signal_A[i] + signal_B[i];
	}
}

float*
convolve_block(float* x_block,
			   float* h,
			   int x_size,
			   int h_size,
			   int block_size)
{
	static float* y_block;

	y_block = (float*)malloc(block_size * sizeof(float));

	if (y_block == NULL)
	{
		printf("A error ocurred in memory allocation for y block.\n");
		exit(1);
	}

	for (int i = 0; i < block_size; i++)
	{
		y_block[i] = 0.0f;
	}

	int operation_counter = 0;
	int h_index			  = 0;
	int h_initial		  = 0;

	/* Process of convolution */
	for (int y_index = 0; y_index < block_size; y_index++)
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
overlap_and_add(float** blocks_of_y,
				float* y,
				int y_size,
				int y_block_size,
				int block_size)
{
	int block_number		= 0;
	int block_index			= 0;
	int overlap_add_counter = y_block_size - block_size;
	bool overlap_add_flag	= true;

	int num_blocks			= y_size / block_size;

	printf("\nStarting the process of Overlapping and Adding...\n");
	printf("block_size = %d\t", block_size);
	printf("num_blocks = %d\t", num_blocks);
	printf("y_block_size = %d\n\n", y_block_size);

	int y_index = 0;
	while (y_index < block_size)
	{
		y[y_index] = blocks_of_y[block_number][block_index];
		printf("y(%d) = y_blocks[%d][%d] = %2.2f\n",
			   y_index,
			   block_number,
			   block_index,
			   y[y_index]);

		y_index++;
		block_index++;
	}

	/* Overlap and Add */
	while (overlap_add_flag)
	{
		if (block_index > (y_block_size - 1))
		{
			block_number++;
			block_index = block_size;
			// overlap_add_flag = false;
		}

		if (y_index > y_size - 1)
		{
			printf("Finished overlapping and Add..\n\n");
			overlap_add_flag = false;
		}

		else
		{
			y[y_index] = blocks_of_y[block_number][block_index] +
						 blocks_of_y[block_number + 1]
									[block_index % block_size];
			printf("Overlap and Adding: blk_idx=%d "
				   "counter=%d\n",
				   block_index,
				   overlap_add_counter);
			printf("y(%d) = y_blocks[%d][%d] + y_blocks[%d][%d] = ",
				   y_index,
				   block_number,
				   block_index,
				   block_number + 1,
				   (block_index % block_size));
			printf("%.2f + %.2f = %.2f\n",
				   blocks_of_y[block_number][block_index],
				   blocks_of_y[block_number + 1]
							  [block_index % block_size],
				   y[y_index]);

			y_index++;
			block_index++;
		}
	}

	y[y_index] = blocks_of_y[block_number][block_index];
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

	int x_padded_size = x_size + (block_size - (x_size % block_size));
	int conv_block_size = block_size + h_size - 1;
	int num_blocks		= x_padded_size / block_size;
	int y_total_size	= x_size + h_size - 1;

	printf("x(n) size: %d\nconv_block_size: %d\nnum_blocks: %d\n\n",
		   x_padded_size,
		   conv_block_size,
		   num_blocks);

	/* Zero Padding for x(n) */
	x_n = realloc(x_n, x_padded_size * sizeof(float));

	for (int i = x_size; i < x_padded_size; i++)
	{
		x_n[i] = 0.0f;
	}

	printf("ZERO PADDING:\nx(n) = ");
	for (int i = 0; i < x_padded_size; i++)
	{
		printf("%2.2f ", x_n[i]);
	}
	printf("\n\n");

	/* Allocation for y(n) */
	y_n = (float*)malloc(y_total_size * sizeof(float));

	if (y_n == NULL)
	{
		/* If isn't possible to allocate memory */
		printf("Error in memory allocation for h(n).\n");
		exit(1);
	}

	else
	{
		/* Initialize with zeros */
		for (int i = 0; i < y_total_size; i++)
		{
			y_n[i] = 0.0f;
		}

		printf("y(n) = ");
		for (int i = 0; i < y_total_size; i++)
		{
			printf("%2.2f ", y_n[i]);
		}
		printf("\n\n");
	}

	/* Division of x(n) in num_blocks */
	x_blocks = (float**)malloc(num_blocks * sizeof(float*));

	printf("%d of blocks of %d elements\n", num_blocks, block_size);
	for (int i = 0; i < num_blocks; i++)
	{
		x_blocks[i] = (float*)malloc(block_size * sizeof(float));
	}

	/* Separate each block_size blocks in a 2d array */
	for (int block = 0; block < num_blocks; block++)
	{
		for (int index = 0; index < block_size; index++)
		{
			x_blocks[block][index] =
				x_n[index + (block_size * block)];
		}
	}

	printf("x_blocks:\n");
	for (int i = 0; i < num_blocks; i++)
	{
		for (int j = 0; j < block_size; j++)
		{
			printf("%6.2f ", x_blocks[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");

	/* Create a block for each
	 * convolution of x_blocks * h_n -> y_blocks */
	y_blocks = (float**)malloc(num_blocks * sizeof(float*));

	for (int i = 0; i < num_blocks; i++)
	{
		y_blocks[i] = (float*)malloc(block_size * sizeof(float));
	}

	/* Initialize with zeros */
	for (int i = 0; i < num_blocks; i++)
	{
		for (int j = 0; j < block_size; j++)
		{
			y_blocks[i][j] = 0.0f;
		}
	}

	/* Process of convolution of each block */
	for (int block = 0; block < num_blocks; block++)
	{
		y_blocks[block] = convolve_block(x_blocks[block],
										 h_n,
										 block_size,
										 h_size,
										 conv_block_size);
	}

	printf("\n\ny_blocks:\n");
	for (int i = 0; i < num_blocks; i++)
	{
		for (int j = 0; j < conv_block_size; j++)
		{
			printf("%6.2f ", y_blocks[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");

	overlap_and_add(y_blocks,
					y_n,
					x_padded_size,
					conv_block_size,
					block_size);

	for (int i = 0; i < num_blocks; i++)
	{
		free(x_blocks[i]);
		free(y_blocks[i]);
	}

	free(y_blocks);

	return y_n;
}

void
output_file(char* file_name, float* signal, int signal_size)
{
	FILE* file_pointer = fopen(file_name, "w");

	if (file_pointer == NULL)
	{
		printf("Error in creating the file %s\n", file_name);
		exit(1);
	}

	for (int n = 0; n < signal_size; n++)
	{
		fprintf(file_pointer, "%.4f\n", signal[n]);
	}
}
