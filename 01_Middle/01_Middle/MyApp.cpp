#include "MyApp.h"
#include "GLUtils.hpp"

#include <GL/GLU.h>
#include <math.h>

#include "ObjParser_OGL3.h"

CMyApp::CMyApp(void)
{
	m_textureID = 0;
	m_mesh = 0;
}


CMyApp::~CMyApp(void)
{
}


GLuint CMyApp::GenTexture()
{
	unsigned char tex[256][256][3];

	for (int i = 0; i<256; ++i)
	for (int j = 0; j<256; ++j)
	{
		tex[i][j][0] = rand() % 256;
		tex[i][j][1] = rand() % 256;
		tex[i][j][2] = rand() % 256;
	}

	GLuint tmpID;

	glGenTextures(1, &tmpID);
	glBindTexture(GL_TEXTURE_2D, tmpID);
	gluBuild2DMipmaps(GL_TEXTURE_2D,
		GL_RGB8, 256, 256,			// define storage on the GPU: RGB, 8bits per channel, 256x256 texture
		GL_RGB, GL_UNSIGNED_BYTE,	// define storate in RAM: RGB layout, unsigned bytes for each channel
		tex);						//						  and source pointer
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// bilinear filter on min and mag
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return tmpID;
}


void CMyApp::add_triangle_to_octa(glm::vec3 &P1,
	glm::vec3 &P2,
	glm::vec3 &P3,
	gVertexBuffer &buffer) {

	buffer.AddData(0, P1.x, P1.y, P1.z); //P1
	buffer.AddData(0, P2.x, P2.y, P2.z); // P2
	buffer.AddData(0, P3.x, P3.y, P3.z); // P3

	glm::vec3 V1 = P1 - P2;//P1-P2
	glm::vec3 V2 = P3 - P2;//P3-P2
	glm::vec3 normal = glm::normalize(glm::cross(V1, V2));

	//normals
	buffer.AddData(1, normal.x, normal.y, normal.z);
	buffer.AddData(1, normal.x, normal.y, normal.z);
	buffer.AddData(1, normal.x, normal.y, normal.z);

	//texture coordinates
	buffer.AddData(2, 0, 0);
	buffer.AddData(2, 0.5, 1);
	buffer.AddData(2, 1, 0);
}

