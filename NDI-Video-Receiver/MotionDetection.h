//
// Created by tohashi on 19/07/15.
//

#ifndef NDI_VIDEO_RECEIVER_MOTIONDETECTION_H
#define NDI_VIDEO_RECEIVER_MOTIONDETECTION_H

#include "ImageProcessing.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/hal/interface.h>
#include <vector>

using namespace std;
using namespace cv;

class MotionDetection : public ImageProcessing {

public:
    MotionDetection();

    void processImage(cv::Mat &mat) override;

    string getProcessingName() override;

    void onStartProcessing() override;

    void onStopProcessing() override;

private:
    bool isInitialized;
    cv::Mat avg_frame;
    int threshold = 10;
};


#endif //NDI_VIDEO_RECEIVER_MOTIONDETECTION_H
