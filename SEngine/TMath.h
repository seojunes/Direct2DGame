#pragma once
#include <d3d11_1.h> 
//#include <d3dx11.h> 
#if !defined(__d3d11_h__) && !defined(__d3d11_x_h__) && !defined(__d3d12_h__) && !defined(__d3d12_x_h__)
#error include d3d11.h or d3d12.h before including TMath.h
#endif

#if !defined(_XBOX_ONE) || !defined(_TITLE)
//#include <dxgi1_2.h>
#endif

#include <functional>
#include <assert.h>
#include <memory.h>

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>

#ifndef XM_CONSTEXPR
#define XM_CONSTEXPR
#endif

using namespace DirectX;
using namespace DirectX::PackedVector;

namespace T
{
	struct TVector2;
	struct TVector3;
	struct TVector4;
	struct Matrix;
	struct TQuaternion;
	struct TPlane;

	//------------------------------------------------------------------------------
	// 2D rectangle
	struct TRectangle
	{
		long x;
		long y;
		long width;
		long height;

		// Creators
		TRectangle() noexcept : x(0), y(0), width(0), height(0) {}
		XM_CONSTEXPR TRectangle(long ix, long iy, long iw, long ih) : x(ix), y(iy), width(iw), height(ih) {}
		explicit TRectangle(const RECT& rct) : x(rct.left), y(rct.top), width(rct.right - rct.left), height(rct.bottom - rct.top) {}

		TRectangle(const TRectangle&) = default;
		TRectangle& operator=(const TRectangle&) = default;

		TRectangle(TRectangle&&) = default;
		TRectangle& operator=(TRectangle&&) = default;

		operator RECT() { RECT rct; rct.left = x; rct.top = y; rct.right = (x + width); rct.bottom = (y + height); return rct; }
#ifdef __cplusplus_winrt
		operator Windows::Foundation::Rect() { return Windows::Foundation::Rect(float(x), float(y), float(width), float(height)); }
#endif

		// Comparison operators
		bool operator == (const TRectangle& r) const { return (x == r.x) && (y == r.y) && (width == r.width) && (height == r.height); }
		bool operator == (const RECT& rct) const { return (x == rct.left) && (y == rct.top) && (width == (rct.right - rct.left)) && (height == (rct.bottom - rct.top)); }

		bool operator != (const TRectangle& r) const { return (x != r.x) || (y != r.y) || (width != r.width) || (height != r.height); }
		bool operator != (const RECT& rct) const { return (x != rct.left) || (y != rct.top) || (width != (rct.right - rct.left)) || (height != (rct.bottom - rct.top)); }

		// Assignment operators
		TRectangle& operator=(_In_ const RECT& rct) { x = rct.left; y = rct.top; width = (rct.right - rct.left); height = (rct.bottom - rct.top); return *this; }

		// TRectangle operations
		TVector2 Location() const;
		TVector2 Center() const;

		bool IsEmpty() const { return (width == 0 && height == 0 && x == 0 && y == 0); }

		bool Contains(long ix, long iy) const { return (x <= ix) && (ix < (x + width)) && (y <= iy) && (iy < (y + height)); }
		bool Contains(const TVector2& point) const;
		bool Contains(const TRectangle& r) const { return (x <= r.x) && ((r.x + r.width) <= (x + width)) && (y <= r.y) && ((r.y + r.height) <= (y + height)); }
		bool Contains(const RECT& rct) const { return (x <= rct.left) && (rct.right <= (x + width)) && (y <= rct.top) && (rct.bottom <= (y + height)); }

		void Inflate(long horizAmount, long vertAmount);

		bool Intersects(const TRectangle& r) const { return (r.x < (x + width)) && (x < (r.x + r.width)) && (r.y < (y + height)) && (y < (r.y + r.height)); }
		bool Intersects(const RECT& rct) const { return (rct.left < (x + width)) && (x < rct.right) && (rct.top < (y + height)) && (y < rct.bottom); }

		void Offset(long ox, long oy) { x += ox; y += oy; }

		// Static functions
		static TRectangle Intersect(const TRectangle& ra, const TRectangle& rb);
		static RECT Intersect(const RECT& rcta, const RECT& rctb);

		static TRectangle Union(const TRectangle& ra, const TRectangle& rb);
		static RECT Union(const RECT& rcta, const RECT& rctb);
	};

	//------------------------------------------------------------------------------
	// 2D vector
	struct TVector2 : DirectX::XMFLOAT2
	{
		TVector2() noexcept : XMFLOAT2(0.f, 0.f) {}
		XM_CONSTEXPR explicit TVector2(float x) : XMFLOAT2(x, x) {}
		XM_CONSTEXPR TVector2(float _x, float _y) : XMFLOAT2(_x, _y) {}
		explicit TVector2(_In_reads_(2) const float *pArray) : XMFLOAT2(pArray) {}
		TVector2(XMFLOAT2 V) { x = V.x; y = V.y; }
		TVector2(const XMFLOAT2& V) { this->x = V.x; this->y = V.y; }
		explicit TVector2(const DirectX::XMVECTORF32& F) { this->x = F.f[0]; this->y = F.f[1]; }

		TVector2(const TVector2&) = default;
		TVector2& operator=(const TVector2&) = default;

		TVector2(TVector2&&) = default;
		TVector2& operator=(TVector2&&) = default;

		operator DirectX::XMVECTOR() const { return XMLoadFloat2(this); }

		// Comparison operators
		bool operator == (const TVector2& V) const;
		bool operator != (const TVector2& V) const;

		// Assignment operators
		TVector2& operator= (const DirectX::XMVECTORF32& F) { x = F.f[0]; y = F.f[1]; return *this; }
		TVector2& operator+= (const TVector2& V);
		TVector2& operator-= (const TVector2& V);
		TVector2& operator*= (const TVector2& V);
		TVector2& operator*= (float S);
		TVector2& operator/= (float S);

		// Unary operators
		TVector2 operator+ () const { return *this; }
		TVector2 operator- () const { return TVector2(-x, -y); }

		// Vector operations
		bool InBounds(const TVector2& Bounds) const;

		float Length() const;
		float LengthSquared() const;

		float Dot(const TVector2& V) const;
		void Cross(const TVector2& V, TVector2& result) const;
		TVector2 Cross(const TVector2& V) const;

		void Normalize();
		void Normalize(TVector2& result) const;

		void Clamp(const TVector2& vmin, const TVector2& vmax);
		void Clamp(const TVector2& vmin, const TVector2& vmax, TVector2& result) const;

		// Static functions
		static float Distance(const TVector2& v1, const TVector2& v2);
		static float DistanceSquared(const TVector2& v1, const TVector2& v2);

		static void Min(const TVector2& v1, const TVector2& v2, TVector2& result);
		static TVector2 Min(const TVector2& v1, const TVector2& v2);

		static void Max(const TVector2& v1, const TVector2& v2, TVector2& result);
		static TVector2 Max(const TVector2& v1, const TVector2& v2);

		static void Lerp(const TVector2& v1, const TVector2& v2, float t, TVector2& result);
		static TVector2 Lerp(const TVector2& v1, const TVector2& v2, float t);

		static void SmoothStep(const TVector2& v1, const TVector2& v2, float t, TVector2& result);
		static TVector2 SmoothStep(const TVector2& v1, const TVector2& v2, float t);

		static void Barycentric(const TVector2& v1, const TVector2& v2, const TVector2& v3, float f, float g, TVector2& result);
		static TVector2 Barycentric(const TVector2& v1, const TVector2& v2, const TVector2& v3, float f, float g);

		static void CatmullRom(const TVector2& v1, const TVector2& v2, const TVector2& v3, const TVector2& v4, float t, TVector2& result);
		static TVector2 CatmullRom(const TVector2& v1, const TVector2& v2, const TVector2& v3, const TVector2& v4, float t);

		static void Hermite(const TVector2& v1, const TVector2& t1, const TVector2& v2, const TVector2& t2, float t, TVector2& result);
		static TVector2 Hermite(const TVector2& v1, const TVector2& t1, const TVector2& v2, const TVector2& t2, float t);

		static void Reflect(const TVector2& ivec, const TVector2& nvec, TVector2& result);
		static TVector2 Reflect(const TVector2& ivec, const TVector2& nvec);

		static void Refract(const TVector2& ivec, const TVector2& nvec, float refractionIndex, TVector2& result);
		static TVector2 Refract(const TVector2& ivec, const TVector2& nvec, float refractionIndex);

		static void Transform(const TVector2& v, const TQuaternion& quat, TVector2& result);
		static TVector2 Transform(const TVector2& v, const TQuaternion& quat);

		static void Transform(const TVector2& v, const Matrix& m, TVector2& result);
		static TVector2 Transform(const TVector2& v, const Matrix& m);
		static void Transform(_In_reads_(count) const TVector2* varray, size_t count, const Matrix& m, _Out_writes_(count) TVector2* resultArray);

		static void Transform(const TVector2& v, const Matrix& m, TVector4& result);
		static void Transform(_In_reads_(count) const TVector2* varray, size_t count, const Matrix& m, _Out_writes_(count) TVector4* resultArray);

		static void TransformNormal(const TVector2& v, const Matrix& m, TVector2& result);
		static TVector2 TransformNormal(const TVector2& v, const Matrix& m);
		static void TransformNormal(_In_reads_(count) const TVector2* varray, size_t count, const Matrix& m, _Out_writes_(count) TVector2* resultArray);

		// Constants
		static const TVector2 Zero;
		static const TVector2 One;
		static const TVector2 UnitX;
		static const TVector2 UnitY;
	};

	// Binary operators
	TVector2 operator+ (const TVector2& V1, const TVector2& V2);
	TVector2 operator- (const TVector2& V1, const TVector2& V2);
	TVector2 operator* (const TVector2& V1, const TVector2& V2);
	TVector2 operator* (const TVector2& V, float S);
	TVector2 operator/ (const TVector2& V1, const TVector2& V2);
	TVector2 operator* (float S, const TVector2& V);

	//------------------------------------------------------------------------------
	// 3D vector
	struct TVector3 : DirectX::XMFLOAT3
	{
		TVector3() noexcept : DirectX::XMFLOAT3(0.f, 0.f, 0.f) {}
		XM_CONSTEXPR explicit TVector3(float x) : DirectX::XMFLOAT3(x, x, x) {}
		XM_CONSTEXPR TVector3(float _x, float _y, float _z) : DirectX::XMFLOAT3(_x, _y, _z) {}
		TVector3(const float *pArray) : DirectX::XMFLOAT3(pArray) {}
		TVector3(DirectX::XMFLOAT3 V) { x = V.x; y = V.y; z = V.z; }
		TVector3(const DirectX::XMFLOAT3& V) { this->x = V.x; this->y = V.y; this->z = V.z; }
		explicit TVector3(const DirectX::XMVECTORF32& F) { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; }

