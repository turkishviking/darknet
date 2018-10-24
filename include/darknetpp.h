#ifndef YOLOPP_H
#define YOLOPP_H

#include <iostream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <opencv2/opencv.hpp>
#include <darknet.h>

struct yoloDetection
{
    std::string label;
    int x;
    int y;
    int width;
    int height;
    float rx;
    float ry;
    float rw;
    float rh;

};
class ArapahoV2;

class Darknetpp
{
public:
    Darknetpp();
    ~Darknetpp();
    bool load(std::string data, std::string cfg, std::string weights);
    std::vector<yoloDetection> detect(cv::Mat &inputImage, float &&threshold, float &&hierThreshold);
private:
    ArapahoV2* arap;
    bool fileExists(const char *file);
    box* boxes;
    std::string* labels;
};

#endif // YOLOPP_H
