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

#include "Utils.h"


struct _set
{
	int res;
} set;


void loadDefaultSettings(void)
{
	set.res = 512;
}

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

	glUseProgram(shader);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	/* Draw one black quad so that we get one fragment covering the
	 * whole screen. */
	glColor3f(0, 0, 0);
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

int main(int argc, char **argv)
{
	loadDefaultSettings();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(set.res, set.res);
	glutCreateWindow("GPU-Tracer");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	loadShaders();

	glutMainLoop();
	exit(EXIT_SUCCESS);
}
