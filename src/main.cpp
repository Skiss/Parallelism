#include "img_processing.hpp"


using namespace cv;

int main( int argc, char** argv )
{
    if (argc != 2)
    {
        std::cerr << "More arguments please." << std::endl;

        return -1;

    }
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