bool CMyApp::Init()
{
	// background color
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	// set cullface and z-buffer ON, cull backfaces
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	//
	// create the geometry
	//

	m_vb.AddAttribute(0, 3); //positions
	m_vb.AddAttribute(1, 3); //normals 
	m_vb.AddAttribute(2, 2); // tex coords

	//positions
	m_vb.AddData(0, -15,  0, -15);
	m_vb.AddData(0,  15,  0, -15);
	m_vb.AddData(0, -15,  0,  15);
	m_vb.AddData(0,  15,  0,  15);

	// normals
	m_vb.AddData(1, 0, 1, 0);
	m_vb.AddData(1, 0, 1, 0);
	m_vb.AddData(1, 0, 1, 0);
	m_vb.AddData(1, 0, 1, 0);

	// tex coords
	m_vb.AddData(2, 0, 0);
	m_vb.AddData(2, 1, 0);
	m_vb.AddData(2, 0, 1);
	m_vb.AddData(2, 1, 1);

	m_vb.AddIndex(1, 0, 2);
	m_vb.AddIndex(1, 2, 3);
	
	m_vb.InitBuffers();

	m_vb_octa.AddAttribute(0, 3); //positions
	m_vb_octa.AddAttribute(1, 3); //normals 
	m_vb_octa.AddAttribute(2, 2); // tex coords
	/*
	add_triangle_to_octa(
		glm::vec3(1, 0, 0), 
		glm::vec3(0, 0, 1),
		glm::vec3(0, 1, 0), 
		m_vb_octa);

	add_triangle_to_octa(
		glm::vec3(0, 1, 0),
		glm::vec3(0, 0, 1),
		glm::vec3(-1, 0, 0), 
		m_vb_octa); 

	add_triangle_to_octa(
		glm::vec3(0, 1, 0),
		glm::vec3(-1, 0, 0),
		glm::vec3(0, 0, -1),
		m_vb_octa);

	add_triangle_to_octa(
		glm::vec3(0, 1, 0),
		glm::vec3(0, 0, -1),
		glm::vec3(1, 0, 0),
		m_vb_octa);*/

	add_triangle_to_octa(
		glm::vec3(1, 0, 0),
		glm::vec3(0, -1, 0),
		glm::vec3(0, 0, 1),
		m_vb_octa);

	add_triangle_to_octa(
		glm::vec3(0, 0, 1),
		glm::vec3(0, -1, 0),
		glm::vec3(-1, 0, 0),
		
		m_vb_octa);


	add_triangle_to_octa(
		glm::vec3(0, 0, -1),
		glm::vec3(0, -1, 0),
		glm::vec3(-1, 0, 0),
		m_vb_octa);


	add_triangle_to_octa(
		glm::vec3(0, 0, -1),
		glm::vec3(1, 0, 0),
		glm::vec3(0, -1, 0),
		m_vb_octa);

	//indices
	m_vb_octa.AddIndex(1, 0, 2);
	m_vb_octa.AddIndex(4, 3, 5);
	m_vb_octa.AddIndex(6, 8, 7);
	m_vb_octa.AddIndex(9, 11, 10);
	m_vb_octa.AddIndex(12, 14, 13);
	m_vb_octa.AddIndex(15, 16, 17);
	m_vb_octa.AddIndex(18, 19, 20);
	m_vb_octa.AddIndex(21, 22, 23);

	m_vb_octa.InitBuffers(); // IMPORTANT!!!! moves vertices to GPU

	//
	// load shaders	
	//
	m_program.AttachShader(GL_VERTEX_SHADER, "dirLight.vert");
	m_program.AttachShader(GL_FRAGMENT_SHADER, "dirLight.frag");

	m_program.BindAttribLoc(0, "vs_in_pos");
	m_program.BindAttribLoc(1, "vs_in_normal");
	m_program.BindAttribLoc(2, "vs_in_tex0");

	if ( !m_program.LinkProgram() )
	{
		return false;
	}

	//
	// misc init
	//

	m_camera.SetProj(45.0f, 640.0f/480.0f, 0.01f, 1000.0f);

	m_textureID = TextureFromFile("leaves.png");

	m_mesh = ObjParser::parse("Suzanne.obj");
	m_mesh->initBuffers();

	m_mesh_tree = ObjParser::parse("tree.obj");
	m_mesh_tree->initBuffers();

	return true;
}

void CMyApp::Clean()
{
	glDeleteTextures(1, &m_textureID);

	m_program.Clean();
}

void CMyApp::Update()
{
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time)/1000.0f;

	m_camera.Update(delta_time);

	last_time = SDL_GetTicks();
}


