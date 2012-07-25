#include "pipeline.hpp"
#include "img_processing.hpp"
#include <iostream>
#include <functional>


typedef std::vector<std::function<cv::Mat(const cv::Mat&, bool)>> ImgProc;
typedef std::vector<std::function<cv::Mat(const cv::Mat&,
                                          const cv::Mat&,
                                          bool)>> VideoProc;

using namespace cv;

void runPipeline(int nbThreads,
                 const std::vector<cv::Mat>& vid,
                 const std::vector<cv::Mat>& vid2,
                 cv::VideoCapture& video)
{
    tbb::parallel_pipeline(
        nbThreads,
        tbb::make_filter<void, Chunk*>(tbb::filter::serial_in_order,
                                       InputVideo(vid, vid2))
        &
        tbb::make_filter<Chunk*, Chunk*>(tbb::filter::parallel,
                                         Transformer())
        &
        tbb::make_filter<Chunk*, void>(tbb::filter::serial_in_order,
                                       OutputVideo(video))
        );
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "More arguments please." << std::endl;
        return -1;
    }

    cv::VideoCapture vid(argv[1]);
    std::vector<cv::Mat> v, v2;

    while (1) {
        cv::Mat frame;

        if (!vid.read(frame))
            break;

        v.push_back(frame.clone());
    }

    if (argc > 2) {
        cv::VideoCapture vid2(argv[2]);

        while (1) {
            cv::Mat frame;

            if (!vid2.read(frame))
                break;

            v2.push_back(frame.clone());
        }
    }

    ImgProc imgProc = {proc::blur};
    VideoProc videoProc = {};

    int nbThreads = tbb::task_scheduler_init::default_num_threads();

    tbb::tick_count  t0 = tbb::tick_count::now();
    runPipeline(nbThreads, v, v2, vid);
    tbb::tick_count  t1 = tbb::tick_count::now();
    std::cout << (t1 - t0).seconds() << " seconds" << std::endl;

    return 0;
}
