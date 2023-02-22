/*
 * decode_captured_gray_code.hpp
 *
 *  Created on: Dec 17, 2019
 *      Author: chenjiee
 */

#ifndef SRC_DECODE_CAPTURED_GRAY_CODE_HPP_
#define SRC_DECODE_CAPTURED_GRAY_CODE_HPP_

#include "opencv2/highgui.hpp"
#include "PGCapture.hpp"
#include "helper.hpp"
#include "decode_captured_gray_code.hpp"
#include <iostream>

Mat assemble_gray_code_from_patterns(int n_frames, int row_col, char dir[100]);

Mat decode_gray_to_index(int n_frames, Mat gray_decode_ind_mat);

vector<Mat> decode_gray_code(int height, int width, char out_dir[250]);

#endif /* SRC_DECODE_CAPTURED_GRAY_CODE_HPP_ */
