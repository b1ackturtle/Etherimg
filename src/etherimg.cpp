#include <iostream>
#include <cstdio>
#include <vector>
#include <unistd.h>
#include <chrono>
#include <array>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <net/ethernet.h>
#include <arpa/inet.h>

#include "lib.h"
#include "etherimg.h"

#define PKT_SIZE_MAX 1491

static struct Etherimg_header{
  u_char dMAC[ETHER_ADDR_LEN];
  u_char sMAC[ETHER_ADDR_LEN];
  u_short etherType;
  u_short height;
  u_short width;
  u_short channels;
  u_short seq;
  u_char data[PKT_SIZE_MAX];
} *etherimg_header;

void etherimg_send(char *netif, cv::Mat img)
{
  static int fd = pkthandler.open_send(netif, 0);;
  int size;
  std::vector< std::vector<cv::Vec3b> > vec;

  int height = img.rows, width = img.cols;
  int channels = img.channels();

  static std::vector<unsigned char> buffer{
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //Destination MAC Address
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, //Source MAC Address
    0x15, 0x15, //EtherType
    (unsigned char)(height>>8), (unsigned char)height, //Image Height
    (unsigned char)(width>>8), (unsigned char)width, //Image Width
    0, (unsigned char)channels, //Image Channels
  };

  mtov(img, vec);

  int num = 0;
  int seq = 0;

  std::vector<unsigned char> packet = buffer;
  packet.push_back(seq>>8); packet.push_back(seq);
  for(int i = 0; i < height; i++) {
    for(int j = 0; j < width; j++) {
      for(int k = 0; k < channels; k++) {
	if(num >= PKT_SIZE_MAX) {
	  num = 0;
	  size = packet.size();
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

  size = packet.size();
  if(size > 0) pkthandler.send(fd, (char *)&packet[0], size);

  return;
}

void etherimg_recv(char* netif, cv::Mat& img)
{
  int size;
  char buffer[65536];
  int height, width, channels;
  int seq, old_seq = -1;

  static int fd = pkthandler.open_recv(netif, 0, NULL);
  static std::array< std::array<cv::Vec3b, 640>, 480> arr;

  while(1){
    size = pkthandler.recv(fd, buffer, sizeof(buffer), NULL);

    if(size <= 0) {std::cerr << "packet size is zero." << std::endl; exit(1);}
  
    etherimg_header = (struct Etherimg_header*)&buffer;
    //printf("ether type:%x\n", ntohs(etherimg_header->etherType));
    //printf("Height:%d\n", ntohs(etherimg_header->height));
    //printf("Width:%d\n", ntohs(etherimg_header->width));
    //printf("Channels:%d\n", ntohs(etherimg_header->channels));
    //printf("sequence:%d\n", ntohs(etherimg_header->seq));
    //printf("size: %d\n", size);

    height = ntohs(etherimg_header->height);
    width = ntohs(etherimg_header->width);
    channels = ntohs(etherimg_header->channels);
    seq = ntohs(etherimg_header->seq);

    if(ntohs(etherimg_header->etherType) == 0x1515) {
      for(int i = 0; i < size-22; i++) {
	*(&arr[0][0][0]+seq*PKT_SIZE_MAX+i) = etherimg_header->data[i];
      }
    }

    if(ntohs(etherimg_header->seq) == height*width*channels/PKT_SIZE_MAX) {
      vtom(arr, img);
      return;
    }
  }

  return;
}

void mtov(cv::Mat src, std::vector< std::vector< cv::Vec3b> >& dst)
{
  for(int i = 0; i < src.rows; i++) {
    dst.push_back(src.row(i));
  }

  return;
}

//void vtom(std::vector< std::vector< cv::Vec3b> > src, cv::Mat& dst)
void vtom(std::array< std::array< cv::Vec3b,640>, 480 > src, cv::Mat& dst)
//void vtom(boost::multi_array<cv::Vec3b, 2> src, cv::Mat& dst, int height, int width)
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
