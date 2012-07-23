#include "pipeline.hpp"
#include "img_processing.hpp"


Chunk::Chunk(const std::vector<cv::Mat>& v,
             const std::vector<cv::Mat>& v2)
    : frames_(v),
      frames2_(v2)
{}

OutputVideo::OutputVideo(cv::VideoCapture& vid)
    : vid("resources/res.avi",
          vid.get(CV_CAP_PROP_FOURCC),
          vid.get(CV_CAP_PROP_FPS),
          cv::Size(vid.get(CV_CAP_PROP_FRAME_WIDTH),
                   vid.get(CV_CAP_PROP_FRAME_HEIGHT)),
          true)
{}

void
OutputVideo::operator()(Chunk* c) const
{
    for (auto f : *c->getFrames())
        vid << f;
}

InputVideo::InputVideo(const std::vector<cv::Mat>& vid,
                       const std::vector<cv::Mat>& vid2)
    : vid_(vid),
      vid2_(vid2),
      offset(0)
{}

Chunk*
InputVideo::operator()(tbb::flow_control& fc) const
{
    unsigned size = 0;

    if (vid_.end() - (vid_.begin() + offset) >= Chunk::chunkSize)
        size = Chunk::chunkSize;
    else {
        size = vid_.end() - (vid_.begin() + offset);
        fc.stop();
    }

    std::vector<cv::Mat> v(vid_.begin() + offset, vid_.begin() + offset + size);
    std::vector<cv::Mat> v2;

    if (!vid2_.empty())
        std::copy(vid2_.begin() + offset,
                  vid2_.begin() + offset + size,
                  std::back_inserter(v2));

    Chunk* c = new Chunk(v, v2);

    offset += size;

    return c;
}

Chunk*
Transformer::operator()(Chunk* c) const
{
    // for (auto& f : *c->getFrames()) {
    //     f = proc::blur(f);
    // }

    return c;
}
