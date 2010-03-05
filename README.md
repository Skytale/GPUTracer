GPUTracer
=========

This program can be used as a basis for ray tracing experiments. It's
written in C++ and GLSL, GLUT is required. Raytracing is done in the
fragment shader of your graphics card.

Features:

* Free movement of the camera (`WASD` + mouse look), based on my
  [SpaceSim](http://github.com/vain/SpaceSim) example.
* Modular shader hierarchy: Choose your objects and ray marching
  algorithms at runtime.
* Two lights: Headlight and a static light.
* Print out current scene information so you can use it in other
  raytracers.

These objects are already implemented:

* Traditional raytracing with algebraic intersection testing: A simple
  sphere.
* Ray marching with final bisection: Mandelbulb, Quaternion Julia
  Fractals and a smooth cube.


Building
--------

Should be as easy as:

	$ scons

Hence you need [SCons](http://www.scons.org/).

This removes all built files:

	$ scons -c


Launching
---------

Due to the modular shaders, this is a bit more complicated.

Only the two files `shader_vertex.glsl` and `shader_fragment_final.glsl`
are loaded by the main program. However, the second one can be
constructed from `shader_fragment.glsl` and two other files using a C
preprocessor:

	$ cpp -P -DOBJECT_FUNCTIONS='"myObject.glsl"' \
		-DRAY_FUNCTIONS='"myRayMarching.glsl"' \
		shader_fragment.glsl shader_fragment_final.glsl

If you have a look at `shader_fragment.glsl`, you'll see that there are
two `#include` statements. GLSL, however, does not support such
statements. Hence you need CPP.

`RAY_FUNCTIONS` must point to a file that defines this method:

	bool findIntersection(in vec3 orig, in vec3 dir,
		inout float dist, inout vec3 hitpoint, inout vec3 normal);

This is called from `main()` to find out whether one particular ray has
an intersection with the object or not. Basically, you can implement
anything you like in that function. `shader_ray_direct.glsl` contains a
simple wrapper function to `getIntersection()` (see below) while
`shader_ray_marching.glsl` implements a ray marching algorithm.

The function `getIntersection()` is supposed to be implemented by
objects that can do algebraic intersection testing.
`shader_object_sphere.glsl` is an example.

`shader_ray_marching.glsl`, instead, implements ray marching. This, in
turn, expects a function called `evalAt()`. That function is supposed to
evaluate your iso surface at a given point. Take the cube as an example:

	x^6 + y^6 + z^6 - 1  =  0

So, the "threshold" or "cutoff value" is always 0.

`getIntersection()` and `evalAt()` are supposed to be implemented in a
separate file. `OBJECT_FUNCTIONS` points to that file.

If you have a look at the call to CPP again, you'll see that this
modular system allows you to share code between different objects:

* The sphere and a mesh, for example, could share the wrapper call to
  `getIntersection()`.
* The mandelbulb or a julia fractal could share the code for ray
  marching.

So it boils down to:

* `main() -> findIntersection() -> getIntersection()` for algebraic
  intersection testing or
* `main() -> findIntersection() -> evalAt()` for ray marching.

In `run.sh` you'll find a wrapper to the CPP calls.


Keys
----

General keys:

* `[w]`, `[a]`, `[s]` and `[d]` move in your current XZ-plane.
* `[q]` and `[e]` roll the camera.
* `[r]` and `[f]` move up and down.
* `[Up]` and `[Down]` control FOV.
* `[R]` resets the camera.
* `[m]` toggles mouse look (the cursor will get captured).
* `[i]` inverts "up" and "down" of your mouse.
* Left or right mouse button while moving temporarily increases your
  speed.
* Use the mouse wheel to permanently alter your speed. `[MiddleMouse]`
  resets your speed.
* `[Space]` prints out scene information such as the camera position.
* `[1]` and `[2]` toggle the lights.
* `[Esc]` quits.

Keys specific to ray marching:

* `[t]` switches to a large initial step size. Expect to get artifacts.
* `[T]` switches to a smaller step size. Expect this to be very slow.
* `[g]` switches to a low accuracy when doing bisection.
* `[G]` switches to a higher accuracy when doing bisection.
