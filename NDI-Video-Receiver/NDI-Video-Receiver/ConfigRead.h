#pragma once
#include <string>
#include <fstream>
#include <sstream>
/* linux����p�w�b�_ */
#ifdef __linux__

#endif // __linux__
/* Windows����p�w�b�_ */
#ifdef _WIN32

#endif // _WIN32

#define CONFIG_FILE_NAME "NdiVideoReceiver.config"

class ConfigRead
{
public:
	ConfigRead();
	~ConfigRead();
	std::string GetStringProperty(std::string op_name);

private:
	std::string m_defvalue; //�f�t�H���g�l�i�[�p
	int m_int_defvalue; //�ݒ荀�ڔԍ��i�[�p
	std::string getDefaultValue();
	void setDefaltValue(std::string op_name);
	std::string getProperty(std::string conf_data, std::string op_name);
	std::string readConfigFile(std::string op_name);
};

