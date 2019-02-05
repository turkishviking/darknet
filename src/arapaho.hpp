/*************************************************************************
 * arapaho                                                               *
 *                                                                       *
 * C++ API for Yolo v2 (Detection)                                       *
 *                                                                       *
 * https://github.com/prabindh/darknet                                   *
 *                                                                       *
 * Forked from, https://github.com/pjreddie/darknet                      *
 *                                                                       *
 *************************************************************************/

#ifndef _ENABLE_ARAPAHO
#define _ENABLE_ARAPAHO

#ifdef LIB_EXPORTS
#if defined(_MSC_VER)
#define LIB_API __declspec(dllexport)
#else
#define LIB_API __attribute__((visibility("default")))
#endif
#else
#if defined(_MSC_VER)
#define LIB_API
#else
#define LIB_API
#endif
#endif

#ifdef __cplusplus

#include "network.h"

extern "C" {
#include "detection_layer.h"
#include "region_layer.h"
#include "cost_layer.h"
#include "utils.h"
#include "parser.h"
#include "box.h"
#include "image.h"
#include "demo.h"
#include "option_list.h"
#include "stb_image.h"

}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <string>
#include <opencv2/opencv.hpp>


//////////////////////////////////////////////////////////////////////////

#define ARAPAHO_MAX_CLASSES (200)

#ifdef _DEBUG
#define DPRINTF printf
#define EPRINTF printf
#else
#define DPRINTF
#define EPRINTF printf
#endif

    //////////////////////////////////////////////////////////////////////////

    struct ArapahoV2Params
    {
        char* datacfg;
        char* cfgfile;
        char* weightfile;
        float nms;
        int maxClasses;
    };

    struct ArapahoV2ImageBuff
    {
        unsigned char* bgr;
        int w;
        int h;
        int channels;
    };
    //////////////////////////////////////////////////////////////////////////

    class ArapahoV2
    {
    public:
        LIB_API ArapahoV2();
        LIB_API ~ArapahoV2();

       LIB_API  bool Setup(ArapahoV2Params & p,
            int & expectedWidth,
            int & expectedHeight);

        LIB_API bool Detect(
            ArapahoV2ImageBuff & imageBuff,
            float thresh,
            float hier_thresh,
            int & objectCount);

        LIB_API bool Detect(
            const cv::Mat & inputMat,
            float thresh,
            float hier_thresh,
            int & objectCount);

        LIB_API bool GetBoxes(box* outBoxes, std::string* outLabels, float *outProbs, int boxCount);
        LIB_API void cleanDetections();
    private:
        detection *dets;
        int     nboxes;
        char    **classNames;
        float   **probs;
        bool    bSetup;
        network net;
        layer   l;
        float   nms;
        int     maxClasses;
        float     threshold;

        void __Detect(float* inData, float thresh, float hier_thresh, int & objectCount);
    };

    //////////////////////////////////////////////////////////////////////////


#endif // _ENABLE_ARAPAHO
#endif // cpulsplus
