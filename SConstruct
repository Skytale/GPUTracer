# Basic environment.
env = Environment()
env.SetOption('num_jobs', 4)
env.Append(CCFLAGS = ['-Wall', '-Wextra'])
env.Append(CCFLAGS = ['-O3', '-march=native', '-mtune=native'])
env.Append(LIBPATH = ['.'])

# Use matrix rotations instead of quaternion rotations?
#env.Append(CPPDEFINES = ['MATRIX_ROTATION'])

# What to build:
env.StaticLibrary('VecMath', ['VecMath.cpp'])
env.Program('tracer', ['GPUTracer.cpp', 'Viewport.cpp'],
	LIBS = ['glut', 'VecMath', 'GL'])
