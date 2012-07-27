#include "videoProcessing.hpp"


namespace video
{
    bool isSwapable(const cv::Vec3b& pix)
    {
        return (pix[1] >= 160 && pix[0] < 50 && pix[2] < 110);
    }

    cv::Mat swapContent(cv::Mat& img1, const cv::Mat& img2, bool para)
    {
        if (para) {
            tbb::blocked_range2d<unsigned, unsigned> range(0, img1.rows, 0, img1.cols);
            tbb::parallel_for(range, [=, &img1](const tbb::blocked_range2d<unsigned, unsigned>& r) {
                for (unsigned i = r.rows().begin(); i < r.rows().end(); ++i) {
                    for (unsigned j = r.cols().begin(); j < r.cols().end(); ++j) {
                        if (isSwapable(img1.at<cv::Vec3b>(i, j))) {
                            img1.at<cv::Vec3b>(i, j) = img2.at<cv::Vec3b>(i, j);
                        }
                    }
                }
            });
        } else {
            for (int i = 0; i < img1.rows; ++i) {
                for (int j = 0; j < img1.cols; ++j) {
                    if (isSwapable(img1.at<cv::Vec3b>(i, j))) {
                        img1.at<cv::Vec3b>(i, j) = img2.at<cv::Vec3b>(i, j);
                    }
                }
            }
        }

        return img1;
    }

    cv::Mat blend(cv::Mat& img1, const cv::Mat& img2, bool para)
    {
        if (para) {
            tbb::blocked_range2d<unsigned, unsigned> range(0, img1.rows, 0, img1.cols);
            tbb::parallel_for(range, [=, &img1](const tbb::blocked_range2d<unsigned, unsigned>& r) {
                for (unsigned i = r.rows().begin(); i < r.rows().end(); ++i) {
                    for (unsigned j = r.cols().begin(); j < r.cols().end(); ++j) {
                        for (int c = 0; c < 3; ++c) {
                            img1.at<cv::Vec3b>(i, j)[c] = (img1.at<cv::Vec3b>(i, j)[c] + img2.at<cv::Vec3b>(i, j)[c])/2;
                        }
                    }
                }
            });
        } else {
            for (int i = 0; i < img1.rows; ++i) {
                for (int j = 0; j < img1.cols; ++j) {
                    for (int c = 0; c < 3; ++c) {
                        img1.at<cv::Vec3b>(i, j)[c] = (img1.at<cv::Vec3b>(i, j)[c] + img2.at<cv::Vec3b>(i, j)[c])/2;
                    }
                }
            }
        }

        return img1;
    }
}
