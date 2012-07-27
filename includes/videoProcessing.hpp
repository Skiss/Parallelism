#ifndef _VIDEOPROCESSING_H_
#define _VIDEOPROCESSING_H_

#include <cv.h>


namespace video
{
    bool isSwapable(const cv::Mat& img);
    cv::Mat swapContent(cv::Mat& img1, const cv::Mat& img2);

    cv::Mat blend(cv::Mat& img1, const cv::Mat& img2);
}

#endif /* _VIDEOPROCESSING_H_ */
