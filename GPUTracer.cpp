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
static GLuint shader;
static GLint handle_rot;
static GLint handle_pos;
static GLint handle_eyedist;
static GLint handle_stepsize;
static GLint handle_accuracy;
static GLint handle_user_params0;
static GLint handle_user_params1;

static bool mouseLook = false;
static bool mouseInverted = true;
static double mouseSpeed = 0.1;
static bool mouseDown = false;
static bool drawCS = true;

static bool raymarching_hq = false;

static float raymarching_stepsize_hi = 0.01;
static float raymarching_stepsize_lo = 0.2;
static float raymarching_stepsize = raymarching_stepsize_lo;

static float raymarching_accuracy_hi = 1e-4;
static float raymarching_accuracy_lo = 1e-2;
static float raymarching_accuracy = raymarching_accuracy_lo;

// Light0 specifies the headlight. Its "position" is added to the
// current position of the eye.
// Light1 is a static light somewhere in the scene.
static float lights[][4] =
	{
		{  0.0, 0.5, 0.0, 0.0 },
		{ 10.0, 0.0, 0.0, 0.0 }
	};
static float lights_diffuse[][4] =
	{
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 0.3, 0.3, 1.0, 1.0 }
	};
static float lights_specular[][4] =
	{
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 0.3, 0.3, 1.0, 1.0 }
	};
static bool lights_enabled[] = { true, true };
static float lights_step = 0.1;

static float user_params[][4] =
	{
		{ 0.0, 0.0, 0.0, 0.0 },
		{ 5.0, 5.0, 5.0, 5.0 }
	};
static float user_params_steps[] = { 0.1, 1.0 };

// 0 = change user settings with F1-F10, 1 = change light settings.
static int settings_target = 0;

void showLog(GLuint shader, const char *which)
{
	std::cout << which << std::endl;
	int len = 0;
	glGetObjectParameterivARB(shader, GL_OBJECT_INFO_LOG_LENGTH_ARB,
			&len);
	// More than the NULL terminator?
	if (len > 1)
	{
		char *log = (char *)malloc(len * sizeof(char));
		int dummy = 0;
		glGetInfoLogARB(shader, len, &dummy, log);
		std::cout << log << std::endl;
		free(log);
	}
	else
	{
		std::cout << "Okay, error log empty." << std::endl;
	}
	std::cout << std::endl;
}

