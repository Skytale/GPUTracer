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
	// The so-called "distel". To be used with ray marching.
	// See http://www.freigeist.cc/gallery.html .
	return
		dot(at, at) + 1000.0 *
		dot(at.xy, at.xy) *
		dot(at.xz, at.xz) *
		dot(at.yz, at.yz) - 1.0;
}