		TVector3(const TVector3&) = default;
		TVector3& operator=(const TVector3&) = default;

		TVector3(TVector3&&) = default;
		TVector3& operator=(TVector3&&) = default;

		operator DirectX::XMVECTOR() const { return XMLoadFloat3(this); }

		float operator [](int i)
		{
			if (i == 0) return x;
			if (i == 1) return y;
			if (i == 2) return z;
			return 0.0f;
		}

		// Comparison operators
		bool operator == (const TVector3& V) const;
		bool operator != (const TVector3& V) const;

		// Assignment operators
		TVector3& operator= (const DirectX::XMVECTORF32& F) { x = F.f[0]; y = F.f[1]; z = F.f[2]; return *this; }
		TVector3& operator+= (const TVector3& V);
		TVector3& operator-= (const TVector3& V);
		TVector3& operator*= (const TVector3& V);
		TVector3& operator*= (float S);
		TVector3& operator/= (float S);

		// Unary operators
		TVector3 operator+ () const { return *this; }
		TVector3 operator- () const;

		// Vector operations
		bool InBounds(const TVector3& Bounds) const;

		float Length() const;
		float LengthSquared() const;

		float Dot(const TVector3& V) const;
		void Cross(const TVector3& V, TVector3& result) const;
		TVector3 Cross(const TVector3& V) const;

		void Normalize();
		void Normalize(TVector3& result) const;

		void Clamp(const TVector3& vmin, const TVector3& vmax);
		void Clamp(const TVector3& vmin, const TVector3& vmax, TVector3& result) const;

		// Static functions
		static float Distance(const TVector3& v1, const TVector3& v2);
		static float DistanceSquared(const TVector3& v1, const TVector3& v2);

		static void Min(const TVector3& v1, const TVector3& v2, TVector3& result);
		static TVector3 Min(const TVector3& v1, const TVector3& v2);

		static void Max(const TVector3& v1, const TVector3& v2, TVector3& result);
		static TVector3 Max(const TVector3& v1, const TVector3& v2);

		static void Lerp(const TVector3& v1, const TVector3& v2, float t, TVector3& result);
		static TVector3 Lerp(const TVector3& v1, const TVector3& v2, float t);

		static void SmoothStep(const TVector3& v1, const TVector3& v2, float t, TVector3& result);
		static TVector3 SmoothStep(const TVector3& v1, const TVector3& v2, float t);

		static void Barycentric(const TVector3& v1, const TVector3& v2, const TVector3& v3, float f, float g, TVector3& result);
		static TVector3 Barycentric(const TVector3& v1, const TVector3& v2, const TVector3& v3, float f, float g);

		static void CatmullRom(const TVector3& v1, const TVector3& v2, const TVector3& v3, const TVector3& v4, float t, TVector3& result);
		static TVector3 CatmullRom(const TVector3& v1, const TVector3& v2, const TVector3& v3, const TVector3& v4, float t);

		static void Hermite(const TVector3& v1, const TVector3& t1, const TVector3& v2, const TVector3& t2, float t, TVector3& result);
		static TVector3 Hermite(const TVector3& v1, const TVector3& t1, const TVector3& v2, const TVector3& t2, float t);

		static void Reflect(const TVector3& ivec, const TVector3& nvec, TVector3& result);
		static TVector3 Reflect(const TVector3& ivec, const TVector3& nvec);

		static void Refract(const TVector3& ivec, const TVector3& nvec, float refractionIndex, TVector3& result);
		static TVector3 Refract(const TVector3& ivec, const TVector3& nvec, float refractionIndex);

		static void Transform(const TVector3& v, const TQuaternion& quat, TVector3& result);
		static TVector3 Transform(const TVector3& v, const TQuaternion& quat);

		static void Transform(const TVector3& v, const Matrix& m, TVector3& result);
		static TVector3 Transform(const TVector3& v, const Matrix& m);
		static void Transform(_In_reads_(count) const TVector3* varray, size_t count, const Matrix& m, _Out_writes_(count) TVector3* resultArray);

		static void Transform(const TVector3& v, const Matrix& m, TVector4& result);
		static void Transform(_In_reads_(count) const TVector3* varray, size_t count, const Matrix& m, _Out_writes_(count) TVector4* resultArray);

		static void TransformNormal(const TVector3& v, const Matrix& m, TVector3& result);
		static TVector3 TransformNormal(const TVector3& v, const Matrix& m);
		static void TransformNormal(_In_reads_(count) const TVector3* varray, size_t count, const Matrix& m, _Out_writes_(count) TVector3* resultArray);

		// Constants
		static const TVector3 Zero;
		static const TVector3 One;
		static const TVector3 UnitX;
		static const TVector3 UnitY;
		static const TVector3 UnitZ;
		static const TVector3 Up;
		static const TVector3 Down;
		static const TVector3 Right;
		static const TVector3 Left;
		static const TVector3 Forward;
		static const TVector3 Backward;
	};

	// Binary operators
	TVector3 operator+ (const TVector3& V1, const TVector3& V2);
	TVector3 operator- (const TVector3& V1, const TVector3& V2);
	TVector3 operator* (const TVector3& V1, const TVector3& V2);
	TVector3 operator* (const TVector3& V, float S);
	TVector3 operator/ (const TVector3& V1, const TVector3& V2);
	TVector3 operator* (float S, const TVector3& V);

	//------------------------------------------------------------------------------
	// 4D vector
	struct TVector4 : public XMFLOAT4
	{
		TVector4() noexcept : XMFLOAT4(0.f, 0.f, 0.f, 0.f) {}
		XM_CONSTEXPR explicit TVector4(float x) : XMFLOAT4(x, x, x, x) {}
		XM_CONSTEXPR TVector4(float _x, float _y, float _z, float _w) : XMFLOAT4(_x, _y, _z, _w) {}
		explicit TVector4(_In_reads_(4) const float *pArray) : XMFLOAT4(pArray) {}
		TVector4(XMFLOAT4 V) { x = V.x; y = V.y; z = V.z; w = V.w; }
		TVector4(const XMFLOAT4& V) { this->x = V.x; this->y = V.y; this->z = V.z; this->w = V.w; }
		explicit TVector4(const XMVECTORF32& F) { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

		TVector4(const TVector4&) = default;
		TVector4& operator=(const TVector4&) = default;

		TVector4(TVector4&&) = default;
		TVector4& operator=(TVector4&&) = default;

		operator XMVECTOR() const { return XMLoadFloat4(this); }

		// Comparison operators
		bool operator == (const TVector4& V) const;
		bool operator != (const TVector4& V) const;

		// Assignment operators
		TVector4& operator= (const XMVECTORF32& F) { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }
		TVector4& operator+= (const TVector4& V);
		TVector4& operator-= (const TVector4& V);
		TVector4& operator*= (const TVector4& V);
		TVector4& operator*= (float S);
		TVector4& operator/= (float S);

		// Unary operators
		TVector4 operator+ () const { return *this; }
		TVector4 operator- () const;

		// Vector operations
		bool InBounds(const TVector4& Bounds) const;

		float Length() const;
		float LengthSquared() const;

		float Dot(const TVector4& V) const;
		void Cross(const TVector4& v1, const TVector4& v2, TVector4& result) const;
		TVector4 Cross(const TVector4& v1, const TVector4& v2) const;

		void Normalize();
		void Normalize(TVector4& result) const;

		void Clamp(const TVector4& vmin, const TVector4& vmax);
		void Clamp(const TVector4& vmin, const TVector4& vmax, TVector4& result) const;

		// Static functions
		static float Distance(const TVector4& v1, const TVector4& v2);
		static float DistanceSquared(const TVector4& v1, const TVector4& v2);

		static void Min(const TVector4& v1, const TVector4& v2, TVector4& result);
		static TVector4 Min(const TVector4& v1, const TVector4& v2);

		static void Max(const TVector4& v1, const TVector4& v2, TVector4& result);
		static TVector4 Max(const TVector4& v1, const TVector4& v2);

		static void Lerp(const TVector4& v1, const TVector4& v2, float t, TVector4& result);
		static TVector4 Lerp(const TVector4& v1, const TVector4& v2, float t);

		static void SmoothStep(const TVector4& v1, const TVector4& v2, float t, TVector4& result);
		static TVector4 SmoothStep(const TVector4& v1, const TVector4& v2, float t);

		static void Barycentric(const TVector4& v1, const TVector4& v2, const TVector4& v3, float f, float g, TVector4& result);
		static TVector4 Barycentric(const TVector4& v1, const TVector4& v2, const TVector4& v3, float f, float g);

		static void CatmullRom(const TVector4& v1, const TVector4& v2, const TVector4& v3, const TVector4& v4, float t, TVector4& result);
		static TVector4 CatmullRom(const TVector4& v1, const TVector4& v2, const TVector4& v3, const TVector4& v4, float t);

		static void Hermite(const TVector4& v1, const TVector4& t1, const TVector4& v2, const TVector4& t2, float t, TVector4& result);
		static TVector4 Hermite(const TVector4& v1, const TVector4& t1, const TVector4& v2, const TVector4& t2, float t);

		static void Reflect(const TVector4& ivec, const TVector4& nvec, TVector4& result);
		static TVector4 Reflect(const TVector4& ivec, const TVector4& nvec);

		static void Refract(const TVector4& ivec, const TVector4& nvec, float refractionIndex, TVector4& result);
		static TVector4 Refract(const TVector4& ivec, const TVector4& nvec, float refractionIndex);

		static void Transform(const TVector2& v, const TQuaternion& quat, TVector4& result);
		static TVector4 Transform(const TVector2& v, const TQuaternion& quat);

		static void Transform(const TVector3& v, const TQuaternion& quat, TVector4& result);
		static TVector4 Transform(const TVector3& v, const TQuaternion& quat);

		static void Transform(const TVector4& v, const TQuaternion& quat, TVector4& result);
		static TVector4 Transform(const TVector4& v, const TQuaternion& quat);

		static void Transform(const TVector4& v, const Matrix& m, TVector4& result);
		static TVector4 Transform(const TVector4& v, const Matrix& m);
		static void Transform(_In_reads_(count) const TVector4* varray, size_t count, const Matrix& m, _Out_writes_(count) TVector4* resultArray);

		// Constants
		static const TVector4 Zero;
		static const TVector4 One;
		static const TVector4 UnitX;
		static const TVector4 UnitY;
		static const TVector4 UnitZ;
		static const TVector4 UnitW;
	};

