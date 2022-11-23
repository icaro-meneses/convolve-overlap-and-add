#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define M		   10
#define M0		   3
#define L		   2
#define N		   (M0 + L - 1)

#define X_ZERO_PAD (N * M0)
#define NUM_BLOCKS N

int
main(void)
{
	int *x, *y;
	int* h;
	int **y_blocks, **x_blocks;

	x = (int*)malloc(M * sizeof(int));
	h = (int*)malloc(L * sizeof(int));

	int operation_counter;

	if (x == NULL)
	{
		printf("Error in memory allocation for x(n).\n");
		exit(1);
	}

	else
	{
		int sample_X_array[M] = {1, 2, 2, 5, -6, 3, -1, 1, 4, -1};

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
		int sample_H_array[L] = {1, -3};

		for (int i = 0; i < L; i++)
		{
			h[i] = sample_H_array[i];
		}
	}

	/* Zero Padding */
	x = realloc(x, X_ZERO_PAD * sizeof(int));

	for (int i = M; i < (X_ZERO_PAD - M - 1); i++)
	{
		x[i] = 0;
	}

	printf("x(n) = ");
	for (int i = 0; i < X_ZERO_PAD; i++)
	{
		printf("%2d ", x[i]);
	}
	printf("\n\n");

	printf("h(n) = ");
	for (int i = 0; i < L; i++)
	{
		printf("%2d ", h[i]);
	}
	printf("\n\n");

	y = (int*)malloc(X_ZERO_PAD * sizeof(int));

	if (y == NULL)
	{
		printf("Error in memory allocation for h(n).\n");
		exit(1);
	}

	else
	{
		for (int i = 0; i < X_ZERO_PAD; i++)
		{
			y[i] = 0;
		}
	}

	/* Division of x(n) in N blocks */
	x_blocks = (int**)malloc(NUM_BLOCKS * sizeof(int*));

	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		x_blocks[i] = (int*)malloc(M0 * sizeof(int));
	}

	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		for (int j = 0; j < M0; j++)
		{
			x_blocks[i][j] = x[j + (M0 * i)];
		}
	}

	printf("x_blocks:\n");
	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		for (int j = 0; j < M0; j++)
		{
			printf("%2d ", x_blocks[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");

	/* Create blocks for y(n) -> y_blocks */
	y_blocks = (int**)malloc(NUM_BLOCKS * sizeof(int*));

	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		y_blocks[i] = (int*)malloc(N * sizeof(int));
	}

	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		for (int j = 0; j < N; j++)
		{
			y_blocks[i][j] = 0;
		}
	}

	/* Convolution of each y_blocks = x_blocks * h */
	for (int block_number = 0; block_number < NUM_BLOCKS;
		 block_number++)
	{
		printf("\nnew block.\n");
		for (int block_index = 0; block_index < N; block_index++)
		{
			operation_counter = block_index;
			printf("op: %d\n", operation_counter);

			for (int h_index = 0; h_index < L; h_index++)
			{
				if (operation_counter > (M0 - 1))
				{
					operation_counter = M0 - 1;
					y_blocks[block_number][block_index] =
						h[L - 1] *
						x_blocks[block_number][operation_counter];

					printf("final: y_blocks[%d][%d] += h[%d] * "
						   "x_blocks[%d][%d] = %d * %d\n",
						   block_number,
						   block_index,
						   0,
						   block_number,
						   operation_counter,
						   h[L - 1],
						   x_blocks[block_number][operation_counter]);
					break;
				}

				printf("y_blocks[%d][%d] += h[%d] * x_blocks[%d][%d] "
					   "= %d * %d\n",
					   block_number,
					   block_index,
					   (L - 1) - h_index,
					   block_number,
					   operation_counter,
					   h[(L - 1) - h_index],
					   x_blocks[block_number][operation_counter]);

				y_blocks[block_number][block_index] +=
					h[h_index] *
					x_blocks[block_number][operation_counter];

				operation_counter--;
				if (operation_counter < 0)
				{
					break;
				}
			}
		}
	}

	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		for (int j = 0; j < N; j++)
		{
			printf("%2d ", y_blocks[i][j]);
		}
		printf("\n");
	}

	/* Overlapping and Adding */
	int block_number = 0;
	int block_index	 = 1;

	y[0]			 = y_blocks[0][0];
	for (int final_index = 1; final_index < X_ZERO_PAD - 1;
		 final_index++)
	{
		printf("blk_num:%2d\tblk_idx:%2d\n",
			   block_number,
			   block_index);
		if (final_index % M0)
		{
			y[final_index] = y_blocks[block_number][block_index];
			block_index++;
		}

		else
		{
			y[final_index] = y_blocks[block_number][block_index] +
							 y_blocks[block_number + 1][0];

			block_number++;
			block_index = 1;
		}
	}
	y[X_ZERO_PAD - 1] = y_blocks[NUM_BLOCKS - 1][N - 1];

	printf("\nResult: \ny(n) = ");
	for (int i = 0; i < X_ZERO_PAD; i++)
	{
		printf("%2d ", y[i]);
	}
	printf("\n");

	/* Freeing memory */
	free(x);
	free(h);
	free(x_blocks);
	free(y_blocks);
	free(y);

	return 0;
}
