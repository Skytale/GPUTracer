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

vec3 light0 = gl_LightSource[0].position.xyz;
vec3 light0_diffuse = gl_LightSource[0].diffuse.xyz;
vec3 light0_specular = gl_LightSource[0].specular.xyz;

vec3 light1 = gl_LightSource[1].position.xyz;
vec3 light1_diffuse = gl_LightSource[1].diffuse.xyz;
vec3 light1_specular = gl_LightSource[1].specular.xyz;

vec3 object_diffuse = vec3(1.0, 0.7, 0.3);
float object_shininess = 10;

// Parameters for ray marching
uniform float stepsize;
uniform float accuracy;
float maxval = 10;
float normalEps = 1e-5;

float evalAt(vec3 at)
{
	// Trig-free order 8 Mandelbulb.

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

	return 1.0 / r - 0.5;
}

bool getIntersection(in vec3 orig, in vec3 dir,
		inout float dist, inout vec3 hitpoint, inout vec3 normal)
{
	// Raymarching with fixed initial step size and final bisection.
	vec3 at = orig + dist * dir;
	float val = evalAt(at);
	bool sit = (val > 0.0);
	bool sitStart = sit;
	float cstep = stepsize;

	for (dist = 0; dist < maxval; dist += cstep)
	{
		at = orig + dist * dir;
		val = evalAt(at);
		sit = (val > 0.0);

		// Situation changed, start bisection.
		if (sit != sitStart)
		{
			float a1 = dist - stepsize;
			float a2 = dist;

			while (cstep > accuracy)
			{
				cstep *= 0.5;
				dist = a1 + cstep;

				at = orig + dist * dir;
				val = evalAt(at);
				sit = (val > 0.0);

				if (sit == sitStart)
					a1 = dist;
			}

			hitpoint = at;

			// "Finite difference thing". :)
			float gx = evalAt(vec3(at.x + normalEps, at.yz));
			float gy = evalAt(vec3(at.x, at.y + normalEps, at.z));
			float gz = evalAt(vec3(at.xy, at.z + normalEps));
			normal = normalize(vec3(val - gx, val - gy, val - gz));

			return true;
		}
	}

	return false;
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
	light_dir = normalize(light0 - hitpoint);
	diffuse = max(dot(light_dir, normal), 0.0);
	specular = max(dot(reflect(-light_dir, normal), eye_dir), 0.0);
	temp = (light0_diffuse * diffuse);
	temp.xyz *= object_diffuse.xyz;
	color += temp;
	color += (light0_specular * pow(specular, object_shininess));

	// Phong shading for: Static light.
	light_dir = normalize(light1 - hitpoint);
	diffuse = max(dot(light_dir, normal), 0.0);
	specular = max(dot(reflect(-light_dir, normal), eye_dir), 0.0);
	temp = (light1_diffuse * diffuse);
	temp.xyz *= object_diffuse.xyz;
	color += temp;
	color += (light1_specular * pow(specular, object_shininess));
}

void main(void)
{
	// Ray from eye to interpolated position on viewing plane.
	vec3 eye = vec3(0.0, 0.0, 0.0);
	vec3 poi = p + vec3(0.0, 0.0, -1.0);
	light0 = eye + vec3(0.0, 0.5, 0.0);

	// Rotate them all according to rotation matrix of main program.
	eye = vec3(rot * vec4(eye, 1.0));
	poi = vec3(rot * vec4(poi, 1.0));
	light0 = vec3(rot * vec4(eye + light0, 1.0));

	// Move them to desired position of the eye.
	eye += pos;
	poi += pos;
	light0 += pos;

	vec3 ray = normalize(poi - eye);

	// Does this ray hit the surface of the object?
	float alpha = 0;
	vec3 hitpoint;
	vec3 normal;
	if (!getIntersection(eye, ray, alpha, hitpoint, normal))
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
