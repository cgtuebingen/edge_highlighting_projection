/*
 * edge_highlighting.cpp
 *
 *  Created on: Dec 17, 2019
 *      Author: chenjiee
 */

#include "opencv2/highgui.hpp"
#include "PGCapture.hpp"
#include "helper.hpp"
#include "edge_highlighting.hpp"
#include <iostream>

int max_bin_value = 255;
int threshold_value = max_bin_value/4;
int threshold_type = 0; // binary type

void edge_highlighting(int height, int width, vector<Mat> index_maps) {
	// project a completely black image
	Mat black;
	black = Mat::zeros(height, width, CV_8UC1);
	namedWindow("Fullscreen Projection", WINDOW_NORMAL);
	moveWindow("Fullscreen Projection", 0, 0);
	setWindowProperty("Fullscreen Projection", WND_PROP_FULLSCREEN,
			WINDOW_FULLSCREEN);
	imshow("Fullscreen Projection", black);
	waitKey(2000);

	// take an image of the scene
	PGCapture *cam = new PGCapture(); // capture with camera
	Mat scene = cam->getImage();
	delete cam;
	imwrite("captured_scene.png", scene);

	// detect the edges, find the pixel indices of the edges of the captured image
	Mat scene_edge;
	int low_threshold = 60;
	int ratio = 2;
	Canny(scene, scene_edge, low_threshold, low_threshold * ratio);

	// post processing the edge image
	GaussianBlur(scene_edge, scene_edge, Size(11, 11), 0);
	normalize(scene_edge, scene_edge, 0, 255, NORM_MINMAX, CV_8UC1);
	threshold(scene_edge, scene_edge, threshold_value, max_bin_value,
			threshold_type);
	imwrite("edge_detected.png", scene_edge);

	// find the pixel indices of the projector image, set the pixel values to be one
	Mat non_zero_coords;
	findNonZero(scene_edge, non_zero_coords);

	// generate the image
	Mat projected_image;
	projected_image = Mat::zeros(height, width, CV_8UC1);
	for (int points_non_zero = 0; points_non_zero < non_zero_coords.total();
			++points_non_zero) {
		int row_ind = (int) index_maps.at(0).at<float>(
				non_zero_coords.at<Point>(points_non_zero).y,
				non_zero_coords.at<Point>(points_non_zero).x);

		int col_ind = (int) index_maps.at(1).at<float>(
				non_zero_coords.at<Point>(points_non_zero).y,
				non_zero_coords.at<Point>(points_non_zero).x);

		projected_image.at<uchar>(row_ind, col_ind) = 255;
	}

	// post processing the image
	GaussianBlur(projected_image, projected_image, Size(3, 3), 0);
	normalize(projected_image, projected_image, 0, 255, NORM_MINMAX, CV_8UC1);
	threshold(projected_image, projected_image, threshold_value, max_bin_value,
				threshold_type);

	// project the image
	imshow("Fullscreen Projection", projected_image);
	waitKey(2000);

	// take an image of the projected scene
	PGCapture *cam_projected_scene = new PGCapture(); // capture with camera
	Mat modulated_projection = cam_projected_scene->getImage();
	delete cam_projected_scene;
	imwrite("edge_highlighting_scene.png", modulated_projection);

	// store the image
	imwrite("projected_image.png", projected_image);
}
