/*
	Copyright 2010 Peter Hofmann

	This file is part of GPUTracer.

	GPUTracer is free software: you can redistribute it and/or modify it
	under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	GPUTracer is distributed in the hope that it will be useful, but
	WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with GPUTracer. If not, see <http://www.gnu.org/licenses/>.
*/


#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "Utils.hpp"
#include "Viewport.hpp"

Viewport win;
static const double rotationDegree = 2;
static const double movingStep = 0.02;
static GLint handle_rot;
static GLint handle_pos;

static bool mouseLook = false;
static bool mouseInverted = true;
static double mouseSpeed = 0.1;
static bool mouseDown = false;

void loadShaders(void)
{
	const char *vs_source = readFile("shader_vertex.glsl");
	const char *fs_source = readFile("shader_fragment.glsl");
	GLuint shader = 0;
	GLuint vs_handle = 0;
	GLuint fs_handle = 0;

	if (vs_source == NULL || fs_source == NULL)
	{
		fprintf(stderr, "Could not load shaders.\n");
		exit(EXIT_FAILURE);
	}

	shader = glCreateProgram();

	vs_handle = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs_handle, 1, &vs_source, NULL);
	glCompileShader(vs_handle);
	glAttachShader(shader, vs_handle);

	fs_handle = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs_handle, 1, &fs_source, NULL);
	glCompileShader(fs_handle);
	glAttachShader(shader, fs_handle);

	glLinkProgram(shader);

	handle_rot = glGetUniformLocation(shader, "rot");
	handle_pos = glGetUniformLocation(shader, "pos");

	glUseProgram(shader);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Copy the orientation matrix to a float array. That's needed so we
	// can pass it to the shaders.
	float oriMatrix[16];
	for (int i = 0; i < 16; i++)
		oriMatrix[i] = win.orientationMatrixPtr()[i];

	// Same for position of the camera.
	float fpos[3];
	fpos[0] = win.pos().x();
	fpos[1] = win.pos().y();
	fpos[2] = win.pos().z();

	glUniformMatrix4fv(handle_rot, 1, true, oriMatrix);
	glUniform3fv(handle_pos, 1, fpos);

	// Draw one quad so that we get one fragment covering the whole
	// screen.
	glBegin(GL_QUADS);
	glVertex3f(-1, -1,  0);
	glVertex3f( 1, -1,  0);
	glVertex3f( 1,  1,  0);
	glVertex3f(-1,  1,  0);
	glEnd();

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glClearColor(0, 0, 0, 1);
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
	double mStep = movingStep;

	// Move faster if a mouse button is pressed.
	if (mouseDown)
		mStep *= 10;

	switch (key)
	{
		case 'q':
			//std::cout << "LEFT ROLL" << std::endl;
			win.rotateAroundAxis(2, -rotationDegree);
			break;
		case 'e':
			//std::cout << "RIGHT ROLL" << std::endl;
			win.rotateAroundAxis(2, rotationDegree);
			break;

		case 'w':
			//std::cout << "Moving forward." << std::endl;
			win.moveAlongAxis(2, mStep);
			break;
		case 's':
			//std::cout << "Moving backward." << std::endl;
			win.moveAlongAxis(2, -mStep);
			break;

		case 'a':
			//std::cout << "Moving left." << std::endl;
			win.moveAlongAxis(0, -mStep);
			break;
		case 'd':
			//std::cout << "Moving right." << std::endl;
			win.moveAlongAxis(0, mStep);
			break;

		case 'r':
			//std::cout << "Moving up." << std::endl;
			win.moveAlongAxis(1, mStep);
			break;
		case 'f':
			//std::cout << "Moving down." << std::endl;
			win.moveAlongAxis(1, -mStep);
			break;

		case 'R':
			std::cout << "Reset." << std::endl;
			win.reset();
			break;

		case ' ':
			win.dumpInfos();
			break;

		case 'm':
			mouseLook = !mouseLook;
			if (mouseLook)
			{
				glutSetCursor(GLUT_CURSOR_NONE);

				// Init the algorithm: Warp to center.
				glutWarpPointer(win.w() * 0.5, win.h() * 0.5);

				std::cout << "Mouse look activated." << std::endl;
			}
			else
			{
				glutSetCursor(GLUT_CURSOR_INHERIT);
				std::cout << "Mouse look deactivated." << std::endl;
			}
			break;

		case 'i':
			mouseInverted = !mouseInverted;
			if (mouseInverted)
				std::cout << "Mouse inverted." << std::endl;
			else
				std::cout << "Mouse not inverted." << std::endl;
			break;

		case 27:
			exit(EXIT_SUCCESS);
			break;
	}

	glutPostRedisplay();
}

void motion(int x, int y)
{
	if (!mouseLook)
		return;

	// Calc the mouse delta and do the desired moving.
	// It's always calculated relative to the window's
	// center because the mouse is warped back there.
	int dx = x - win.w() * 0.5;
	int dy = y - win.h() * 0.5;

	// Don't proceed if there's no change. Prevents the
	// infinite loop that would happen due to mouse
	// warping.
	if (dx == 0 && dy == 0)
		return;

	// Okay, rotate.
	// mouseSpeed is a factor that's commonly known as
	// "mouse sensitivity".
	win.rotateAroundAxis(0, (mouseInverted ? 1 : -1) * mouseSpeed * dy);
	win.rotateAroundAxis(1, -mouseSpeed * dx);

	// Now warp the pointer back to the center.
	glutWarpPointer(win.w() * 0.5, win.h() * 0.5);

	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		mouseDown = true;
	}
	else
	{
		mouseDown = false;
	}
}

int main(int argc, char **argv)
{
	win.setSize(512, 512);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(win.w(), win.h());
	glutCreateWindow("GPU-Tracer");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(motion);

	loadShaders();

	// We don't start at (0, 0, 0). Most objects are centered at that
	// position so we push the cam a little bit.
	win.setInitialPosition(Vec3(0, 0, -5));
	win.reset();

	glutMainLoop();
	exit(EXIT_SUCCESS);
}