	// Binary operators
	TVector4 operator+ (const TVector4& V1, const TVector4& V2);
	TVector4 operator- (const TVector4& V1, const TVector4& V2);
	TVector4 operator* (const TVector4& V1, const TVector4& V2);
	TVector4 operator* (const TVector4& V, float S);
	TVector4 operator/ (const TVector4& V1, const TVector4& V2);
	TVector4 operator* (float S, const TVector4& V);

	//------------------------------------------------------------------------------
	// 4x4 Matrix (assumes right-handed cooordinates)
	struct Matrix : public XMFLOAT4X4
	{
		Matrix() noexcept
			: XMFLOAT4X4(1.f, 0, 0, 0,
				0, 1.f, 0, 0,
				0, 0, 1.f, 0,
				0, 0, 0, 1.f) {}
		XM_CONSTEXPR Matrix(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33)
			: XMFLOAT4X4(m00, m01, m02, m03,
				m10, m11, m12, m13,
				m20, m21, m22, m23,
				m30, m31, m32, m33) {}
		explicit Matrix(const TVector3& r0, const TVector3& r1, const TVector3& r2)
			: XMFLOAT4X4(r0.x, r0.y, r0.z, 0,
				r1.x, r1.y, r1.z, 0,
				r2.x, r2.y, r2.z, 0,
				0, 0, 0, 1.f) {}
		explicit Matrix(const TVector4& r0, const TVector4& r1, const TVector4& r2, const TVector4& r3)
			: XMFLOAT4X4(r0.x, r0.y, r0.z, r0.w,
				r1.x, r1.y, r1.z, r1.w,
				r2.x, r2.y, r2.z, r2.w,
				r3.x, r3.y, r3.z, r3.w) {}
		Matrix(const XMFLOAT4X4& M) { memcpy_s(this, sizeof(float) * 16, &M, sizeof(XMFLOAT4X4)); }
		Matrix(const XMFLOAT3X3& M);
		Matrix(const XMFLOAT4X3& M);

		explicit Matrix(_In_reads_(16) const float *pArray) : XMFLOAT4X4(pArray) {}
		Matrix(CXMMATRIX M) { XMStoreFloat4x4(this, M); }

		Matrix(const Matrix&) = default;
		Matrix& operator=(const Matrix&) = default;

		Matrix(Matrix&&) = default;
		Matrix& operator=(Matrix&&) = default;

		operator XMFLOAT4X4() const { return *this; }

		// Comparison operators
		bool operator == (const Matrix& M) const;
		bool operator != (const Matrix& M) const;

		// Assignment operators
		Matrix& operator= (const XMFLOAT3X3& M);
		Matrix& operator= (const XMFLOAT4X3& M);
		Matrix& operator+= (const Matrix& M);
		Matrix& operator-= (const Matrix& M);
		Matrix& operator*= (const Matrix& M);
		Matrix& operator*= (float S);
		Matrix& operator/= (float S);

		Matrix& operator/= (const Matrix& M);
		// Element-wise divide

		// Unary operators
		Matrix operator+ () const { return *this; }
		Matrix operator- () const;

		// Properties
		TVector3 Up() const { return TVector3(_21, _22, _23); }
		void Up(const TVector3& v) { _21 = v.x; _22 = v.y; _23 = v.z; }

		TVector3 Down() const { return TVector3(-_21, -_22, -_23); }
		void Down(const TVector3& v) { _21 = -v.x; _22 = -v.y; _23 = -v.z; }

		TVector3 Right() const { return TVector3(_11, _12, _13); }
		void Right(const TVector3& v) { _11 = v.x; _12 = v.y; _13 = v.z; }

		TVector3 Left() const { return TVector3(-_11, -_12, -_13); }
		void Left(const TVector3& v) { _11 = -v.x; _12 = -v.y; _13 = -v.z; }

		TVector3 Forward() const { return TVector3(-_31, -_32, -_33); }
		void Forward(const TVector3& v) { _31 = -v.x; _32 = -v.y; _33 = -v.z; }

		TVector3 Backward() const { return TVector3(_31, _32, _33); }
		void Backward(const TVector3& v) { _31 = v.x; _32 = v.y; _33 = v.z; }

		TVector3 Translation() const { return TVector3(_41, _42, _43); }
		void Translation(const TVector3& v) { _41 = v.x; _42 = v.y; _43 = v.z; }

		// Matrix operations
		bool Decompose(TVector3& scale, TQuaternion& rotation, TVector3& translation);

		Matrix Transpose() const;
		void Transpose(Matrix& result) const;

		Matrix Invert() const;
		void Invert(Matrix& result) const;

		float Determinant() const;

		// Static functions
		static Matrix CreateBillboard(const TVector3& object, const TVector3& cameraPosition, const TVector3& cameraUp, _In_opt_ const TVector3* cameraForward = nullptr);

		static Matrix CreateConstrainedBillboard(const TVector3& object, const TVector3& cameraPosition, const TVector3& rotateAxis,
			_In_opt_ const TVector3* cameraForward = nullptr, _In_opt_ const TVector3* objectForward = nullptr);

		static Matrix CreateTranslation(const TVector3& position);
		static Matrix CreateTranslation(float x, float y, float z);

		static Matrix CreateScale(const TVector3& scales);
		static Matrix CreateScale(float xs, float ys, float zs);
		static Matrix CreateScale(float scale);

		static Matrix CreateRotationX(float radians);
		static Matrix CreateRotationY(float radians);
		static Matrix CreateRotationZ(float radians);

		static Matrix CreateFromAxisAngle(const TVector3& axis, float angle);

		static Matrix CreatePerspectiveFieldOfView(float fov, float aspectRatio, float nearPlane, float farPlane);
		static Matrix CreatePerspective(float width, float height, float nearPlane, float farPlane);
		static Matrix CreatePerspectiveOffCenter(float left, float right, float bottom, float top, float nearPlane, float farPlane);
		static Matrix CreateOrthographic(float width, float height, float zNearPlane, float zFarPlane);
		static Matrix CreateOrthographicOffCenter(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane);

		static Matrix CreateLookAt(const TVector3& position, const TVector3& target, const TVector3& up);
		static Matrix CreateWorld(const TVector3& position, const TVector3& forward, const TVector3& up);

		static Matrix CreateFromQuaternion(const TQuaternion& quat);

		static Matrix CreateFromYawPitchRoll(float yaw, float pitch, float roll);

		static Matrix CreateShadow(const TVector3& lightDir, const TPlane& plane);

		static Matrix CreateReflection(const TPlane& plane);

		static void Lerp(const Matrix& M1, const Matrix& M2, float t, Matrix& result);
		static Matrix Lerp(const Matrix& M1, const Matrix& M2, float t);

		static void Transform(const Matrix& M, const TQuaternion& rotation, Matrix& result);
		static Matrix Transform(const Matrix& M, const TQuaternion& rotation);

		// Constants
		static const Matrix Identity;
	};

	// Binary operators
	Matrix operator+ (const Matrix& M1, const Matrix& M2);
	Matrix operator- (const Matrix& M1, const Matrix& M2);
	Matrix operator* (const Matrix& M1, const Matrix& M2);
	Matrix operator* (const Matrix& M, float S);
	Matrix operator/ (const Matrix& M, float S);
	Matrix operator/ (const Matrix& M1, const Matrix& M2);
	// Element-wise divide
	Matrix operator* (float S, const Matrix& M);


	//-----------------------------------------------------------------------------
	// TPlane
	struct TPlane : public XMFLOAT4
	{
		TPlane() noexcept : XMFLOAT4(0.f, 1.f, 0.f, 0.f) {}
		XM_CONSTEXPR TPlane(float _x, float _y, float _z, float _w) : XMFLOAT4(_x, _y, _z, _w) {}
		TPlane(const TVector3& normal, float d) : XMFLOAT4(normal.x, normal.y, normal.z, d) {}
		TPlane(const TVector3& point1, const TVector3& point2, const TVector3& point3);
		TPlane(const TVector3& point, const TVector3& normal);
		explicit TPlane(const TVector4& v) : XMFLOAT4(v.x, v.y, v.z, v.w) {}
		explicit TPlane(_In_reads_(4) const float *pArray) : XMFLOAT4(pArray) {}
		TPlane(XMFLOAT4 V) { x = V.x; y = V.y; z = V.z; w = V.w; }
		TPlane(const XMFLOAT4& p) { this->x = p.x; this->y = p.y; this->z = p.z; this->w = p.w; }
		explicit TPlane(const XMVECTORF32& F) { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

		TPlane(const TPlane&) = default;
		TPlane& operator=(const TPlane&) = default;

		TPlane(TPlane&&) = default;
		TPlane& operator=(TPlane&&) = default;

		operator XMVECTOR() const { return XMLoadFloat4(this); }

		// Comparison operators
		bool operator == (const TPlane& p) const;
		bool operator != (const TPlane& p) const;

		// Assignment operators
		TPlane& operator= (const XMVECTORF32& F) { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }

		// Properties
		TVector3 Normal() const { return TVector3(x, y, z); }
		void Normal(const TVector3& normal) { x = normal.x; y = normal.y; z = normal.z; }

		float D() const { return w; }
		void D(float d) { w = d; }

		// TPlane operations
		void Normalize();
		void Normalize(TPlane& result) const;

		float Dot(const TVector4& v) const;
		float DotCoordinate(const TVector3& position) const;
		float DotNormal(const TVector3& normal) const;

		// Static functions
		static void Transform(const TPlane& plane, const Matrix& M, TPlane& result);
		static TPlane Transform(const TPlane& plane, const Matrix& M);

		static void Transform(const TPlane& plane, const TQuaternion& rotation, TPlane& result);
		static TPlane Transform(const TPlane& plane, const TQuaternion& rotation);
		// Input quaternion must be the inverse transpose of the transformation
	};

