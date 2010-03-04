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


#include <GL/glut.h>
#include <stdlib.h>


struct _set
{
	int res;
} set;


void loadDefaultSettings(void)
{
	set.res = 512;
}

int main(int argc, char **argv)
{
	loadDefaultSettings();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(set.res, set.res);
	glutCreateWindow("GPU-Tracer");

	glutMainLoop();
	exit(EXIT_SUCCESS);
}
