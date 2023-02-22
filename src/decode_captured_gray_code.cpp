/*
 * decode_captured_gray_code.cpp
 *
 *  Created on: Dec 17, 2019
 *      Author: chenjiee
 */

#include "opencv2/highgui.hpp"
#include "PGCapture.hpp"
#include "helper.hpp"
#include "decode_captured_gray_code.hpp"
#include <iostream>

Mat assemble_gray_code_from_patterns(int n_frames, int row_col, char dir[100]) {
	char filename0[250];
	char filename1[250];
	vector<Mat> temp_img_array;
	vector<Mat> temp_invert_img_array;
	vector<Mat> temp_corrected_img_array;

	for (int frames = 0; frames < n_frames; ++frames) {
		if (row_col == 0) {
			sprintf(filename0, "%s/captured_gray_code_row_%d.png", dir, frames);
			sprintf(filename1, "%s/captured_gray_code_row_inverted_%d.png", dir,
					frames);
		} else {
			sprintf(filename0, "%s/captured_gray_code_col_%d.png", dir, frames);
			sprintf(filename1, "%s/captured_gray_code_col_inverted_%d.png", dir,
					frames);
		}
		Mat temp0 = imread(filename0, IMREAD_COLOR);
		Mat temp0_gray;
		imwrite("test.png", temp0);
		cvtColor(temp0, temp0_gray, COLOR_BGR2GRAY);
		temp_img_array.push_back(temp0_gray);
		Mat temp1 = imread(filename1, IMREAD_COLOR);
		Mat temp1_gray;
		cvtColor(temp0, temp1_gray, COLOR_BGR2GRAY);
		temp_invert_img_array.push_back(temp1_gray);
	}

	// pre-processing of gray code images with their inverts
	for (int frames = 0; frames < n_frames; ++frames) {
		Mat temp;
		Mat temp_invert;
		Mat zero_mat;
		zero_mat = Mat::zeros(temp_img_array.at(0).rows,
				temp_img_array.at(0).cols, temp_img_array.at(0).type());
		// use the completely black image as the threshold
		subtract(temp_img_array.at(frames), temp_img_array.at(0), temp);
		max(temp, zero_mat, temp);
		subtract(temp_invert_img_array.at(frames), temp_img_array.at(0),
				temp_invert);
		max(temp_invert, zero_mat, temp_invert);
		subtract(temp, temp_invert, temp);
		max(temp, zero_mat, temp);
		temp_corrected_img_array.push_back(temp);
	}

	// height: height of the gray code images
	// width: width of the gray code images
	int height = temp_corrected_img_array.at(0).rows;
	int width = temp_corrected_img_array.at(0).cols;
	Mat gray_decode_ind_mat;
	gray_decode_ind_mat = Mat::zeros(height, width, CV_32FC1);
	for (int ind_y = 0; ind_y < height; ++ind_y) {
		// row, horizontal
		for (int ind_x = 0; ind_x < width; ++ind_x) {
			string temp_binary_string;
			for (int frames = 0; frames < n_frames - 1; ++frames) {
				temp_binary_string += to_string(
						(int) temp_img_array[n_frames - frames - 1].at<uchar>(
								ind_y, ind_x) / 255); // assemble of the gray code has nothing to do with the dimension
			}
			gray_decode_ind_mat.at<float>(ind_y, ind_x) =
					(float) binary_to_decimal(temp_binary_string); // gray code has nothing to do with the dimension
		}
	}
	return gray_decode_ind_mat;
}

Mat decode_gray_to_index(int n_frames, Mat gray_decode_ind_mat) {
	int height = gray_decode_ind_mat.rows;
	int width = gray_decode_ind_mat.cols;
	Mat index_image;
	index_image = Mat::zeros(height, width, CV_32FC1);

	for (int row_ind = 0; row_ind < height; ++row_ind) { // height: height of the gray code images
		for (int col_ind = 0; col_ind < width; ++col_ind) { // width: width of the gray code images
			int index_temp = gray_to_bin(
					(int) gray_decode_ind_mat.at<float>(row_ind, col_ind)); // decoding gray code has nothing to do with the dimension
			index_image.at<float>(row_ind, col_ind) = index_temp;
		}
	}
	return index_image;
}

vector<Mat> decode_gray_code(int height, int width, char out_dir[100]) {
	vector<Mat> index_maps;
	// assemble index gray code
	int row_col = 0;
	int bits_height = ceil(log2(height)) + 1; // how many bits is there
	Mat gray_decode_ind_row_mat = assemble_gray_code_from_patterns(bits_height,
			row_col, out_dir);
	Mat row_index_map = decode_gray_to_index(bits_height,
			gray_decode_ind_row_mat);
	index_maps.push_back(row_index_map);

	imwrite("test_row_decode.png", 255 * gray_decode_ind_row_mat / height);
	imwrite("test_row_index_map.png", 255 * row_index_map / height);

	// vertical
	row_col = 1;
	int bits_width = ceil(log2(width)) + 1;
	Mat gray_decode_ind_col_mat = assemble_gray_code_from_patterns(bits_width,
			row_col, out_dir);
	Mat col_index_map = decode_gray_to_index(bits_width,
			gray_decode_ind_col_mat);
	index_maps.push_back(col_index_map);

	imwrite("test_col_decode.png", 255 * gray_decode_ind_col_mat / width);
	imwrite("test_col_index_map.png", 255 * col_index_map / width);

	return index_maps;
}
