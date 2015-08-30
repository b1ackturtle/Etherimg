#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <netinet/in.h>

void mtov(cv::Mat, std::vector< std::vector< cv::Vec3b> >&);
void vtom(std::vector< std::vector< cv::Vec3b> >, cv::Mat&);

int main(int argc, char *argv[])
{
  std::vector< std::vector<cv::Vec3b> > vec;

  cv::VideoCapture cam(atoi(argv[2]));
  cam.set(CV_CAP_PROP_FRAME_WIDTH, atoi(argv[3]));
  cam.set(CV_CAP_PROP_FRAME_HEIGHT, atoi(argv[4]));

  cv::Mat img;

  while(1) {
    cam >> img;

    imshow("test", img);
    
    if(cv::waitKey(1) == 'q') break;
  }

  std::cout << img.cols << " " << img.rows << std::endl;

  mtov(img, vec);

  // for(int i = 0; i < vec.size(); i++) {
  //   for(int j = 0; j < vec[i].size(); j++) {
  //     for(int k = 0; k < 3; k++) {
  // 	printf("%d ", vec[i][j][k]);
  //     } printf("\n");
  //   }
  // }

  // cv::Mat tmp(vec.size(), vec[0].size(), CV_8UC3);
  // for(int i = 0; i < vec.size(); i++) {
  //   for(int j = 0; j < vec[0].size(); j++) {
  //     for(int k = 0; k < 3; k++) {
  // 	tmp.at<cv::Vec3b>(i, j) = vec[i][j];
  //     }
  //   }
  // }

  // std::cout << tmp << std::endl;
  // tmp.reshape(480, 640);

  cv::Mat tmp;
  vtom(vec, tmp);
  cv::imshow("hoge", tmp); cv::waitKey(5000);

  return 0;
}

void mtov(cv::Mat src, std::vector< std::vector< cv::Vec3b> >& dst)
{
  for(int i = 0; i < src.rows; i++) {
    dst.push_back(src.row(i));
  }

  return;
}

void vtom(std::vector< std::vector< cv::Vec3b> > src, cv::Mat& dst)
{
  int height = src.size(), width = src[0].size();

  dst = cv::Mat(height, width, CV_8UC3);
  for(int i = 0; i < height; i++) {
    for(int j = 0; j < width; j++) {
      for(int k = 0; k < 3; k++) {
	dst.at<cv::Vec3b>(i, j) = src[i][j];
      }
    }
  }

  dst.reshape(height, width);

  return;
}

  
