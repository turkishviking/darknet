#include <yolo_v2_class.hpp>
#include <darknetpp.h>




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

bool Darknetpp::load(std::string names, std::string cfg, std::string weights)
{
    std::fstream file;
    file.open(names, std::ofstream::in);
    if(file.is_open())
    {
        for(std::string line; std::getline(file, line) ; )
        {
            labels.push_back(line);
        }
    }
    std::cout << "found " << labels.size() << " categories" << std::endl;
    detector = std::make_shared<Detector>(cfg, weights);
    return true;
}

std::vector<yoloDetection> Darknetpp::detect(cv::Mat &inputImage, float threshold, float hierThreshold)
{
    std::vector<yoloDetection> result;



    std::vector<bbox_t> boxes = detector->detect(inputImage, threshold);

    for(bbox_t &box : boxes)
    {
        if(box.obj_id < labels.size())
        {
            int objId = 0;
            int leftTopX = 0, leftTopY = 0, rightBotX = 0,rightBotY = 0;
            yoloDetection yoDet;
            yoDet.label = labels[box.obj_id];
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
        else
        {
            throw std::invalid_argument("not enough categories (" + std::to_string(labels.size()) + ") to handle object category: " +std::to_string(box.obj_id) );
        }

    }

    return result;

}


Darknetpp::~Darknetpp()
{
}
