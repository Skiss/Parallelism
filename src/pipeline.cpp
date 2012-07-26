#include "pipeline.hpp"
#include "img_processing.hpp"
#include "videoProcessing.hpp"


Chunk::Chunk(cv::Mat* offset1, unsigned size, cv::Mat* offset2)
    : offset1_(offset1),
      offset2_(offset2),
      size_(size)
{}

OutputVideo::OutputVideo(char* output, cv::VideoCapture& vid)
    : vid(output,
          vid.get(CV_CAP_PROP_FOURCC),
          vid.get(CV_CAP_PROP_FPS),
          cv::Size(vid.get(CV_CAP_PROP_FRAME_WIDTH),
                   vid.get(CV_CAP_PROP_FRAME_HEIGHT)),
          true)
{}

void
OutputVideo::operator()(Chunk* c) const
{
    auto v = c->getFrames().first;
    unsigned size = c->getFrames().second;

    for (unsigned i = 0; i < size; ++i)
        vid << v[i];

    delete c;
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
    cv::Mat* v2 = nullptr;

    if (vid_.end() - (vid_.begin() + offset) >= Chunk::chunkSize)
        size = Chunk::chunkSize;
    else {
        size = vid_.end() - (vid_.begin() + offset);
        fc.stop();
    }

    if (!vid2_.empty())
        v2 = vid2_.data() + offset;

    Chunk* c = new Chunk(vid_.data() + offset, size, v2);

    offset += size;

    return c;
}

Transformer::Transformer(int filter)
    : filter_(filter)
{}

Chunk*
Transformer::operator()(Chunk* c) const
{
    auto v = c->getFrames().first;
    unsigned size = c->getFrames().second;
    auto v2 = c->getFrames2().first;

    if (v2) {
        for (unsigned i = 0; i < size; ++i)
            v[i] = proc::blur(v[i]);
    }
    else {
        if (filter_ == BLUR){
            for (unsigned i = 0; i < size; ++i)
                v[i] = proc::blur(v[i]);
        } else if (filter_ == BLUR_P){
            for (unsigned i = 0; i < size; ++i)
                v[i] = proc::blur(v[i], true);
        } else if (filter_ == SHARPEN){
            for (unsigned i = 0; i < size; ++i)
                v[i] = proc::sharpen(v[i]);
        } else if (filter_ == SHARPEN_P){
            for (unsigned i = 0; i < size; ++i)
                v[i] = proc::sharpen(v[i], true);
        } else if (filter_ == EDGE){
            for (unsigned i = 0; i < size; ++i)
                v[i] = proc::edge_detect(v[i]);
        } else if (filter_ == EDGE_P){
            for (unsigned i = 0; i < size; ++i)
                v[i] = proc::edge_detect(v[i], true);
        } else if (filter_ == LIGHT){
            for (unsigned i = 0; i < size; ++i)
                v[i] = proc::light(v[i]);
        } else if (filter_ == LIGHT_P){
            for (unsigned i = 0; i < size; ++i)
                v[i] = proc::light(v[i], true);
        } else if (filter_ == DARK){
            for (unsigned i = 0; i < size; ++i)
                v[i] = proc::dark(v[i]);
        } else if (filter_ == DARK_P){
            for (unsigned i = 0; i < size; ++i)
                v[i] = proc::dark(v[i], true);
        } else if (filter_ == INVERT){
            for (unsigned i = 0; i < size; ++i)
                v[i] = proc::invert(v[i]);
        } else if (filter_ == INVERT_P){
            for (unsigned i = 0; i < size; ++i)
                v[i] = proc::invert(v[i], true);
        } else if (filter_ == MIRROR){
            for (unsigned i = 0; i < size; ++i)
                v[i] = proc::mirror(v[i]);
        } else if (filter_ == MIRROR_P){
            for (unsigned i = 0; i < size; ++i)
                v[i] = proc::mirror(v[i], true);
        }
    }

    return c;
}
