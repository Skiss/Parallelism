#include "img_processing.hpp"


using namespace cv;

int main( int argc, char** argv )
{
    tbb::task_scheduler_init init;

    if (argc != 2)
    {
        std::cerr << "More arguments please." << std::endl;
        return -1;
    }

    // tbb::tick_count  t0 = tbb::tick_count::now();
    // Mat res = proc::blur(image);
    // tbb::tick_count  t1 = tbb::tick_count::now();
    // std::cout << "Sequential : " << (t1 - t0).seconds() << "s" << std::endl;

    // tbb::tick_count  t2 = tbb::tick_count::now();
    // res = proc::blur(image, true);
    // tbb::tick_count  t3 = tbb::tick_count::now();
    // std::cout << "Parallel : " << (t3 - t2).seconds() << "s" << std::endl;

    cv::VideoCapture vid(argv[1]);
    std::cout << vid.get(CV_CAP_PROP_FRAME_COUNT) << std::endl;

    // namedWindow( "Display Image", CV_WINDOW_AUTOSIZE );
    // imshow( "Blur", res);
    // imshow("Display Image", image);

    // waitKey(0);

    return 0;
}
