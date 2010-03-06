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
uniform float eyedist;

uniform vec4 user_params0;
uniform vec4 user_params1;

vec3 light0 = gl_LightSource[0].position.xyz;
vec3 light0_diffuse = gl_LightSource[0].diffuse.xyz;
vec3 light0_specular = gl_LightSource[0].specular.xyz;

vec3 light1 = gl_LightSource[1].position.xyz;
vec3 light1_diffuse = gl_LightSource[1].diffuse.xyz;
vec3 light1_specular = gl_LightSource[1].specular.xyz;

vec3 object_diffuse = vec3(1.0, 0.7, 0.3);
float object_shininess = 10;

// This will be processed by CPP. You have to call something like this:
//
//     $ cpp -P -DOBJECT_FUNCTIONS='"myObject.glsl"' \
//         -DRAY_FUNCTIONS='"myRayMarching.glsl"' \
//         shader_fragment.glsl shader_fragment_final.glsl
//
// This will include your object code at this point.
#include OBJECT_FUNCTIONS
#include RAY_FUNCTIONS

void lighting(in vec3 eye, in vec3 hitpoint, in vec3 normal,
	inout vec3 color)
{
	vec3 eye_dir = normalize(eye - hitpoint);
	vec3 light_dir;
	vec3 temp;
	float diffuse;
	float specular;

	// Phong shading for: Headlight.
	if (gl_LightSource[0].spotCutoff == 1.0)
	{
		light_dir = normalize(light0 - hitpoint);
		diffuse = max(dot(light_dir, normal), 0.0);
		specular = max(dot(reflect(-light_dir, normal), eye_dir), 0.0);
		temp = (light0_diffuse * diffuse);
		temp.xyz *= object_diffuse.xyz;
		color += temp;
		color += (light0_specular * pow(specular, object_shininess));
	}

	// Phong shading for: Static light.
	if (gl_LightSource[1].spotCutoff == 1.0)
	{
		light_dir = normalize(light1 - hitpoint);
		diffuse = max(dot(light_dir, normal), 0.0);
		specular = max(dot(reflect(-light_dir, normal), eye_dir), 0.0);
		temp = (light1_diffuse * diffuse);
		temp.xyz *= object_diffuse.xyz;
		color += temp;
		color += (light1_specular * pow(specular, object_shininess));
	}
}

void main(void)
{
	// Ray from eye to interpolated position on viewing plane.
	vec3 eye = vec3(0.0, 0.0, 0.0);
	vec3 poi = p + vec3(0.0, 0.0, -eyedist);

	// Rotate them all according to rotation matrix of main program.
	eye = vec3(rot * vec4(eye, 1.0));
	poi = vec3(rot * vec4(poi, 1.0));
	light0 = vec3(rot * vec4(light0, 1.0));

	// Move them to desired position of the eye.
	eye += pos;
	poi += pos;
	light0 += pos;

	vec3 ray = normalize(poi - eye);

	// Does this ray hit the surface of the object?
	float alpha = 0;
	vec3 hitpoint;
	vec3 normal;
	if (!findIntersection(eye, ray, alpha, hitpoint, normal))
	{
		// Draw a dark grey on ray misses. Makes debugging easier.
		gl_FragColor = vec4(0.05, 0.05, 0.05, 1);
		return;
	}

	// There's an intersection with the object, so do lighting.
	vec3 col = vec3(0, 0, 0);
	lighting(eye, hitpoint, normal, col);
	gl_FragColor = vec4(col, 1);
}
