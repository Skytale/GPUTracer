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
	// Two "metapills", unoptimized. To be used with ray marching.

	// Read first center from first user settings.
	vec3 acenter = user_params0.xyz;
	vec3 adir = normalize(vec3(1, 0, 0));
	float alen = 2.0;
	alen *= 0.5;
	
	float aval = 0.0;
	float aproj = dot(at - acenter, adir);
	if (aproj > -alen && aproj < alen)
	{
		// Calc distance to line segment
		aval = distance(at, acenter + aproj * adir);
	}
	else
	{
		// Calc distance to A and B, return minimum.
		aval = min(distance(at, acenter + alen * adir),
			distance(at, acenter - alen * adir));
	}

	// Second pill.
	vec3 bcenter = user_params1.xyz;
	vec3 bdir = normalize(vec3(0, 0, 1));
	float blen = 2.0;
	blen *= 0.5;
	
	float bval = 0.0;
	float bproj = dot(at - bcenter, bdir);
	if (bproj > -blen && bproj < blen)
		bval = distance(at, bcenter + bproj * bdir);
	else
		bval = min(distance(at, bcenter + blen * bdir),
			distance(at, bcenter - blen * bdir));

	// Radii are read from last entries of user settings and limit them
	// to safe intervals.
	float arad = user_params0.w;
	float brad = user_params1.w;
	return -(arad / aval + brad / bval) + 1.0;
}
