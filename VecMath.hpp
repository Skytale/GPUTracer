/*
	General purpose vector/matrix library
	Copyright (C) 2009, 2010  P. Hofmann

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef VECMATH_HPP
#define VECMATH_HPP


// sqrt
#include <cmath>
// dump
#include <iostream>


#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795028841971
#endif


class Vec3
{
	private:
		double v[3];

	public:
		// Constructors
		Vec3();
		Vec3(double x, double y, double z);

		// Array-style access for reading and writing
		double& operator [] (int i);

		// Vector-style access for reading and writing
		double& x();
		double& y();
		double& z();

		// Addition, Subtraction
		Vec3 operator + (Vec3& o);
		Vec3 operator - (Vec3& o);
		Vec3& operator += (Vec3& o);
		Vec3& operator -= (Vec3& o);

		// Scalar operations
		Vec3 operator * (double s);
		Vec3 operator / (double s);
		Vec3& operator *= (double s);
		Vec3& operator /= (double s);

		// Invert sign
		Vec3 operator - ();

		// Dot product
		double dot(Vec3& o);
		double operator * (Vec3& o);

		// Cross product
		Vec3 cross(Vec3& o);
		Vec3 operator ^ (Vec3& o);

		// Length stuff
		double lengthSquared();
		double length();
		void normalize();
		Vec3 normalized();

		// Relations to other vectors
		double distanceSquared(Vec3& o);
		double distance(Vec3& o);
		bool operator == (Vec3& o);
		bool operator != (Vec3& o);
		void dump();
};


class Mat4
{
	private:
		double m[16];

	public:
		// Constructors
		Mat4();
		Mat4(double m0, double m1, double m2, double m3,
				double m4, double m5, double m6, double m7,
				double m8, double m9, double m10, double m11,
				double m12, double m13, double m14, double m15);
		Mat4(Mat4 *o);
		Mat4(Vec3 a, double r);

		// Comparison
		bool operator == (Mat4& B);
		bool operator != (Mat4& B);

		// Array-style access for reading and writing
		double& operator [] (int i);

		// Addition, Subtraction
		Mat4 operator + (Mat4& B);
		Mat4 operator - (Mat4& B);
		Mat4& operator += (Mat4& B);
		Mat4& operator -= (Mat4& B);

		// Multiplication
		Mat4& operator *= (Mat4& B);
		Mat4 operator * (Mat4& B);

		// Transponation
		Mat4 transposed();

		// Scalar operations
		Mat4& operator *= (double s);
		Mat4 operator * (double s);
		Mat4& operator /= (double s);
		Mat4 operator / (double s);

		// Fill an array, can be used in OpenGL
		void copyToArray(double *target);

		// Some simple operations
		void clear();
		void makeIdentity();
		void makeFullOne();
		Vec3 column(int i);
		Vec3 row(int i);
		void dump();
};


class Quat4
{
	private:
		double q[4];

	public:
		Quat4();
		Quat4(Quat4 *o);
		Quat4(double a, double b, double c, double d);
		Quat4(Vec3 a, double r);

		// Comparison
		bool operator == (Quat4& p);
		bool operator != (Quat4& p);

		// Array-style access for reading and writing
		double& operator [] (int i);

		// Vector-style access for reading and writing
		double& a();
		double& b();
		double& c();
		double& d();

		// Rotational
		Mat4 makeRotate();

		// Multiplication
		Quat4& operator *= (Quat4& B);
		Quat4 operator * (Quat4& B);

		// Simple stuff
		void makeIdentity();
		double length();
		void normalize();
		void dump();
};

#endif // VECMATH_HPP
