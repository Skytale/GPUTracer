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


#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

#include "VecMath.hpp"

class Viewport
{
	private:
		int _w;
		int _h;
		double _fov;
		Vec3 _pos;
		Vec3 _initPos;
		double _movingStep;
		double _initMovingStep;

#ifdef MATRIX_ROTATION
		Mat4 _ori;
#else
		Quat4 _ori;
#endif

	public:
		Vec3& pos();
		int w();
		int h();
		double fov();
		double ratio();

#ifdef MATRIX_ROTATION
		Mat4& ori();
#else
		Quat4& ori();
#endif

		void rotateAroundAxis(int whichAxis, double degree);
		void moveAlongAxis(int whichAxis, int dir, bool faster);
		Mat4 orientationMatrix();

		void setInitialConfig(Vec3 p, double step, double initialfov);
		void setSize(int w, int h);
		void increaseSpeed();
		void decreaseSpeed();
		void resetSpeed();
		void reset();
		void setFOV(double f);
		float eyedist();
		void dumpInfos();
};

#endif // VIEWPORT_HPP