void CMyApp::draw_octa(glm::mat4 matWorld, glm::vec4 color) {

	m_program.On();

	glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
	
	glm::mat4 mvp = m_camera.GetViewProj() *matWorld;

	m_program.SetUniform("world", matWorld);
	m_program.SetUniform("worldIT", matWorldIT);
	m_program.SetUniform("MVP", mvp);
	m_program.SetUniform("eye_pos", m_camera.GetEye());
	m_program.SetUniform("Kd", color);
	m_program.SetUniform("Ka", color);
	m_program.SetUniform("Ks", color);

	m_program.SetTexture("texImage", 0, m_textureID);

	// draw with VAO
	m_vb_octa.On();

	m_vb_octa.DrawIndexed(GL_TRIANGLES, 0, 12, 0); //IB

	m_vb_octa.Off();

	m_program.Off();

}
void CMyApp::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw object 1 (ground)

	m_program.SetUniform("Kd", glm::vec4(0.0f, 0.25f, 0.125f, 1));
	m_program.On();

		glm::mat4 matWorld = glm::mat4(1.0f);
		glm::mat4 matWorldIT = glm::transpose( glm::inverse( matWorld ) );
		glm::mat4 mvp = m_camera.GetViewProj() *matWorld;

		m_program.SetUniform( "world", matWorld );
		m_program.SetUniform( "worldIT", matWorldIT );
		m_program.SetUniform( "MVP", mvp );
		m_program.SetUniform( "eye_pos", m_camera.GetEye() );

		m_program.SetTexture("texImage", 0, m_textureID);

		// draw with VAO
		m_vb.On();

			m_vb.DrawIndexed(GL_TRIANGLES, 0, 6, 0); //IB

		m_vb.Off();

	m_program.Off();

	// draw octa

	for (int i = 0; i < 8; i++) {
		float t = SDL_GetTicks() / 5000.0f;

		float angle = t * 2 * M_PI;
		angle += 2 * M_PI / 8 * i;

		float x = cos(angle) * 5;
		float y = 20 * (1-sin(t));
		float z = sin(angle) * 5;

		float scale_param = 2.0 / 8 * i + 0.1;

		glm::mat4 octa_world_transformation =
			glm::translate<float>(glm::vec3(x, y, z)) *
			glm::scale<float>(glm::vec3(scale_param, scale_param + y / 3, scale_param));
		

		glm::vec4 color = glm::vec4(1.f/8 * i, 0, 0, 1.0f);
		draw_octa(octa_world_transformation, color);

		//if (pressed_key == i) { //draw suzanne

		//	m_program.On();
		//	matWorld = glm::translate<float>(glm::vec3(0, 3, 0)) * 
		//		glm::translate<float>(glm::vec3(x, y, z));
		//	matWorldIT = glm::transpose(glm::inverse(matWorld));
		//	mvp = m_camera.GetViewProj() *matWorld;

		//	m_program.SetUniform("world", matWorld);
		//	m_program.SetUniform("worldIT", matWorldIT);
		//	m_program.SetUniform("MVP", mvp);
		//	m_program.SetUniform("eye_pos", m_camera.GetEye());

		//	m_program.SetTexture("texImage", 0, m_textureID);

		//	m_mesh->draw();

		//	m_program.Off();
		//}
	}


	// draw object 1 (suzanne)
	/*
	m_program.On();

		matWorld = glm::translate<float>(glm::vec3(0, 1, 0));
		matWorldIT = glm::transpose( glm::inverse( matWorld ) );
		mvp = m_camera.GetViewProj() *matWorld;

		m_program.SetUniform( "world", matWorld );
		m_program.SetUniform( "worldIT", matWorldIT );
		m_program.SetUniform( "MVP", mvp );
		m_program.SetUniform( "eye_pos", m_camera.GetEye() );

		m_program.SetTexture("texImage", 0, m_textureID);

		m_mesh->draw();

	m_program.Off();
	*/
	

	//// draw object 2 (tree)
	m_program.On();

	matWorld = glm::translate<float>(glm::vec3(0, 1, 0)) * 
		glm::scale<float>(glm::vec3(1, 1, 1)) *
		glm::rotate<float>(-M_PI , glm::vec3(1, 0, 0));

	matWorldIT = glm::transpose(glm::inverse(matWorld));
	mvp = m_camera.GetViewProj() *matWorld;

	m_program.SetUniform("world", matWorld);
	m_program.SetUniform("worldIT", matWorldIT);
	m_program.SetUniform("MVP", mvp);
	m_program.SetUniform("eye_pos", m_camera.GetEye());

	m_program.SetTexture("texImage", 0, m_textureID);

	m_mesh_tree->draw();

	m_program.Off();
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	if (key.keysym.sym == SDLK_SPACE) {
		textureChanged = !textureChanged;
	}
	else if (key.keysym.sym == SDLK_1)
	{
		if (fading == 1) fading = -1;
		else fading = 1;
	}
	m_camera.KeyboardDown(key);

	switch (key.keysym.sym) {
	case SDLK_1:
		pressed_key = 0;
		break;
	case SDLK_2:
		pressed_key = 1;
		break;
	case SDLK_3:
		pressed_key = 2;
		break;
	case SDLK_4:
		pressed_key = 3;
		break;
	case SDLK_5:
		pressed_key = 4;
		break;
	case SDLK_6:
		pressed_key = 5;
		break;
	case SDLK_7:
		pressed_key = 6;
		break;
	case SDLK_8:
		pressed_key = 7;
		break;
	}
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardUp(key);
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	m_camera.MouseMove(mouse);
}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	m_camera.Resize(_w, _h);
}