	//------------------------------------------------------------------------------
	// TQuaternion
	struct TQuaternion : public XMFLOAT4
	{
		TQuaternion() noexcept : XMFLOAT4(0, 0, 0, 1.f) {}
		XM_CONSTEXPR TQuaternion(float _x, float _y, float _z, float _w) : XMFLOAT4(_x, _y, _z, _w) {}
		TQuaternion(const TVector3& v, float scalar) : XMFLOAT4(v.x, v.y, v.z, scalar) {}
		explicit TQuaternion(const TVector4& v) : XMFLOAT4(v.x, v.y, v.z, v.w) {}
		explicit TQuaternion(_In_reads_(4) const float *pArray) : XMFLOAT4(pArray) {}
		TQuaternion(XMFLOAT4 V) { x = V.x; y = V.y; z = V.z; w = V.w; }
		TQuaternion(const XMFLOAT4& q) { this->x = q.x; this->y = q.y; this->z = q.z; this->w = q.w; }
		explicit TQuaternion(const XMVECTORF32& F) { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

		TQuaternion(const TQuaternion&) = default;
		TQuaternion& operator=(const TQuaternion&) = default;

		TQuaternion(TQuaternion&&) = default;
		TQuaternion& operator=(TQuaternion&&) = default;

		//operator int () const { return 0; }
		//SampleClass f;
		//int i = f; //  f.operator int () 를 호출하고 초기화 및 반한됨.
		operator XMVECTOR() const { return XMLoadFloat4(this); }

		// Comparison operators
		bool operator == (const TQuaternion& q) const;
		bool operator != (const TQuaternion& q) const;

		// Assignment operators
		TQuaternion& operator= (const XMVECTORF32& F) { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }
		TQuaternion& operator+= (const TQuaternion& q);
		TQuaternion& operator-= (const TQuaternion& q);
		TQuaternion& operator*= (const TQuaternion& q);
		TQuaternion& operator*= (float S);
		TQuaternion& operator/= (const TQuaternion& q);

		// Unary operators
		TQuaternion operator+ () const { return *this; }
		TQuaternion operator- () const;

		// TQuaternion operations
		float Length() const;
		float LengthSquared() const;

		void Normalize();
		void Normalize(TQuaternion& result) const;

		void Conjugate();
		void Conjugate(TQuaternion& result) const;

		void Inverse(TQuaternion& result) const;

		float Dot(const TQuaternion& Q) const;

		// Static functions
		static TQuaternion CreateFromAxisAngle(const TVector3& axis, float angle);
		static TQuaternion CreateFromYawPitchRoll(float yaw, float pitch, float roll);
		static TQuaternion CreateFromRotationMatrix(const Matrix& M);

		static void Lerp(const TQuaternion& q1, const TQuaternion& q2, float t, TQuaternion& result);
		static TQuaternion Lerp(const TQuaternion& q1, const TQuaternion& q2, float t);

		static void Slerp(const TQuaternion& q1, const TQuaternion& q2, float t, TQuaternion& result);
		static TQuaternion Slerp(const TQuaternion& q1, const TQuaternion& q2, float t);

		static void Concatenate(const TQuaternion& q1, const TQuaternion& q2, TQuaternion& result);
		static TQuaternion Concatenate(const TQuaternion& q1, const TQuaternion& q2);

		// Constants
		static const TQuaternion Identity;
	};

	// Binary operators
	TQuaternion operator+ (const TQuaternion& Q1, const TQuaternion& Q2);
	TQuaternion operator- (const TQuaternion& Q1, const TQuaternion& Q2);
	TQuaternion operator* (const TQuaternion& Q1, const TQuaternion& Q2);
	TQuaternion operator* (const TQuaternion& Q, float S);
	TQuaternion operator/ (const TQuaternion& Q1, const TQuaternion& Q2);
	TQuaternion operator* (float S, const TQuaternion& Q);

	//------------------------------------------------------------------------------
	// TColor
	struct TColor : public XMFLOAT4
	{
		TColor() noexcept : XMFLOAT4(0, 0, 0, 1.f) {}
		XM_CONSTEXPR TColor(float _r, float _g, float _b) : XMFLOAT4(_r, _g, _b, 1.f) {}
		XM_CONSTEXPR TColor(float _r, float _g, float _b, float _a) : XMFLOAT4(_r, _g, _b, _a) {}
		explicit TColor(const TVector3& clr) : XMFLOAT4(clr.x, clr.y, clr.z, 1.f) {}
		explicit TColor(const TVector4& clr) : XMFLOAT4(clr.x, clr.y, clr.z, clr.w) {}
		explicit TColor(_In_reads_(4) const float *pArray) : XMFLOAT4(pArray) {}
		TColor(XMFLOAT3 V) { x = V.x; y = V.y; z = V.z; }
		TColor(const XMFLOAT4& c) { this->x = c.x; this->y = c.y; this->z = c.z; this->w = c.w; }
		explicit TColor(const XMVECTORF32& F) { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

		explicit TColor(const DirectX::PackedVector::XMCOLOR& Packed);
		// BGRA Direct3D 9 D3DCOLOR packed color

		explicit TColor(const DirectX::PackedVector::XMUBYTEN4& Packed);
		// RGBA XNA Game Studio packed color

		TColor(const TColor&) = default;
		TColor& operator=(const TColor&) = default;

		TColor(TColor&&) = default;
		TColor& operator=(TColor&&) = default;

		operator XMVECTOR() const { return XMLoadFloat4(this); }
		operator const float*() const { return reinterpret_cast<const float*>(this); }

		// Comparison operators
		bool operator == (const TColor& c) const;
		bool operator != (const TColor& c) const;

		// Assignment operators
		TColor& operator= (const XMVECTORF32& F) { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }
		TColor& operator= (const DirectX::PackedVector::XMCOLOR& Packed);
		TColor& operator= (const DirectX::PackedVector::XMUBYTEN4& Packed);
		TColor& operator+= (const TColor& c);
		TColor& operator-= (const TColor& c);
		TColor& operator*= (const TColor& c);
		TColor& operator*= (float S);
		TColor& operator/= (const TColor& c);

		// Unary operators
		TColor operator+ () const { return *this; }
		TColor operator- () const;

		// Properties
		float R() const { return x; }
		void R(float r) { x = r; }

		float G() const { return y; }
		void G(float g) { y = g; }

		float B() const { return z; }
		void B(float b) { z = b; }

		float A() const { return w; }
		void A(float a) { w = a; }

		// TColor operations
		DirectX::PackedVector::XMCOLOR BGRA() const;
		DirectX::PackedVector::XMUBYTEN4 RGBA() const;

		TVector3 ToVector3() const;
		TVector4 ToVector4() const;

		void Negate();
		void Negate(TColor& result) const;

		void Saturate();
		void Saturate(TColor& result) const;

		void Premultiply();
		void Premultiply(TColor& result) const;

		void AdjustSaturation(float sat);
		void AdjustSaturation(float sat, TColor& result) const;

		void AdjustContrast(float contrast);
		void AdjustContrast(float contrast, TColor& result) const;

		// Static functions
		static void Modulate(const TColor& c1, const TColor& c2, TColor& result);
		static TColor Modulate(const TColor& c1, const TColor& c2);

		static void Lerp(const TColor& c1, const TColor& c2, float t, TColor& result);
		static TColor Lerp(const TColor& c1, const TColor& c2, float t);
	};

	// Binary operators
	TColor operator+ (const TColor& C1, const TColor& C2);
	TColor operator- (const TColor& C1, const TColor& C2);
	TColor operator* (const TColor& C1, const TColor& C2);
	TColor operator* (const TColor& C, float S);
	TColor operator/ (const TColor& C1, const TColor& C2);
	TColor operator* (float S, const TColor& C);

	//------------------------------------------------------------------------------
	// TRay
	class TRay
	{
	public:
		TVector3 position;
		TVector3 direction;

		TRay() noexcept : position(0, 0, 0), direction(0, 0, 1) {}
		TRay(const TVector3& pos, const TVector3& dir) : position(pos), direction(dir) {}

		TRay(const TRay&) = default;
		TRay& operator=(const TRay&) = default;

		TRay(TRay&&) = default;
		TRay& operator=(TRay&&) = default;

		// Comparison operators
		bool operator == (const TRay& r) const;
		bool operator != (const TRay& r) const;

		// TRay operations
		bool Intersects(const BoundingSphere& sphere, _Out_ float& Dist) const;
		bool Intersects(const BoundingBox& box, _Out_ float& Dist) const;
		bool Intersects(const TVector3& tri0, const TVector3& tri1, const TVector3& tri2, _Out_ float& Dist) const;
		bool Intersects(const TPlane& plane, _Out_ float& Dist) const;
	};

	//------------------------------------------------------------------------------
	// TViewport
	class TViewport
	{
	public:
		float x;
		float y;
		float width;
		float height;
		float minDepth;
		float maxDepth;

		TViewport() noexcept :
			x(0.f), y(0.f), width(0.f), height(0.f), minDepth(0.f), maxDepth(1.f) {}
		XM_CONSTEXPR TViewport(float ix, float iy, float iw, float ih, float iminz = 0.f, float imaxz = 1.f) :
			x(ix), y(iy), width(iw), height(ih), minDepth(iminz), maxDepth(imaxz) {}
		explicit TViewport(const RECT& rct) :
			x(float(rct.left)), y(float(rct.top)),
			width(float(rct.right - rct.left)),
			height(float(rct.bottom - rct.top)),
			minDepth(0.f), maxDepth(1.f) {}

#if defined(__d3d11_h__) || defined(__d3d11_x_h__)
		// Direct3D 11 interop
		explicit TViewport(const D3D11_VIEWPORT& vp) :
			x(vp.TopLeftX), y(vp.TopLeftY),
			width(vp.Width), height(vp.Height),
			minDepth(vp.MinDepth), maxDepth(vp.MaxDepth) {}

		operator D3D11_VIEWPORT() { return *reinterpret_cast<const D3D11_VIEWPORT*>(this); }
		const D3D11_VIEWPORT* Get11() const { return reinterpret_cast<const D3D11_VIEWPORT*>(this); }
		TViewport& operator= (const D3D11_VIEWPORT& vp);
#endif

#if defined(__d3d12_h__) || defined(__d3d12_x_h__)
		// Direct3D 12 interop
		explicit TViewport(const D3D12_VIEWPORT& vp) :
			x(vp.TopLeftX), y(vp.TopLeftY),
			width(vp.Width), height(vp.Height),
			minDepth(vp.MinDepth), maxDepth(vp.MaxDepth) {}

		operator D3D12_VIEWPORT() { return *reinterpret_cast<const D3D12_VIEWPORT*>(this); }
		const D3D12_VIEWPORT* Get12() const { return reinterpret_cast<const D3D12_VIEWPORT*>(this); }
		TViewport& operator= (const D3D12_VIEWPORT& vp);
#endif

