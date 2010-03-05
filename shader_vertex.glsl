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


varying vec3 p;

void main(void)
{
	// Keep the original positions of the vertices of the quad.
	gl_Position = gl_Vertex;

	// Save the positions of all four vertices of the quad. However,
	// This is a vertex shader and "p" is a varying variable. That
	// means, this position will get interpolated over the quad. So we
	// get a point on the viewing plane -- for each pixel.
	p = gl_Vertex;
}
