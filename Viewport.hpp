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
		Vec3& pos() { return _pos; }
		int w() { return _w; }
		int h() { return _h; }

#ifdef MATRIX_ROTATION
		Mat4& ori() { return _ori; }
#else
		Quat4& ori() { return _ori; }
#endif

		void rotateAroundAxis(int whichAxis, double degree)
		{
			// Convert to radiant
			double radiant = degree * (M_PI / 180);

#ifdef MATRIX_ROTATION
			// What is ori()?
			//
			// It's the matrix that transforms vectors of the GLOBAL
			// system into vectors of the LOCAL system. Hence, it
			// can be multiplied with OpenGL's transformation matrix
			// in display() so that we can write down global vectors.
			//
			// What we want to do right here, is "rotating the local
			// system relative to the global system". In fact, we want
			// to set
			//     ori()' = ori() * ROTATION .
			// That is, vectors that will be drawn later on (given in
			// global coordinates) shall be rotated before they're
			// converted into our local system. This rotation must
			// take place in GLOBAL coordinates (because our vectors
			// are global). So what we must do first, is getting the
			// matrix T that transforms LOCAL coordinates into GLOBAL
			// coordinates so we can do
			//           1
			//     T * ( 0 ) = ...
			//           0
			// to get the current x-axis of that system in global
			// coordinates (for example). Since ori() is orthonormal
			// we can simply transpose it.
			//
			// I don't really transpose it, I just pick its ROW entries:
			Vec3 axis = ori().row(whichAxis);

			// Now create a proper rotation matrix and apply it
			// on our current orientation.
			Mat4 rotMat = Mat4(axis, radiant);

			// By multiplying our current matrix with this particular
			// rotation matrix, we "store" that rotation. So we've
			// successfully modified our local coordinate system.
			ori() *= rotMat;
#else
			// Find the rotation-axis for this operation
			Mat4 currentAxes = ori().makeRotate();
			Vec3 axis = currentAxes.row(whichAxis);
			axis.normalize();

			// Build the Quat4 according to:
			// http://en.wikipedia.org/wiki/Quaternion_rotation
			Quat4 mult(axis, radiant);

			// Now multiply our existing quaternion with the new one.
			ori() *= mult;
			ori().normalize();
#endif
		}

		void moveAlongAxis(int whichAxis, int dir, bool faster)
		{
#ifdef MATRIX_ROTATION
			Vec3 axis = ori().row(whichAxis);
#else
			Mat4 currentAxes = ori().makeRotate();
			Vec3 axis = currentAxes.row(whichAxis);
#endif

			axis.normalize();
			axis *= dir;
			axis *= (faster ? _movingStep * 10 : _movingStep);
			pos() += axis;
		}

		Mat4 orientationMatrix()
		{
#ifdef MATRIX_ROTATION
			return ori();
#else
			return ori().makeRotate();
#endif
		}

		void setInitialConfig(Vec3 p, double step)
		{
			_initPos = p;
			_initMovingStep = step;
		}

		void setSize(int w, int h)
		{
			_w = w;
			_h = h;
		}

		void increaseSpeed()
		{
			_movingStep *= 1.2;
			std::cout << "Moving speed: " << _movingStep << std::endl;
		}

		void decreaseSpeed()
		{
			_movingStep /= 1.2;
			std::cout << "Moving speed: " << _movingStep << std::endl;
		}

		void resetSpeed()
		{
			_movingStep = _initMovingStep;
			std::cout << "Resetting moving speed." << std::endl;
		}

		void reset()
		{
			ori().makeIdentity();
			pos() = _initPos;
			_movingStep = _initMovingStep;
			std::cout << "Resetting everything." << std::endl;
		}

		void dumpInfos()
		{
			Mat4 T = orientationMatrix();

			std::cout << "camera" << std::endl;
			std::cout << "\tfov 90" << std::endl;
			std::cout << "\torigin "
				<< pos().x() << " "
				<< pos().y() << " "
				<< pos().z() << " " << std::endl;

			// Actually, this is: T * (0, 0, -1).
			// Same reason as above, we want to get the negative z-axis
			// in local coordinates.
			std::cout << "\tviewdir "
				<< -T[2] << " "
				<< -T[6] << " "
				<< -T[10] << std::endl;

			// That's T * (0, 1, 0).
			std::cout << "\tupdir "
				<< T[1] << " "
				<< T[5] << " "
				<< T[9] << std::endl;

			std::cout << "end" << std::endl;
			std::cout << std::endl;
		}
};


#endif // VIEWPORT_HPP
