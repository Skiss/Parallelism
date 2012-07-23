#include "img_processing.hpp"


namespace proc
{
    cv::Mat* blur(const cv::Mat& img, bool para)
    {
        cv::Mat filter = (cv::Mat_<double>(3, 3) << 0.1111F, 0.1111F, 0.1111F,
                          0.1111F, 0.1111F, 0.1111F, 0.1111F, 0.1111F, 0.1111F);
        cv::Mat* res = new cv::Mat(cv::Mat::zeros(img.size(), img.type()));

        if (para) {
        tbb::blocked_range2d<unsigned, unsigned> range(1, img.rows - 1, 1, img.cols - 1);
        tbb::parallel_for(range,
                          [=, &res](const tbb::blocked_range2d<unsigned, unsigned>& r)
                          {
                              for (unsigned i = r.rows().begin(); i < r.rows().end(); ++i) {
                                  for (unsigned j = r.cols().begin(); j < r.cols().end(); ++j) {
                                      cv::Vec3b value(0, 0, 0);
                                      for (int c = 0; c < 3; ++c) {
                                          for (int a = 0; a < filter.rows; ++a) {
                                              for (int b = 0; b < filter.cols; ++b) {
                                                  value[c] += filter.at<double>(a, b) *
                                                      img.at<cv::Vec3b>(i - 1 + a, j - 1 + b)[c];
                                              }
                                          }
                                          res->at<cv::Vec3b>(i, j)[c] = value[c];
                                      }
                                  }
                              }
                          });
        }
        else {
            for (int i = 1; i < img.rows - 1; ++i) {
                for (int j = 1; j < img.cols - 1; ++j) {
                    cv::Vec3b value(0, 0, 0);
                    for (int c = 0; c < 3; ++c) {
                        for (int a = 0; a < filter.rows; ++a) {
                            for (int b = 0; b < filter.cols; ++b) {
                                value[c] += filter.at<double>(a, b) * img.at<cv::Vec3b>(i - 1 + a,
                                                                                        j - 1 + b)[c];
                            }
                        }
                        res->at<cv::Vec3b>(i, j)[c] = value[c];
                    }
                }
            }
        }

        return res;
    }
};
