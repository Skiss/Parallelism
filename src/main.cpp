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
                 cv::VideoCapture& video,
                 int filter,
                 char* output)
{
    tbb::parallel_pipeline(
        nbThreads,
        tbb::make_filter<void, Chunk*>(tbb::filter::serial_in_order,
                                       InputVideo(vid, vid2))
        &
        tbb::make_filter<Chunk*, Chunk*>(tbb::filter::parallel,
                                       Transformer(filter))
        &
        tbb::make_filter<Chunk*, void>(tbb::filter::serial_in_order,
                                       OutputVideo(output, video))
        );
}

int main(int argc, char** argv)
{
    if (argc < 4)
    {
        std::cerr << "More arguments please." << std::endl;
        return -1;
    } else if (argc > 4)
    {
        std::cerr << "Too much arguments." << std::endl;
        return -1;
    }

    cv::VideoCapture vid(argv[2]);
    std::vector<cv::Mat> v, v2;

    while (1) {
        cv::Mat frame;

        if (!vid.read(frame))
            break;

        v.push_back(frame.clone());
    }

    /*if (argc > 2) {
        cv::VideoCapture vid2(argv[2]);

        while (1) {
            cv::Mat frame;

            if (!vid2.read(frame))
                break;

            v2.push_back(frame.clone());
        }
    }

    ImgProc imgProc = {proc::blur};
    VideoProc videoProc = {};*/

    int filter;

    if (strcmp(argv[1], "--blur") == 0)
        filter = BLUR;
    if (strcmp(argv[1], "--sharpen") == 0)
        filter = SHARPEN;
    if (strcmp(argv[1], "--edge") == 0)
        filter = EDGE;
    if (strcmp(argv[1], "--light") == 0)
        filter = LIGHT;
    if (strcmp(argv[1], "--dark") == 0)
        filter = DARK;
    if (strcmp(argv[1], "--invert") == 0)
        filter = INVERT;
    if (strcmp(argv[1], "--mirror") == 0)
        filter = MIRROR;
    if (strcmp(argv[1], "--blur-para") == 0)
        filter = BLUR_P;
    if (strcmp(argv[1], "--sharpen-para") == 0)
        filter = SHARPEN_P;
    if (strcmp(argv[1], "--edge-para") == 0)
        filter = EDGE_P;
    if (strcmp(argv[1], "--light-para") == 0)
        filter = LIGHT_P;
    if (strcmp(argv[1], "--dark-para") == 0)
        filter = DARK_P;
    if (strcmp(argv[1], "--invert-para") == 0)
        filter = INVERT_P;
    if (strcmp(argv[1], "--mirror-para") == 0)
        filter = MIRROR_P;
    
    int nbThreads = tbb::task_scheduler_init::default_num_threads();

    tbb::tick_count  t0 = tbb::tick_count::now();
    runPipeline(nbThreads, v, v2, vid, filter, argv[3]);
    tbb::tick_count  t1 = tbb::tick_count::now();
    std::cout << (t1 - t0).seconds() << " seconds" << std::endl;

    return 0;
}
