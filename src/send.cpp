#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <unistd.h>
#include <chrono>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "lib.h"
#include "etherimg.h"

int main(int argc, char *argv[])
{
  cv::VideoCapture cam(atoi(argv[2]));
  cam.set(CV_CAP_PROP_FRAME_WIDTH, atoi(argv[3]));
  cam.set(CV_CAP_PROP_FRAME_HEIGHT, atoi(argv[4]));

  cv::Mat img;

  while(1) {
    cam >> img;

    imshow("test", img);
    
    etherimg_send(argv[1], img);
    
    if(cv::waitKey(20) == 'q') break;
  }

  return 0;
}
