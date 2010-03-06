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
	// Trig-free order 8 Mandelbulb. To be used with ray marching.

	float eps = 1e-7;
	vec3 z = at;
	vec3 c = at;
	float r = 0.0;

	// Read nMax from first item of second user settings.
	for (float count = 0; count < user_params1.s - 1.0; count += 1)
	{
		vec3 z2 = z * z;
		r = sqrt(dot(z, z));

		if (r > 2.0)
			break;

		float planeXY = sqrt(z2.x + z2.y) + eps;
		r += eps;

		float sinPhi = z.y / planeXY;
		float cosPhi = z.x / planeXY;
		float sinThe = planeXY / r;
		float cosThe = z.z / r;

		// First cascade level.
		sinPhi = 2.0 * sinPhi * cosPhi;
		cosPhi = 2.0 * cosPhi * cosPhi - 1.0;
		sinThe = 2.0 * sinThe * cosThe;
		cosThe = 2.0 * cosThe * cosThe - 1.0;

		// Second cascade level.
		sinPhi = 2.0 * sinPhi * cosPhi;
		cosPhi = 2.0 * cosPhi * cosPhi - 1.0;
		sinThe = 2.0 * sinThe * cosThe;
		cosThe = 2.0 * cosThe * cosThe - 1.0;

		// Third cascade level.
		sinPhi = 2.0 * sinPhi * cosPhi;
		cosPhi = 2.0 * cosPhi * cosPhi - 1.0;
		sinThe = 2.0 * sinThe * cosThe;
		cosThe = 2.0 * cosThe * cosThe - 1.0;

		// rPow = pow(r, 8)
		float rPow = r * r;
		rPow *= rPow;
		rPow *= rPow;

		// Set new z.
		z.x = sinThe * cosPhi;
		z.y = sinThe * sinPhi;
		z.z = cosThe;
		z *= rPow;
		z += c;
	}

	return r - 2.0;
}
