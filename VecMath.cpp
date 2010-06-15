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


#include "VecMath.hpp"


// --- Vec3 ---

Vec3::Vec3()
{
	v[0] = 0.0;
	v[1] = 0.0;
	v[2] = 0.0;
}
Vec3::Vec3(double x, double y, double z)
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

// Array-style access for reading and writing
double& Vec3::operator [] (int i)
{
	return v[i];
}

// Vector-style access for reading and writing
double& Vec3::x() { return v[0]; }
double& Vec3::y() { return v[1]; }
double& Vec3::z() { return v[2]; }

// Addition, Subtraction
Vec3 Vec3::operator + (Vec3& o)
{
	return Vec3(
			v[0] + o[0],
			v[1] + o[1],
			v[2] + o[2]
			);
}

Vec3 Vec3::operator - (Vec3& o)
{
	return Vec3(
			v[0] - o[0],
			v[1] - o[1],
			v[2] - o[2]
			);
}

Vec3& Vec3::operator += (Vec3& o)
{
	v[0] += o[0];
	v[1] += o[1];
	v[2] += o[2];
	return *this;
}

Vec3& Vec3::operator -= (Vec3& o)
{
	v[0] -= o[0];
	v[1] -= o[1];
	v[2] -= o[2];
	return *this;
}

// Scalar operations
Vec3 Vec3::operator * (double s)
{
	return Vec3(s * v[0], s * v[1], s * v[2]);
}

Vec3 Vec3::operator / (double s)
{
	return *this * (1.0 / s);
}

Vec3& Vec3::operator *= (double s)
{
	v[0] *= s;
	v[1] *= s;
	v[2] *= s;
	return *this;
}

Vec3& Vec3::operator /= (double s)
{
	*this *= (1.0 / s);
	return *this;
}

// Invert sign
Vec3 Vec3::operator - ()
{
	return Vec3(-v[0], -v[1], -v[2]);
}

// Dot product
double Vec3::dot(Vec3& o)
{
	return x() * o.x()  +  y() * o.y()  +  z() * o.z();
}
double Vec3::operator * (Vec3& o)
{
	return dot(o);
}

// Cross product
Vec3 Vec3::cross(Vec3& o)
{
	return Vec3(
			v[1] * o[2]  -  v[2] * o[1],
			v[2] * o[0]  -  v[0] * o[2],
			v[0] * o[1]  -  v[1] * o[0]
			);
}
Vec3 Vec3::operator ^ (Vec3& o)
{
	return cross(o);
}

// Length stuff
double Vec3::lengthSquared()
{
	return dot(*this);
}

double Vec3::length()
{
	return sqrt(lengthSquared());
}

void Vec3::normalize()
{
	*this /= length();
}

Vec3 Vec3::normalized()
{
	return *this / length();
}

// Relations to other vectors
double Vec3::distanceSquared(Vec3& o)
{
	return (*this - o).lengthSquared();
}

double Vec3::distance(Vec3& o)
{
	return sqrt(distanceSquared(o));
}

bool Vec3::operator == (Vec3& o)
{
	return (
			v[0] == o[0] &&
			v[1] == o[1] &&
			v[2] == o[2]
		   );
}

bool Vec3::operator != (Vec3& o)
{
	return !(*this == o);
}

void Vec3::dump()
{
	std::cout << v[0] << ", " << v[1] << ", " << v[2] << std::endl;
}


// --- Mat4 ---

void Mat4::clear()
{
	for (int i = 0; i < 16; i++)
		m[i] = 0.0;
}

// Constructors
Mat4::Mat4()
{
	clear();
}

Mat4::Mat4(double m0, double m1, double m2, double m3,
		double m4, double m5, double m6, double m7,
		double m8, double m9, double m10, double m11,
		double m12, double m13, double m14, double m15)
{
	m[0] = m0;
	m[1] = m1;
	m[2] = m2;
	m[3] = m3;

	m[4] = m4;
	m[5] = m5;
	m[6] = m6;
	m[7] = m7;

	m[8] = m8;
	m[9] = m9;
	m[10] = m10;
	m[11] = m11;

	m[12] = m12;
	m[13] = m13;
	m[14] = m14;
	m[15] = m15;
}

Mat4::Mat4(Mat4 *o)
{
	for (int i = 0; i < 16; i++)
		m[i] = (*o)[i];
}

