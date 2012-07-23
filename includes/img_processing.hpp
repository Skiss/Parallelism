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
    cv::Mat* blur(const cv::Mat& img, bool para = false);
};

#endif /* _IMG_PROCESSING_H_ */
