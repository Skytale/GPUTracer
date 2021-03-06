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


bool findIntersection(in vec3 orig, in vec3 dir, inout vec3 hitpoint,
	inout vec3 normal)
{
	// The object can do direct intersection tests. That's used for
	// primitives such as spheres.
	return getIntersection(orig, dir, hitpoint, normal);
}
