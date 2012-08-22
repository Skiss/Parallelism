#include "pipeline.hpp"
#include "img_processing.hpp"
#include "videoProcessing.hpp"


OutputVideo::OutputVideo(const std::string& output, cv::VideoCapture&& vid)
    : vid_(output,
           vid.get(CV_CAP_PROP_FOURCC),
           vid.get(CV_CAP_PROP_FPS),
           cv::Size(vid.get(CV_CAP_PROP_FRAME_WIDTH),
                    vid.get(CV_CAP_PROP_FRAME_HEIGHT)),
           true)
{}

void
OutputVideo::operator()(Chunk* c) const
{
    // We add the frames of the chunk to the final video
    for (auto f : c->getFrames())
        vid_ << f;

    delete c;
}

InputVideo::InputVideo(const std::vector<std::string>& videos)
{
    for (auto name : videos)
        videos_.push_back(cv::VideoCapture(name));
}

Chunk*
InputVideo::operator()(tbb::flow_control& fc) const
{
    std::vector<cv::Mat> frames[2];

    unsigned count = 0;

    // We get the frames from the video
    while (count++ < Chunk::chunkSize) {
        cv::Mat frame;

        if (!videos_.at(0).read(frame)) {
            fc.stop(); // No more frames to read, we stop
            break;
        }

        frames[0].push_back(frame.clone());
    }

    // If there is a second video, we do the same thing
    if (videos_.size() == 2) {
        unsigned i = 0;

        while (++i < count) {
            cv::Mat frame;

            if (!videos_.at(1).read(frame))
                break;

            frames[1].push_back(frame.clone());
        }
    }

    // We create a chunk containing the frames
    Chunk* c = new Chunk(frames[0], frames[1]);

    return c;
}

Transformer::Transformer(const ImgProc& imgProc, const VideoProc& videoProc)
    : imgProc_(imgProc),
      videoProc_(videoProc)
{}

Chunk*
Transformer::operator()(Chunk* c) const
{
    auto frames = c->getFrames();
    auto frames2 = c->getFrames2();

    // If there is only one video, we apply the image processing functions
    if (frames2.empty()) {
        for (auto f : imgProc_)
            for (auto frame : frames)
                f(frame, false);
    }
    else { // Else, we apply the video processing functions
        for (auto f : videoProc_) {
            for (unsigned i = 0; i < frames.size(); ++i)
                f(frames.at(i), frames2.at(i), false);
        }
    }

    return c;
}