void loadShaders(void)
{
	const char *vs_source = readFile("shader_vertex.glsl");
	const char *fs_source = readFile("shader_fragment_final.glsl");
	shader = 0;
	GLuint shader_handle = 0;

	if (vs_source == NULL || fs_source == NULL)
	{
		fprintf(stderr, "Could not load shaders.\n");
		exit(EXIT_FAILURE);
	}

	shader = glCreateProgram();

	shader_handle = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shader_handle, 1, &vs_source, NULL);
	glCompileShader(shader_handle);
	showLog(shader_handle, "Vertex shader:");
	glAttachShader(shader, shader_handle);

	shader_handle = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader_handle, 1, &fs_source, NULL);
	glCompileShader(shader_handle);
	showLog(shader_handle, "Fragment shader:");
	glAttachShader(shader, shader_handle);

	glLinkProgram(shader);

	handle_rot = glGetUniformLocation(shader, "rot");
	handle_pos = glGetUniformLocation(shader, "pos");
	handle_eyedist = glGetUniformLocation(shader, "eyedist");
	handle_stepsize = glGetUniformLocation(shader, "stepsize");
	handle_accuracy = glGetUniformLocation(shader, "accuracy");
	handle_user_params0 = glGetUniformLocation(shader, "user_params0");
	handle_user_params1 = glGetUniformLocation(shader, "user_params1");
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shader);

	// Enable light sources.
	glEnable(GL_LIGHTING);
	if (lights_enabled[0])
	{
		// Abuse GL_SPOT_CUTOFF as a switch to toggle the light.
		glEnable(GL_LIGHT0);
		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 1.0f);
		glLightfv(GL_LIGHT0, GL_POSITION, lights[0]);
		glLightfv(GL_LIGHT0, GL_DIFFUSE,  lights_diffuse[0]);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lights_specular[0]);
	}
	else
		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 0.0f);

	if (lights_enabled[1])
	{
		glEnable(GL_LIGHT1);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 1.0f);
		glLightfv(GL_LIGHT1, GL_POSITION, lights[1]);
		glLightfv(GL_LIGHT1, GL_DIFFUSE,  lights_diffuse[1]);
		glLightfv(GL_LIGHT1, GL_SPECULAR, lights_specular[1]);
	}
	else
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 0.0f);

	// Copy the orientation matrix to a float array. That's needed so we
	// can pass it to the shaders.
	float oriMatrix[16];
	Mat4 T = win.orientationMatrix();
	for (int i = 0; i < 16; i++)
		oriMatrix[i] = T[i];

	// Same for position of the camera.
	float fpos[3];
	fpos[0] = win.pos().x();
	fpos[1] = win.pos().y();
	fpos[2] = win.pos().z();

	glUniformMatrix4fv(handle_rot, 1, true, oriMatrix);
	glUniform3fv(handle_pos, 1, fpos);
	glUniform1f(handle_eyedist, win.eyedist());
	glUniform1f(handle_stepsize, raymarching_stepsize);
	glUniform1f(handle_accuracy, raymarching_accuracy);
	glUniform4fv(handle_user_params0, 1, user_params[0]);
	glUniform4fv(handle_user_params1, 1, user_params[1]);

	// Draw one quad so that we get one fragment covering the whole
	// screen.
	double r = win.ratio();
	glBegin(GL_QUADS);
	glVertex3f(-r, -1,  0);
	glVertex3f( r, -1,  0);
	glVertex3f( r,  1,  0);
	glVertex3f(-r,  1,  0);
	glEnd();

	// Draw coordinate system?
	if (drawCS)
	{
		glUseProgram(0);
		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glPushMatrix();

		glLineWidth(3.0);

		// In y direction, move to -0.75.
		// In x direction, move to  0.75. From that point on, add the
		// difference of width and height in world coordinates. This
		// will keep the (drawn) coordinate system at a position with a
		// fixed margin to the window borders.
		glTranslated(0.75 + (win.w() - win.h()) / (double)win.h(),
				-0.75, 0);
		glScaled(0.2, 0.2, 0.2);
		glMultMatrixf(oriMatrix);

		glBegin(GL_LINES);

		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0, 0, 0);
		glVertex3f(1, 0, 0);

		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 1, 0);

		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 1);

		glEnd();

		glPopMatrix();
		glDisable(GL_DEPTH_TEST);
	}

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glClearColor(0, 0, 0, 1);
	glViewport(0, 0, w, h);

	win.setSize(w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-win.ratio(), win.ratio(), -1, 1, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void tellUserParams()
{
	std::cout << "User parameters:" << std::endl;
	std::cout << "----------------" << std::endl;
	std::cout << "user_params0 = vec4("
		<< user_params[0][0] << ", "
		<< user_params[0][1] << ", "
		<< user_params[0][2] << ", "
		<< user_params[0][3] << ");"
		<< std::endl;
	std::cout << "user_params1 = vec4("
		<< user_params[1][0] << ", "
		<< user_params[1][1] << ", "
		<< user_params[1][2] << ", "
		<< user_params[1][3] << ");"
		<< std::endl;
	std::cout << "user_params0_step = "
		<< user_params_steps[0] << ";" << std::endl;
	std::cout << "user_params1_step = "
		<< user_params_steps[1] << ";" << std::endl;
	std::cout << std::endl;
}

void tellLights()
{
	Mat4 T = win.orientationMatrix();

	if (lights_enabled[0])
	{
		std::cout << "# Headlight:" << std::endl;
		std::cout << "spherelight" << std::endl;
		std::cout << "\torigin "
			<< ( T[0]*lights[0][0]
					+ T[1]*lights[0][1]
					+ T[2]*lights[0][2]
					+ T[3] + win.pos().x()) << " "
			<< (T[4]*lights[0][0]
					+ T[5]*lights[0][1]
					+ T[6]*lights[0][2]
					+ T[7] + win.pos().y()) << " "
			<< (T[8]*lights[0][0]
					+ T[9]*lights[0][1]
					+ T[10]*lights[0][2]
					+ T[11] + win.pos().z()) << " "
			<< std::endl;
		std::cout << "\tcolor "
			<< lights_diffuse[0][0] << " "
			<< lights_diffuse[0][1] << " "
			<< lights_diffuse[0][2] << std::endl;
		std::cout << "\tintensity 0.1" << std::endl;
		std::cout << "end" << std::endl;
		std::cout << std::endl;
	}

	if (lights_enabled[1])
	{
		std::cout << "# Static light:" << std::endl;
		std::cout << "spherelight" << std::endl;
		std::cout << "\torigin "
			<< lights[1][0] << " "
			<< lights[1][1] << " "
			<< lights[1][2] << std::endl;
		std::cout << "\tcolor "
			<< lights_diffuse[1][0] << " "
			<< lights_diffuse[1][1] << " "
			<< lights_diffuse[1][2] << std::endl;
		std::cout << "\tintensity 1.0" << std::endl;
		std::cout << "end" << std::endl;
		std::cout << std::endl;
	}

	std::cout << "Lights step size: " << lights_step << std::endl;
	std::cout << std::endl;
}

void keyboard(unsigned char key, int x, int y)
{
	bool changed = true;

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
			win.moveAlongAxis(2, -1, mouseDown);
			break;
		case 's':
			//std::cout << "Moving backward." << std::endl;
			win.moveAlongAxis(2, 1, mouseDown);
			break;

		case 'a':
			//std::cout << "Moving left." << std::endl;
			win.moveAlongAxis(0, -1, mouseDown);
			break;
		case 'd':
			//std::cout << "Moving right." << std::endl;
			win.moveAlongAxis(0, 1, mouseDown);
			break;

		case 'r':
			//std::cout << "Moving up." << std::endl;
			win.moveAlongAxis(1, 1, mouseDown);
			break;
		case 'f':
			//std::cout << "Moving down." << std::endl;
			win.moveAlongAxis(1, -1, mouseDown);
			break;

		case 'R':
			win.reset();
			break;

		case ' ':
			std::cout << std::endl;
			win.dumpInfos();
			tellLights();
			changed = false;
			break;

		case 't':
			raymarching_stepsize = raymarching_stepsize_lo;
			raymarching_hq = false;
			break;

		case 'T':
			raymarching_stepsize = raymarching_stepsize_hi;
			raymarching_hq = false;
			break;

		case 'g':
			raymarching_accuracy = raymarching_accuracy_lo;
			raymarching_hq = false;
			break;

		case 'G':
			raymarching_accuracy = raymarching_accuracy_hi;
			raymarching_hq = false;
			break;

		case 'h':
			raymarching_hq = !raymarching_hq;
			if (raymarching_hq)
			{
				raymarching_stepsize = raymarching_stepsize_hi;
				raymarching_accuracy = raymarching_accuracy_hi;
			}
			else
			{
				raymarching_stepsize = raymarching_stepsize_lo;
				raymarching_accuracy = raymarching_accuracy_lo;
			}
			break;

		case '1':
			lights_enabled[0] = !lights_enabled[0];
			break;

		case '2':
			lights_enabled[1] = !lights_enabled[1];
			break;

		case 'c':
			drawCS = !drawCS;
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
			changed = false;
			break;

		case 'i':
			mouseInverted = !mouseInverted;
			if (mouseInverted)
				std::cout << "Mouse inverted." << std::endl;
			else
				std::cout << "Mouse not inverted." << std::endl;
			changed = false;
			break;

		case 27:
			exit(EXIT_SUCCESS);
			break;

		case 13:
			if (settings_target == 0)
				settings_target = 1;
			else
				settings_target = 0;
			std::cout << "Settings target: "
				<< (settings_target == 0 ? "user_params" : "lights")
				<< std::endl;
			changed = false;
			break;
	}

	if (changed)
		glutPostRedisplay();
}

