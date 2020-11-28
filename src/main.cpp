#include <iostream>
#include <string>
#include "SDL_video.h"
#include "shaderhandler.hpp"
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>

#define WIDTH 1280
#define HEIGHT 720

SDL_Window *window;
SDL_Renderer *renderer;
std::string shaderFilePath = "../src/Basic.glsl";

void ShaderJoyInit()
{
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("OpenGL",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WIDTH,
		HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	SDL_GLContext glcontext = SDL_GL_CreateContext(window);
	if(glcontext == NULL)
	{
		std::cerr << "Error: can't create sdl context.\n";
		exit(-1);
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_GL_SetSwapInterval(0);
	
	GLuint glew = glewInit();
	if(glew != GLEW_OK)
	{
		std::cerr << "Error: can't init glew library.\n";
		exit(-1);
	}
}

int main(void)
{
	int running = 0;
	SDL_Event event;
	static float getTime;
	static float lastTime;
	static int frames = 0;
	
	/* Init all libs */
	ShaderJoyInit();

	int glProg = glCreateProgram();
	
	/* Work with shaders */
	ShaderHandler shaderObj(shaderFilePath, glProg);
	shaderObj.ShaderExec(GL_VERTEX_SHADER);
	shaderObj.ShaderExec(GL_FRAGMENT_SHADER);

	glLinkProgram(glProg);
	glUseProgram(glProg);

	GLint uniformTime = glGetUniformLocation(glProg, "iTime");
	if((GLuint)uniformTime == 0xFFFFFFFF || uniformTime == -1)
	{
		std::cerr << "Error: glGetUniformLocation at line: " << __LINE__ << " in file: " << __FILE__ << std::endl;
	}
	
	GLint uniformResolution = glGetUniformLocation(glProg, "iResolution");
	if((GLuint)uniformTime == 0xFFFFFFFF || uniformTime == -1)
	{
		std::cerr << "Error: glGetUniformLocation at line: " << __LINE__ << " in file: " << __FILE__ << std::endl;
	}
	
	lastTime = SDL_GetTicks();
	/* Main loop */
	while(running != 1)
	{
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_QUIT:
				running = 1;
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					running = 1;
					break;
				}
				break;
			}
		}
		getTime = SDL_GetTicks();
		glUniform1f(uniformTime, getTime * 0.0025 + 1);
		glUniform2f(uniformResolution, WIDTH, HEIGHT);
		glRectf(-getTime, -getTime, getTime, getTime);
		
		/* Fps counter */
		frames++;
		if (getTime - lastTime >= 5000.0)
		{
			std::cout << "Frames per 5 second: " << frames << " frames/sec\n";
			std::cout << "Frame time: " << 5000.0 / static_cast<float>(frames) << "ms/frame\n";
			frames = 0;
			lastTime += 5000.0;
		}
		
		glFlush();
		SDL_GL_SwapWindow(window);
	}
	glDeleteProgram(glProg);
	SDL_Quit();
	return 0;
}
