#include "videoProcessing.hpp"


namespace video
{
    bool isSwapable(const cv::Vec3b& pix)
    {
        cv::Vec3b val(0, 0, 255);

        // std::cout << (uchar)pix[0];
        // std::cout << (uchar)pix[1];
        // std::cout << (uchar)pix[2] << std::endl;

        return true;
    }

    cv::Mat swapContent(const cv::Mat& img1, const cv::Mat& img2)
    {
        // cv::Mat res = cv::Mat(cv::Mat::zeros(img1.size(), img1.type()));

        for (int i = 0; i < img1.rows; ++i) {
            for (int j = 0; j < img1.cols; ++j) {
                if (isSwapable(img1.at<cv::Vec3b>(i, j))) {

                }
                // filter.at<double>(a, b) * img.at<cv::Vec3b>(i - 1 + a,
                //                                             j - 1 + b)[c];
            }
        }

        return img1;
    }
}
