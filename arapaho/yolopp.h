#ifndef YOLOPP_H
#define YOLOPP_H
#include "arapaho.hpp"
#include <sys/types.h>
#include <sys/stat.h>

struct yoloDetections
{
    std::string label;
    int x;
    int y;
    int width;
    int height;
};

class Yolopp
{
public:
    Yolopp();
    ~Yolopp();
    void load(std::string data, std::string cfg, std::string weights);
    std::vector<yoloDetections> detect(cv::Mat &inputImage, float &threshold, float &hierThreshold);
private:
    ArapahoV2* arap;
    bool fileExists(const char *file);
    box* boxes;
    std::string* labels;
};

#endif // YOLOPP_H
