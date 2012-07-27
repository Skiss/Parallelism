#ifndef _IMG_PROCESSING_H_
#define _IMG_PROCESSING_H_

#include <cv.h>
#include <highgui.h>

#include <tbb/task_scheduler_init.h>
#include <tbb/blocked_range.h>
#include <tbb/blocked_range2d.h>
#include <tbb/parallel_for.h>
#include <tbb/tick_count.h>


namespace proc
{
    void blur(cv::Mat& img, bool para = false);
    void sharpen(cv::Mat& img, bool para = false);
    void edge_detect(cv::Mat& img, bool para = false);
    void light(cv::Mat& img, bool para = false);
    void dark(cv::Mat& img, bool para = false);
    void apply_filter_para(const cv::Mat& filter, cv::Mat& img);
    void apply_filter(const cv::Mat& filter, cv::Mat& img);
    void invert(cv::Mat& img, bool para = false);
    void mirror(cv::Mat& img, bool para = false);
};

#endif /* _IMG_PROCESSING_H_ */