		TViewport(const TViewport&) = default;
		TViewport& operator=(const TViewport&) = default;

		TViewport(TViewport&&) = default;
		TViewport& operator=(TViewport&&) = default;

		// Comparison operators
		bool operator == (const TViewport& vp) const;
		bool operator != (const TViewport& vp) const;

		// Assignment operators
		TViewport& operator= (const RECT& rct);

		// TViewport operations
		float AspectRatio() const;

		TVector3 Project(const TVector3& p, const Matrix& proj, const Matrix& view, const Matrix& world) const;
		void Project(const TVector3& p, const Matrix& proj, const Matrix& view, const Matrix& world, TVector3& result) const;

		TVector3 Unproject(const TVector3& p, const Matrix& proj, const Matrix& view, const Matrix& world) const;
		void Unproject(const TVector3& p, const Matrix& proj, const Matrix& view, const Matrix& world, TVector3& result) const;

		// Static methods
		static RECT __cdecl ComputeDisplayArea(DXGI_SCALING scaling, UINT backBufferWidth, UINT backBufferHeight, int outputWidth, int outputHeight);
		static RECT __cdecl ComputeTitleSafeArea(UINT backBufferWidth, UINT backBufferHeight);
	};

	
	///////////////////////////////////////// static ///////////////////////////////////////////
	//https://docs.microsoft.com/en-us/windows/win32/dxmath/pg-xnamath-migration-d3dx
	//--------------------------
	// 2D Vector
	//--------------------------

	static float D3DXVec2Length(CONST TVector2 *pV)
	{
		return pV->Length();
	}

	static float D3DXVec2LengthSq(CONST TVector2 *pV)
	{
		return 0.0f;
	}

	static float D3DXVec2Dot(CONST TVector2 *pV1, CONST TVector2 *pV2)
	{
		return 0.0f;
	}

	// Z component of ((x1,y1,0) cross (x2,y2,0))
	static float D3DXVec2CCW(CONST TVector2 *pV1, CONST TVector2 *pV2)
	{
		return 0.0f;
	}

	static TVector2* D3DXVec2Add(TVector2 *pOut, CONST TVector2 *pV1, CONST TVector2 *pV2)
	{
		return pOut;
	}

	static TVector2* D3DXVec2Subtract(TVector2 *pOut, CONST TVector2 *pV1, CONST TVector2 *pV2)
	{
		return pOut;
	}

	// Minimize each component.  x = min(x1, x2), y = min(y1, y2)
	static TVector2* D3DXVec2Minimize(TVector2 *pOut, CONST TVector2 *pV1, CONST TVector2 *pV2)
	{
		return pOut;
	}

	// Maximize each component.  x = max(x1, x2), y = max(y1, y2)
	static TVector2* D3DXVec2Maximize(TVector2 *pOut, CONST TVector2 *pV1, CONST TVector2 *pV2)
	{
		return pOut;
	}

	static TVector2* D3DXVec2Scale(TVector2 *pOut, CONST TVector2 *pV, float s)
	{
		return pOut;
	}

	// Linear interpolation. V1 + s(V2-V1)
	static TVector2* D3DXVec2Lerp(TVector2 *pOut, CONST TVector2 *pV1, CONST TVector2 *pV2,
		float s)
	{
		*pOut = TVector2::Lerp(*pV1, *pV2, s);
		return pOut;
	}
	static TVector2* D3DXVec2Normalize(TVector2 *pOut, CONST TVector2 *pV)
	{
		return pOut;
	}

	// Hermite interpolation between position V1, tangent T1 (when s == 0)
	// and position V2, tangent T2 (when s == 1).
	static TVector2* D3DXVec2Hermite(TVector2 *pOut, CONST TVector2 *pV1, CONST TVector2 *pT1,
		CONST TVector2 *pV2, CONST TVector2 *pT2, float s)
	{
		return pOut;
	}

	// CatmullRom interpolation between V1 (when s == 0) and V2 (when s == 1)
	static TVector2* D3DXVec2CatmullRom(TVector2 *pOut, CONST TVector2 *pV0, CONST TVector2 *pV1,
		CONST TVector2 *pV2, CONST TVector2 *pV3, float s)
	{
		return pOut;
	}

	// Barycentric coordinates.  V1 + f(V2-V1) + g(V3-V1)
	static TVector2* D3DXVec2BaryCentric(TVector2 *pOut, CONST TVector2 *pV1, CONST TVector2 *pV2,
		CONST TVector2 *pV3, float f, float g)
	{
		return pOut;
	}

	// Transform (x, y, 0, 1) by matrix.
	static TVector4* D3DXVec2Transform(TVector4 *pOut, CONST TVector2 *pV, CONST Matrix *pM)
	{
		return pOut;
	}

	// Transform (x, y, 0, 1) by matrix, project result back into w=1.
	static TVector2* D3DXVec2TransformCoord(TVector2 *pOut, CONST TVector2 *pV, CONST Matrix *pM)
	{
		return pOut;
	}

	// Transform (x, y, 0, 0) by matrix.
	static TVector2* D3DXVec2TransformNormal(TVector2 *pOut, CONST TVector2 *pV, CONST Matrix *pM)
	{
		return pOut;
	}

	// Transform Array (x, y, 0, 1) by matrix.
	static TVector4* D3DXVec2TransformArray(TVector4 *pOut, UINT OutStride, CONST TVector2 *pV, UINT VStride, CONST Matrix *pM, UINT n)
	{
		return pOut;
	}

	// Transform Array (x, y, 0, 1) by matrix, project result back into w=1.
	static TVector2* D3DXVec2TransformCoordArray(TVector2 *pOut, UINT OutStride, CONST TVector2 *pV, UINT VStride, CONST Matrix *pM, UINT n)
	{
		return pOut;
	}

	// Transform Array (x, y, 0, 0) by matrix.
	static TVector2* D3DXVec2TransformNormalArray(TVector2 *pOut, UINT OutStride, CONST TVector2 *pV, UINT VStride, CONST Matrix *pM, UINT n)
	{
		return pOut;
	}
	//--------------------------
	// 3D Vector
	//--------------------------

	static  float D3DXVec3Dot(CONST TVector3 *pV1, CONST TVector3 *pV2)
	{
		return pV1->Dot(*pV2);
	}
	static TVector3* D3DXVec3Cross(TVector3 *pOut, CONST TVector3 *pV1, CONST TVector3 *pV2)
	{
		*pOut = pV1->Cross(*pV2);
		return pOut;
	}
	static TVector3* D3DXVec3Normalize(TVector3 *pOut, CONST TVector3 *pV)
	{
		pV->Normalize(*pOut);
		return pOut;
	}
	static TVector3* D3DXVec3TransformCoord(TVector3 *pOut, CONST TVector3 *pV, CONST Matrix *pM)
	{
		*pOut = TVector3::Transform(*pV, *pM);
		return pOut;
	}
	static float D3DXVec3Length(CONST TVector3 *pV)
	{
		return pV->Length();
	}



	static float D3DXVec3LengthSq(CONST TVector3 *pV)
	{
		return pV->LengthSquared();
	}


	static TVector3* D3DXVec3Add(TVector3 *pOut, CONST TVector3 *pV1, CONST TVector3 *pV2)
	{
		*pOut = *pV1 + *pV2;
		return pOut;
	}

	static TVector3* D3DXVec3Subtract(TVector3 *pOut, CONST TVector3 *pV1, CONST TVector3 *pV2)
	{
		*pOut = *pV1 - *pV2;
		return pOut;
	}

	// Minimize each component.  x = min(x1, x2), y = min(y1, y2), ...
	static TVector3* D3DXVec3Minimize(TVector3 *pOut, CONST TVector3 *pV1, CONST TVector3 *pV2) {
	}

	// Maximize each component.  x = max(x1, x2), y = max(y1, y2), ...
	static TVector3* D3DXVec3Maximize(TVector3 *pOut, CONST TVector3 *pV1, CONST TVector3 *pV2)
	{
		return pOut;
	}

	static TVector3* D3DXVec3Scale(TVector3 *pOut, CONST TVector3 *pV, float s)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(pV);
		XMVECTOR X = XMVectorScale(v1, s);
		TVector3 R;
		XMStoreFloat3(&R, X);
		*pOut = R;
		return pOut;
	}

	// Linear interpolation. V1 + s(V2-V1)
	static TVector3* D3DXVec3Lerp(TVector3 *pOut, CONST TVector3 *pV1, CONST TVector3 *pV2, float s)
	{		
		*pOut = TVector3::Lerp(*pV1, *pV2, s);
		return pOut;
	}


	// Hermite interpolation between position V1, tangent T1 (when s == 0)
	// and position V2, tangent T2 (when s == 1).
	static TVector3* D3DXVec3Hermite(TVector3 *pOut, CONST TVector3 *pV1, CONST TVector3 *pT1,
		CONST TVector3 *pV2, CONST TVector3 *pT2, float s)
	{
		*pOut = TVector3::Hermite(*pV1, *pT1, *pV2, *pT2, s);
		return pOut;
	}

	// CatmullRom interpolation between V1 (when s == 0) and V2 (when s == 1)
	static TVector3* D3DXVec3CatmullRom(TVector3 *pOut, CONST TVector3 *pV0, CONST TVector3 *pV1,
		CONST TVector3 *pV2, CONST TVector3 *pV3, float s)
	{
		*pOut = TVector3::CatmullRom(*pV0, *pV1, *pV2, *pV3, s);
		return pOut;
	}

	// Barycentric coordinates.  V1 + f(V2-V1) + g(V3-V1)
	static TVector3* D3DXVec3BaryCentric(TVector3 *pOut, CONST TVector3 *pV1, CONST TVector3 *pV2,
		CONST TVector3 *pV3, float f, float g)
	{
		*pOut = TVector3::Barycentric(*pV1, *pV2, *pV3, f, g);
		return pOut;
	}

	// Transform (x, y, z, 1) by matrix.
	static TVector4* D3DXVec3Transform(TVector4 *pOut, CONST TVector3 *pV, CONST Matrix *pM)
	{
		return pOut;
	}

	// Transform (x, y, z, 0) by matrix.  If you transforming a normal by a 
	// non-affine matrix, the matrix you pass to this function should be the 
	// transpose of the inverse of the matrix you would use to transform a coord.
	static TVector3* D3DXVec3TransformNormal(TVector3 *pOut, CONST TVector3 *pV, CONST Matrix *pM)
	{
		*pOut = TVector3::TransformNormal(*pV, *pM);
		return pOut;
	}