Mat4::Mat4(Vec3 a, double r)
{
	// Create a rotation matrix for an arbitrary axis.
	// See Wikipedia or whatever for the formulae.

	double cosR = cos(r);
	double sinR = sin(r);

	m[0] = cosR + a.x() * a.x() * (1 - cosR);
	m[1] = a.y() * a.x() * (1 - cosR) + a.z() * sinR;
	m[2] = a.z() * a.x() * (1 - cosR) - a.y() * sinR;
	m[3] = 0;

	m[4] = a.x() * a.y() * (1 - cosR) - a.z() * sinR;
	m[5] = cosR + a.y() * a.y() * (1 - cosR);
	m[6] = a.z() * a.y() * (1 - cosR) + a.x() * sinR;
	m[7] = 0;

	m[8]  = a.x() * a.z() * (1 - cosR) + a.y() * sinR;
	m[9]  = a.y() * a.z() * (1 - cosR) - a.x() * sinR;
	m[10] = cosR + a.z() * a.z() * (1 - cosR);
	m[11] = 0;

	m[12] = 0;
	m[13] = 0;
	m[14] = 0;
	m[15] = 1;
}

// Comparison
bool Mat4::operator == (Mat4& B)
{
	for (int i = 0; i < 16; i++)
		if (m[i] != B[i])
			return false;
	return true;
}

bool Mat4::operator != (Mat4& B)
{
	return !(*this == B);
}

// Array-style access for reading and writing
double& Mat4::operator [] (int i)
{
	return m[i];
}

// Addition, Subtraction
Mat4 Mat4::operator + (Mat4& B)
{
	Mat4 C;
	for (int i = 0; i < 16; i++)
		C[i] = m[i] + B[i];
	return C;
}

Mat4 Mat4::operator - (Mat4& B)
{
	Mat4 C;
	for (int i = 0; i < 16; i++)
		C[i] = m[i] - B[i];
	return C;
}

Mat4& Mat4::operator += (Mat4&  B)
{
	for (int i = 0; i < 16; i++)
		m[i] += B[i];
	return *this;
}

Mat4& Mat4::operator -= (Mat4&  B)
{
	for (int i = 0; i < 16; i++)
		m[i] -= B[i];
	return *this;
}

// Multiplication
Mat4& Mat4::operator *= (Mat4& B)
{
	Mat4 A = Mat4(this);

	m[0] = A[0]*B[0] + A[4]*B[1] + A[8]*B[2] + A[12]*B[3];
	m[1] = A[1]*B[0] + A[5]*B[1] + A[9]*B[2] + A[13]*B[3];
	m[2] = A[2]*B[0] + A[6]*B[1] + A[10]*B[2] + A[14]*B[3];
	m[3] = A[3]*B[0] + A[7]*B[1] + A[11]*B[2] + A[15]*B[3];

	m[4] = A[0]*B[4] + A[4]*B[5] + A[8]*B[6] + A[12]*B[7];
	m[5] = A[1]*B[4] + A[5]*B[5] + A[9]*B[6] + A[13]*B[7];
	m[6] = A[2]*B[4] + A[6]*B[5] + A[10]*B[6] + A[14]*B[7];
	m[7] = A[3]*B[4] + A[7]*B[5] + A[11]*B[6] + A[15]*B[7];

	m[8] = A[0]*B[8] + A[4]*B[9] + A[8]*B[10] + A[12]*B[11];
	m[9] = A[1]*B[8] + A[5]*B[9] + A[9]*B[10] + A[13]*B[11];
	m[10] = A[2]*B[8] + A[6]*B[9] + A[10]*B[10] + A[14]*B[11];
	m[11] = A[3]*B[8] + A[7]*B[9] + A[11]*B[10] + A[15]*B[11];

	m[12] = A[0]*B[12] + A[4]*B[13] + A[8]*B[14] + A[12]*B[15];
	m[13] = A[1]*B[12] + A[5]*B[13] + A[9]*B[14] + A[13]*B[15];
	m[14] = A[2]*B[12] + A[6]*B[13] + A[10]*B[14] + A[14]*B[15];
	m[15] = A[3]*B[12] + A[7]*B[13] + A[11]*B[14] + A[15]*B[15];

	return *this;
}

Mat4 Mat4::operator * (Mat4& B)
{
	Mat4 A = Mat4(this);
	A *= B;
	return A;
}

// Transponation
Mat4 Mat4::transposed()
{
	Mat4 T;

	T[0] = m[0];
	T[1] = m[4];
	T[2] = m[8];
	T[3] = m[12];

	T[4] = m[1];
	T[5] = m[5];
	T[6] = m[9];
	T[7] = m[13];

	T[8] = m[2];
	T[9] = m[6];
	T[10] = m[10];
	T[11] = m[14];

	T[12] = m[3];
	T[13] = m[7];
	T[14] = m[11];
	T[15] = m[15];

	return T;
}

// Scalar operations
Mat4& Mat4::operator *= (double s)
{
	for (int i = 0; i < 16; i++)
		m[i] *= s;
	return *this;
}

Mat4 Mat4::operator * (double s)
{
	Mat4 A = Mat4(this);
	A *= s;
	return A;
}

