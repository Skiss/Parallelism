#include "videoProcessing.hpp"


namespace video
{
    bool isSwapable(const cv::Vec3b& pix)
    {
        return (pix[1] >= 160 && pix[0] < 50 && pix[2] < 110);
    }

    cv::Mat swapContent(cv::Mat& img1, const cv::Mat& img2)
    {
        for (int i = 0; i < img1.rows; ++i) {
            for (int j = 0; j < img1.cols; ++j) {
                if (isSwapable(img1.at<cv::Vec3b>(i, j))) {
                    img1.at<cv::Vec3b>(i, j) = img2.at<cv::Vec3b>(i, j);
                }
            }
        }

        return img1;
    }

    cv::Mat blend(cv::Mat& img1, const cv::Mat& img2)
    {
        cv::Mat res = img1;

        addWeighted(img1, 0.5, img2, 0.5, 0.0, res);

        return res;
    }
}