	// Transform Array (x, y, z, 1) by matrix. 
	static TVector4* D3DXVec3TransformArray(TVector4 *pOut, UINT OutStride, CONST TVector3 *pV, UINT VStride, CONST Matrix *pM, UINT n)
	{
		return pOut;
	}

	// Transform Array (x, y, z, 1) by matrix, project result back into w=1.
	static TVector3* D3DXVec3TransformCoordArray(TVector3 *pOut, UINT OutStride, CONST TVector3 *pV, UINT VStride, CONST Matrix *pM, UINT n)
	{
		return pOut;
	}

	// Transform (x, y, z, 0) by matrix.  If you transforming a normal by a 
	// non-affine matrix, the matrix you pass to this function should be the 
	// transpose of the inverse of the matrix you would use to transform a coord.
	static TVector3* D3DXVec3TransformNormalArray(TVector3 *pOut, UINT OutStride, CONST TVector3 *pV, UINT VStride, CONST Matrix *pM, UINT n)
	{
		return pOut;
	}

	// Project vector from object space into screen space
	static TVector3* D3DXVec3Project(TVector3 *pOut, CONST TVector3 *pV, CONST D3D10_VIEWPORT *pViewport,
		CONST Matrix *pProjection, CONST Matrix *pView, CONST Matrix *pWorld)
	{
		//*pOut = TViewport::Project();
		return pOut;
	}

	// Project vector from screen space into object space
	static TVector3* D3DXVec3Unproject(TVector3 *pOut, CONST TVector3 *pV, CONST D3D10_VIEWPORT *pViewport,
		CONST Matrix *pProjection, CONST Matrix *pView, CONST Matrix *pWorld)
	{
		//*pOut = TViewport::Unproject();
		return pOut;
	}

	// Project vector Array from object space into screen space
	static TVector3* D3DXVec3ProjectArray(TVector3 *pOut, UINT OutStride, CONST TVector3 *pV, UINT VStride, CONST D3D10_VIEWPORT *pViewport,
		CONST Matrix *pProjection, CONST Matrix *pView, CONST Matrix *pWorld, UINT n)
	{
		return pOut;
	}

	// Project vector Array from screen space into object space
	static TVector3* D3DXVec3UnprojectArray(TVector3 *pOut, UINT OutStride, CONST TVector3 *pV, UINT VStride, CONST D3D10_VIEWPORT *pViewport,
		CONST Matrix *pProjection, CONST Matrix *pView, CONST Matrix *pWorld, UINT n)
	{
		return pOut;
	}

	//--------------------------
	// 4D Vector
	//--------------------------

	static float D3DXVec4Length(CONST TVector4 *pV)
	{
		return pV->Length();
	}


	static float D3DXVec4LengthSq(CONST TVector4 *pV)
	{
		return 0.0f;
	}

	static float D3DXVec4Dot(CONST TVector4 *pV1, CONST TVector4 *pV2)
	{
		return 0.0f;
	}
	static TVector4* D3DXVec4Add(TVector4 *pOut, CONST TVector4 *pV1, CONST TVector4 *pV2)
	{
		return pOut;
	}

	static TVector4* D3DXVec4Subtract(TVector4 *pOut, CONST TVector4 *pV1, CONST TVector4 *pV2)
	{
		return pOut;
	}

	// Minimize each component.  x = min(x1, x2), y = min(y1, y2), ...
	static TVector4* D3DXVec4Minimize(TVector4 *pOut, CONST TVector4 *pV1, CONST TVector4 *pV2)
	{
		return pOut;
	}

	// Maximize each component.  x = max(x1, x2), y = max(y1, y2), ...
	static TVector4* D3DXVec4Maximize(TVector4 *pOut, CONST TVector4 *pV1, CONST TVector4 *pV2)
	{
		return pOut;
	}
	static TVector4* D3DXVec4Scale(TVector4 *pOut, CONST TVector4 *pV, float s) {
		return pOut;
	}

	// Linear interpolation. V1 + s(V2-V1)
	static TVector4* D3DXVec4Lerp(TVector4 *pOut, CONST TVector4 *pV1, CONST TVector4 *pV2, float s)
	{
		*pOut = TVector4::Lerp(*pV1, *pV2, s);
		return pOut;
	}

	// Cross-product in 4 dimensions.
	static TVector4* D3DXVec4Cross(TVector4 *pOut, CONST TVector4 *pV1, CONST TVector4 *pV2,
		CONST TVector4 *pV3)
	{
		return pOut;
	}

	static TVector4* D3DXVec4Normalize(TVector4 *pOut, CONST TVector4 *pV)
	{
		return pOut;
	}

	// Hermite interpolation between position V1, tangent T1 (when s == 0)
	// and position V2, tangent T2 (when s == 1).
	static TVector4* D3DXVec4Hermite(TVector4 *pOut, CONST TVector4 *pV1, CONST TVector4 *pT1,
		CONST TVector4 *pV2, CONST TVector4 *pT2, float s)
	{
		return pOut;
	}

	// CatmullRom interpolation between V1 (when s == 0) and V2 (when s == 1)
	static TVector4* D3DXVec4CatmullRom(TVector4 *pOut, CONST TVector4 *pV0, CONST TVector4 *pV1,
		CONST TVector4 *pV2, CONST TVector4 *pV3, float s)
	{
		return pOut;
	}

	// Barycentric coordinates.  V1 + f(V2-V1) + g(V3-V1)
	static TVector4* D3DXVec4BaryCentric(TVector4 *pOut, CONST TVector4 *pV1, CONST TVector4 *pV2,
		CONST TVector4 *pV3, float f, float g)
	{
		return pOut;
	}

	// Transform vector by matrix.
	static TVector4* D3DXVec4Transform(TVector4 *pOut, CONST TVector4 *pV, CONST Matrix *pM)
	{
		return pOut;
	}

	// Transform vector array by matrix.
	static TVector4* D3DXVec4TransformArray(TVector4 *pOut, UINT OutStride, CONST TVector4 *pV, UINT VStride, CONST Matrix *pM, UINT n)
	{
		return pOut;
	}


	static Matrix* D3DXMatrixIdentity(Matrix *pOut)
	{
		XMStoreFloat4x4(pOut, XMMatrixIdentity());
		return pOut;
	}
	static Matrix* D3DXMatrixRotationQuaternion(Matrix *pOut, CONST TQuaternion *pQ)
	{
		*pOut = Matrix::CreateFromQuaternion(*pQ);
		return pOut;
	}
	static TQuaternion* D3DXQuaternionRotationMatrix(TQuaternion *pOut, CONST Matrix *pM)
	{
		*pOut = TQuaternion::CreateFromRotationMatrix(*pM);
		return pOut;
	}

	static float D3DXMatrixDeterminant(CONST Matrix *pM) {
		return pM->Determinant();
	}

	static HRESULT D3DXMatrixDecompose(TVector3 *pOutScale, TQuaternion *pOutRotation,
		TVector3 *pOutTranslation, Matrix *pM)
	{
		if (pM->Decompose(*pOutScale, *pOutRotation, *pOutTranslation) == false)
		{
			return S_FALSE;
		}
		return S_OK;
	}

	static Matrix* D3DXMatrixTranspose(Matrix *pOut, CONST Matrix *pM) {

		*pOut = pM->Transpose();
		return pOut;
	}

	// Matrix multiplication.  The result represents the transformation M2
	// followed by the transformation M1.  (Out = M1 * M2)
	static Matrix*  D3DXMatrixMultiply(Matrix *pOut, CONST Matrix *pM1, CONST Matrix *pM2) {
		*pOut = (*pM1) * (*pM2);
		return pOut;
	}

	// Matrix multiplication, followed by a transpose. (Out = T(M1 * M2))
	static Matrix*  D3DXMatrixMultiplyTranspose(Matrix *pOut, CONST Matrix *pM1, CONST Matrix *pM2) {

		*pOut = (*pM1) * (*pM2);
		pOut->Transpose();
		return pOut;
	}

	// Calculate inverse of matrix.  Inversion my fail, in which case NULL will
	// be returned.  The determinant of pM is also returned it pfDeterminant
	// is non-NULL.
	static Matrix*  D3DXMatrixInverse(Matrix *pOut, float *pDeterminant, CONST Matrix *pM) {
		pM->Invert(*pOut);
		if (pDeterminant != nullptr)
		{
			*pDeterminant = pM->Determinant();
		}
		return pOut;
	}

	// Build a matrix which scales by (sx, sy, sz)
	static Matrix*  D3DXMatrixScaling(Matrix *pOut, float sx, float sy, float sz) {
		*pOut = Matrix::CreateScale(sx, sy, sz);
		return pOut;
	}

	// Build a matrix which translates by (x, y, z)
	static Matrix*  D3DXMatrixTranslation(Matrix *pOut, float x, float y, float z) {
		*pOut = Matrix::CreateTranslation(x, y, z);
		return pOut;
	}

	// Build a matrix which rotates around the X axis
	static Matrix*  D3DXMatrixRotationX(Matrix *pOut, float Angle) {
		*pOut = Matrix::CreateRotationX(Angle);
		return pOut;
	}

	// Build a matrix which rotates around the Y axis
	static Matrix*  D3DXMatrixRotationY(Matrix *pOut, float Angle) {
		*pOut = Matrix::CreateRotationY(Angle);
		return pOut;
	}

	// Build a matrix which rotates around the Z axis
	static Matrix*  D3DXMatrixRotationZ(Matrix *pOut, float Angle) {
		*pOut = Matrix::CreateRotationZ(Angle);
		return pOut;
	}

	// Build a matrix which rotates around an arbitrary axis
	static Matrix*  D3DXMatrixRotationAxis(Matrix *pOut, CONST TVector3 *pV, float Angle) {
		*pOut = Matrix::CreateFromAxisAngle(*pV, Angle);
		return pOut;
	}

	// Yaw around the Y axis, a pitch around the X axis,
	// and a roll around the Z axis.
	static Matrix*  D3DXMatrixRotationYawPitchRoll(Matrix *pOut, float Yaw, float Pitch, float Roll) {
		*pOut = Matrix::CreateFromYawPitchRoll(Yaw, Pitch, Roll);
		return pOut;
	}

	// Build transformation matrix.  NULL arguments are treated as identity.
	// Mout = Msc-1 * Msr-1 * Ms * Msr * Msc * Mrc-1 * Mr * Mrc * Mt
	static Matrix*  D3DXMatrixTransformation(Matrix *pOut, CONST TVector3 *pScalingCenter,
		CONST TQuaternion *pScalingRotation, CONST TVector3 *pScaling,
		CONST TVector3 *pRotationCenter, CONST TQuaternion *pRotation,
		CONST TVector3 *pTranslation)
	{
		return pOut;
	}

