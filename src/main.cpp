#include "pipeline.hpp"
#include "img_processing.hpp"
#include <iostream>


using namespace cv;

void runPipeline(int nbThreads,
                 const std::vector<cv::Mat>& vid,
                 cv::VideoCapture& video)
{
    tbb::parallel_pipeline(
        nbThreads,
        tbb::make_filter<void, Chunk*>(tbb::filter::serial_in_order,
                                       InputVideo(vid))
        &
        tbb::make_filter<Chunk*, Chunk*>(tbb::filter::parallel,
                                         Transformer())
        &
        tbb::make_filter<Chunk*, void>(tbb::filter::serial_in_order,
                                       OutputVideo(video))
        );
}

int main( int argc, char** argv )
{
    if (argc != 2)
    {
        std::cerr << "More arguments please." << std::endl;
        return -1;
    }

    cv::VideoCapture vid(argv[1]);
    std::vector<cv::Mat> v;

    while (1) {
        cv::Mat frame;

        if (!vid.read(frame))
            break;

        v.push_back(frame.clone());
    }

    int nbThreads = tbb::task_scheduler_init::default_num_threads();

    runPipeline(nbThreads, v, vid);

    return 0;
}
