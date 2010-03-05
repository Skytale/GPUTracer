#!/bin/bash

RAY=${1:-shader_ray_direct.glsl}
OBJECT=${2:-shader_object_sphere.glsl}

cpp -DOBJECT_FUNCTIONS=\"$OBJECT\" \
	-DRAY_FUNCTIONS=\"$RAY\" \
	shader_fragment.glsl shader_fragment_final.glsl || exit 1
./tracer
