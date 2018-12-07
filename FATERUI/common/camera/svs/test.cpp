#include "CameraSVS.h"
#include "stdio.h"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

// g++ -std=c++11 CameraSVS.cpp test.cpp -o test -lsvgige -lpthread `pkg-config --cflags --libs opencv` -L/usr/local/cuda-7.5/lib64

int main(int argc, char const *argv[])
{
    CameraSVS* _camera = new CameraSVS();
    _camera->open();

    cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );
    while(1)
    {
        unsigned char* _RgbBuffer = _camera->capture_one_frame();
        if(_RgbBuffer == NULL)
        {
            continue;
        }
        int width = 0;
        int height = 0;
        _camera->get_frame_w_h(&width, &height);

        cv::Mat cvImage;
        cvImage = cv::Mat(height,
                        width,
                        CV_8UC3,
                        _RgbBuffer);
        cv::cvtColor(cvImage, cvImage, CV_RGB2BGR);

        double _rate = _camera->get_frame_rate();
        printf("rate is %f\n", (float)_rate);

        cv::imshow("Display window", cvImage);
        // cv::imwrite("test.jpg", cvImage);
        cv::waitKey(1000);

        delete[] _RgbBuffer;
    }
    return 0;
}
