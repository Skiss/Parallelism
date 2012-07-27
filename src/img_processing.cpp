#include "img_processing.hpp"


namespace proc
{
    cv::Mat blur(const cv::Mat& img, bool para)
    {
        static cv::Mat filter = (cv::Mat_<double>(3, 3) <<
                                0.1111F, 0.1111F, 0.1111F,
                                0.1111F, 0.1111F, 0.1111F,
                                0.1111F, 0.1111F, 0.1111F);

        if (para) {
            return apply_filter_para(filter, img);
        } else {
            return apply_filter(filter, img);
        }
    }

    cv::Mat sharpen(const cv::Mat& img, bool para)
    {
        static cv::Mat filter = (cv::Mat_<double>(3, 3) <<
                                -1.0F, -1.0F, -1.0F,
                                -1.0F, 9.0F, -1.0F,
                                -1.0F, -1.0F, -1.0F);
        if (para) {
            return apply_filter_para(filter, img);
        } else {
            return apply_filter(filter, img);
        }
    }

    cv::Mat edge_detect(const cv::Mat& img, bool para)
    {
        static cv::Mat filter = (cv::Mat_<double>(3, 3) <<
                                -0.5F, -0.5F, -0.5F,
                                -0.5F, 4.0F, -0.5F,
                                -0.5F, -0.5F, -0.5F);
        if (para) {
            return apply_filter_para(filter, img);
        } else {
            return apply_filter(filter, img);
        }
    }

    cv::Mat light(const cv::Mat& img, bool para)
    {
        static cv::Mat filter = (cv::Mat_<double>(3, 3) <<
                                0.1F, 0.1F, 0.1F,
                                0.1F, 1.0F, 0.1F,
                                0.1F, 0.1F, 0.1F);
        if (para) {
            return apply_filter_para(filter, img);
        } else {
            return apply_filter(filter, img);
        }
    }

    cv::Mat dark(const cv::Mat& img, bool para)
    {
        static cv::Mat filter = (cv::Mat_<double>(3, 3) <<
                                0.01F, 0.01F, 0.01F,
                                0.01F, 0.5F, 0.01F,
                                0.01F, 0.01F, 0.01F);
        if (para) {
            return apply_filter_para(filter, img);
        } else {
            return apply_filter(filter, img);
        }
    }

    cv::Mat apply_filter_para(cv::Mat filter, const cv::Mat& img)
    {
        cv::Mat res;
        res = cv::Mat::zeros(img.rows, img.cols, img.type());

        tbb::blocked_range2d<unsigned, unsigned> range(1, img.rows - 1, 1, img.cols - 1);
        tbb::parallel_for(range,
                          [=, &res](const tbb::blocked_range2d<unsigned, unsigned>& r)
                          {
                              for (unsigned i = r.rows().begin(); i < r.rows().end(); ++i) {
                                  for (unsigned j = r.cols().begin(); j < r.cols().end(); ++j) {
                                      for (int c = 0; c < 3; ++c) {
                                          int tmp = 0;
                                          for (int a = 0; a < filter.rows; ++a) {
                                              for (int b = 0; b < filter.cols; ++b) {
                                                  tmp += filter.at<double>(a, b) * img.at<cv::Vec3b>(i - 1 + a, j - 1 + b)[c];
                                              }
                                          }
                                          if (tmp > 255) tmp = 255;
                                          if (tmp < 0) tmp = 0;
                                          res.at<cv::Vec3b>(i, j)[c] = tmp;
                                      }
                                  }
                              }
                          });
        return res;
    }

    cv::Mat apply_filter(cv::Mat filter, const cv::Mat& img)
    {
        cv::Mat res;
        res = cv::Mat::zeros(img.rows, img.cols, img.type());

        for (int i = 1; i < img.rows - 1; ++i) {
            for (int j = 1; j < img.cols - 1; ++j) {
                for (int c = 0; c < 3; ++c) {
                    int tmp = 0;
                    for (int a = 0; a < filter.rows; ++a) {
                        for (int b = 0; b < filter.cols; ++b) {
                            tmp += filter.at<double>(a, b) * img.at<cv::Vec3b>(i - 1 + a, j - 1 + b)[c];
                        }
                    }
                    if (tmp > 255) tmp = 255;
                    if (tmp < 0) tmp = 0;
                    res.at<cv::Vec3b>(i, j)[c] = tmp;
                }
            }
        }

        return res;
    }
    
    cv::Mat invert(const cv::Mat& img, bool para)
    {
        cv::Mat res = img;

        if (para) {
            tbb::blocked_range2d<unsigned, unsigned> range(1, img.rows - 1, 1, img.cols - 1);
            cv::Vec3b inv(255, 255, 255);
            tbb::parallel_for(range,
                          [=, &res](const tbb::blocked_range2d<unsigned, unsigned>& r)
                          {
                              for (unsigned i = r.rows().begin(); i < r.rows().end(); ++i) {
                                  for (unsigned j = r.cols().begin(); j < r.cols().end(); ++j) {
                                          res.at<cv::Vec3b>(i, j) = inv - img.at<cv::Vec3b>(i, j);
                                  }
                              }
                          });
        } else {
            for (int i = 1; i < img.rows - 1; ++i) {
                for (int j = 1; j < img.cols - 1; ++j) {
                    for (int c = 0; c < 3; ++c) {
                        res.at<cv::Vec3b>(i, j)[c] = 255 - img.at<cv::Vec3b>(i, j)[c];
                    }
                }
            }
        }

        return res;
    }

    cv::Mat mirror(const cv::Mat& img, bool para)
    {
        cv::Mat res = img;

        if (para) {
            tbb::blocked_range2d<unsigned, unsigned> range(0, img.rows, 0, img.cols);
            tbb::parallel_for(range,
                          [=, &res](const tbb::blocked_range2d<unsigned, unsigned>& r)
                          {
                              for (unsigned i = r.rows().begin(); i < r.rows().end(); ++i) {
                                  for (unsigned j = r.cols().begin(); j < r.cols().end(); ++j) {
                                      res.at<cv::Vec3b>(i, j) = img.at<cv::Vec3b>(i, img.cols - j);
                                  }
                              }
                          });
        } else {
            for (int i = 0; i < img.rows; ++i) {
                for (int j = 0; j < img.cols; ++j) {
                    for (int c = 0; c < 3; ++c) {
                        res.at<cv::Vec3b>(i, j)[c] = img.at<cv::Vec3b>(i, img.cols - j)[c];
                    }
                }
            }
        }

        return res;
    }
};
