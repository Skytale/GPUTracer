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


float evalAt(vec3 at)
{
	// "3D Mandelbrot" as suggested by David Makin.
	// To be used with ray marching.

	// See:
	// http://www.fractalforums.com/3d-fractal-generation/true-3d-mandlebrot-type-fractal/msg8242/#msg8242
	// http://www.bugman123.com/Hypercomplex/index.html

	vec3 z = at;

	// Read julia parameters from first user settings.
	vec3 c = user_params0.xyz;

	float r = 0.0;

	// Read nMax from first item of second user settings.
	for (float count = 0; count < user_params1.s - 1.0; count += 1)
	{
		r = length(z);

		if (r > 2.0)
			break;

		vec3 z2 = vec3(
			z.x * z.x - z.y * z.y - z.z * z.z,
			2 * z.x * z.y,
			2 * (z.x - z.y) * z.z
		);

		z = z2 + c;
	}

	return r - 2.0;
}
