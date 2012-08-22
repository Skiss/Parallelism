#include "pipeline.hpp"
#include "img_processing.hpp"
#include "videoProcessing.hpp"

#include <iostream>
#include <functional>


/// Get the right image processing function
std::function<void(cv::Mat&, bool)>
getImgProc(const std::string& s)
{
    if (s == "--blur")
        return img::blur;
    else if (s == "--sharpen")
        return img::sharpen;
    else if (s == "--edge")
        return img::edge_detect;
    else if (s == "--light")
        return img::light;
    else if (s == "--dark")
        return img::dark;

    return nullptr;
}

/// Get the right video processing function
std::function<void(cv::Mat&, const cv::Mat&, bool)>
getVideoProc(const std::string& s)
{
    if (s == "-swap")
        return video::swapContent;
    else if (s == "-blend")
        return video::blend;

    return nullptr;
}

/// Parses the arguments in order to get the videos
/// and the process to apply on them
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
    std::cout << "Usage: ./prpa options input output"<< std::endl
              << std::endl
              << "Options:" << std::endl
              << "\t--help\t\t: display this help" << std::endl
              << std::endl
              << "Arguments:" << std::endl
              << "\tinput\t\t: video input" << std::endl
              << "\toutput\t\t: video output" << std::endl
              << std::endl
              << "Filters:" << std::endl
              << "\t--blur\t\t: blur"<< std::endl
              << "\t--sharpen\t: sharpen"<< std::endl
              << "\t--edge\t\t: edge detection"<< std::endl
              << "\t--light\t\t: light filter"<< std::endl
              << "\t--dark\t\t: dark filter"<< std::endl;
}


void runPipeline(int nbThreads,
                 std::vector<std::string>& videos,
                 const ImgProc& imgProc,
                 const VideoProc& videoProc)
{
    std::string output = videos.back();
    videos.pop_back();

    tbb::parallel_pipeline(
        nbThreads,
        tbb::make_filter<void, Chunk*>(tbb::filter::serial_in_order,
                                       InputVideo(videos))
        &
        tbb::make_filter<Chunk*, Chunk*>(tbb::filter::parallel,
                                         Transformer(imgProc, videoProc))
        &
        tbb::make_filter<Chunk*, void>(tbb::filter::serial_in_order,
                                       OutputVideo(output, cv::VideoCapture(videos.front())))
        );
}

int main(int argc, char** argv)
{
    if (std::string(argv[1]) == "--help") {
        display_help();
        return 0;
    }

    if (argc < 4) {
        std::cerr << "More arguments please." << std::endl;
        return -1;
    }

    std::vector<std::string> videos;
    ImgProc imgProc;
    VideoProc videoProc;

    parseArgs(argc, argv, imgProc, videoProc, videos);

    if (videos.size() < 2) {
        display_help();
        return -1;
    }

    int nbThreads = tbb::task_scheduler_init::default_num_threads();

    tbb::tick_count  t0 = tbb::tick_count::now();
    runPipeline(nbThreads, videos, imgProc, videoProc);
    tbb::tick_count  t1 = tbb::tick_count::now();
    std::cout << (t1 - t0).seconds() << " seconds" << std::endl;

    return 0;
}
