#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>

#define W 1280
#define H 720

SDL_Window *window;
SDL_Renderer *renderer;
const std::string shader_vtx = {
"varying vec4 p;"
"void main(){gl_Position=p=gl_Vertex;p.z=length(p.xy);}"
};
const std::string shader_frg = {
"varying vec4 p;"
"uniform float iTime;"
"void main(){"
"vec3 color = 0.5 + 0.5*cos(iTime + p.yxy + vec3(1,2,4));"
"gl_FragColor = vec4(color, 1.0);"
"}"
};

void Shader(const std::string &src, int type, int GlProg)
{
	GLint checkShader = 0;
	GLuint shader = glCreateShader(type);
	const char* source = src.c_str();
	glShaderSource(shader, 1, &source, 0);
	glCompileShader(shader);
	glAttachShader(GlProg, shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &checkShader);
	if(checkShader == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		GLchar errorLog[maxLength];
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

		printf("Shader error:\n%s", errorLog);
		glDeleteShader(shader);
		return;
	}
}

int main(void)
{
	int running = 0;
	SDL_Event event;
	static float getTime;
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	SDL_GLContext glcontext = SDL_GL_CreateContext(window);
	if(glcontext == NULL)
	{
		std::cerr << "Error: can't create sdl context\n";
		return -1;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_GL_SetSwapInterval(1);
	
	GLuint glew = glewInit();
	if(glew != GLEW_OK)
	{
		std::cerr << "Error: can't init glew library\n";
		return -1;
	}

	int glProg = glCreateProgram();

	Shader(shader_vtx, GL_VERTEX_SHADER, glProg);
	Shader(shader_frg, GL_FRAGMENT_SHADER, glProg);

	glLinkProgram(glProg);
	glUseProgram(glProg);

	GLint uniformTime = glGetUniformLocation(glProg, "iTime");
	if((GLuint)uniformTime == 0xFFFFFFFF || uniformTime == -1)
	{
		printf("Error in glGetUniformLocation");
		return -1;
	}

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
		//glRectf(-1, -1, 1, 1);
		glRectf(-getTime, -getTime, getTime, getTime);
		glFlush();
		SDL_GL_SwapWindow(window);
	}
	SDL_Quit();
	return 0;
}
