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

float*
create_signal(int signal_size, char* signal_name)
{
	float* signal_pointer;

	signal_pointer = (float*)malloc(signal_size * sizeof(float));

	if (signal_pointer == NULL)
	{
		/* If isn't possible to allocate memory */
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
		/* If isn't possible to allocate memory */
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

#ifdef DEBUG_MODE
			printf("y_block[%d] += ", y_index);
			printf("x[%d] * h[%d] = ", operation_counter, h_index);
			printf("%.2f * %.2f = %2.2f\n",
				   x_block[operation_counter],
				   h[h_index],
				   y_block[y_index]);
#endif

			operation_counter--;
			if (operation_counter < 0)
			{
				break;
			}

			h_index++;
		}
	}
#ifdef DEBUG_MODE
	printf("\n\n");
#endif

	return y_block;
}

void
overlap_and_add(float** blocks_of_y,
				float* y,
				int y_size,
				int y_block_size,
				int block_size)
{
	int block_number	  = 0;
	int block_index		  = 0;
	int ovp_add_counter	  = 0;

	/* Operation Mode
	 * Mode 0: NORMAL
	 * Mode 1: OVERLAP AND ADD
	 * Mode 2: FINISH
	 */
	char operation_mode	  = 0;
	bool overlap_add_flag = true;
	int num_blocks		  = y_size / block_size;

#ifdef DEBUG_MODE
	printf("\nStarting the process of Overlapping and Adding...\n");
	printf("block_size = %d\t", block_size);
	printf("num_blocks = %d\t", num_blocks);
	printf("y_block_size = %d\t", y_block_size);
	printf("y_size = %d\n\n", y_size);
#endif

	int y_index = 0;
	int head	= block_size;
	int tail	= 0;
	while (overlap_add_flag)
	{
		/* Test for end of y(n) */
		if (y_index > (y_size - 1))
		{
#ifdef DEBUG_MODE
			printf("Overlap-and-Add finished...\n");
#endif
			overlap_add_flag = false;
		}

		else
		{
			/* Works like a Finite State Machine */
			switch (operation_mode)
			{
				case 0: /* NORMAL Mode */
					if ((y_index > (y_size - block_size + 1)) &&
						(ovp_add_counter >= block_size))
					{
						block_index = head;
						head		= block_size;
						tail		= 0;

#ifdef DEBUG_MODE
						printf("Number of Overlap-and-Add "
							   "operations > %d:  %d\n",
							   num_blocks - 1,
							   ovp_add_counter);
						printf("Resetting variables to: "
							   "blk_idx=%d\tblk_num=%d\t"
							   "head=%d\ttail=%d\n",
							   block_index,
							   block_number,
							   head,
							   tail);
						printf("Going to FINISH Mode\n\n");
#endif
						operation_mode = 2;
					}

					else if (block_index < block_size)
					{
						y[y_index] =
							blocks_of_y[block_number][block_index];

#ifdef DEBUG_MODE
						printf("NORMAL Mode:\n");
						printf("y(%d) = y_blocks[%d][%d] = %2.2f\n",
							   y_index,
							   block_number,
							   block_index,
							   y[y_index]);
#endif

						y_index++;
						block_index++;
					}

					else
					{
#ifdef DEBUG_MODE
						printf("Going to OVERLAP AND ADD Mode\n\n");
#endif
						operation_mode = 1;
					}
					break;

				case 1: /* OVERLAP AND ADD Mode*/
					if ((y_index > (y_size - block_size + 1)) &&
						(ovp_add_counter >= num_blocks))
					{
						block_index = head;
						head		= block_size;
						tail		= 0;

#ifdef DEBUG_MODE
						printf("Number of Overlap-and-Add "
							   "operations > %d:  %d\n",
							   num_blocks - 1,
							   ovp_add_counter);
						printf("Resetting variables to: "
							   "blk_idx=%d\tblk_num=%d\t"
							   "head=%d\ttail=%d\n",
							   block_index,
							   block_number,
							   head,
							   tail);
						printf("Going to FINISH Mode\n\n");
#endif
						operation_mode = 2;
					}

					else if (head < y_block_size)
					{
#ifdef DEBUG_MODE
						printf(
							"Attempting to perform operations with:\n"
							"blk_num=%d\tblk_idx=%d\thead=%d\t"
							"tail=%d\n",
							block_number,
							block_index,
							head,
							tail);
#endif

						y[y_index] =
							blocks_of_y[block_number][head] +
							blocks_of_y[block_number + 1][tail];

#ifdef DEBUG_MODE
						printf("OVERLAP AND ADD Mode:\n"
							   "blk_idx=%d\thead=%d\ttail=%d\n",
							   block_index,
							   head,
							   tail);
						printf("y(%d) = y_blocks[%d][%d] + "
							   "y_blocks[%d][%d] = ",
							   y_index,
							   block_number,
							   head,
							   block_number + 1,
							   tail);
						printf("%.2f + %.2f = %.2f\n",
							   blocks_of_y[block_number][head],
							   blocks_of_y[block_number + 1][tail],
							   y[y_index]);
#endif

						y_index++;
						block_index++;

						head++;
						tail++;
					}

					else
					{
						block_index = tail;
						block_number++;
						head = block_size;
						tail = 0;
						ovp_add_counter++;

#ifdef DEBUG_MODE
						printf("Resetting variables to: "
							   "blk_idx=%d\tblk_num=%d\t"
							   "head=%d\ttail=%d\n",
							   block_index,
							   block_number,
							   head,
							   tail);
						printf("Overlap-and-Add operations "
							   "performed: %d\n",
							   ovp_add_counter);
						printf("Going to NORMAL Mode\n\n");
#endif
						operation_mode = 0;
					}
					break;

				case 2: /* FINISH Mode*/
					if (y_index < y_size)
					{
						y[y_index] =
							blocks_of_y[block_number][block_index];

#ifdef DEBUG_MODE
						printf("FINISH Mode:\n");
						printf("y(%d) = y_blocks[%d][%d] = %2.2f\n",
							   y_index,
							   block_number,
							   block_index,
							   y[y_index]);
#endif

						y_index++;
						block_index++;
					}

					else
					{
#ifdef DEBUG_MODE
						printf("Overlap-and-Add finished...\n");
#endif
						overlap_add_flag = false;
					}

					break;

				default:
#ifdef DEBUG_MODE
					printf("A error ocurred in"
						   " Overlapping and Adding.\n");
#endif
					break;
			}
		}
	}
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

#ifdef DEBUG_MODE
	printf("x(n) padded size: %d\nconv_block_size: %d\nnum_blocks: "
		   "%d\n\n",
		   x_padded_size,
		   conv_block_size,
		   num_blocks);
#endif

	/* Zero Padding for x(n) */
	x_n = realloc(x_n, x_padded_size * sizeof(float));

	for (int i = x_size; i < x_padded_size; i++)
	{
		x_n[i] = 0.0f;
	}

#ifdef DEBUG_MODE
	printf("ZERO PADDING:\nx(n) = ");
	for (int i = 0; i < x_padded_size; i++)
	{
		printf("%2.2f ", x_n[i]);
	}
	printf("\n\n");
#endif

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

#ifdef DEBUG_MODE
		printf("y(n) = ");
		for (int i = 0; i < y_total_size; i++)
		{
			printf("%2.2f ", y_n[i]);
		}
		printf("\n\n");
#endif
	}

	/* Division of x(n) in num_blocks */
	x_blocks = (float**)malloc(num_blocks * sizeof(float*));
	if (x_blocks == NULL)
	{
		printf("Error in memory allocation for %s 2d array.\n",
			   "y_blocks");
		exit(1);
	}

