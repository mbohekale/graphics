#pragma once

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "gCamera.h"
#include "gShaderProgram.h"
#include "gVertexBuffer.h"
#include "Mesh_OGL3.h"

class CMyApp
{
public:
	CMyApp(void);
	~CMyApp(void);

	bool Init();
	void Clean();

	void Update();
	void Render();

	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);
	void Resize(int, int);
protected:
	GLuint GenTexture();
	void draw_octa(glm::mat4 matWorld, glm::vec4 color);

	GLuint m_textureID; 
	void CMyApp::add_triangle_to_octa(glm::vec3& P1,
		glm::vec3& P2,
		glm::vec3& P3,
		gVertexBuffer& buffer);

	

	gCamera			m_camera;
	gShaderProgram	m_program;
	gVertexBuffer	m_vb;

	gVertexBuffer	m_vb_octa; //octahedron

	Mesh			*m_mesh;
	Mesh			*m_mesh_tree;
	bool textureChanged = false;

	int index = 20;
	int fading = 0;

	int pressed_key = -1;
};

