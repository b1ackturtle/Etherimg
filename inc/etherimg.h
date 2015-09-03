#ifndef _ETHER_IMG_H_INCLUDED_
#define _ETHER_IMG_H_INCLUDED_

void etherimg_send(char*, cv::Mat);
void mtov(cv::Mat, std::vector< std::vector< cv::Vec3b> >&);
void vtom(std::vector< std::vector< cv::Vec3b> >, cv::Mat&);

#endif
