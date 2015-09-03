#include <iostream>
#include <cstdio>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "lib.h"
#include "etherimg.h"

static std::vector<unsigned char> buffer{
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //Destination MAC Address
  0x00, 0x11, 0x22, 0x33, 0x44, 0x55, //Source MAC Address
  0x15, 0x15, //EtherType
};

void etherimg_send(char *netif, cv::Mat img)
{
  int fd, size;
  std::vector< std::vector<cv::Vec3b> > vec;
  int packet_max = 1493;
  static bool flag = false;

  mtov(img, vec);

  int height = img.rows, width = img.cols;
  int channels = img.channels();

  if(!flag) {fd = pkthandler.open_send(netif, 0); flag = true;}

  buffer.push_back(height>>8); buffer.push_back(height);
  buffer.push_back(width>>8); buffer.push_back(width);
  buffer.push_back(channels);

  int num = 0;
  int seq = 0;

  std::vector<unsigned char> packet = buffer;
  packet.push_back(seq>>8); packet.push_back(seq);
  for(int i = 0; i < height; i++) {
    for(int j = 0; j < width; j++) {
      for(int k = 0; k < channels; k++) {
	if(num >= packet_max) {
	  num = 0;
	  size = packet.size();
	  //for(auto v: packet) printf("%x ", v); std::cout << std::endl;
	  if(size > 0) pkthandler.send(fd, (char *)&packet[0], size);
	  packet.clear(); packet = buffer;
	  seq++;
	  packet.push_back(seq>>8); packet.push_back(seq);
	}
	num++;
	packet.push_back(vec[i][j][k]);
      }
    }
  }

  //for(auto v: packet) printf("%x ", v); std::cout << std::endl;

  size = packet.size();
  if(size > 0) pkthandler.send(fd, (char *)&packet[0], size);

  return;
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
