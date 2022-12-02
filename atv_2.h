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

float*
create_signal(int signal_size, char* signal_name);

void
sum_signals(float* signal_A,
			float* signal_B,
			float* signal_result,
			int size_result);

float*
convolve_block(float* x_block,
			   float* h,
			   int x_size,
			   int h_size,
			   int block_size);

void
overlap_and_add(float** blocks_of_y,
				float* y,
				int y_size,
				int y_block_size,
				int block_size);

float*
convolve_overlap_and_add(float* x_n,
						 float* h_n,
						 int x_size,
						 int h_size,
						 int block_size);

void
output_file(char* file_name, float* signal, int signal_size);

#endif
