#ifndef _PIPELINE_H_
#define _PIPELINE_H_

#include <highgui.h>
#include <vector>
#include <tbb/pipeline.h>
#include <iostream>

class Chunk
{
public:
    static const unsigned chunkSize = 20;

    Chunk(const std::vector<cv::Mat>& v,
          const std::vector<cv::Mat>& v2);
    ~Chunk() = default;

    std::vector<cv::Mat>*
    getFrames() { return &frames_; }

private:
    std::vector<cv::Mat> frames_;
    std::vector<cv::Mat> frames2_;
};

class OutputVideo
{
public:
    OutputVideo(cv::VideoCapture& vid);
    ~OutputVideo() = default;

    void operator()(Chunk* chunk) const;

private:
    mutable cv::VideoWriter vid;
};

class InputVideo
{
public:
    InputVideo(const std::vector<cv::Mat>& vid,
               const std::vector<cv::Mat>& vid2);
    ~InputVideo() = default;

    Chunk* operator()(tbb::flow_control& fc) const;

private:
    mutable std::vector<cv::Mat> vid_;
    mutable std::vector<cv::Mat> vid2_;
    mutable unsigned offset;
};

class Transformer
{
public:
    Transformer() = default;
    ~Transformer() = default;

    Chunk* operator()(Chunk* chunk) const;

private:
};



#endif /* _PIPELINE_H_ */
