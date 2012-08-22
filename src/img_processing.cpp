#include "img_processing.hpp"

namespace img
{
    void blur(cv::Mat& img, bool para)
    {
        // Convolution matrix
        static cv::Mat filter = (cv::Mat_<double>(3, 3) <<
                                 0.1111F, 0.1111F, 0.1111F,
                                 0.1111F, 0.1111F, 0.1111F,
                                 0.1111F, 0.1111F, 0.1111F);

        if (para)
            apply_filter_para(filter, img);
        else
            apply_filter(filter, img);
    }

    void sharpen(cv::Mat& img, bool para)
    {
        // Convolution matrix
        static cv::Mat filter = (cv::Mat_<double>(3, 3) <<
                                 -1.0F, -1.0F, -1.0F,
                                 -1.0F, 9.0F, -1.0F,
                                 -1.0F, -1.0F, -1.0F);
        if (para) {
            apply_filter_para(filter, img);
        } else {
            apply_filter(filter, img);
        }
    }

    void edge_detect(cv::Mat& img, bool para)
    {
        // Convolution matrix
        static cv::Mat filter = (cv::Mat_<double>(3, 3) <<
                                 -0.5F, -0.5F, -0.5F,
                                 -0.5F, 4.0F, -0.5F,
                                 -0.5F, -0.5F, -0.5F);
        if (para)
            apply_filter_para(filter, img);
        else
            apply_filter(filter, img);
    }

    void light(cv::Mat& img, bool para)
    {
        // Convolution matrix
        static cv::Mat filter = (cv::Mat_<double>(3, 3) <<
                                 0.1F, 0.1F, 0.1F,
                                 0.1F, 1.0F, 0.1F,
                                 0.1F, 0.1F, 0.1F);
        if (para)
            apply_filter_para(filter, img);
        else
            apply_filter(filter, img);
    }

    void dark(cv::Mat& img, bool para)
    {
        // Convolution matrix
        static cv::Mat filter = (cv::Mat_<double>(3, 3) <<
                                 0.01F, 0.01F, 0.01F,
                                 0.01F, 0.5F, 0.01F,
                                 0.01F, 0.01F, 0.01F);
        if (para) {
            apply_filter_para(filter, img);
        } else {
            apply_filter(filter, img);
        }
    }

    /// We apply the filter using multithreading
    void apply_filter_para(const cv::Mat& filter, cv::Mat& img)
    {
        cv::Mat res = cv::Mat::zeros(img.rows, img.cols, img.type());

        tbb::blocked_range2d<unsigned, unsigned> range(1, img.rows - 1, 1, img.cols - 1);

        tbb::parallel_for(range, [=, &res](const tbb::blocked_range2d<unsigned, unsigned>& r) {
                for (unsigned i = r.rows().begin(); i < r.rows().end(); ++i) {
                    for (unsigned j = r.cols().begin(); j < r.cols().end(); ++j) {
                        for (int c = 0; c < 3; ++c) {
                            int tmp = 0;

                            for (int a = 0; a < filter.rows; ++a)
                                for (int b = 0; b < filter.cols; ++b)
                                    tmp += filter.at<double>(a, b) * img.at<cv::Vec3b>(i - 1 + a, j - 1 + b)[c];

                            if (tmp > 255) tmp = 255;
                            if (tmp < 0) tmp = 0;

                            res.at<cv::Vec3b>(i, j)[c] = tmp;
                        }
                    }
                }
            });

        res.copyTo(img);
    }

    /// We apply the filter without using multithreading
    void apply_filter(const cv::Mat& filter, cv::Mat& img)
    {
        cv::Mat res = cv::Mat::zeros(img.rows, img.cols, img.type());

        for (int i = 1; i < img.rows - 1; ++i) {
            for (int j = 1; j < img.cols - 1; ++j) {
                for (int c = 0; c < 3; ++c) {
                    int tmp = 0;

                    for (int a = 0; a < filter.rows; ++a)
                        for (int b = 0; b < filter.cols; ++b)
                            tmp += filter.at<double>(a, b) * img.at<cv::Vec3b>(i - 1 + a, j - 1 + b)[c];

                    if (tmp > 255) tmp = 255;
                    if (tmp < 0) tmp = 0;

                    res.at<cv::Vec3b>(i, j)[c] = tmp;
                }
            }
        }

        res.copyTo(img);
    }
};
