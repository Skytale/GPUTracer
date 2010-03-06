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


// Parameters for ray marching
uniform float stepsize;
uniform float accuracy;
float maxval = 10;
float normalEps = 1e-5;

bool findIntersection(in vec3 orig, in vec3 dir, inout vec3 hitpoint,
	inout vec3 normal)
{
	// Raymarching with fixed initial step size and final bisection.
	// The object has to define evalAt().
	float cstep = stepsize;
	float alpha = cstep;

	vec3 at = orig + alpha * dir;
	float val = evalAt(at);
	bool sit = (val < 0.0);

	alpha += cstep;

	bool sitStart = sit;

	while (alpha < maxval)
	{
		at = orig + alpha * dir;
		val = evalAt(at);
		sit = (val < 0.0);

		// Situation changed, start bisection.
		if (sit != sitStart)
		{
			float a1 = alpha - stepsize;

			while (cstep > accuracy)
			{
				cstep *= 0.5;
				alpha = a1 + cstep;

				at = orig + alpha * dir;
				val = evalAt(at);
				sit = (val < 0.0);

				if (sit == sitStart)
					a1 = alpha;
			}

			hitpoint = at;

			// "Finite difference thing". :)
			normal.x = evalAt(at + vec3(normalEps, 0, 0));
			normal.y = evalAt(at + vec3(0, normalEps, 0));
			normal.z = evalAt(at + vec3(0, 0, normalEps));
			normal -= val;
			normal = normalize(normal);

			return true;
		}

		alpha += cstep;
	}

	return false;
}

