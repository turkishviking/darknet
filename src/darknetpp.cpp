#include "yolo_v2_class.hpp"
#include "darknetpp.h"




Darknetpp::Darknetpp()
{
}

bool Darknetpp::fileExists(const char *file)
{
    struct stat st;
    if(!file) return false;
    int result = stat(file, &st);
    return (0 == result);
}

bool Darknetpp::load(std::string data, std::string cfg, std::string weights)
{
    detector = std::make_shared<Detector>(cfg, weights);
    return true;
}

std::vector<yoloDetection> Darknetpp::detect(cv::Mat &inputImage, float threshold, float hierThreshold)
{
    std::vector<yoloDetection> result;



    std::vector<bbox_t> boxes = detector->detect(inputImage, threshold);

    for(bbox_t &box : boxes)
    {
        int objId = 0;
        int leftTopX = 0, leftTopY = 0, rightBotX = 0,rightBotY = 0;
        /*leftTopX = 1 + box.x - box.w / 2;
        leftTopY = 1 + imageHeightPixels*(boxes[objId].y - boxes[objId].h / 2);
        rightBotX = 1 + imageWidthPixels*(boxes[objId].x + boxes[objId].w / 2);
        rightBotY = 1 + imageHeightPixels*(boxes[objId].y + boxes[objId].h / 2);*/

        yoloDetection yoDet;
        yoDet.label = std::to_string(box.obj_id);
        yoDet.x = box.x;
        yoDet.y = box.y;
        yoDet.width = box.w;
        yoDet.height = box.h;
        yoDet.rx = yoDet.x / (float)inputImage.cols;
        yoDet.ry = yoDet.y / (float)inputImage.rows;
        yoDet.rw = yoDet.width / (float)inputImage.cols;
        yoDet.rh = yoDet.height / (float)inputImage.rows;
        yoDet.prob = box.prob;
        result.emplace_back(yoDet);

    }

    return result;

}


Darknetpp::~Darknetpp()
{
}
