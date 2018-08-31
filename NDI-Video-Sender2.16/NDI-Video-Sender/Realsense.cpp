#include "Realsense.h"


using namespace std;

/****************************************************************************
*
*�@�R���X�g���N�^�E�f�X�g���N�^
*
*****************************************************************************/
/* �R���X�g���N�^ */
Realsense::Realsense()
{
	// �����Ȃ�
}

/* �R���X�g���N�^ */
Realsense::Realsense(int argv_no) : NdiCom::NdiCom(argv_no)
{
	// �J����������
	initializeSensor();
}

/* �f�X�g���N�^ */
Realsense::~Realsense()
{
	// �����Ȃ�
}

/* ������ */
void Realsense::initializeSensor()
{
	// RGB�J���������l
	m_color_width = m_xres; // �������𑜓x�̎w��
	m_color_height = m_yres; // �c�����𑜓x�̎w��
	m_color_fps = m_sndfps; // �t���[�����[�g�̎w��

	// Depth�J���������l
	m_depth_width = m_xres; // �������𑜓x�̎w��
	m_depth_height = m_yres; // �c�����𑜓x�̎w��
	m_depth_fps = m_sndfps; // �t���[�����[�g�̎w��

	// IR�J�����ݒ�l
	m_IR_width = m_xres; // �������𑜓x�̎w��
	m_IR_height = m_yres; // �c�����𑜓x�̎w��
	m_IR_fps = m_sndfps; // �t���[�����[�g�̎w��

	rs2::config config; // RealSense�J�����̐ݒ�p
	rs2::pipeline_profile pipeline_profile; // �p�C�v���C���̒��g�Q�Ɨp

	// �R���t�B�O�̒��g�ݒ�
	config.enable_device(NdiCom::m_other_camera); // �f�o�C�X�̃V���A���w��
	config.enable_stream(rs2_stream::RS2_STREAM_COLOR, m_xres, m_yres, rs2_format::RS2_FORMAT_BGR8, m_color_fps); // RGB�J�����L����
	config.enable_stream(rs2_stream::RS2_STREAM_DEPTH, m_xres, m_yres, rs2_format::RS2_FORMAT_Z16, m_depth_fps); // Depth�J�����L����
	config.enable_stream(rs2_stream::RS2_STREAM_INFRARED, 1, m_IR_width, m_IR_height, rs2_format::RS2_FORMAT_Y8, m_IR_fps); // IR�J�����L����
	config.enable_stream(rs2_stream::RS2_STREAM_INFRARED, 2, m_IR_width, m_IR_height, rs2_format::RS2_FORMAT_Y8, m_IR_fps); // IR�J�����L����


	// �p�C�v���C���X�^�[�g
	pipeline_profile = m_pipeline.start(config);
}

