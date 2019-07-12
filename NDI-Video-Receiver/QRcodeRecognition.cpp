//
// Created by tohashi on 19/07/12.
//

#include "QRcodeRecognition.h"

void QRcodeRecognition::processImage(cv::Mat &mat) {
    vector<decodedObject> decodedObjects;

    decode(mat, decodedObjects);
    writeDecodeResult(mat, decodedObjects);

}

void QRcodeRecognition::decode(cv::Mat &im, vector<QRcodeRecognition::decodedObject> &decodedObjects) {

    ImageScanner scanner;

    //スキャナを設定
    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

    // グーレースケールに変換
    cv::Mat imGray;
    cv::cvtColor(im, imGray, cv::COLOR_BGR2GRAY);

    // 入力画像をzBar用に変換
    Image image(im.cols, im.rows, "Y800", (uchar *) imGray.data, im.cols * im.rows);

    // QEコードをスキャン
    scanner.scan(image);


    // 結果を格納
    for (Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol) {
        decodedObject obj;

        obj.type = symbol->get_type_name();
        obj.data = symbol->get_data();


        // 場所を格納
        for (int i = 0; i < symbol->get_location_size(); i++) {
            obj.location.emplace_back(cv::Point(symbol->get_location_x(i), symbol->get_location_y(i)));
        }

        decodedObjects.push_back(obj);
    }

}

void QRcodeRecognition::writeDecodeResult(cv::Mat &im, vector<QRcodeRecognition::decodedObject> &decodedObjects) {

    // デコード済みオブジェクトすべてについて結果を書き込み
    for (decodedObject d : decodedObjects) {
        cv::Point point = d.location[3];
        string data = d.data;

        cv::putText(
                im, // 画像
                data, // 文字列
                point, // 座標
                cv::FONT_HERSHEY_SIMPLEX, // フォントの種類
                1.2, // 文字の大きさ
                cv::Scalar(255, 255, 0), // 文字の色
                10, // 線の太さ
                cv::LINE_AA
        );
        cv::putText( // インラインフォント
                im, // 画像
                data, // 文字列
                point, // 座標
                cv::FONT_HERSHEY_SIMPLEX, // フォントの種類
                1.2, // 文字の大きさ
                cv::Scalar(0, 0, 0), // 文字の色
                3, // 線の太さ
                cv::LINE_AA // アンチエイリアス
        );
    }
}

string QRcodeRecognition::getProcessingName() {
    return "QR";
}
