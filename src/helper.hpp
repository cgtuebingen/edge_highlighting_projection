/*
 * helper.hpp
 *
 *  Created on: Dec 10, 2019
 *      Author: chenjiee
 */

#ifndef SRC_HELPER_HPP_
#define SRC_HELPER_HPP_

#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include<math.h>
#include <iostream>

using namespace std;
using namespace cv;

unsigned int bin_to_gray(unsigned int num);

unsigned int gray_to_bin(unsigned int num);

void decimal_to_binary(int n, int bits, int row, Mat &ind_mat);

int binary_to_decimal(string number);

void read_resolution(int &height, int &width);

void affine_transformation_gray_code(int height, int width);

#endif /* SRC_HELPER_HPP_ */
