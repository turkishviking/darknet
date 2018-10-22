#include <yolopp.h>
#include <opencv2/opencv.hpp>
int main()
{
    Yolopp y;

    // std::string path = "/home/charlie/Documents/Two-i/mono-runtime/yoloCFG/V3-416/";
    // std::string cfg = path + "input.cfg";
    // std::string data = path + "input.data";
    // std::string w = path + "input.weights";
    // y.load(data, cfg, w);
    // cv::Mat img = cv::imread("/home/charlie/Documents/Two-i/darknet_pj/arapaho/darknetcpplogo.png");

    // std::string path = "/home/pj/tmp/darknet-wrksx/";
    // std::string cfg = path + "cfg/yolov3.cfg";
    // std::string data = path + "cfg/coco.data";
    // std::string w = path + "data/yolov3.weights";
    // paths relative to the current directory
    std::string data = "data/coco.data";
    std::string cfg = "data/yolov3.cfg";
    std::string w = "data/yolov3.weights";
    y.load(data, cfg, w);

    // the images we want to test
    std::string test_images[4] = {
        "data/dog.jpg",
        "data/eagle.jpg",
        "data/giraffe.jpg",
        "data/horses.jpg",
    };

        // Print Strings
    for (int i = 0; i < 4; i++) {
        cv::Mat img = cv::imread(test_images[i]);
        std::vector<yoloDetections> v = y.detect(img, 0.45, 0.5);
        for(yoloDetections det : v) {
            std::cout << det.label << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
