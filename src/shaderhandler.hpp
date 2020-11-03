#ifndef SHADERHANDLER_H
#define SHADERHANDLER_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <GL/glew.h>
#include <GL/gl.h>

// A struct that combines shader sources into a single type.
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

/**
 * Class to handle shaders
 */
class ShaderHandler
{
public:
	ShaderHandler(std::string shaderFilePath, int argGlProg);
	~ShaderHandler();
	void ShaderExec(int type);

private:
	int glProg = 0;
	GLuint shaderGL = 0;
	std::string shaderPath = "\0";
	ShaderProgramSource shaderSources = {};
	ShaderProgramSource ShaderLoadPharse(std::string shaderPath);
};

#endif // SHADERHANDLER_H