#ifdef DEBUG_MODE
	printf("%d of blocks of %d elements\n", num_blocks, block_size);
#endif
	for (int i = 0; i < num_blocks; i++)
	{
		x_blocks[i] = (float*)malloc(block_size * sizeof(float));

		if (x_blocks[i] == NULL)
		{
			printf("Error in memory allocation for %s[%d] array.\n",
				   "x_blocks",
				   i);
			exit(1);
		}
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

#ifdef DEBUG_MODE
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
#endif

	/* Create a block for each
	 * convolution of x_blocks * h_n -> y_blocks */
	y_blocks = (float**)malloc(num_blocks * sizeof(float*));

	if (y_blocks == NULL)
	{
		printf("Error in memory allocation for %s 2d array.\n",
			   "y_blocks");
		exit(1);
	}

	for (int i = 0; i < num_blocks; i++)
	{
		y_blocks[i] = (float*)malloc(conv_block_size * sizeof(float));

		if (y_blocks[i] == NULL)
		{
			printf("Error in memory allocation for %s[%d] array.\n",
				   "y_blocks",
				   i);
			exit(1);
		}
	}

	/* Initialize with zeros */
	for (int i = 0; i < num_blocks; i++)
	{
		for (int j = 0; j < conv_block_size; j++)
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

#ifdef DEBUG_MODE
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
#endif

	overlap_and_add(y_blocks,
					y_n,
					y_total_size,
					conv_block_size,
					block_size);

	for (int i = 0; i < num_blocks; i++)
	{
		free(x_blocks[i]);
		free(y_blocks[i]);
	}

	free(x_n);
	free(x_blocks);
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

	fclose(file_pointer);
}