void keyboardSpecial(int key, int x, int y)
{
	bool isShift = ((glutGetModifiers() & GLUT_ACTIVE_SHIFT) != 0);
	bool changed = true;
	int target = -1;
	int target_index = -1;
	int step_target = -1;

	switch (key)
	{
		case GLUT_KEY_DOWN:
			win.setFOV(win.fov() * 1.05);
			break;

		case GLUT_KEY_UP:
			win.setFOV(win.fov() / 1.05);
			break;

		case GLUT_KEY_F1:
			target = 0;
			target_index = 0;
			break;

		case GLUT_KEY_F2:
			target = 0;
			target_index = 1;
			break;

		case GLUT_KEY_F3:
			target = 0;
			target_index = 2;
			break;

		case GLUT_KEY_F4:
			target = 0;
			target_index = 3;
			break;

		case GLUT_KEY_F5:
			target = 1;
			target_index = 0;
			break;

		case GLUT_KEY_F6:
			target = 1;
			target_index = 1;
			break;

		case GLUT_KEY_F7:
			target = 1;
			target_index = 2;
			break;

		case GLUT_KEY_F8:
			target = 1;
			target_index = 3;
			break;

		case GLUT_KEY_F9:
			step_target = 0;
			changed = false;
			break;

		case GLUT_KEY_F10:
			step_target = 1;
			changed = false;
			break;
	}

	if (target != -1)
	{
		if (settings_target == 0)
		{
			// Set user parameters
			float add;
			if (isShift)
				add = -user_params_steps[target];
			else
				add = +user_params_steps[target];

			user_params[target][target_index] += add;

			tellUserParams();
		}
		else if (settings_target == 1 && target < 3)
		{
			// Set lights
			float add;
			if (isShift)
				add = -lights_step;
			else
				add = +lights_step;

			lights_diffuse[target][target_index] += add;

			// Clip
			if (lights_diffuse[target][target_index] > 1.0)
				lights_diffuse[target][target_index] = 1.0;
			else if (lights_diffuse[target][target_index] < 0.0)
				lights_diffuse[target][target_index] = 0.0;

			tellLights();
		}
	}

	if (step_target != -1)
	{
		if (settings_target == 0)
		{
			// Set user_params step size
			if (isShift)
				user_params_steps[step_target] /= 1.1;
			else
				user_params_steps[step_target] *= 1.1;

			tellUserParams();
		}
		else if (settings_target == 1)
		{
			// Set lights step size
			if (isShift)
				lights_step /= 1.1;
			else
				lights_step *= 1.1;

			tellLights();
		}
	}

	if (changed)
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
	win.rotateAroundAxis(0, (mouseInverted ? -1 : 1) * mouseSpeed * dy);
	win.rotateAroundAxis(1, mouseSpeed * dx);

	// Now warp the pointer back to the center.
	glutWarpPointer(win.w() * 0.5, win.h() * 0.5);

	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		switch (button)
		{
			case 1:
				win.resetSpeed();
				break;

			case 3:
				win.increaseSpeed();
				break;

			case 4:
				win.decreaseSpeed();
				break;

			default:
				mouseDown = true;
		}
	}
	else
	{
		mouseDown = false;
	}
}

