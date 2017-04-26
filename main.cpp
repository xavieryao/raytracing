#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include "Objects.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    auto white = cv::Vec3b(0x99,0xff,0x66);
    auto image = cv::Mat(301,260, CV_8UC3, cv::Scalar(0x99, 0xff, 0x66));
    for (int i = 0; i < 50; ++i) {
        image.at<cv::Vec3b>(5, i) = cv::Vec3b(255,255,255);
    }
    cv::imshow("image", image);
    cv::waitKey();

    Sphere sp;
    return 0;
}