	// Build 2D transformation matrix in XY plane.  NULL arguments are treated as identity.
	// Mout = Msc-1 * Msr-1 * Ms * Msr * Msc * Mrc-1 * Mr * Mrc * Mt
	static Matrix*  D3DXMatrixTransformation2D(Matrix *pOut, CONST TVector2* pScalingCenter,
		float ScalingRotation, CONST TVector2* pScaling,
		CONST TVector2* pRotationCenter, float Rotation,
		CONST TVector2* pTranslation) {
		return pOut;
	}

	// Build affine transformation matrix.  NULL arguments are treated as identity.
	// Mout = Ms * Mrc-1 * Mr * Mrc * Mt
	static Matrix*  D3DXMatrixAffineTransformation(Matrix *pOut, float Scaling, CONST TVector3 *pRotationCenter,
		CONST TQuaternion *pRotation, CONST TVector3 *pTranslation) 
	{
		XMVECTOR S = XMVectorReplicate(Scaling);//XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMVECTOR O = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		if (pRotationCenter != NULL)
		{
			O = DirectX::XMLoadFloat3(pRotationCenter);
		}		
		XMVECTOR P = DirectX::XMLoadFloat3(pTranslation);
		XMVECTOR Q = DirectX::XMLoadFloat4(pRotation);
		*pOut = DirectX::XMMatrixAffineTransformation(S, O, Q, P);
		return pOut;
	}

	// Build 2D affine transformation matrix in XY plane.  NULL arguments are treated as identity.
	// Mout = Ms * Mrc-1 * Mr * Mrc * Mt
	static Matrix*  D3DXMatrixAffineTransformation2D(Matrix *pOut, float Scaling, CONST TVector2* pRotationCenter,
		float Rotation, CONST TVector2* pTranslation) {
		return pOut;
	}

	// Build a lookat matrix. (right-handed)
	static Matrix*  D3DXMatrixLookAtRH(Matrix *pOut, CONST TVector3 *pEye, CONST TVector3 *pAt, CONST TVector3 *pUp) {
		return pOut;
	}

	// Build a lookat matrix. (left-handed)
	static Matrix*  D3DXMatrixLookAtLH(Matrix *pOut, CONST TVector3 *pEye, CONST TVector3 *pAt, CONST TVector3 *pUp) {
		*pOut = Matrix::CreateLookAt(*pEye, *pAt, *pUp);
		return pOut;
	}

	// Build a perspective projection matrix. (right-handed)
	static Matrix*  D3DXMatrixPerspectiveRH(Matrix *pOut, float w, float h, float zn, float zf) {
		return pOut;
	}

	// Build a perspective projection matrix. (left-handed)
	static Matrix*  D3DXMatrixPerspectiveLH(Matrix *pOut, float w, float h, float zn, float zf) {
		*pOut = Matrix::CreatePerspective(w, h, zn, zf);
		return pOut;
	}

	// Build a perspective projection matrix. (right-handed)
	static Matrix*  D3DXMatrixPerspectiveFovRH(Matrix *pOut, float fovy, float Aspect, float zn, float zf) 
	{
		using namespace DirectX;		
		XMStoreFloat4x4(pOut, XMMatrixPerspectiveFovRH(fovy, Aspect, zn, zf));
		return pOut;
	}

	// Build a perspective projection matrix. (left-handed)
	static Matrix*  D3DXMatrixPerspectiveFovLH(Matrix *pOut, float fovy, float Aspect, float zn, float zf) 
	{
		using namespace DirectX;
		*pOut = Matrix::CreatePerspectiveFieldOfView(fovy, Aspect, zn, zf);
		return pOut;
	}

	// Build a perspective projection matrix. (right-handed)
	static Matrix*  D3DXMatrixPerspectiveOffCenterRH(Matrix *pOut, float l, float r, float b, float t, float zn, float zf) {		
		return pOut;
	}

	// Build a perspective projection matrix. (left-handed)
	static Matrix*  D3DXMatrixPerspectiveOffCenterLH(Matrix *pOut, float l, float r, float b, float t, float zn, float zf) {
		*pOut = Matrix::CreatePerspectiveOffCenter(l, r, b, t, zn, zf);
		return pOut;
	}

	// Build an ortho projection matrix. (right-handed)
	static Matrix*  D3DXMatrixOrthoRH(Matrix *pOut, float w, float h, float zn, float zf) {
		return pOut;
	}

	// Build an ortho projection matrix. (left-handed)
	static Matrix*  D3DXMatrixOrthoLH(Matrix *pOut, float w, float h, float zn, float zf) {
		*pOut = Matrix::CreateOrthographic(w, h, zn, zf);
		return pOut;
	}

	// Build an ortho projection matrix. (right-handed)
	static Matrix*  D3DXMatrixOrthoOffCenterRH(Matrix *pOut, float l, float r, float b, float t, float zn, float zf) {
		return pOut;
	}

	// Build an ortho projection matrix. (left-handed)
	static Matrix*  D3DXMatrixOrthoOffCenterLH(Matrix *pOut, float l, float r, float b, float t, float zn, float zf) {
		*pOut = Matrix::CreateOrthographicOffCenter(l, r, b, t, zn, zf);
		return pOut;
	}

	// Build a matrix which flattens geometry into a plane, as if casting
	// a shadow from a light.
	static Matrix*  D3DXMatrixShadow(Matrix *pOut, CONST TVector4 *pLight, CONST TPlane *pPlane) 
	{
		CONST TVector3 pLightLight = TVector3(pLight->x, pLight->y, pLight->z);
		*pOut = Matrix::CreateShadow(pLightLight, *pPlane);
		return pOut;
	}

	// Build a matrix which reflects the coordinate system about a plane
	static Matrix*  D3DXMatrixReflect(Matrix *pOut, CONST TPlane *pPlane) {
		*pOut = Matrix::CreateReflection(*pPlane);
		return pOut;
	}

	//--------------------------
	// Quaternion
	//--------------------------

	// inline

	static float D3DXQuaternionLength(CONST TQuaternion *pQ)
	{
		return pQ->Length();
	};

	// Length squared, or "norm"
	static float D3DXQuaternionLengthSq(CONST TQuaternion *pQ) 
	{
		return pQ->LengthSquared();
	};

	static float D3DXQuaternionDot(CONST TQuaternion *pQ1, CONST TQuaternion *pQ2) 
	{		
		return pQ1->Dot(*pQ2);;
	};

	// (0, 0, 0, 1)
	static TQuaternion* D3DXQuaternionIdentity(TQuaternion *pOut) {
		//*pOut = TQuaternion::Identity;
		XMStoreFloat4(pOut, XMQuaternionIdentity());
		return pOut;
	};

	static BOOL D3DXQuaternionIsIdentity(CONST TQuaternion *pQ) {
		return TRUE;
	};

	// (-x, -y, -z, w)
	static TQuaternion* D3DXQuaternionConjugate(TQuaternion *pOut, CONST TQuaternion *pQ) {
		return pOut;
	};


	// Compute a quaternin's axis and angle of rotation. Expects unit quaternions.
	static void D3DXQuaternionToAxisAngle(CONST TQuaternion *pQ, TVector3 *pAxis, float *pAngle) 
	{
		
	};

	static TQuaternion* D3DXQuaternionRotationAxis(TQuaternion *pOut, CONST TVector3 *pV, FLOAT Angle)
	{
		*pOut = TQuaternion::CreateFromAxisAngle(*pV, Angle);
		return pOut;
	}

	// Yaw around the Y axis, a pitch around the X axis,
	// and a roll around the Z axis.
	static TQuaternion* D3DXQuaternionRotationYawPitchRoll(TQuaternion *pOut, float Yaw, float Pitch, float Roll) 
	{
		*pOut = TQuaternion::CreateFromYawPitchRoll(Yaw,Pitch,Roll);
		return pOut;
	};

	// Quaternion multiplication.  The result represents the rotation Q2
	// followed by the rotation Q1.  (Out = Q2 * Q1)
	static TQuaternion* D3DXQuaternionMultiply(TQuaternion *pOut, CONST TQuaternion *pQ1,
		CONST TQuaternion *pQ2) 
	{
		*pOut = *pQ1 * *pQ2;
		return pOut;
	};

	static TQuaternion* D3DXQuaternionNormalize(TQuaternion *pOut, CONST TQuaternion *pQ)
	{
		*pOut = *pQ;
		pOut->Normalize();
		return pOut;
	};

	// Conjugate and re-norm
	static TQuaternion* D3DXQuaternionInverse(TQuaternion *pOut, CONST TQuaternion *pQ) 
	{
		TQuaternion qRet = *pQ;
		pQ->Inverse(qRet);
		*pOut = qRet;
		return pOut;
	};

	// Expects unit quaternions.
	// if q = (cos(theta), sin(theta) * v); ln(q) = (0, theta * v)
	static TQuaternion* D3DXQuaternionLn(TQuaternion *pOut, CONST TQuaternion *pQ) {
		return pOut;
	};

	// Expects pure quaternions. (w == 0)  w is ignored in calculation.
	// if q = (0, theta * v); exp(q) = (cos(theta), sin(theta) * v)
	static TQuaternion* D3DXQuaternionExp(TQuaternion *pOut, CONST TQuaternion *pQ) {
		return pOut;
	};

	// Spherical linear interpolation between Q1 (t == 0) and Q2 (t == 1).
	// Expects unit quaternions.
	static TQuaternion* D3DXQuaternionSlerp(TQuaternion *pOut, CONST TQuaternion *pQ1,	CONST TQuaternion *pQ2, float t) 
	{
		*pOut = TQuaternion::Slerp(*pQ1, *pQ2, t);
		return pOut;
	};

	// Spherical quadrangle interpolation.
	// Slerp(Slerp(Q1, C, t), Slerp(A, B, t), 2t(1-t))
	static TQuaternion* D3DXQuaternionSquad(TQuaternion *pOut, CONST TQuaternion *pQ1,
		CONST TQuaternion *pA, CONST TQuaternion *pB,
		CONST TQuaternion *pC, float t) {
		return pOut;
	};

	// Setup control points for spherical quadrangle interpolation
	// from Q1 to Q2.  The control points are chosen in such a way 
	// to ensure the continuity of tangents with adjacent segments.
	static void D3DXQuaternionSquadSetup(TQuaternion *pAOut, TQuaternion *pBOut, TQuaternion *pCOut,
		CONST TQuaternion *pQ0, CONST TQuaternion *pQ1,
		CONST TQuaternion *pQ2, CONST TQuaternion *pQ3) {
	};

