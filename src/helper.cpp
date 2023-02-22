/*
 * helper.cpp
 *
 *  Created on: Dec 10, 2019
 *      Author: chenjiee
 */

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include<math.h>
#include <iostream>

using namespace std;
using namespace cv;

unsigned int bin_to_gray(unsigned int num) {
	return num ^ (num >> 1);
}

unsigned int gray_to_bin(unsigned int num) {
	unsigned int mask;
	for (mask = num >> 1; mask != 0; mask = mask >> 1) {
		num = num ^ mask;
	}
	return num;
}

void decimal_to_binary(int n, int bits, int row, Mat &ind_mat) {
	int i = 0;

	while (n > 0) {
		ind_mat.at<uchar>(row, i) = 255 * (n % 2);
		n = n / 2;
		i++;
	}

}

int binary_to_decimal(string number) {
	int result = 0, pow = 1;
	for (int iter = number.length() - 1; iter >= 0; iter--, pow *= 2) {
		result += (number[iter] - '0') * pow;
	}
	return result;
}

void read_resolution(int &height, int &width) {
	cout << "Please enter the resolution" << endl;
	cin >> height;
	cin >> width;
	cout << height << endl;
	cout << width << endl;
}

//test with affine transformed gray code
void affine_transformation_gray_code(int height, int width) {
	// load images
	char dir[100];
	cout << "Please enter the name of the directory of stored gray code images:"
			<< endl;
	cin >> dir;

	// enter transformation data (angle and scale)
	cout << "Please enter the rotation angle:" << endl;
	float angle;
	cin >> angle;

	cout << "Please enter the scaling factor:" << endl;
	float scale;
	cin >> scale;

	Mat rot_mat( 2, 3, CV_32FC1 );

	// perform transformation (rotation)
	char filename[100];
	char outname[100];
	vector<Mat> temp_img_array;
	int bits_height = ceil(log2(height)); // how many layers are there
	int bits_width = ceil(log2(width));
	int n_frames;

	for (int row_col = 0; row_col < 2; ++row_col) {
		if (row_col == 0) {
			n_frames = bits_height;
		} else {
			n_frames = bits_width;
		}
		for (int frames = 0; frames < n_frames; ++frames) {
			if (row_col == 0) {
				sprintf(filename, "%s/gray_code_row_%d.png", dir, frames);
			} else {
				sprintf(filename, "%s/gray_code_col_%d.png", dir, frames);
			}
			Mat temp = imread(filename, CV_8UC1);
			Mat temp_rotated;
			temp_rotated = Mat::zeros(height, width, temp.type());

			Point center = Point(temp_rotated.cols / 2, temp_rotated.rows / 2);

			rot_mat = getRotationMatrix2D(center, angle, scale);

			/// Rotate the warped image
			warpAffine(temp, temp_rotated, rot_mat, temp_rotated.size());

			// store images
			if (row_col == 0) {
				sprintf(outname, "%s/gray_code_row_%d_rotated.png", dir, frames);
			} else {
				sprintf(outname, "%s/gray_code_col_%d_rotated.png", dir, frames);
			}
			imwrite(outname, temp_rotated);
		}
	}
}
