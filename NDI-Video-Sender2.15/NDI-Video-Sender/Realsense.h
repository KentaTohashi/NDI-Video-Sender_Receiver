#pragma once
#include "NdiCom.h"
#include <librealsense2/rs.hpp>

#define RGB_CAMERA_VIDEOS  0
#define DEPTH_CAMERA_VIDEOS 1
#define IR_LEFT_CAMERA_VIDEOS 2
#define IR_RIGHT_CAMERA_VIDEOS 3

class Realsense :
	public NdiCom
{
public:
	Realsense();
	Realsense(int argv_no);
	~Realsense();
	void initializeSensor();
	void sndVideo() override;
	void CretateSndVideoThread() override;

private:
	// RGB�J�����ݒ�l
	uint32_t m_color_width; // �������𑜓x�̊i�[�p
	uint32_t m_color_height; // �c�����𑜓x�̊i�[�p
	uint32_t m_color_fps; // �t���[�����[�g�̊i�[�p

	// �f�v�X�J�����ݒ�l
	uint32_t m_depth_width; // �������𑜓x�̊i�[�p
	uint32_t m_depth_height; // �c�����𑜓x�̊i�[�p
	uint32_t m_depth_fps; // �t���[�����[�g�̊i�[�p

	// IR�J�����ݒ�l
	uint32_t m_IR_width; // �������𑜓x�̊i�[�p
	uint32_t m_IR_height; // �c�����𑜓x�̊i�[�p
	uint32_t m_IR_fps; // �t���[�����[�g�̊i�[�p
	
	rs2::pipeline m_pipeline; // �p�C�v���C��
};

