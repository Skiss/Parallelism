#ifndef _PIPELINE_H_
#define _PIPELINE_H_

#include <highgui.h>
#include <vector>
#include <tbb/pipeline.h>
#include <iostream>


typedef std::vector<std::function<void(cv::Mat&, bool)>> ImgProc;
typedef std::vector<std::function<cv::Mat(cv::Mat&,
                                          const cv::Mat&,
                                          bool)>> VideoProc;

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
    OutputVideo(const std::string& output, cv::VideoCapture& vid);
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
    Transformer(const ImgProc& imgProc, const VideoProc& videoProc);
    ~Transformer() = default;

    Chunk* operator()(Chunk* chunk) const;

private:
    ImgProc imgProc_;
    VideoProc videoProc_;
};

#endif /* _PIPELINE_H_ */
