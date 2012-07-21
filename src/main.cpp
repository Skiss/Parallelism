#include "img_processing.hpp"


using namespace cv;

int main( int argc, char** argv )
{
    tbb::task_scheduler_init init;

    Mat image;
    image = imread( argv[1], 1 );

    if (argc != 2)
    {
        std::cerr << "More arguments please" << std::endl;
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

    // namedWindow( "Display Image", CV_WINDOW_AUTOSIZE );
    // imshow( "Blur", res);
    // imshow("Display Image", image);

    // waitKey(0);
    cv::VideoCapture vid(argv[1]);
    cv::VideoWriter res("resources/res.avi",
                        vid.get(CV_CAP_PROP_FOURCC),
                        vid.get(CV_CAP_PROP_FPS),
                        cv::Size(vid.get(CV_CAP_PROP_FRAME_WIDTH),
                                 vid.get(CV_CAP_PROP_FRAME_HEIGHT)),
                        true);

    Mat frame;
    while (vid.read(frame))
    {
        // Mat frame;
        // vid >> frame; // get a new frame from camera
        res << proc::blur(frame);

        // if(waitKey(30) >= 0) break;
    }

    return 0;
}
