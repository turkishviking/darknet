#include "darknetpp.h"
#include "arapaho.hpp"


Darknetpp::Darknetpp()
{
    arap = new ArapahoV2();
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
    if(arap)
        delete arap;
    arap = new ArapahoV2();
    bool ret = false;
    int expectedW = 0, expectedH = 0;

    ArapahoV2Params ap;

    char * INPUT_DATA_FILE   = new char[data.length() + 1];
    strcpy(INPUT_DATA_FILE, data.c_str());
    char * INPUT_CFG_FILE    = new char[cfg.length() + 1];
    strcpy(INPUT_CFG_FILE, cfg.c_str());
    char * INPUT_WEIGHTS_FILE = new char[weights.length() + 1];
    strcpy(INPUT_WEIGHTS_FILE, weights.c_str());

    if(!fileExists(INPUT_DATA_FILE) || !fileExists(INPUT_CFG_FILE) || !fileExists(INPUT_WEIGHTS_FILE))
    {
        std::cout << fileExists(INPUT_DATA_FILE) << std::endl;
        EPRINTF("Setup failed as input files do not exist or not readable!\n");
        return false;
    }
    ap.datacfg = INPUT_DATA_FILE;
    ap.cfgfile = INPUT_CFG_FILE;
    ap.weightfile = INPUT_WEIGHTS_FILE;
    ap.nms = 0.4;
    ap.maxClasses = 200;

    // Always setup before detect
    ret = arap->Setup(ap, expectedW, expectedH);

    if(false == ret)
    {
        EPRINTF("Setup failed!\n");
        if(arap) delete arap;
        arap = 0;
        return false;
    }

    boxes = 0;
    return true;
}

std::vector<yoloDetection> Darknetpp::detect(cv::Mat &inputImage, float &&threshold, float &&hierThreshold)
{
    std::vector<yoloDetection> result;

    int imageWidthPixels = 0, imageHeightPixels = 0;
    imageWidthPixels = inputImage.size().width;
    imageHeightPixels = inputImage.size().height;

    // Using expectedW/H, can optimise scaling using HW in platforms where available

    int numObjects = 0;

    bool success = arap->Detect(inputImage, threshold, hierThreshold, numObjects);
    if(!success)
        throw std::string("Detection failed");


    if(numObjects > 0) // Realistic maximum
    {
        boxes = new box[numObjects];
        labels = new std::string[numObjects];
        probs = new float[numObjects];
        if(!boxes)
        {
            if(arap) delete arap;
            arap = 0;
        }
        if(!labels)
        {
            if(arap) delete arap;
            arap = 0;
            if(boxes)
            {
                delete[] boxes;
                boxes = NULL;
            }
        }

        // Get boxes and labels
        arap->GetBoxes(boxes, labels, probs, numObjects);

        int objId = 0;
        int leftTopX = 0, leftTopY = 0, rightBotX = 0,rightBotY = 0;
        for (objId = 0; objId < numObjects; objId++)
        {

            leftTopX = 1 + imageWidthPixels*(boxes[objId].x - boxes[objId].w / 2);
            leftTopY = 1 + imageHeightPixels*(boxes[objId].y - boxes[objId].h / 2);
            rightBotX = 1 + imageWidthPixels*(boxes[objId].x + boxes[objId].w / 2);
            rightBotY = 1 + imageHeightPixels*(boxes[objId].y + boxes[objId].h / 2);
            DPRINTF("Box #%d: center {x,y}, box {w,h} = [%f, %f, %f, %f]\n",
                    objId, boxes[objId].x, boxes[objId].y, boxes[objId].w, boxes[objId].h);

            //prevent detection to go out images
            if(leftTopX < 0)
                leftTopX = 0;
            if(leftTopY < 0)
                leftTopY = 0;
            if(inputImage.cols < rightBotX)
                rightBotX = inputImage.cols;
            if(inputImage.rows < rightBotY)
                rightBotY = inputImage.rows;

            if (labels[objId].c_str())
            {
                yoloDetection yoDet;
                yoDet.label = labels[objId];
                yoDet.x = leftTopX;
                yoDet.y = leftTopY;
                yoDet.width = rightBotX - leftTopX;
                yoDet.height = rightBotY - leftTopY;
                yoDet.rx = yoDet.x / (float)inputImage.cols;
                yoDet.ry = yoDet.y / (float)inputImage.rows;
                yoDet.rw = yoDet.width / (float)inputImage.cols;
                yoDet.rh = yoDet.height / (float)inputImage.rows;
                yoDet.prob = probs[objId];
                result.emplace_back(yoDet);
            }

        }

        if (boxes)
        {
            delete[] boxes;
            boxes = NULL;
        }
        if (labels)
        {
            delete[] labels;
            labels = NULL;
        }
        if (probs)
        {
            delete[] probs;
            probs = NULL;
        }
        arap->cleanDetections();

    }
    return result;

}


Darknetpp::~Darknetpp()
{
    if(arap)
        delete arap;
}
