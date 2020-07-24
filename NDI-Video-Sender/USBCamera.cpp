#include "USBCamera.h"


/**
 * USBカメラデバイスを開く
 * @param camera_number カメラ番号
 */
USBCamera::USBCamera(int camera_number):VideoSource(camera_number)
{
    auto *config_read = new ConfigRead(); // コンフィグファイル読み込み処理用インスタンス
    string str_int; // 起動引数で指定した値からカメラ番号を設定するための変数
    str_int = "_CAM";
    str_int += to_string(camera_number); // 指定したカメラ番号を格納

    USBCam_path = config_read->GetStringProperty("Camera_ID" + str_int);

    //デバイスを開く
    capture = new cv::VideoCapture();

	delete config_read;
}



USBCamera::~USBCamera()
{
    detach();
    delete capture;
}

/**
 * フレームを得る
 * @return カメラ画像のフレーム
 */
cv::Mat USBCamera::getFrame() {
    if(!open_failed) {
        cv::Mat frame;
        cv::Mat bgra_frame;
        int fail_count = 0;
        //読み込み失敗回数を数え、連続で失敗した場合にはカメラに接続できないとみなす
        while (fail_count <= 5 && frame.empty()) {
            if (!capture->read(frame)) {
                fail_count++;
            } else {
                break;
            }
        }
        //連続で失敗したことを検知した場合、カメラを無効化する。
        if (fail_count >= 5) {
            open_failed = true;
            cerr << getFailedToGetFrameMessage() << endl;
            capture->release();
            return getErrorFrame();
        }
        cv::cvtColor(frame, bgra_frame, cv::COLOR_BGR2BGRA);
        return bgra_frame;
    }else{
        return getErrorFrame();
    }
}

void USBCamera::setCameraMode(cameraMode _camera_mode)
{
    return;
}

/**
 * USBカメラデバイスのキャプチャを開始する
 * @param なし
 */
void USBCamera::attach(void) {
    capture->open(USBCam_path, cv::CAP_V4L);

    if (!capture->isOpened()) {
        cerr << getFailedToOpenMessage() << endl;
        open_failed = true;
        capture->release();
    } else {
        open_failed = false;

        capture->set(cv::CAP_PROP_FRAME_WIDTH, m_xres);  // 幅
        capture->set(cv::CAP_PROP_FRAME_HEIGHT, m_yres); // 高さ
        capture->set(cv::CAP_PROP_FPS, m_sndfps); // フレームレート

        // フレームサイズ更新（カメラの性能以上要求によるエラー落ち対策）
        m_xres = (int)capture->get(cv::CAP_PROP_FRAME_WIDTH); // 横方向
        m_yres = (int)capture->get(cv::CAP_PROP_FRAME_HEIGHT); // 縦方向
    }
}

/**
 * USBカメラデバイスのキャプチャを終了する
 * @param なし
 */
void USBCamera::detach(void) {
    capture->release();
}
