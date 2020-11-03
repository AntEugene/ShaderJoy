#include "shaderhandler.hpp"

ShaderHandler::ShaderHandler(std::string shaderFilePath, int argGlProg) : glProg(argGlProg), shaderPath(shaderFilePath)
{
	this->shaderSources = this->ShaderLoadPharse(shaderFilePath);
}

ShaderHandler::~ShaderHandler()
{
	glDeleteShader(this->shaderGL);
	shaderPath = '\0';
	shaderSources = {};
}

/*
 * Load and pharse shader from file.
 * @param shaderPath Path to file with shader code.
 * @return ShaderProgramSource StringStream with shaders code from file.
 */
ShaderProgramSource ShaderHandler::ShaderLoadPharse(std::string shaderPath)
{
	std::string buffer;
	std::stringstream strStream[2]; // One for vertex, one for fragment.
	std::ifstream reader(shaderPath, std::ios::in);
	if (!reader)
	{
		std::cerr << "Error: can't open the shader file: " << shaderPath << std::endl;
		exit(-1);
	}
	
	enum class Shadertype
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	Shadertype type = Shadertype::NONE;
	
	while(std::getline(reader, buffer))
	{
		if(buffer.find("#shader") != std::string::npos)
		{
			if(buffer.find("vertex") != std::string::npos)
				type = Shadertype::VERTEX;
			else if(buffer.find("fragment") != std::string::npos)
				type = Shadertype::FRAGMENT;
		}
		else
		{
			strStream[(int)type] << buffer << std::endl;
		}
	}
	return {strStream[0].str(), strStream[1].str()};
}

/* Compile and load shader from src string.
 * @param src String with shader code.
 * @param type Shader type (e.g. GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, ...)
 * @param GlProg The pointer that returned glCreateProgram func.
 */
void ShaderHandler::ShaderExec(int type)
{
	GLint checkShader = 0;
	this->shaderGL = glCreateShader(type);
	const char* source;
	
	if(type == GL_VERTEX_SHADER)
		source = this->shaderSources.VertexSource.c_str();
	else if(type == GL_FRAGMENT_SHADER)
		source = this->shaderSources.FragmentSource.c_str();
	else
	{
		std::cerr << "Error: This type of shaders is not supported.\n";
		exit(-1);
	}
	
	glShaderSource(this->shaderGL, 1, &source, 0);
	glCompileShader(this->shaderGL);
	glAttachShader(this->glProg, this->shaderGL);
	
	/* Get shader compile errors */
	glGetShaderiv(this->shaderGL, GL_COMPILE_STATUS, &checkShader);
	if(checkShader == GL_FALSE)
	{
		/* Print shader compile errors */
		GLint maxLength = 0;
		glGetShaderiv(this->shaderGL, GL_INFO_LOG_LENGTH, &maxLength);

		GLchar errorLog[maxLength];
		glGetShaderInfoLog(this->shaderGL, maxLength, &maxLength, &errorLog[0]);

		std::cerr << "Error: Compile shader error:\n" << errorLog << std::endl;
		glDeleteShader(this->shaderGL);
		return;
	}
}
