#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string_view>
#include <cstdint>

#include <gl/glew.h>

class Shader
{
public:
	// construct shader by providing path to vertex
	// and fragment shader source file
	Shader(std::string_view vertexSourceDir,
		   std::string_view fragmentSourceDir);

	// use created shader program
	auto use() -> void;

	// get shader program ID
	auto getProgramId() const -> std::uint32_t;

	~Shader();

private:
	std::uint32_t m_programId{};
};

inline Shader::Shader(std::string_view vertexSourceDir,
					  std::string_view fragmentSourceDir)
{
	std::string vertexShaderSource{};
	std::string pixelShaderSource{};

	std::ifstream vertexShaderFile{};
	std::ifstream pixelShaderFile{};

	std::stringstream vertexShaderStream{};
	std::stringstream pixelShaderStream{};

	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	pixelShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vertexShaderFile.open(vertexSourceDir.data());
		pixelShaderFile.open(fragmentSourceDir.data());

		vertexShaderStream << vertexShaderFile.rdbuf();
		pixelShaderStream << pixelShaderFile.rdbuf();

		vertexShaderFile.close();
		pixelShaderFile.close();

		vertexShaderSource = vertexShaderStream.str();
		pixelShaderSource = pixelShaderStream.str();
	}
	catch (std::ifstream::failure fileExcp)
	{
		std::cerr << "Failed to read from files...\n";
	}
	catch (...)
	{
		std::cerr << "Some other exceptions...\n";
	}

	const char *vertexShaderCstring{vertexShaderSource.c_str()};
	const char *pixelShaderCstring{pixelShaderSource.c_str()};

	std::uint32_t vertexShaderID{};
	std::int32_t success{};

	constexpr auto bufferSize{512};
	char infoLog[bufferSize];

	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vertexShaderCstring, nullptr);
	glCompileShader(vertexShaderID);
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShaderID, bufferSize, nullptr, infoLog);
		std::cerr << "Error on vertex shader compilation: " << infoLog << '\n';
	}

	std::uint32_t pixelShaderID{};

	pixelShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(pixelShaderID, 1, &pixelShaderCstring, nullptr);
	glCompileShader(pixelShaderID);
	glGetShaderiv(pixelShaderID, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(pixelShaderID, bufferSize, nullptr, infoLog);
		std::cerr << "Error on fragment shader compilation: " << infoLog << '\n';
	}

	this->m_programId = glCreateProgram();
	glAttachShader(this->m_programId, vertexShaderID);
	glAttachShader(this->m_programId, pixelShaderID);
	glLinkProgram(this->m_programId);

	glGetShaderiv(this->m_programId, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(this->m_programId, bufferSize, nullptr, infoLog);
		std::cerr << "Error on program linking: " << infoLog << '\n';
	}

	// cleanup
	glDeleteShader(vertexShaderID);
	glDeleteShader(pixelShaderID);
}

inline void Shader::use()
{
	glUseProgram(this->m_programId);
}

inline std::uint32_t Shader::getProgramId() const
{
	return this->m_programId;
}

inline Shader::~Shader()
{
	glDeleteProgram(this->m_programId);
}

#endif // END SAHDER_H
