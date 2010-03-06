#!/bin/bash

RAY=${1:-ray/marching.glsl}
OBJECT=${2:-objects/m_mandelbulb.glsl}

cpp -P \
	-DOBJECT_FUNCTIONS=\"$OBJECT\" \
	-DRAY_FUNCTIONS=\"$RAY\" \
	shader_fragment.glsl shader_fragment_final.glsl || exit 1
./tracer
