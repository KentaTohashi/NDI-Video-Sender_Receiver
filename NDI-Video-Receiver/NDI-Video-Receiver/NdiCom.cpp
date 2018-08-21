
#include "NdiCom.h"


using namespace std;


/****************************************************************************
*																			
*�@�R���X�g���N�^�E�f�X�g���N�^												
*																			
*****************************************************************************/
/* �R���X�g���N�^ */
NdiCom::NdiCom(const int channel_no, const NDIlib_source_t& p_source)
	: m_pNDI_recv(NULL), m_channel_no(channel_no)
{
	//������
	m_exit_rec_loop = true;
	m_isHighest = true;
	m_isRecv = true;

	// Display the frames per second
	printf("Channel %d is connecting to %s.\n", m_channel_no, p_source.p_ndi_name);
	m_str_resname = "CH" + to_string(m_channel_no) + " " + p_source.p_ndi_name;//�`�����l���ԍ��{���\�[�X���̊i�[

	NDIlib_recv_create_t recv_create_desc = { p_source, NDIlib_recv_color_format_e_BGRX_BGRA, NDIlib_recv_bandwidth_highest, true }; // FULL
//	NDIlib_recv_create_t m_recv_create_desc = { p_source, NDIlib_recv_color_format_e_BGRX_BGRA, NDIlib_recv_bandwidth_lowest, true };  // ��𑜓x����

	//�𑜓x�ݒ�
	if (recv_create_desc.bandwidth == NDIlib_recv_bandwidth_lowest)
	{
		m_isHighest = false;//���𑜓x�t���O�I�t
	}

	// Create the receiver
	m_pNDI_recv = NDIlib_recv_create_v2(&recv_create_desc);

	//�C���X�^���X�̐����`�F�b�N
	if (!m_pNDI_recv)
	{
		printf("NDIlib_recv_instance_t create failure.\n");
		return;
	}

	// Start a thread to receive frames
	creatRecVideoThread();//��M�pThread�̍쐬
}

/* �f�X�g���N�^ */
NdiCom::~NdiCom()
{
}


/****************************************************************************
*																			
*�@getter���\�b�h															
*																			
*****************************************************************************/
/* ��M�t���O */
bool NdiCom::GetIsRecFlg()
{
	return (m_isRecv);
}

/****************************************************************************
*																			
*�@setter���\�b�h															
*																			
*****************************************************************************/


/****************************************************************************
*																			
*�@��M����																	
*																			
*****************************************************************************/
/* ��M�����{�� */
void NdiCom::recVideo()
{
	//���[�J���ϐ��錾
	string strFps;//FPS�i�[�p
	char crrent_time_c[20]; //�����i�[�p�ϐ�
	int frame_no = 0;//�t���[���ԍ��i�[�p
	bool isInitialized = false; //�����ς݃t���O
	auto prev_time = std::chrono::high_resolution_clock::now();//���ݎ����擾
	NDIlib_video_frame_v2_t video_frame;// �f���p�t���[���̐���
	cv::Point point(30, 30);//�^�C���X�^���v�̍��W�w��
	m_exit_rec_loop = false;//��M�I���t���O

	while (!m_exit_rec_loop)
	{
		//��M�J�n
		if (NDIlib_recv_capture_v2(m_pNDI_recv, &video_frame, NULL, NULL, 1000) == NDIlib_frame_type_video)
		{
			// Every 1000 frames we check how long it has taken (1000�t���[�����ƂɎB�e���Ԃ��m�F���Ă��܂�)
			if (frame_no == 200)
			{
				// Get the time (���Ԃ𓾂�)
				const auto this_time = chrono::high_resolution_clock::now();

				strFps = m_str_resname + " " + "FPS:" + to_string((float)frame_no / chrono::duration_cast<chrono::duration<float>>(this_time - prev_time).count());

				// ���ݎ����擾
				const auto current_time = chrono::system_clock::now();//���Ԏ擾
				time_t current_time_t = std::chrono::system_clock::to_time_t(current_time);//time_t�^�ϊ�
				struct tm *now_tm = localtime(&current_time_t);//���{���ԂɕύX
				strftime(crrent_time_c, 128, "%Y/%m/%d %H:%M:%S", now_tm);

				// Display the frames per second and current time.    (���b�̃t���[���ƌ��ݎ�����\������)
				printf("[%s] %s\n", crrent_time_c, strFps.c_str());

				// Cycle the timers and reset the count (�^�C�}�[���񂵂ăJ�E���g�����Z�b�g����)
				prev_time = this_time;
				frame_no = 0;
			}
			else ++frame_no;//�t���[�������Z

			//�f���t���[���̏c���̉𑜓x���擾
			m_frame_xres = video_frame.xres;
			m_frame_yres = video_frame.yres;
			m_frame_xy = m_frame_xres * m_frame_yres;

			//1��ڂ̂ݏ������s��
			if (!isInitialized)
			{
				m_rcvframe = cv::Mat::zeros(cv::Size(m_frame_xres, m_frame_yres), CV_8UC4);//������
				isInitialized = true;//����������
			}

			memcpy(m_rcvframe.data, video_frame.p_data, m_frame_xy * 4);//NDI�t���[���f�[�^��OpenCV�̃t���[���ɃR�s�[

			if (!m_isHighest)
			{
				//��ʂ̃��T�C�Y
				resize(m_rcvframe, m_rcvframe, cv::Size(), 1, 2);// ��𑜓x����
			}
			
			imshow(m_str_resname, m_rcvframe);//�t���[���̕\��

			//�L�[���͂�҂�
			switch (cv::waitKey(1))
			{
			case 3://imshow����Ctrl+c�����͂��ꂽ��I��
			case 227://windows����Ctrl+c��3�ALinux����Ctrl+c���������F���ł��Ȃ��B227���Ԃ�
				m_exit_rec_loop = true;
				cv::destroyWindow(m_str_resname);
				break;
			default:
				break;
			}
			
			NDIlib_recv_free_video_v2(m_pNDI_recv, &video_frame);// �f����M�p�I�u�W�F�N�g�̔j��
		}
	}
	m_isRecv = false;

}

/* ��M�pThread�쐬 */
void NdiCom::creatRecVideoThread()
{
	m_receive_thread = thread(&NdiCom::recVideo, this);//��MThread�J�n
}

/* ��M�pThread��� */
void NdiCom::DeleteRecVideoThread()
{
	m_exit_rec_loop = true;//��M���[�v�I���t���O�I��
	for (;;)//�������[�v
	{
		// Sleep
		chrono::milliseconds dura(100);
		this_thread::sleep_for(dura);

		//��M�����I���܂őҋ@
		if (!GetIsRecFlg())
		{
			//Thread��join�\������
			if (m_receive_thread.joinable())
			{
				m_receive_thread.join();//�X���b�h��join����܂őҋ@
				break;//for���[�v������
			}
		}
	}
}
