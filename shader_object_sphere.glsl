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


bool getIntersection(in vec3 orig, in vec3 dir,
		inout float dist, inout vec3 hitpoint, inout vec3 normal)
{
	// Simple sphere intersection. To be used with direct rays.
	vec3 sphere_origin = vec3(0, 0, 0);
	float sphere_radius2 = 1.0;

	hitpoint = vec3(0, 0, 0);
	normal   = vec3(0, 0, 0);

	float alpha = -dot(dir, (orig - sphere_origin));
	vec3 q = orig + alpha * dir - sphere_origin;

	float distToCenter2 = dot(q, q);
	if (distToCenter2 > sphere_radius2)
		return false;

	float a = sqrt(sphere_radius2 - distToCenter2);
	if (alpha >= a)
		dist = alpha - a;
	else if (alpha + a > 0)
		dist = alpha + a;
	else
		return false;

	// We finally got our hitpoint and the normal at this point.
	hitpoint = orig + dist * dir;
	normal   = normalize(hitpoint - sphere_origin);

	// Push hitpoint by an epsilon to avoid artifacts.
	hitpoint += 1e-5 * normal;
	return true;
}
