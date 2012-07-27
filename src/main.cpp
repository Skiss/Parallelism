#include "pipeline.hpp"
#include "img_processing.hpp"
#include "videoProcessing.hpp"

#include <iostream>
#include <functional>


using namespace cv;

std::function<void(cv::Mat&, bool)>
getImgProc(char* s)
{
    if (strcmp(s, "--blur") == 0)
        return proc::blur;
    else if (strcmp(s, "--sharpen") == 0)
        return proc::sharpen;
    else if (strcmp(s, "--edge") == 0)
        return proc::edge_detect;
    else if (strcmp(s, "--light") == 0)
        return proc::light;
    else if (strcmp(s, "--dark") == 0)
        return proc::dark;
    else if (strcmp(s, "--invert") == 0)
        return proc::invert;
    else if (strcmp(s, "--mirror") == 0)
        return proc::mirror;

    return nullptr;
}

std::function<cv::Mat(cv::Mat&, const cv::Mat&)>
getVideoProc(char* s)
{
    if (strcmp(s, "-swap") == 0)
        return video::swapContent;
    else if (strcmp(s, "-blend") == 0)
        return video::blend;

    return nullptr;
}

void parseArgs(int argc, char** argv,
               ImgProc& imgProc,
               VideoProc& vidProc,
               std::vector<std::string>& videos)
{
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-' && argv[i][1] == '-')
            imgProc.push_back(getImgProc(argv[i]));
        else if (argv[i][0] == '-' && argv[i][1] != '-')
            vidProc.push_back(getVideoProc(argv[i]));
        else
            videos.push_back(argv[i]);
    }
}

void display_help()
{
    std::cout << "Usage: ./prpa option arg1 arg2"<< std::endl
              << std::endl
              << "Options:" << std::endl
              << "\t--help\t\t: display this help" << std::endl
              << std::endl
              << "Arguments:" << std::endl
              << "\targ1\t\t: video input" << std::endl
              << "\targ2\t\t: video output (AVI format)" << std::endl
              << std::endl
              << "Filters:" << std::endl
              << "\t--blur\t\t: normal blur"<< std::endl
              << "\t--blur-para\t: parallelized blur"<< std::endl
              << "\t--sharpen\t: normal sharpen"<< std::endl
              << "\t--sharpen-para\t: parallelized sharpen"<< std::endl
              << "\t--edge\t\t: normal edge detection"<< std::endl
              << "\t--edge-para\t: parallelized edge detection"<< std::endl
              << "\t--light\t\t: normal light filter"<< std::endl
              << "\t--light-para\t: parallelized light filter"<< std::endl
              << "\t--dark\t\t: normal dark filter"<< std::endl
              << "\t--dark-para\t: parallelized dark filter"<< std::endl
              << "\t--invert\t: normal inversion of color"<< std::endl
              << "\t--invert-para\t: parallelized inversion of color"<< std::endl
              << "\t--mirror\t: normal mirror"<< std::endl
              << "\t--mirror-para\t: parallelized mirror"<< std::endl;
}

void runPipeline(int nbThreads,
                 const std::vector<cv::Mat>& vid,
                 const std::vector<cv::Mat>& vid2,
                 cv::VideoCapture& video,
                 const ImgProc& imgProc,
                 const VideoProc& videoProc,
                 const std::string& output)
{
    tbb::parallel_pipeline(
        nbThreads,
        tbb::make_filter<void, Chunk*>(tbb::filter::serial_in_order,
                                       InputVideo(vid, vid2))
        &
        tbb::make_filter<Chunk*, Chunk*>(tbb::filter::parallel,
                                         Transformer(imgProc, videoProc))
        &
        tbb::make_filter<Chunk*, void>(tbb::filter::serial_in_order,
                                       OutputVideo(output, video))
        );
}

int main(int argc, char** argv)
{
    if ((argc == 2) && (strcmp(argv[1], "--help")) == 0) {
        display_help();
        return 0;
    }

    if (argc < 4) {
        std::cerr << "More arguments please." << std::endl;
        return -1;
    }

    std::vector<std::string> videos;
    std::vector<cv::Mat> v, v2;
    ImgProc imgProc;
    VideoProc videoProc;

    parseArgs(argc, argv, imgProc, videoProc, videos);

    if (videos.size() < 2) {
        std::cerr << "Not enough arguments." << std::endl;
        return -1;
    }

    cv::VideoCapture vid(videos.at(0));
    while (1) {
        cv::Mat frame;

        if (!vid.read(frame))
            break;

        v.push_back(frame.clone());
    }

    if (videos.size() == 3) {
        cv::VideoCapture vid2(videos.at(1));
        while (1) {
            cv::Mat frame;

            if (!vid2.read(frame))
                break;

            v2.push_back(frame.clone());
        }
    }

    int nbThreads = tbb::task_scheduler_init::default_num_threads();

    tbb::tick_count  t0 = tbb::tick_count::now();
    runPipeline(nbThreads, v, v2, vid, imgProc, videoProc, videos.back());
    tbb::tick_count  t1 = tbb::tick_count::now();
    std::cout << (t1 - t0).seconds() << " seconds" << std::endl;

    return 0;
}
