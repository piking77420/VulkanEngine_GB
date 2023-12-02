#include "Quaternion.h"
#include "ToolBoxMathHeaders.h"

#include "Matrix4X4.h"
#include "Matrix3X3.h"




float NormalizeAngle(float angle)
{
	while (angle > (float)M_PI * 2.f)
		angle -= (float)M_PI * 2.f;
	while (angle < 0)
		angle += (float)M_PI * 2.f;
	return angle;
}
Vector3 NormalizeAngles(Vector3 angles)
{
	angles.x = NormalizeAngle(angles.x);
	angles.y = NormalizeAngle(angles.y);
	angles.z = NormalizeAngle(angles.z);
	return angles;
}



Matrix3X3 Quaternion::ToRotationMatrix3X3(const Quaternion& Q1)
{

	float b = Q1[0];
	float c = Q1[1];
	float d = Q1[2];
	float a = Q1[3];

	float aa = a * a;
	float bb = b * b;
	float cc = c * c;
	float dd = d * d;

	float bc = b * c;
	float da = d * a;
	float bd = b * d;
	float ca = c * a;
	float ba = b * a;
	float cd = c * d;

	return {
				{2.f * (aa + bb) - 1.f,2.f * (bc + da), 2.f * (bd - ca)},
				{2.f * (bc - da), 2.f * (aa + cc) - 1.f, 2.f * (cd + ba) },
				{2.f * (bd + ca),2.f * (cd - ba),2.f * (aa + dd) - 1.f},
				
	};
}

Matrix4X4 Quaternion::ToRotationMatrix4X4(const Quaternion& Q1)
{

	float b = Q1[0];
	float c = Q1[1];
	float d = Q1[2];
	float a = Q1[3];

	float aa = a * a;
	float bb = b * b;
	float cc = c * c;
	float dd = d * d;

	float bc = b * c;
	float da = d * a;
	float bd = b * d;
	float ca = c * a;
	float ba = b * a;
	float cd = c * d;

	return {
				{2.f * (aa + bb) -  1.f,2.f * (bc + da), 2.f * (bd - ca),0},
				{2.f * (bc - da), 2.f * (aa + cc) - 1.f, 2.f * (cd + ba),0.f },
				{2.f * (bd + ca),2.f * (cd - ba),2.f * (aa + dd) - 1.f,0},
				{0, 0,0, 1.f}
	};
}

Vector3 Quaternion::ToEulerAngle() const
{

		constexpr static float poleValue = 0.4995f;

		float sqw = w * w;
		float sqx = imaginary.x * imaginary.x;
		float sqy = imaginary.y * imaginary.y;
		float sqz = imaginary.z * imaginary.z;
		float unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
		float test = imaginary.x * w - imaginary.y * imaginary.z;
		Vector3 v;

		if (test > poleValue * unit)
		{ // singularity at north pole
			v.y = 2.f * std::atan2(imaginary.y, imaginary.x);
			v.x = (float)M_PI / 2.f;
			v.z = 0;
			return NormalizeAngles(v);
		}
		if (test < -poleValue * unit)
		{ // singularity at south pole
			v.y = -2.f * std::atan2(imaginary.y, imaginary.x);
			v.x = -(float)M_PI / 2.f;
			v.z = 0.f;
			return NormalizeAngles(v);
		}

		Quaternion rot = Quaternion(imaginary.z, imaginary.x, imaginary.y, w);
		v.y = std::atan2(2.f * rot.imaginary.x * rot.w + 2.f * rot.imaginary.y * rot.imaginary.z, 1 - 2.f * (rot.imaginary.z * rot.imaginary.z + rot.w * rot.w));     // Yaw
		v.x = std::asin(2.f * (rot.imaginary.x * rot.imaginary.z - rot.w * rot.imaginary.y));                             // Pitch
		v.z = std::atan2(2.f * rot.imaginary.x * rot.imaginary.y + 2.f * rot.imaginary.z * rot.w, 1 - 2.f * (rot.imaginary.y * rot.imaginary.y + rot.imaginary.z * rot.imaginary.z));      // Roll
		return NormalizeAngles(v);


	}






std::ostream& operator<<(std::ostream& stream, const Quaternion& q)
{
	stream << " i " << q[0] << " j " << q[1] << " k  " << q[2] << " w " << q[3] << '\n';

	return stream;
}
