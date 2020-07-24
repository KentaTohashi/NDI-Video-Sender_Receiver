//
// Created by tohashi on 19/07/15.
//

#include "MotionDetection.h"
#include <iostream>

using namespace cv;

MotionDetection::MotionDetection() {
    isInitialized = false;
}

void MotionDetection::processImage(cv::Mat &mat) {
    cv::Mat gray_image;
    cv::Mat blur_image;
    cv::Mat delta_image;
    cv::Mat abs_image;
    cv::Mat thresh_image;
    vector<cv::Point> nz_points;
    //ガウシアンフィルタのカーネルサイズ
    //大きくするとよりぼかしが強くなる
    cv::Size kernel_size(9, 9);
    int sigma = 5;
    int min_x, max_x;
    int min_y, max_y;


    //画像をグレースケールに変換
    cv::cvtColor(mat, gray_image, cv::COLOR_BGR2GRAY);
    //ガウシアンフィルタをかける
    //ぼかすことで軽微なノイズを無視する
    cv::GaussianBlur(gray_image, blur_image, kernel_size, sigma);
    //背景画像を準備する
    if (!isInitialized) {
        //初回は背景画像を現在の画像で初期化
        blur_image.copyTo(avg_frame);
        //accumulateWeighted()の引数は浮動小数点型を必要とするので変換
        avg_frame.convertTo(avg_frame, CV_32F);
        isInitialized = true;
    } else {
        //現在の画像と背景画像の平均を取り、新たな背景画像とする
        cv::accumulateWeighted(blur_image, avg_frame, 0.5);
    }
    //背景画像の絶対値を計算
    cv::convertScaleAbs(avg_frame, abs_image);
    //背景画像と現在の画像の差分を計算
    cv::absdiff(blur_image, abs_image, delta_image);
    //差分を0/255で2値化
    cv::threshold(delta_image, thresh_image, threshold, 255, cv::THRESH_BINARY);

    //0でない画素を集める
    //nz_pointsに変化があった画素の座標が入る
    cv::findNonZero(thresh_image, nz_points);
    if (!nz_points.empty()) {
        int non_zero_point_number = nz_points.size();
        max_x = 0;
        min_x = mat.cols - 1;
        max_y = 0;
        min_y = mat.rows - 1;
        //変化があった画素の数が一定数あったら表示
        if (non_zero_point_number > 100) {
            for (const cv::Point p:nz_points) {
                //変化があった領域を囲む矩形を計算
                if (max_x < p.x) {
                    max_x = p.x;
                } else if (min_x > p.x) {
                    min_x = p.x;
                }
                if (max_y < p.y) {
                    max_y = p.y;
                } else if (min_y > p.y) {
                    min_y = p.y;
                }
            }
            //矩形を元画像に描画
            cv::rectangle(mat, cv::Point(min_x, min_y), cv::Point(max_x, max_y), cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
        }
    }


}

string MotionDetection::getProcessingName() {
    return "Motion Detection";
}

void MotionDetection::onStartProcessing() {
    isInitialized = false;
    ImageProcessing::onStartProcessing();
}

void MotionDetection::onStopProcessing() {
    ImageProcessing::onStopProcessing();
}

