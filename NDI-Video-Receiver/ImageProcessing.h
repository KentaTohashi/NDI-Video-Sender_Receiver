//
// Created by tohashi on 19/07/12.
//

#ifndef NDI_VIDEO_RECEIVER_IMAGEPROCESSING_H
#define NDI_VIDEO_RECEIVER_IMAGEPROCESSING_H

#include <opencv2/core.hpp>


using namespace std;

class ImageProcessing {
public:
    ImageProcessing() = default;

    virtual void processImage(cv::Mat &mat);

    virtual string getProcessingName();

};


#endif //NDI_VIDEO_RECEIVER_IMAGEPROCESSING_H
