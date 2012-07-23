#include "pipeline.hpp"
#include "img_processing.hpp"


Chunk::Chunk(const std::vector<cv::Mat>& v)
    : frames_(v)
{
    // static unsigned nbFramesRead = 0;
    // std::vector<int> compression_params;
    // compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    // compression_params.push_back(9);

    // for (auto f : v)
    // {
    //     std::ostringstream oss;
    //     oss << nbFramesRead++;
    //     std::string name = "/media/Data/" + oss.str() + ".png";

    //     cv::imwrite(name, f, compression_params);
    // }
}

// bool
// Chunk::append(cv::Mat* frame)
// {
//     frames_.push_back(frame);

//     if (frames_.size() == chunkSize)
//         return false;

//     return true;
// }

OutputVideo::OutputVideo(cv::VideoCapture& vid)
    : vid("resources/res.avi",
          vid.get(CV_CAP_PROP_FOURCC),
          vid.get(CV_CAP_PROP_FPS),
          cv::Size(vid.get(CV_CAP_PROP_FRAME_WIDTH),
                   vid.get(CV_CAP_PROP_FRAME_HEIGHT)),
          true)
{}

// OutputVideo::~OutputVideo()
// {}

void
OutputVideo::operator()(Chunk* c) const
{
    auto v = c->getFrames();

    for (auto f : v)
        vid << f;

    // static unsigned size = 0;
    // size += v.size();
    // std::cout << "size : " << size << std::endl;

    // std::copy(v.begin(), v.end(), std::back_inserter(vid_));

    // static unsigned n = 0;
    // for (auto frame : c->getFrames()) {
    //     vid_ << *frame;
    //     std::cout << ++n << std::endl;
    // }
}

InputVideo::InputVideo(const std::vector<cv::Mat>& vid)
    : vid_(vid),
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

    std::vector<cv::Mat> v(vid_.begin() + offset,
                           vid_.begin() + offset + size);
    Chunk* c = new Chunk(v);

    offset += size;

    // static unsigned nbFramesRead = 0;
    // std::vector<int> compression_params;
    // compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    // compression_params.push_back(9);

    // while (1)
    // {
    //     cv:: Mat* frame = new cv::Mat;

    //     if (!vid_.read(*frame))
    //     {
    //         fc.stop();
    //         break;
    //     }

        // std::ostringstream oss;
        // oss << nbFramesRead;
        // std::string name = "/media/Data/" + oss.str() + ".png";

        // cv::imwrite(name, *frame, compression_params);

    //     if (!c->append(frame))
    //         break;
    // }

    // if (nbFramesRead >= vid_.get(CV_CAP_PROP_FRAME_COUNT))
    //     fc.stop();

    return c;
}

Chunk*
Transformer::operator()(Chunk* c) const
{
    // for (auto f : c->getFrames()) {
    //     f = proc::blur(*f);
    // }

    return c;
}
