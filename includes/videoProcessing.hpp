#ifndef _VIDEOPROCESSING_H_
#define _VIDEOPROCESSING_H_

#include <cv.h>

#include <tbb/task_scheduler_init.h>
#include <tbb/blocked_range.h>
#include <tbb/blocked_range2d.h>
#include <tbb/parallel_for.h>
#include <tbb/tick_count.h>

namespace video
{
    bool isSwapable(const cv::Mat& img);

    /// Here, the para arguments indicates whether we want
    /// the process to be done in parallel or not.

    void swapContent(cv::Mat& img1, const cv::Mat& img2, bool para);
    void blend(cv::Mat& img1, const cv::Mat& img2, bool para);
}

#endif /* _VIDEOPROCESSING_H_ */
