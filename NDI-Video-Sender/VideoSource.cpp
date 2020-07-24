#include "VideoSource.h"



VideoSource::VideoSource(int camera_number)
{
    auto *config_read = new ConfigRead(); // コンフィグファイル読み込み処理用インスタンス
    string str_int; // 起動引数で指定した値からカメラ番号を設定するための変数
    str_int = "_CAM";
    str_int += to_string(camera_number); // 指定したカメラ番号を格納

    //x方向解像度取得
    m_xres = config_read->GetIntProperty("xres" + str_int);
    //y方向解像度取得
    m_yres = config_read->GetIntProperty("yres" + str_int);
    //fps取得
    m_sndfps = config_read->GetIntProperty("FPS" + str_int);

    delete config_read; // コンフィグファイル読み込みインスタンスの削除
}


VideoSource::~VideoSource() {
}

/***
 * エラーが発生した際のダミーフレームを返す
 * @return 青一色のフレーム
 */
cv::Mat VideoSource::getErrorFrame() const {
    return cv::Mat(cv::Size(m_xres, m_yres), CV_8UC4, cv::Scalar(255, 0, 0, 255));
}

/***
 * カメラを開けなかった場合のエラーメッセージを得る
 * @return エラーメッセージ
 */
string VideoSource::getFailedToOpenMessage() const {
    return "Failed to open No." + to_string(camera_number) + " camera";
}

/***
 * フレームを得られなかった場合のエラーメッセージを得る
 * @return エラーメッセージ
 */
string VideoSource::getFailedToGetFrameMessage() const {

    return "Failed to get frame from No." + to_string(camera_number) + " camera";
}
