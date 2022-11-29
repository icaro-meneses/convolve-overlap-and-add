/*
 * ======================================================================
 *
 *       Filename:  atv_2.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  25/11/22 00:12:38
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Icaro Meneses Ferreira de Santana
 *					(Icaro Meneses),
 *					icaro.santana.dev@hotmail.com
 *
 * ======================================================================
 */

#ifndef LIB_ATV_2_H
#define LIB_ATV_2_H

#define M			  10
#define L			  4
#define M0			  3
#define N_SIZE		  (M + L - 1)

#define N_BLOCK_SIZE  (M0 + L - 1)
#define X_PADDED_SIZE (M + (M0 - (M % M0)))
#define NUM_BLOCKS	  (X_PADDED_SIZE / N_SIZE)

float*
convolve_block(float* x_block, float* h, int x_size, int h_size);

void
overlap_and_add(float** blocks_of_y, float* y);

float*
convolve_overlap_and_add(float* x_n,
						 float* h_n,
						 int x_size,
						 int h_size,
						 int block_size);

#endif
