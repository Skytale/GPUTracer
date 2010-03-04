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


varying vec3 eye, pos_on_plane;
varying vec3 light1;

void main(void)
{
	gl_Position = gl_Vertex;

	/*
	 * The eye is located at (0, 0, 1).
	 * Viewing direction is (0, 0, -1).
	 */
	eye = vec3(0.0, 0.0, 1.0);
	pos_on_plane = gl_Vertex.xyz;

	/* Headlight two units above the eye. */
	light1 = eye + vec3(0.0, 2.0, 0.0);

	/* TODO: Multiplication with gl_ModelViewMatrix to allow moving. */
}
