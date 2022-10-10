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
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "Failed to compile Shader!" << std::endl;
		std::cout << message << std::endl;

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

void Re_Shader::Bind()
{
	glUseProgram(program_id);
}

void Re_Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(program_id, name.c_str()), (int)value);
}

void Re_Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
}

void Re_Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(program_id, name.c_str()), value);
}

void Re_Shader::SetFloat3(const std::string& name, float v1, float v2, float v3) const
{
	glUniform3f(glGetUniformLocation(program_id, name.c_str()), v1, v2, v3);
}

void Re_Shader::SetFloat4(const std::string& name, float v1, float v2, float v3, float v4) const
{
	glUniform4f(glGetUniformLocation(program_id, name.c_str()), v1, v2, v3, v4);
}

void Re_Shader::SetMatFloat4v(const std::string& name, const float* value)
{
	glUniformMatrix4fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, value);
}