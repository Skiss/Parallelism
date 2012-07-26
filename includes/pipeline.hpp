#ifndef _PIPELINE_H_
#define _PIPELINE_H_

#include <highgui.h>
#include <vector>
#include <tbb/pipeline.h>
#include <iostream>

#define BLUR      0
#define SHARPEN   1
#define EDGE      2
#define LIGHT     3
#define DARK      4
#define INVERT    5
#define MIRROR    6
#define BLUR_P    7
#define SHARPEN_P 8
#define EDGE_P    9
#define LIGHT_P   10
#define DARK_P    11
#define INVERT_P  12
#define MIRROR_P  13

class Chunk
{
public:
    static const unsigned chunkSize = 200;

    Chunk(cv::Mat* offset1, unsigned size, cv::Mat* offset2 = nullptr);
    ~Chunk() = default;

    std::pair<cv::Mat*, unsigned>
    getFrames() { return std::make_pair(offset1_, size_); }
    std::pair<cv::Mat*, unsigned>
    getFrames2() { return std::make_pair(offset2_, size_); }

private:
    cv::Mat* offset1_;
    cv::Mat* offset2_;
    unsigned size_;
};

class OutputVideo
{
public:
    OutputVideo(char* output, cv::VideoCapture& vid);
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
    Transformer(int filter);
    ~Transformer() = default;

    Chunk* operator()(Chunk* chunk) const;

private:
    int filter_;
};

#endif /* _PIPELINE_H_ */
