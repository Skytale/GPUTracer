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


varying vec3 p;

uniform mat4 rot;
uniform vec3 pos;

vec3 light1 = vec3(0, 0, 0);
vec3 light1_diffuse = vec3(1.0, 1.0, 1.0);
vec3 light1_specular = vec3(1.0, 1.0, 1.0);

vec3 light2 = vec3(10.0, 0.0, 0);
vec3 light2_diffuse = vec3(0.3, 0.3, 1.0);
vec3 light2_specular = vec3(0.3, 0.3, 1.0);

vec3 object_diffuse = vec3(1.0, 0.7, 0.3);
float object_shininess = 10;

bool getIntersection(in vec3 orig, in vec3 dir,
		inout float dist, inout vec3 hitpoint, inout vec3 normal)
{
	// Simple sphere intersection for now.
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

void lighting(in vec3 eye, in vec3 hitpoint, in vec3 normal,
	inout vec3 color)
{
	vec3 eye_dir = normalize(eye - hitpoint);
	vec3 light_dir;
	vec3 temp;
	float diffuse;
	float specular;

	// Phong shading for: Headlight.
	light_dir = normalize(light1 - hitpoint);
	diffuse = max(dot(light_dir, normal), 0.0);
	specular = max(dot(reflect(-light_dir, normal), eye_dir), 0.0);
	temp = (light1_diffuse * diffuse);
	temp.xyz *= object_diffuse.xyz;
	color += temp;
	color += (light1_specular * pow(specular, object_shininess));

	// Phong shading for: Static light.
	light_dir = normalize(light2 - hitpoint);
	diffuse = max(dot(light_dir, normal), 0.0);
	specular = max(dot(reflect(-light_dir, normal), eye_dir), 0.0);
	temp = (light2_diffuse * diffuse);
	temp.xyz *= object_diffuse.xyz;
	color += temp;
	color += (light2_specular * pow(specular, object_shininess));
}

void main(void)
{
	// Ray from eye to interpolated position on viewing plane.
	vec3 eye = vec3(0.0, 0.0, 0.0);
	vec3 poi = p + vec3(0.0, 0.0, 1.0);
	light1 = eye + vec3(0.0, 2.0, 0.0);

	// Rotate them all according to rotation matrix of main program.
	eye = vec3(rot * vec4(eye, 1.0));
	poi = vec3(rot * vec4(poi, 1.0));
	light1 = vec3(rot * vec4(light1, 1.0));

	// Move them to desired position of the eye.
	eye += pos;
	poi += pos;
	light1 += pos;

	vec3 ray = normalize(poi - eye);

	// Does this ray hit the surface of the object?
	float alpha = 0;
	vec3 hitpoint;
	vec3 normal;
	if (!getIntersection(eye, ray, alpha, hitpoint, normal))
	{
		// Draw a dark blue on ray misses. Makes debugging easier.
		gl_FragColor = vec4(0, 0, 0.2, 1);
		return;
	}

	// There's an intersection with the object, so do lighting.
	vec3 col = vec3(0, 0, 0);
	lighting(eye, hitpoint, normal, col);
	gl_FragColor = vec4(col, 1);
}
