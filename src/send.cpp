#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "lib.h"

void mtov(cv::Mat, std::vector< std::vector< cv::Vec3b> >&);
void vtom(std::vector< std::vector< cv::Vec3b> >, cv::Mat&);

int main(int argc, char *argv[])
{
  int fd, size;

  std::vector< std::vector<cv::Vec3b> > vec;
  std::vector<unsigned char> buffer{
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //Destination MAC Address
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, //Source MAC Address
    0x15, 0x15 //EtherType
  };

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

  int height = vec.size(), width = vec[0].size();

  buffer.push_back(height>>8); buffer.push_back(height);
  buffer.push_back(width>>8); buffer.push_back(width);
  buffer.push_back(img.channels());

  for(int i = 0; i < height; i++) {
    for(int j = 0; j < width; j++) {
      for(int k = 0; k < 3; k++) {
	//printf("%d ", vec[i][j][k]);
	buffer.push_back(vec[i][j][k]);
      } //printf("\n");
    }
  }
  //  for(int i = 0; i < 10000; i++) buffer.push_back(0x00);

  for(auto v: buffer) printf("%x ", v); std::cout << std::endl;

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

  //cv::Mat tmp;
  //vtom(vec, tmp);
  //cv::imshow("hoge", tmp); cv::waitKey(5000);

  fd = pkthandler.open_send(argv[1], 0);
  size = buffer.size();

  if(size > 0) pkthandler.send(fd, (char *)&buffer[0], size);

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

  
