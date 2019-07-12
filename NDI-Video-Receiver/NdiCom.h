#pragma once

#include <time.h>
#include <iostream>
#include <chrono>
#include <string>
#include <thread>
#include <atomic>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <Processing.NDI.Lib.h>
#include <cstdlib>
#include <vector>
#include "ImageProcessing.h"
#include "QRcodeRecognition.h"

using namespace std;
typedef enum _img_proc {
    NONE = 0,
    QR
} imageProcessingType;


class NdiCom {
public:

    NdiCom(const int channel_no, const NDIlib_source_t &p_source);

    ~NdiCom();

    bool GetIsRecFlg();

    void creatRecVideoThread();

    void DeleteRecVideoThread();

private:
    int m_frame_xres, m_frame_yres, m_frame_xy; // 受信フレームの解像度格納用
    const int m_channel_no; // チャンネル数格納用
    bool m_exit_rec_loop; // 受信ループ終了フラグ
    bool m_isRecv; // 受信フラグ
    bool m_isHighest; // 高解像度フラグ
    std::string m_str_resname; // リソース名格納用
    std::thread m_receive_thread; // 受信Thread用変数
    NDIlib_recv_instance_t m_pNDI_recv; // 受信用インスタンス
    NDIlib_metadata_frame_t camera_mode;//RealSenseカメラの動作モード切り替え
    cv::Mat m_rcvframe; // 受信用フレーム
    imageProcessingType img_proc;

    vector<ImageProcessing *> *imageProcessing;

    void recVideo();

};

