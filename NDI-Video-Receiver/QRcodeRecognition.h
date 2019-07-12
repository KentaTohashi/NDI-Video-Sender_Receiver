//
// Created by tohashi on 19/07/12.
//

#ifndef NDI_VIDEO_RECEIVER_QRCODERECOGNITION_H
#define NDI_VIDEO_RECEIVER_QRCODERECOGNITION_H

#include "ImageProcessing.h"
#include <vector>
#include <zbar.h>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace zbar;

class QRcodeRecognition : public ImageProcessing {
public:
    QRcodeRecognition() = default;

    void processImage(cv::Mat &mat) override;

    string getProcessingName() override;

private:
    typedef struct {
        string type;
        string data;
        vector<cv::Point> location;
    } decodedObject;

    void decode(cv::Mat &im, vector<decodedObject> &decodedObjects);

    void writeDecodeResult(cv::Mat &im, vector<decodedObject> &decodedObjects);
};


#endif //NDI_VIDEO_RECEIVER_QRCODERECOGNITION_H
