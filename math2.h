#pragma once

#define M_PI 3.14159265358979323846264338327950288419716939937510

struct FEncHandler final
{
public:
	UINT16                                        Index;                                             // 0x0000(0x0002)(ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	INT8                                          bEncrypted;                                        // 0x0002(0x0001)(ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	UINT8                                         Pad_3[0x1];                                        // 0x0003(0x0001)(Fixing Struct Size After Last Property [ DeltaForce-7 ])
};

class c_vec3 {
public:
	float X;
	float Y;
	float Z;
	struct FEncHandler EncHandler;
};

class Vector2
{
public:
	Vector2() : x(0.f), y(0.f) {}
	Vector2(float _x, float _y) : x(_x), y(_y) {}
	~Vector2() {}
	float x, y;
};

class Vector3
{
public:
	Vector3() : x(0.f), y(0.f), z(0.f) {}
	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	~Vector3() {}
	float x, y, z;
	inline float dot(Vector3 v) { return x * v.x + y * v.y + z * v.z; }
	inline float distance(Vector3 v) { return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0))); }
	inline float length() { return sqrt(x * x + y * y + z * z); }
	bool valid() const {
		return (x != 0.0) && (y != 0.0) && (z != 0.0);
	}

	Vector3 operator+(Vector3 v)
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3 operator-(Vector3 v)
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	Vector3 operator*(float number) const
	{
		return Vector3(x * number, y * number, z * number);
	}
	Vector3 operator/(float flDiv) {
		return Vector3(x / flDiv, y / flDiv, z / flDiv);
	}
	Vector3 operator-=(Vector3 v) {

		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

};










// Size: 0xC (Inherited: 0x0)
struct FRotator {
	float Pitch; // 0x0(0x4)
	float Yaw; // 0x4(0x4)
	float Roll; // 0x8(0x4)
};

struct FEncVector final
{
public:
	float                                         X;                                                 // 0x0000(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         Y;                                                 // 0x0004(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         Z;                                                 // 0x0008(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	struct FEncHandler                            EncHandler;                                        // 0x000C(0x0004)(Transient, RepSkip, NoDestructor, NativeAccessSpecifierPrivate)


	float distance(const FEncVector& v) const {
		float dx = v.X - X;
		float dy = v.Y - Y;
		float dz = v.Z - Z;
		return std::sqrtf(dx * dx + dy * dy + dz * dz);
	}

	float distance(const Vector3& v) const {
		float dx = v.x - X;
		float dy = v.y - Y;
		float dz = v.z - Z;
		return std::sqrtf(dx * dx + dy * dy + dz * dz);
	}

	float distance(const c_vec3& v) const
	{
		float dx = v.X - X;
		float dy = v.Y - Y;
		float dz = v.Z - Z;
		return std::sqrtf(dx * dx + dy * dy + dz * dz);
	}

};



// ScriptStruct Engine.MinimalViewInfo
// 0x09E0 (0x09E0 - 0x0000)
struct FMinimalViewInfo final
{
public:
	struct FEncVector                             Location;                                          // 0x0000(0x0010)(Edit, BlueprintVisible, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	struct FRotator                               Rotation;                                          // 0x0010(0x000C)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
	float                                         FOV;                                               // 0x001C(0x0004)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         DesiredFOV;                                        // 0x0020(0x0004)(ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         OrthoWidth;                                        // 0x0024(0x0004)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         OrthoNearClipPlane;                                // 0x0028(0x0004)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, Interp, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         OrthoFarClipPlane;                                 // 0x002C(0x0004)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, Interp, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         AspectRatio;                                       // 0x0030(0x0004)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	UINT8                                         bConstrainAspectRatio : 1;                         // 0x0034(0x0001)(BitIndex: 0x00, PropSize: 0x0001 (Edit, BlueprintVisible, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic))
	UINT8                                         bUseFieldOfViewForLOD : 1;                         // 0x0034(0x0001)(BitIndex: 0x01, PropSize: 0x0001 (Edit, BlueprintVisible, NoDestructor, AdvancedDisplay, HasGetValueTypeHash, NativeAccessSpecifierPublic))
	UINT8                                         Pad_35[0x3];                                       // 0x0035(0x0003)(Fixing Size After Last Property [ DeltaForce-7 ])
};

// ScriptStruct Engine.CameraCacheEntry
// Size: 0x16D70 (Inherited: 0x0)
struct FCameraCacheEntry {
	float Timestamp; // 0x0(0x4)
	char pad_4[0xC]; // 0x4(0xC)
	struct FMinimalViewInfo POV; // 0x10(0x9E0)
};

