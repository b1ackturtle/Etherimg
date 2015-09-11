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
    
    const auto startTime = std::chrono::system_clock::now();
    etherimg_send(argv[1], img);
    const auto endTime = std::chrono::system_clock::now();
    const auto timeSpan = endTime - startTime;
    printf("%lld\n", std::chrono::duration_cast<std::chrono::milliseconds>(timeSpan).count());
    
    if(cv::waitKey(200) == 'q') break;
  }

  //std::cout << img.cols << " " << img.rows << std::endl;

  //const auto startTime = std::chrono::system_clock::now();
  //etherimg_send(argv[1], img);
  //const auto endTime = std::chrono::system_clock::now();
  //const auto timeSpan = endTime - startTime;
  //std::cout << "処理時間:" << std::chrono::duration_cast<std::chrono::milliseconds>(timeSpan).count() << "[ms]" << std::endl;

  return 0;
}