void loadDefaultUserSettings(void)
{
	std::cout
		<< "Trying to load user settings from `user.conf'... "
		<< "errors ignored."
		<< std::endl;
	char *data = readFile("user.conf");
	if (data == NULL)
	{
		std::cout << "Okay, nevermind." << std::endl << std::endl;
		return;
	}

	sscanf(data, "%f %f %f %f\n%f %f %f %f\n%f %f",
			&user_params[0][0],
			&user_params[0][1],
			&user_params[0][2],
			&user_params[0][3],

			&user_params[1][0],
			&user_params[1][1],
			&user_params[1][2],
			&user_params[1][3],

			&user_params_steps[0],
			&user_params_steps[1]);

	std::cout << "User settings read." << std::endl << std::endl;
	free(data);
}

int main(int argc, char **argv)
{
	win.setSize(640, 400);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(win.w(), win.h());
	glutCreateWindow("GPU-Tracer");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardSpecial);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(motion);

	loadShaders();
	loadDefaultUserSettings();

	// We don't start at (0, 0, 0). Most objects are centered at that
	// position so we push the cam a little bit. This also sets the
	// initial moving step.
	win.setInitialConfig(Vec3(0, 0, 2.5), 0.02, 60.0);
	win.reset();

	glutMainLoop();
	exit(EXIT_SUCCESS);
}
