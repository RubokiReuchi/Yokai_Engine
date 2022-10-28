#include "Re_Shader.h"

static std::string OpenShader(const std::string& path)
{
	std::ifstream stream(path);

	std::string line;
	std::stringstream ss;

	while (getline(stream, line))
	{
		ss << line << "\n";
	}

	return ss.str();
}

static unsigned int CompileShader(const std::string& source, unsigned int type)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> message(length);
		glGetShaderInfoLog(id, length, &length, &message[0]);

		glDeleteShader(id);
		return 0;
	}

	return id;
}

Re_Shader::Re_Shader()
{
}

Re_Shader::Re_Shader(const char* vertexPath, const char* fragmentPath)
{
	CreateShader(vertexPath, fragmentPath);
}

void Re_Shader::CreateShader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexSource = OpenShader(vertexPath);
	std::string fragmentSource = OpenShader(fragmentPath);

	unsigned int vertexShaderID = CompileShader(vertexSource, GL_VERTEX_SHADER);
	unsigned int fragmentShaderID = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);

	program_id = glCreateProgram();
	glAttachShader(program_id, vertexShaderID);
	glAttachShader(program_id, fragmentShaderID);
	glLinkProgram(program_id);
	glValidateProgram(program_id);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}