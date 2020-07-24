#pragma one
#include "VideoSource.h"
#include <librealsense2/rs.hpp>
#include <opencv2/imgproc.hpp>


/**
 * RealSense用映像ソースクラス
 */
class RealSense :
        public VideoSource {
public:
    RealSense(int camera_number);

    ~RealSense();

    void setCameraMode(cameraMode _camera_mode);

    cv::Mat getFrame() override;

    void attach(void) override;

    void detach(void) override;

private:
    rs2::pipeline m_pipeline;
    rs2::config m_config;
    rs2::colorizer color_map;
    int camera_mode;
};

