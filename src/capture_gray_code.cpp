/*
 * capture_gray_code.cpp
 *
 *  Created on: Dec 17, 2019
 *      Author: chenjiee
 */
#include "opencv2/highgui.hpp"
#include "PGCapture.hpp"
#include "helper.hpp"
#include "capture_gray_code.hpp"
#include <iostream>

void capture_gray_code(int height, int width) {
	PGCapture *cam = new PGCapture(); // capture with camera
	char gray_code_filename[250];
	char captured_gray_code_filename[250];

	cout << "Please enter the directory of stored gray code images:" << endl;
	char in_dir[250];
	cin >> in_dir;

	cout << "Please enter the directory of captured gray code images:" << endl;
	char out_dir[250];
	cin >> out_dir;

	int n_frames;
	for (int row_col = 0; row_col < 2; ++row_col) {

		if (row_col == 0) {
			n_frames = ceil(log2(height));
		} else {
			n_frames = ceil(log2(width));
		}

		for (int invert = 0; invert < 2; ++invert) {
			for (int frames = 0; frames < n_frames + 1; ++frames) {
				Mat gray_code_capture;
				if (invert == 0) {
					// display gray code image
					if (row_col == 0) {
						sprintf(gray_code_filename, "%s/gray_code_row_%d.png",
								in_dir, frames);
					} else {
						sprintf(gray_code_filename, "%s/gray_code_col_%d.png",
								in_dir, frames);
					}
				} else {
					// display gray code image
					if (row_col == 0) {
						sprintf(gray_code_filename,
								"%s/gray_code_row_inverted_%d.png", in_dir,
								frames);
					} else {
						sprintf(gray_code_filename,
								"%s/gray_code_col_inverted_%d.png", in_dir,
								frames);
					}
				}

				Mat gray_code = imread(gray_code_filename);
				namedWindow("Fullscreen Projection", WINDOW_NORMAL);
				moveWindow("Fullscreen Projection", 0, 0);
				setWindowProperty("Fullscreen Projection", WND_PROP_FULLSCREEN,
						WINDOW_FULLSCREEN);
				imshow("Fullscreen Projection", gray_code);
				waitKey(2000);
				cout << "Display Gray Code " << endl;

				// capture gray code image
				Mat capture = cam->getImage();
				if (invert == 0) {
					if (row_col == 0) {
						sprintf(captured_gray_code_filename,
								"%s/captured_gray_code_row_%d.png", out_dir,
								frames);
					} else {
						sprintf(captured_gray_code_filename,
								"%s/captured_gray_code_col_%d.png", out_dir,
								frames);
					}
				} else {
					if (row_col == 0) {
						sprintf(captured_gray_code_filename,
								"%s/captured_gray_code_row_inverted_%d.png",
								out_dir, frames);
					} else {
						sprintf(captured_gray_code_filename,
								"%s/captured_gray_code_col_inverted_%d.png",
								out_dir, frames);
					}
				}
				imwrite(captured_gray_code_filename, capture);
			}
		}
	}
	delete cam;
}