struct FQuat
{
	float x, y, z, w;
};

struct FTransform
{
	FQuat Rot;
	Vector3 Translation;
	char Pad[4];
	Vector3 Scale;
	char Pad1[4];

	D3DMATRIX ToMatrixWithScale()
	{
		D3DMATRIX M{};
		M._41 = Translation.x;
		M._42 = Translation.y;
		M._43 = Translation.z;

		float X2 = Rot.x + Rot.x;
		float Y2 = Rot.y + Rot.y;
		float Z2 = Rot.z + Rot.z;
		float XX2 = Rot.x * X2;
		float YY2 = Rot.y * Y2;
		float ZZ2 = Rot.z * Z2;

		M._11 = (1.0f - (YY2 + ZZ2)) * Scale.x;
		M._22 = (1.0f - (XX2 + ZZ2)) * Scale.y;
		M._33 = (1.0f - (XX2 + YY2)) * Scale.z;

		float YZ2 = Rot.y * Z2;
		float WX2 = Rot.w * X2;
		M._32 = (YZ2 - WX2) * Scale.z;
		M._23 = (YZ2 + WX2) * Scale.y;

		float XY2 = Rot.x * Y2;
		float WZ2 = Rot.w * Z2;
		M._21 = (XY2 - WZ2) * Scale.y;
		M._12 = (XY2 + WZ2) * Scale.x;

		float XZ2 = Rot.x * Z2;
		float WY2 = Rot.w * Y2;
		M._31 = (XZ2 + WY2) * Scale.z;
		M._13 = (XZ2 - WY2) * Scale.x;

		M._14 = 0.0f;
		M._24 = 0.0f;
		M._34 = 0.0f;
		M._44 = 1.0f;

		return M;
	}
};


inline D3DMATRIX MatrixMultiplication(D3DMATRIX PM1, D3DMATRIX PM2)
{
	D3DMATRIX POut{};
	POut._11 = PM1._11 * PM2._11 + PM1._12 * PM2._21 + PM1._13 * PM2._31 + PM1._14 * PM2._41;
	POut._12 = PM1._11 * PM2._12 + PM1._12 * PM2._22 + PM1._13 * PM2._32 + PM1._14 * PM2._42;
	POut._13 = PM1._11 * PM2._13 + PM1._12 * PM2._23 + PM1._13 * PM2._33 + PM1._14 * PM2._43;
	POut._14 = PM1._11 * PM2._14 + PM1._12 * PM2._24 + PM1._13 * PM2._34 + PM1._14 * PM2._44;

	POut._21 = PM1._21 * PM2._11 + PM1._22 * PM2._21 + PM1._23 * PM2._31 + PM1._24 * PM2._41;
	POut._22 = PM1._21 * PM2._12 + PM1._22 * PM2._22 + PM1._23 * PM2._32 + PM1._24 * PM2._42;
	POut._23 = PM1._21 * PM2._13 + PM1._22 * PM2._23 + PM1._23 * PM2._33 + PM1._24 * PM2._43;
	POut._24 = PM1._21 * PM2._14 + PM1._22 * PM2._24 + PM1._23 * PM2._34 + PM1._24 * PM2._44;

	POut._31 = PM1._31 * PM2._11 + PM1._32 * PM2._21 + PM1._33 * PM2._31 + PM1._34 * PM2._41;
	POut._32 = PM1._31 * PM2._12 + PM1._32 * PM2._22 + PM1._33 * PM2._32 + PM1._34 * PM2._42;
	POut._33 = PM1._31 * PM2._13 + PM1._32 * PM2._23 + PM1._33 * PM2._33 + PM1._34 * PM2._43;
	POut._34 = PM1._31 * PM2._14 + PM1._32 * PM2._24 + PM1._33 * PM2._34 + PM1._34 * PM2._44;

	POut._41 = PM1._41 * PM2._11 + PM1._42 * PM2._21 + PM1._43 * PM2._31 + PM1._44 * PM2._41;
	POut._42 = PM1._41 * PM2._12 + PM1._42 * PM2._22 + PM1._43 * PM2._32 + PM1._44 * PM2._42;
	POut._43 = PM1._41 * PM2._13 + PM1._42 * PM2._23 + PM1._43 * PM2._33 + PM1._44 * PM2._43;
	POut._44 = PM1._41 * PM2._14 + PM1._42 * PM2._24 + PM1._43 * PM2._34 + PM1._44 * PM2._44;

	return POut;
}

