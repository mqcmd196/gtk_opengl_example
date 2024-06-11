#include <opencv2/opencv.hpp>

int main(){
    int width = 640, height = 480;

    cv::Mat image = cv::Mat::zeros(height, width, CV_8UC3);

    cv::Point center(width / 2., height / 2.);
    int radius = 50;

    cv::Scalar color(255, 255, 255);
    int thickness = -1;
    cv::circle(image, center, radius, color, thickness);

    cv::imshow("Test image", image);
    cv::waitKey(0);

    return 0;
}
