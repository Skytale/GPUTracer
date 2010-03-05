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
	for (float count = 0; count < 4; count += 1)
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

		for (float cascade = 0; cascade < 3; cascade += 1)
		{
			float sinPhi2 = 2.0 * sinPhi * cosPhi;
			float cosPhi2 = 2.0 * cosPhi * cosPhi - 1.0;
			float sinThe2 = 2.0 * sinThe * cosThe;
			float cosThe2 = 2.0 * cosThe * cosThe - 1.0;

			sinPhi = sinPhi2;
			cosPhi = cosPhi2;
			sinThe = sinThe2;
			cosThe = cosThe2;
		}

		float rPow = 1;
		for (float i = 0; i < 8; i += 1)
			rPow *= r;

		z.x = rPow * sinThe * cosPhi  +  c.x;
		z.y = rPow * sinThe * sinPhi  +  c.y;
		z.z = rPow * cosThe           +  c.z;
	}

	return r - 2.0;
}