/****************************************************************************
*
*�@���M����
*
*****************************************************************************/
void Realsense::sndVideo() 
{
	// ���[�J���ϐ�
	rs2::frame color_frame; // RGB�f���p�t���[��
	rs2::frame depth_frame; // Depth�f���p�t���[��
	rs2::frame ir1_frame; // IR�f���p�t���[��
	rs2::frame ir2_frame; // IR�f���p�t���[��
	cv::Mat myframe, sndframe; // OpenCV�p�t���[��
	cv::Point point(30, 30); // �^�C���X�^���v�ʒu
	string strTimecode; // �^�C���X�^���v�p�����i�[�p�ϐ�
	rs2::frameset frameset; // �t���[���Z�b�g
	rs2::colorizer color_map; // �J���[���c�[��
	int camera_mode = RGB_CAMERA_VIDEOS; // RGB�J������
	int cv_color; // ���͂����L�[�̔ԍ����i�[���邽�߂̕ϐ�
	NDIlib_metadata_frame_t metadata_desc; // ���^��M�p�t���[��

	// ���M�p�C���X�^���X����
	sendCreate();

	// �f���p�t���[���̐����Ə�����
	NDIlib_video_frame_v2_t video_frame; // �f���t���[���̐錾
	video_frame.xres = m_xres; // �������𑜓x�̎w��
	video_frame.yres = m_yres; // �c�����𑜓x�̎w��
	video_frame.FourCC = NDIlib_FourCC_type_BGRA; // m_sndNDIColor�w���B�t���[���̃J���[�t�H�[�}�b�g�w��
	video_frame.frame_format_type = NDIlib_frame_format_type_interleaved;
	video_frame.p_data = (uint8_t*)malloc(m_xres * m_yres * 4); // �f�[�^�T�C�Y�̎w��
	video_frame.line_stride_in_bytes = m_xres * 4;

	// Send Start!
	printf("%s SENDING...\n", m_resources_id.c_str());
	m_exit_snd_loop = false;

	// ���M����
	while (!m_exit_snd_loop)
	{
		if (NDIlib_send_capture(m_pNDI_send, &metadata_desc, 0))
		{	
			string camera_change = metadata_desc.p_data; // ���^�f�[�^�̖{�̂�string�^�̕ϐ��Ɋi�[
			
			// �J�������[�h�̃`�F���W
			if (camera_change.find("<RGB_mode enabled=\"true\"/>") != -1)
			{
				camera_mode = RGB_CAMERA_VIDEOS;
				printf("RGB mode enabled\n");
			}
			else if (camera_change.find("<Depth_mode enabled=\"true\"/>") != -1)
			{
				camera_mode = DEPTH_CAMERA_VIDEOS;
				printf("Depth mode enabled\n");
			}
			else if (camera_change.find("<IR_left_mode enabled=\"true\"/>") != -1)
			{
				camera_mode = IR_LEFT_CAMERA_VIDEOS;
				printf("IR left mode enabled\n");
			}
			else if (camera_change.find("<IR_right_mode enabled=\"true\"/>") != -1)
			{
				camera_mode = IR_RIGHT_CAMERA_VIDEOS;
				printf("IR right mode enabled\n");
			}

			// �����Ńf�[�^���������K�v������܂�
			NDIlib_send_free_metadata(m_pNDI_send, &metadata_desc);
		}
		// �J��������ǂݍ��ݏ���
		frameset = m_pipeline.wait_for_frames(); // �t���[���̍X�V

		// �J�����̓��샂�[�h�ؑ�
		switch (camera_mode)
		{
		case RGB_CAMERA_VIDEOS:
			// RGB�t���[���擾
			color_frame = frameset.get_color_frame();

			// �t���[���T�C�Y�̍X�V
			m_color_width = color_frame.as<rs2::video_frame>().get_width();
			m_color_height = color_frame.as<rs2::video_frame>().get_height();

			// OpenCV�p�t���[���Ɋi�[
			myframe = cv::Mat(m_color_height, m_color_width, CV_8UC3, const_cast<void *>(color_frame.get_data()));

			// �ϊ��̐F��ύX
			cv_color = CV_BGR2BGRA;

			break;

		case DEPTH_CAMERA_VIDEOS:
			// Depth�t���[���擾(�F�t��)
			depth_frame = frameset.get_depth_frame();
			depth_frame = depth_frame.apply_filter(color_map);

			// �t���[���T�C�Y�̍X�V
			m_depth_width = depth_frame.as<rs2::video_frame>().get_width();
			m_depth_height = depth_frame.as<rs2::video_frame>().get_height();

			// OpenCV�p�t���[���Ɋi�[
			myframe = cv::Mat(m_depth_height, m_depth_width, CV_8UC3, const_cast<void*>(depth_frame.get_data()));

			// �ϊ��̐F��ύX
			cv_color = CV_BGR2BGRA;
			
			break;

		case IR_LEFT_CAMERA_VIDEOS:
			// IR�t���[���擾(�F�t��)
			ir1_frame = frameset.get_infrared_frame(1);

			// �t���[���T�C�Y�̍X�V
			m_IR_width = ir1_frame.as<rs2::video_frame>().get_width();
			m_IR_height = ir1_frame.as<rs2::video_frame>().get_height();

			// OpenCV�p�t���[���Ɋi�[
			myframe = cv::Mat(m_IR_height, m_IR_width, CV_8UC1, const_cast<void*>(ir1_frame.get_data()));

			// �ϊ��̐F��ύX
			cv_color = CV_GRAY2BGRA;
			
			break;

		case IR_RIGHT_CAMERA_VIDEOS:
			// IR�t���[���擾(�F�t��)
			ir2_frame = frameset.get_infrared_frame(2);

			// �t���[���T�C�Y�̍X�V
			m_IR_width = ir2_frame.as<rs2::video_frame>().get_width();
			m_IR_height = ir2_frame.as<rs2::video_frame>().get_height();

			// OpenCV�p�t���[���Ɋi�[
			myframe = cv::Mat(m_IR_height, m_IR_width, CV_8UC1, const_cast<void*>(ir2_frame.get_data()));

			// �ϊ��̐F��ύX
			cv_color = CV_GRAY2BGRA;
			
			break;
		}

		// �ǂݍ��񂾃t���[������
		if (myframe.empty())
		{
			continue;
		}

		// �^�C���X�^���v�}�����邩
		if (m_sndtmcode_flg)
		{
			// �^�C���X�^���v�}��
			strTimecode = getCurrentTime();

			cv::putText(
				myframe,  // �摜
				strTimecode, // ������
				point, // ���W
				cv::FONT_HERSHEY_SIMPLEX, // �t�H���g�̎��
				0.8, // �����̑傫��
				cv::Scalar(255, 255, 255), // �����̐F
				3 // ���̑���
			);

			cv::putText( // �C�����C���t�H���g
				myframe, // �摜
				strTimecode, // ������
				point, // ���W
				cv::FONT_HERSHEY_SIMPLEX, // �t�H���g�̎��
				0.8, // �����̑傫��
				cv::Scalar(0, 0, 0), // �����̐F
				1, // ���̑���
				CV_AA // �A���`�G�C���A�X
			);
		}

		
		cv::cvtColor(myframe, sndframe, cv_color); // m_sndCVColor�̎w���B�F�̕ϊ�(NDI���M�p�t���[���ɏ悹��ɂ�BGRX�ɕϊ�����K�v������)

		
		memcpy((void*)video_frame.p_data, sndframe.data, (m_xres * m_yres * 4)); // OpenCV�̃t���[����NDI�t���[���f�[�^�ɃR�s�[

		// ���M
		NDIlib_send_send_video_v2(m_pNDI_send, &video_frame);

		if (m_preview_flg)
		{
			cv::imshow(m_str_resname, myframe); // �摜��\��

			// �L�[���͂�҂�
			switch (cv::waitKey(1))
			{
			case 3: // imshow����Ctrl+c�����͂��ꂽ��I��
			case 227: // windows����Ctrl+c��3�ALinux����Ctrl+c���������F���ł��Ȃ��B227���Ԃ�
				m_exit_snd_loop = true;
				cv::destroyWindow(m_str_resname);
				break;
			default:
				break;
			}
		}
	}
	m_pipeline.stop(); // �p�C�v���C���X�g�b�v
	free(video_frame.p_data); // �f���t���[���̃f�[�^�̈�����
	NDIlib_send_destroy(m_pNDI_send); // ���M�p�t���[���̊J��
	m_isRecflg = false;
	
}

/* ���MThread�̍쐬 */
void Realsense::CretateSndVideoThread() 
{
	if (m_use_flg) 
	{
		m_isRecflg = true; // ���M�t���O�I��
		m_thread_video = thread(&Realsense::sndVideo, this); // ���M�X���b�h�J�n
	}
}

