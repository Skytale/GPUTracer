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


vec4 quatProd(vec4 x, vec4 y)
{
	return vec4(
			x[0] * y[0] - x[1] * y[1] - x[2] * y[2] - x[3] * y[3],
			x[0] * y[1] + x[1] * y[0] + x[2] * y[3] - x[3] * y[2],
			x[0] * y[2] - x[1] * y[3] + x[2] * y[0] + x[3] * y[1],
			x[0] * y[3] + x[1] * y[2] - x[2] * y[1] + x[3] * y[0]);
}

vec4 quatSq(vec4 z)
{
	return vec4(
			z[0] * z[0] - z[1] * z[1] - z[2] * z[2] - z[3] * z[3],
			2.0 * z[0] * z[1],
			2.0 * z[0] * z[2],
			2.0 * z[0] * z[3]);
}

float evalAt(vec3 at)
{
	// Quaternion Julia Fractals. To be used with ray marching.
	vec4 z  = vec4(at, 0.0);
	vec4 z2 = vec4(1, 0, 0, 0);

	// Hardcoded constant. Maybe change this later.
	vec4 c = vec4(-0.2, 0.6, 0.2, 0.2);

	float n = 0;
	float sqr_abs_z = 0.0;
	while (n < 8)
	{
		z2 = quatProd(z, z2) * 2.0;
		z  = quatSq(z) + c;

		sqr_abs_z = dot(z, z);
		if (sqr_abs_z >= 4.0)
			break;

		n++;
	}

	return sqr_abs_z - 4.0;
}
