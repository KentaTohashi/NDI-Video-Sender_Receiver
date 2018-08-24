#pragma once
#include <malloc.h>
#include <time.h>
#include <iostream>
#include <chrono>
#include <string>
#include <thread>
#include <atomic>
#include <opencv/cv.hpp>    // cvtColor
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <Processing.NDI.Lib.h>

/* linux����p�w�b�_ */
#ifdef __linux__
#include <csignal>
#include <cstddef>
#include <cstring>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <sys/time.h>
#endif // __linux__

/* Windows����p�w�b�_ */
#ifdef _WIN32
#include <signal.h>
#endif


#define NDI_REC_MAX     6



class NdiCom
{
public:
	
	NdiCom(const int channel_no, const NDIlib_source_t & p_source);
	~NdiCom();
	bool GetIsRecFlg();
	void creatRecVideoThread();
	void DeleteRecVideoThread();

private:
	int m_frame_xres, m_frame_yres, m_frame_xy; // ��M�t���[���̉𑜓x�i�[�p
	const int m_channel_no; // �`�����l�����i�[�p
	bool m_exit_rec_loop; // ��M���[�v�I���t���O
	bool m_isRecv; // ��M�t���O
	bool m_isHighest; // ���𑜓x�t���O
	bool m_is_realsense; // RealSense�J�����t���O
	std::string m_str_resname; // ���\�[�X���i�[�p
	std::thread m_receive_thread; // ��MThread�p�ϐ�
	NDIlib_recv_instance_t m_pNDI_recv; // ��M�p�C���X�^���X
	NDIlib_metadata_frame_t camera_mode;//RealSense�J�����̓��샂�[�h�؂�ւ�
	cv::Mat m_rcvframe; // ��M�p�t���[��

	void recVideo();
};

