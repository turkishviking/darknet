#include <yolopp.h>
#include <opencv2/opencv.hpp>
using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cerr << "Expecting exactly 1 argument: path to the directory containing the config files";
        return 1;
    }

    // get data dir path from the command line arguments
    // add a slash at the end if it's not present
    string path = argv[1];
    if (path[path.length()-1] != '/')
        path = path + '/';

    // files used to configure the network
    string cfg = path + "yolov3-tiny.cfg";
    string weights = path + "yolov3-tiny.weights";

    // images we want to test
    string imgfile = path + "dog.jpg";
    
    // ouptut for the command line
    cout << endl;
    cout << "Using the following files to configure the network:" << endl;
    cout << " - " << data << endl;
    cout << " - " << cfg << endl;
    cout << " - " << weights << endl;
    cout << endl;
    cout << "Using the following files to test the network:" << endl;
    cout << " - " << data << endl;
    cout << endl;

    Yolopp y;
    bool loaded = y.load(data, cfg, weights);
    if (!loaded) 
        return 2;
    
    try 
    {
        cv::Mat img = cv::imread(imgfile);
        vector<yoloDetections> v = y.detect(img, 0.45, 0.5);
        for (yoloDetections det : v)
            cout << det.label << " ";
        
        cout << endl;
    }
    catch (string error)
    {
        return 3;
    }

    return 0;
}