inline D3DMATRIX Matrix(Vector3 Rot, Vector3 Origin = Vector3(0, 0, 0))
{
	float RadPitch = (Rot.x * M_PI / 180);
	float RadYaw = (Rot.y * M_PI / 180);
	float RadRoll = (Rot.z * M_PI / 180);
	float Sp = sinf(RadPitch);
	float Cp = cosf(RadPitch);
	float Sy = sinf(RadYaw);
	float Cy = cosf(RadYaw);
	float Sr = sinf(RadRoll);
	float Cr = cosf(RadRoll);

	D3DMATRIX Matrix{};

	Matrix.m[0][0] = Cp * Cy;
	Matrix.m[0][1] = Cp * Sy;
	Matrix.m[0][2] = Sp;
	Matrix.m[0][3] = 0.f;

	Matrix.m[1][0] = Sr * Sp * Cy - Cr * Sy;
	Matrix.m[1][1] = Sr * Sp * Sy + Cr * Cy;
	Matrix.m[1][2] = -Sr * Cp;
	Matrix.m[1][3] = 0.f;

	Matrix.m[2][0] = -(Cr * Sp * Cy + Sr * Sy);
	Matrix.m[2][1] = Cy * Sr - Cr * Sp * Sy;
	Matrix.m[2][2] = Cr * Cp;
	Matrix.m[2][3] = 0.f;

	Matrix.m[3][0] = Origin.x;
	Matrix.m[3][1] = Origin.y;
	Matrix.m[3][2] = Origin.z;
	Matrix.m[3][3] = 1.f;

	return Matrix;
}

enum BoneID {
	Head = 31,
	Neck = 30,
	Pelvis = 1,
	LeftThigh = 58,
	LeftCalf = 59,
	LeftFoot = 60,
	RightThigh = 62,
	RightCalf = 63,
	RightFoot = 64,
	LeftUpperArm = 34,
	LeftLowerArm = 76,
	LeftHand = 42,
	RightUpperArm = 6,
	RightLowerArm = 75,
	RightHand = 14
};

double get_cross_distance(double x1, double y1, double x2, double y2)
{
	return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

// Normalize an angle to the range [-180, 180]
float NormalizeAngle(float angle)
{
	while (angle > 180.0f)
		angle -= 360.0f;
	while (angle < -180.0f)
		angle += 360.0f;
	return angle;
}

// Linearly interpolate between two angles with proper wrapping
Vector3 LerpAngle(const Vector3& current, const Vector3& target, float factor)
{
	Vector3 delta;
	delta.x = NormalizeAngle(target.x - current.x);
	delta.y = NormalizeAngle(target.y - current.y);
	delta.z = NormalizeAngle(target.z - current.z);

	Vector3 result;
	result.x = current.x + delta.x * factor;
	result.y = current.y + delta.y * factor;
	result.z = current.z + delta.z * factor;

	// Normalize result angles if needed (optional)
	result.x = NormalizeAngle(result.x);
	result.y = NormalizeAngle(result.y);
	result.z = NormalizeAngle(result.z);

	return result;
}

inline void normalize(Vector3& in)
{
	if (in.x > 89.f) in.x -= 360.f;
	else if (in.x < -89.f) in.x += 360.f;

	// in.y = fmodf(in.y, 360.0f);
	while (in.y > 180)in.y -= 360;
	while (in.y < -180)in.y += 360;
	in.z = 0;
}

inline Vector3 VectorToRotation(Vector3 relativeLocation)
{
	constexpr auto radToUnrRot = 57.2957795f;

	return Vector3(
		atan2(relativeLocation.z, sqrt((relativeLocation.x * relativeLocation.x) + (relativeLocation.y * relativeLocation.y))) * radToUnrRot,
		atan2(relativeLocation.y, relativeLocation.x) * radToUnrRot,
		0.f);
}

inline Vector3 AimAtVector(Vector3 targetLocation, Vector3 cameraLocation)
{
	return VectorToRotation(targetLocation - cameraLocation);
}

__forceinline void Clamp(Vector3& Ang) {
	if (Ang.x < 0.f)
		Ang.x += 360.f;

	if (Ang.x > 360.f)
		Ang.x -= 360.f;

	if (Ang.y < 0.f) Ang.y += 360.f;
	if (Ang.y > 360.f) Ang.y -= 360.f;
	Ang.z = 0.f;
}

inline Vector3 SmoothAngles(Vector3 rot1, Vector3 rot2, float smooth)
{
	Vector3 ret(0, 0, 0);

	ret.x = (rot2.x - rot1.x) / smooth + rot1.x;
	ret.y = (rot2.y - rot1.y) / smooth + rot1.y;
	ret.z = (rot2.z - rot1.z) / smooth + rot1.z;

	return ret;
}

