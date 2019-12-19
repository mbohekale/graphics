#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <GL/glew.h>

#include <SDL_image.h>

/*

Based on http://www.opengl-tutorial.org/

*/
GLuint loadShader(GLenum _shaderType, const char* _fileName)
{
	// create shader resource ID
	GLuint loadedShader = glCreateShader(_shaderType);

	if (loadedShader == 0)
	{
		fprintf(stderr, "[loadShader] glCreateShader failed");
		return 0;
	}

	// load the shader code (text file)
	std::string shaderCode = "";

	std::ifstream shaderStream(_fileName);

	if (!shaderStream.is_open())
	{
		fprintf(stderr, "[shaderStream] Cannot open shader file: %s", _fileName);
		return 0;
	}

	std::string line = "";
	while (std::getline(shaderStream, line))
	{
		shaderCode += line + "\n";
	}

	shaderStream.close();

	// attach the code to the shader resource
	const char* sourcePointer = shaderCode.c_str();
	glShaderSource(loadedShader, 1, &sourcePointer, NULL);

	// compile the shader
	glCompileShader(loadedShader);

	// check compilation status
	GLint result = GL_FALSE;
	int infoLogLength;

	glGetShaderiv(loadedShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(loadedShader, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (GL_FALSE == result)
	{
		// compilation failed
		std::vector<char> VertexShaderErrorMessage(infoLogLength);
		glGetShaderInfoLog(loadedShader, infoLogLength, NULL, &VertexShaderErrorMessage[0]);

		fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);
	}

	return loadedShader;
}

GLuint loadProgramVSGSFS(const char* _fileNameVS, const char* _fileNameGS, const char* _fileNameFS)
{
	GLuint vs_ID = loadShader(GL_VERTEX_SHADER, _fileNameVS);
	GLuint gs_ID = loadShader(GL_GEOMETRY_SHADER, _fileNameGS);
	GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER, _fileNameFS);

	if (vs_ID == 0 || gs_ID == 0 || fs_ID == 0)
	{
		return 0;
	}

	GLuint program_ID = glCreateProgram();

	fprintf(stdout, "Linking program\n");
	glAttachShader(program_ID, vs_ID);
	glAttachShader(program_ID, gs_ID);
	glAttachShader(program_ID, fs_ID);

	glLinkProgram(program_ID);

	GLint infoLogLength = 0, result = 0;

	glGetProgramiv(program_ID, GL_LINK_STATUS, &result);
	glGetProgramiv(program_ID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (GL_FALSE == result)
	{
		std::vector<char> ProgramErrorMessage(infoLogLength);
		glGetProgramInfoLog(program_ID, infoLogLength, NULL, &ProgramErrorMessage[0]);
		fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(vs_ID);
	glDeleteShader(gs_ID);
	glDeleteShader(fs_ID);

	return program_ID;
}

int invert_image(int pitch, int height, void* image_pixels)
{
	int index;
	void* temp_row;
	int height_div_2;

	temp_row = (void*)malloc(pitch);
	if (NULL == temp_row)
	{
		SDL_SetError("Not enough memory for image inversion");
		return -1;
	}
	//if height is odd, don't need to swap middle row
	height_div_2 = (int)(height * .5);
	for (index = 0; index < height_div_2; index++) {
		//uses string.h
		memcpy((Uint8*)temp_row,
			(Uint8*)(image_pixels)+
			pitch * index,
			pitch);

		memcpy(
			(Uint8*)(image_pixels)+
			pitch * index,
			(Uint8*)(image_pixels)+
			pitch * (height - index - 1),
			pitch);
		memcpy(
			(Uint8*)(image_pixels)+
			pitch * (height - index - 1),
			temp_row,
			pitch);
	}
	free(temp_row);
	return 0;
}

// 
int SDL_InvertSurface(SDL_Surface* image)
{
	if (NULL == image)
	{
		SDL_SetError("Surface is NULL");
		return -1;
	}

	return invert_image(image->pitch, image->h, image->pixels);
}

GLuint TextureFromFile(const char* filename)
{

	// Load image file
	SDL_Surface* loaded_img = IMG_Load(filename);
	if (loaded_img == nullptr)
	{
		std::cout << "[TextureFromFile] Error while loading image: " << filename << std::endl;
		return 0;
	}

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
	Uint32 format = SDL_PIXELFORMAT_ABGR8888;
#else
	Uint32 format = SDL_PIXELFORMAT_RGBA8888;
#endif

	SDL_Surface* formattedSurf = SDL_ConvertSurfaceFormat(loaded_img, format, 0);
	if (formattedSurf == nullptr)
	{
		std::cout << "[TextureFromFile] Error while processing image: " << SDL_GetError() << std::endl;
		SDL_FreeSurface(loaded_img);
		return 0;
	}

	if (SDL_InvertSurface(formattedSurf) == -1) {
		std::cout << "[TextureFromFile] Error while processing image: " << SDL_GetError() << std::endl;
		SDL_FreeSurface(formattedSurf);
		SDL_FreeSurface(loaded_img);
		return 0;
	}

	GLuint tex;
	glGenTextures(1, &tex);

	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, formattedSurf->w, formattedSurf->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, formattedSurf->pixels);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	SDL_FreeSurface(formattedSurf);
	SDL_FreeSurface(loaded_img);

	return tex;
}