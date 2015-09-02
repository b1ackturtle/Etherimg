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

void etherimg_send(char*, cv::Mat);
void mtov(cv::Mat, std::vector< std::vector< cv::Vec3b> >&);
void vtom(std::vector< std::vector< cv::Vec3b> >, cv::Mat&);

bool flag = false;

static std::vector<unsigned char> buffer{
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //Destination MAC Address
  0x00, 0x11, 0x22, 0x33, 0x44, 0x55, //Source MAC Address
  0x15, 0x15, //EtherType
      //0x45, 0x00, 0x00, 0x2c, 0x02, 0x25, 0x00, 0x00, 0x40, 0x11,
      //0xf5, 0x48, 0xc0, 0xa8, 0x01, 0x01, 0xc0, 0xa8, 0x01, 0x02,
      //0x5f, 0xee, 0x27, 0x10, 0x00, 0x18, 0x83, 0x7d,
      //0x55, 0x44, 0x50, 0x20, 0x74, 0x65, 0x73, 0x74,
      //0x20, 0x70, 0x61, 0x63, 0x6b, 0x65, 0x74, 0x0a,
};

int main(int argc, char *argv[])
{
  //int fd, size;

  //std::vector< std::vector<cv::Vec3b> > vec;
  // std::vector<unsigned char> buffer{
  //   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //Destination MAC Address
  //   0x00, 0x11, 0x22, 0x33, 0x44, 0x55, //Source MAC Address
  //     0x15, 0x15, //EtherType
  //     //0x45, 0x00, 0x00, 0x2c, 0x02, 0x25, 0x00, 0x00, 0x40, 0x11,
  //     //0xf5, 0x48, 0xc0, 0xa8, 0x01, 0x01, 0xc0, 0xa8, 0x01, 0x02,
  //     //0x5f, 0xee, 0x27, 0x10, 0x00, 0x18, 0x83, 0x7d,
  //     //0x55, 0x44, 0x50, 0x20, 0x74, 0x65, 0x73, 0x74,
  //     //0x20, 0x70, 0x61, 0x63, 0x6b, 0x65, 0x74, 0x0a,
  // };

  cv::VideoCapture cam(atoi(argv[2]));
  cam.set(CV_CAP_PROP_FRAME_WIDTH, atoi(argv[3]));
  cam.set(CV_CAP_PROP_FRAME_HEIGHT, atoi(argv[4]));

  cv::Mat img;

  while(1) {
    cam >> img;

    imshow("test", img);
    
    etherimg_send(argv[1], img);
    
    if(cv::waitKey(200) == 'q') break;
  }

  //std::cout << img.cols << " " << img.rows << std::endl;

  //const auto startTime = std::chrono::system_clock::now();
  etherimg_send(argv[1], img);
  //const auto endTime = std::chrono::system_clock::now();
  //const auto timeSpan = endTime - startTime;
  //std::cout << "処理時間:" << std::chrono::duration_cast<std::chrono::milliseconds>(timeSpan).count() << "[ms]" << std::endl;

  // mtov(img, vec);

  // int height = vec.size(), width = vec[0].size();

  // buffer.push_back(height>>8); buffer.push_back(height);
  // buffer.push_back(width>>8); buffer.push_back(width);
  // buffer.push_back(img.channels());

  // for(int i = 0; i < height; i++) {
  //   for(int j = 0; j < width; j++) {
  //     for(int k = 0; k < 3; k++) {
  // 	buffer.push_back(vec[i][j][k]);
  //     }
  //   }
  // }
  // //  for(int i = 0; i < 10000; i++) buffer.push_back(0x00);

  // for(auto v: buffer) printf("%x ", v); std::cout << std::endl;

  // fd = pkthandler.open_send(argv[1], 0);
  // size = buffer.size();

  // std::cout << size << std::endl;

  // if(size > 0) pkthandler.send(fd, (char *)&buffer[0], size);

  return 0;
}

void etherimg_send(char *netif, cv::Mat img)
{
  static int fd;
  int size;
  std::vector< std::vector<cv::Vec3b> > vec;
  int packet_max = 1493;

  mtov(img, vec);

  //int height = vec.size(), width = vec[0].size();
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
