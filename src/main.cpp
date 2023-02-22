/*
 * This program
 * (1) captures the gray code images with the camera projector setup.
 * (2) decodes the gray code images into index maps
 * (3) captures an image with normal lightings
 * (4) project the highlighted edges
 */

#include "opencv2/highgui.hpp"
#include "PGCapture.hpp"
#include "helper.hpp"
#include "edge_highlighting.hpp"
#include "capture_gray_code.hpp"
#include "decode_captured_gray_code.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, const char **argv) {

	// read resolution
	int height = 768;
	int width = 1024;
//	read_resolution(height, width);

	// capture images
//	capture_gray_code(height, width);
	// /home/chenjiee/Projects/practices/Displays_GrayCode_Endecoder/gray_code_patterns; /home/chenjiee/Projects/practices/Displays_EdgeHighlightingProjection_v2019/captured_gray_code_images
	cout << "Gray code captured!" << endl;

	// decode gray code
	char out_dir[100];
	cout << "Please enter the directory that stores the captured gray code images:" << endl;
	cin >> out_dir;
//	sprintf(out_dir, "/home/chenjiee/Projects/practices/Displays_EdgeHighlightingProjection_v2019/captured_gray_code_images");
	vector<Mat> index_maps = decode_gray_code(height, width, out_dir);
	cout << "Decoding finished!" << endl;

	// edge highlighting
	edge_highlighting(height, width, index_maps);
	cout << "Edge highlighted!" << endl;

	return 0;
}