Mat4& Mat4::operator /= (double s)
{
	for (int i = 0; i < 16; i++)
		m[i] /= s;
	return *this;
}

Mat4 Mat4::operator / (double s)
{
	Mat4 A = Mat4(this);
	A /= s;
	return A;
}

// Fill an array, can be used in OpenGL
void Mat4::copyToArray(double *target)
{
	for (int i = 0; i < 16; i++)
		target[i] = m[i];
}

// Some simple operations
void Mat4::makeIdentity()
{
	clear();

	m[0] = 1.0;
	m[5] = 1.0;
	m[10] = 1.0;
	m[15] = 1.0;
}

void Mat4::makeFullOne()
{
	for (int i = 0; i < 16; i++)
		m[i] = 1;
}

Vec3 Mat4::column(int i)
{
	return Vec3(m[i*4 + 0], m[i*4 + 1], m[i*4 + 2]);
}

Vec3 Mat4::row(int i)
{
	return Vec3(m[i], m[4 + i], m[8 + i]);
}

void Mat4::dump()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << "[" << (j*4 + i) << "] " << m[j*4 + i] << "\t\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}


// --- Quat4 ---
Quat4::Quat4()
{
	q[0] = 0; // scalar
	q[1] = 0;
	q[2] = 0;
	q[3] = 0;
}

Quat4::Quat4(Quat4 *o)
{
	for (int i = 0; i < 4; i++)
		q[i] = (*o)[i];
}

Quat4::Quat4(double a, double b, double c, double d)
{
	q[0] = a;
	q[1] = b;
	q[2] = c;
	q[3] = d;
}

Quat4::Quat4(Vec3 a, double r)
{
	double sinHPhi = sin(r * 0.5);

	q[0] = cos(r * 0.5);
	q[1] = a[0] * sinHPhi;
	q[2] = a[1] * sinHPhi;
	q[3] = a[2] * sinHPhi;
}

// Comparison
bool Quat4::operator == (Quat4& p)
{
	for (int i = 0; i < 4; i++)
		if (q[i] != p[i])
			return false;
	return true;
}

bool Quat4::operator != (Quat4& p)
{
	return !(*this == p);
}

// Array-style access for reading and writing
double& Quat4::operator [] (int i)
{
	return q[i];
}

// Vector-style access for reading and writing
double& Quat4::a() { return q[0]; }
double& Quat4::b() { return q[1]; }
double& Quat4::c() { return q[2]; }
double& Quat4::d() { return q[3]; }

// Rotational
Mat4 Quat4::makeRotate()
{
	// Convert this quaternion to the corresponding
	// rotation matrix. See:
	// http://www.calc3d.com/help/gquaternion.html
	// http://en.wikipedia.org/wiki/Quaternion_rotation

	return Mat4(
			1 - 2 * (c()*c() + d()*d()),
			2 * (b()*c() + a()*d()),
			2 * (b()*d() - a()*c()),
			0,

			2 * (b()*c() - a()*d()),
			1 - 2 * (d()*d() + b()*b()),
			2 * (c()*d() + a()*b()),
			0,

			2 * (b()*d() + a()*c()),
			2 * (c()*d() - a()*b()),
			1 - 2 * (b()*b() + c()*c()),
			0,

			0,
			0,
			0,
			1
			);
}

// Multiplication
Quat4& Quat4::operator *= (Quat4& B)
{
	Quat4 A = Quat4(this);

	q[0] = A[0]*B[0] - A[1]*B[1] - A[2]*B[2] - A[3]*B[3];
	q[1] = A[0]*B[1] + A[1]*B[0] + A[2]*B[3] - A[3]*B[2];
	q[2] = A[0]*B[2] - A[1]*B[3] + A[2]*B[0] + A[3]*B[1];
	q[3] = A[0]*B[3] + A[1]*B[2] - A[2]*B[1] + A[3]*B[0];

	return *this;
}

Quat4 Quat4::operator * (Quat4& B)
{
	Quat4 A = Quat4(this);
	A *= B;
	return A;
}

// Simple stuff
void Quat4::makeIdentity()
{
	q[0] = 1;
	q[1] = 0;
	q[2] = 0;
	q[3] = 0;
}

double Quat4::length()
{
	return sqrt(a()*a() + b()*b() + c()*c() + d()*d());
}

void Quat4::normalize()
{
	double len = length();
	if (len > 0.0)
	{
		q[0] /= len;
		q[1] /= len;
		q[2] /= len;
		q[3] /= len;
	}
}

void Quat4::dump()
{
	std::cout << "Length: " << length() << std::endl;
	std::cout << q[0] << std::endl;
	std::cout << q[1] << std::endl;
	std::cout << q[2] << std::endl;
	std::cout << q[3] << std::endl;
}
