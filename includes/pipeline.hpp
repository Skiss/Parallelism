#ifndef _PIPELINE_H_
#define _PIPELINE_H_

#include <highgui.h>
#include <tbb/pipeline.h>

#include <vector>
#include <iostream>


/// Vector for image processing functions
typedef std::vector<std::function<void(cv::Mat&, bool)>> ImgProc;
/// Vector for video processing functions
typedef std::vector<std::function<void(cv::Mat&, const cv::Mat&, bool)>> VideoProc;

/// A chunk contains a part of a video (e.g. a certain number of frames)
class Chunk
{
public:
    static const unsigned chunkSize = 200;

    Chunk(const std::vector<cv::Mat>& v,
          const std::vector<cv::Mat>& v2) : frames_(v), frames2_(v2) {}
    ~Chunk() = default;

    std::vector<cv::Mat> getFrames() { return frames_; }
    std::vector<cv::Mat> getFrames2() { return frames2_; }

private:
    std::vector<cv::Mat> frames_, frames2_;
};

/// The final video, resulting from the input video on which frames
/// we applied some processing (blur, blend, ...)
class OutputVideo
{
public:
    OutputVideo(const std::string& output, cv::VideoCapture&& vid);
    ~OutputVideo() = default;

    void operator()(Chunk* chunk) const;

private:
    mutable cv::VideoWriter vid_;
};

/// The input video on which we need to work on
class InputVideo
{
public:
    InputVideo(const std::vector<std::string>& videos);
    ~InputVideo() = default;

    Chunk* operator()(tbb::flow_control& fc) const;

private:
    mutable std::vector<cv::VideoCapture> videos_;
};

/// Takes a chunk an create a new one after processing the frames
/// of the first one
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
