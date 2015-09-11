#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <unistd.h>
#include <chrono>

#include <thread>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "lib.h"
#include "etherimg.h"

#include <unistd.h>

int main(int argc, char *argv[])
{
  cv::Mat img;

  while(1) {
    etherimg_recv(argv[1], img);
    //std::cout << "main" << std::endl;
    //usleep(10);
    imshow("res", img);

    if(cv::waitKey(1) == 'q') break;
  }

  //  while(1) {
    //img = etherimg_recv(argv[1]);

    //imshow("res", img);
    //if(cv::waitKey(1) == 'q') break;
    //}

  return 0;
}
