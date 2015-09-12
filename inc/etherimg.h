#ifndef _ETHER_IMG_H_INCLUDED_
#define _ETHER_IMG_H_INCLUDED_

//typedef std::array<cv::Vec3b, 5000000> Array3b;

void etherimg_send(char*, cv::Mat);
void etherimg_recv(char*, cv::Mat&);
//void mtov(cv::Mat, std::vector< std::vector< cv::Vec3b> >&);
void mtov(cv::Mat, std::vector<cv::Vec3b>&);
//void vtom(std::vector< std::vector< cv::Vec3b> >, cv::Mat&);
//void vtom(std::array< std::array< cv::Vec3b, 640>, 480>, cv::Mat&);
//void vtom(boost::multi_array<cv::Vec3b, 2>, cv::Mat&, int, int);
void vtom(std::vector<cv::Vec3b>&, cv::Mat&, int, int);
//void vtom(Array3b&, cv::Mat&, int, int);

#endif
