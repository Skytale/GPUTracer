# -*- coding: utf-8 -*-

env = Environment(CCFLAGS = '-Wall -Wextra')
env.SetOption('num_jobs', 4)

env.Program('tracer', Glob('*.cpp'), LIBS = ['glut'])
