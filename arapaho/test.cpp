#include <yolopp.h>
#include <opencv2/opencv.hpp>
int main()
{
    Yolopp y;
    std::string path = "/home/charlie/Documents/Two-i/mono-runtime/yoloCFG/V3-416/";
    std::string cfg = path + "input.cfg";
    std::string data = path + "input.data";
    std::string w = path + "input.weights";
    y.load(data, cfg, w);
    cv::Mat img = cv::imread("/home/charlie/Documents/Two-i/darknet_pj/arapaho/darknetcpplogo.png");
    std::vector<yoloDetections> v = y.detect(img, 0.45, 0.5);
    for(yoloDetections det : v)
        std::cout << det.label << std::endl;
    return 0;
}
