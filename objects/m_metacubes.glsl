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
	// Two metacubes. To be used with ray marching.

	// Control blob position with user_params.xyz and blob hardness
	// with user_params.w.
	vec3 a = user_params0.xyz - at;
	vec3 b = user_params1.xyz - at;
	a = a * a * a;
	b = b * b * b;
	float adist = dot(a, a);
	float bdist = dot(b, b);
	adist = pow(adist, max(user_params0.w, 1.0));
	bdist = pow(bdist, max(user_params1.w, 1.0));
	return -(1.0 / adist + 1.0 / bdist) + 1.0;
}