	// Barycentric interpolation.
	// Slerp(Slerp(Q1, Q2, f+g), Slerp(Q1, Q3, f+g), g/(f+g))
	static TQuaternion* D3DXQuaternionBaryCentric(TQuaternion *pOut, CONST TQuaternion *pQ1,
		CONST TQuaternion *pQ2, CONST TQuaternion *pQ3,
		float f, float g) {
		return pOut;
	};


	//--------------------------
	// Plane
	//--------------------------

	// inline

	// ax + by + cz + dw
	static float D3DXPlaneDot(CONST TPlane *pP, CONST TVector4 *pV) {
		return 0.0f;
	};
	// ax + by + cz + d
	static float D3DXPlaneDotCoord(CONST TPlane *pP, CONST TVector3 *pV) {
		return 0.0f;
	};
	// ax + by + cz
	static float D3DXPlaneDotNormal(CONST TPlane *pP, CONST TVector3 *pV) {
		return 0.0f;
	};
	static TPlane* D3DXPlaneScale(TPlane *pOut, CONST TPlane *pP, float s) {
		return pOut;
	};


	// Normalize plane (so that |a,b,c| == 1)
	static TPlane* D3DXPlaneNormalize(TPlane *pOut, CONST TPlane *pP) {
		return pOut;
	};

	// Find the intersection between a plane and a line.  If the line is
	// parallel to the plane, NULL is returned.
	static TVector3* D3DXPlaneIntersectLine(TVector3 *pOut, CONST TPlane *pP, CONST TVector3 *pV1,
		CONST TVector3 *pV2) {
		return pOut;
	};

	// Construct a plane from a point and a normal
	static TPlane* D3DXPlaneFromPointNormal(TPlane *pOut, CONST TVector3 *pPoint, CONST TVector3 *pNormal) {
		return pOut;
	};

	// Construct a plane from 3 points
	static TPlane* D3DXPlaneFromPoints(TPlane *pOut, CONST TVector3 *pV1, CONST TVector3 *pV2,
		CONST TVector3 *pV3) {
		return pOut;
	};

	// Transform a plane by a matrix.  The vector (a,b,c) must be normal.
	// M should be the inverse transpose of the transformation desired.
	static TPlane* D3DXPlaneTransform(TPlane *pOut, CONST TPlane *pP, CONST Matrix *pM) {
		return pOut;
	};
	// Transform an array of planes by a matrix.  The vectors (a,b,c) must be normal.
// M should be the inverse transpose of the transformation desired.
	static TPlane* D3DXPlaneTransformArray(TPlane *pOut, UINT OutStride, CONST TPlane *pP, UINT PStride, CONST Matrix *pM, UINT n) {
		return pOut;
	};



	//--------------------------
	// Color
	//--------------------------

	// inline

	// (1-r, 1-g, 1-b, a)
	static TColor* D3DXColorNegative(TColor *pOut, CONST TColor *pC) {
		return pOut;
	};

	static TColor* D3DXColorAdd(TColor *pOut, CONST TColor *pC1, CONST TColor *pC2) {
		return pOut;
	};

	static TColor* D3DXColorSubtract(TColor *pOut, CONST TColor *pC1, CONST TColor *pC2) {
		return pOut;
	};

	static TColor* D3DXColorScale(TColor *pOut, CONST TColor *pC, float s) {
		return pOut;
	};

	// (r1*r2, g1*g2, b1*b2, a1*a2)
	static TColor* D3DXColorModulate(TColor *pOut, CONST TColor *pC1, CONST TColor *pC2) {
		return pOut;
	};

	// Linear interpolation of r,g,b, and a. C1 + s(C2-C1)
	static TColor* D3DXColorLerp(TColor *pOut, CONST TColor *pC1, CONST TColor *pC2, float s) {
		return pOut;
	};


	// Interpolate r,g,b between desaturated color and color.
	// DesaturatedColor + s(Color - DesaturatedColor)
	static TColor* D3DXColorAdjustSaturation(TColor *pOut, CONST TColor *pC, float s) {
		return pOut;
	};
	// Interpolate r,g,b between 50% grey and color.  Grey + s(Color - Grey)
	static TColor* D3DXColorAdjustContrast(TColor *pOut, CONST TColor *pC, float c) {
		return pOut;
	};





	//--------------------------
	// Misc
	//--------------------------
		// Calculate Fresnel term given the cosine of theta (likely obtained by
		// taking the dot of two normals), and the refraction index of the material.
	static float D3DXFresnelTerm(float CosTheta, float RefractionIndex)
	{
		return 0.0f;
	};
}

//  ------------------------------------------------------------------------------
// Support for TMath and Standard C++ Library containers
namespace std
{
	template<> struct less<T::TRectangle>
	{
		bool operator()(const T::TRectangle& r1, const T::TRectangle& r2) const
		{
			return ((r1.x < r2.x)
				|| ((r1.x == r2.x) && (r1.y < r2.y))
				|| ((r1.x == r2.x) && (r1.y == r2.y) && (r1.width < r2.width))
				|| ((r1.x == r2.x) && (r1.y == r2.y) && (r1.width == r2.width) && (r1.height < r2.height)));
		}
	};

	template<> struct less<T::TVector2>
	{
		bool operator()(const T::TVector2& V1, const T::TVector2& V2) const
		{
			return ((V1.x < V2.x) || ((V1.x == V2.x) && (V1.y < V2.y)));
		}
	};

	template<> struct less<T::TVector3>
	{
		bool operator()(const T::TVector3& V1, const T::TVector3& V2) const
		{
			return ((V1.x < V2.x)
				|| ((V1.x == V2.x) && (V1.y < V2.y))
				|| ((V1.x == V2.x) && (V1.y == V2.y) && (V1.z < V2.z)));
		}
	};

	template<> struct less<T::TVector4>
	{
		bool operator()(const T::TVector4& V1, const T::TVector4& V2) const
		{
			return ((V1.x < V2.x)
				|| ((V1.x == V2.x) && (V1.y < V2.y))
				|| ((V1.x == V2.x) && (V1.y == V2.y) && (V1.z < V2.z))
				|| ((V1.x == V2.x) && (V1.y == V2.y) && (V1.z == V2.z) && (V1.w < V2.w)));
		}
	};

	template<> struct less<T::Matrix>
	{
		bool operator()(const T::Matrix& M1, const T::Matrix& M2) const
		{
			if (M1._11 != M2._11) return M1._11 < M2._11;
			if (M1._12 != M2._12) return M1._12 < M2._12;
			if (M1._13 != M2._13) return M1._13 < M2._13;
			if (M1._14 != M2._14) return M1._14 < M2._14;
			if (M1._21 != M2._21) return M1._21 < M2._21;
			if (M1._22 != M2._22) return M1._22 < M2._22;
			if (M1._23 != M2._23) return M1._23 < M2._23;
			if (M1._24 != M2._24) return M1._24 < M2._24;
			if (M1._31 != M2._31) return M1._31 < M2._31;
			if (M1._32 != M2._32) return M1._32 < M2._32;
			if (M1._33 != M2._33) return M1._33 < M2._33;
			if (M1._34 != M2._34) return M1._34 < M2._34;
			if (M1._41 != M2._41) return M1._41 < M2._41;
			if (M1._42 != M2._42) return M1._42 < M2._42;
			if (M1._43 != M2._43) return M1._43 < M2._43;
			if (M1._44 != M2._44) return M1._44 < M2._44;

			return false;
		}
	};

	template<> struct less<T::TPlane>
	{
		bool operator()(const T::TPlane& P1, const T::TPlane& P2) const
		{
			return ((P1.x < P2.x)
				|| ((P1.x == P2.x) && (P1.y < P2.y))
				|| ((P1.x == P2.x) && (P1.y == P2.y) && (P1.z < P2.z))
				|| ((P1.x == P2.x) && (P1.y == P2.y) && (P1.z == P2.z) && (P1.w < P2.w)));
		}
	};

	template<> struct less<T::TQuaternion>
	{
		bool operator()(const T::TQuaternion& Q1, const T::TQuaternion& Q2) const
		{
			return ((Q1.x < Q2.x)
				|| ((Q1.x == Q2.x) && (Q1.y < Q2.y))
				|| ((Q1.x == Q2.x) && (Q1.y == Q2.y) && (Q1.z < Q2.z))
				|| ((Q1.x == Q2.x) && (Q1.y == Q2.y) && (Q1.z == Q2.z) && (Q1.w < Q2.w)));
		}
	};

	template<> struct less<T::TColor>
	{
		bool operator()(const T::TColor& C1, const T::TColor& C2) const
		{
			return ((C1.x < C2.x)
				|| ((C1.x == C2.x) && (C1.y < C2.y))
				|| ((C1.x == C2.x) && (C1.y == C2.y) && (C1.z < C2.z))
				|| ((C1.x == C2.x) && (C1.y == C2.y) && (C1.z == C2.z) && (C1.w < C2.w)));
		}
	};

	template<> struct less<T::TRay>
	{
		bool operator()(const T::TRay& R1, const T::TRay& R2) const
		{
			if (R1.position.x != R2.position.x) return R1.position.x < R2.position.x;
			if (R1.position.y != R2.position.y) return R1.position.y < R2.position.y;
			if (R1.position.z != R2.position.z) return R1.position.z < R2.position.z;

			if (R1.direction.x != R2.direction.x) return R1.direction.x < R2.direction.x;
			if (R1.direction.y != R2.direction.y) return R1.direction.y < R2.direction.y;
			if (R1.direction.z != R2.direction.z) return R1.direction.z < R2.direction.z;

			return false;
		}
	};

	template<> struct less<T::TViewport>
	{
		bool operator()(const T::TViewport& vp1, const T::TViewport& vp2) const
		{
			if (vp1.x != vp2.x) return (vp1.x < vp2.x);
			if (vp1.y != vp2.y) return (vp1.y < vp2.y);

			if (vp1.width != vp2.width) return (vp1.width < vp2.width);
			if (vp1.height != vp2.height) return (vp1.height < vp2.height);

			if (vp1.minDepth != vp2.minDepth) return (vp1.minDepth < vp2.minDepth);
			if (vp1.maxDepth != vp2.maxDepth) return (vp1.maxDepth < vp2.maxDepth);

			return false;
		}
	};


};
//using namespace T;
