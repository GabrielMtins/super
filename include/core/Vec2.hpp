#ifndef VEC2_HPP
#define VEC2_HPP

#include <cmath>

struct Vec2 {
	float x, y;

	Vec2() : x(0.0f), y(0.0f) {}
	Vec2(float x, float y): x(x), y(y) {}

	inline Vec2 operator+(const Vec2& other) const {
		return Vec2(x + other.x, y + other.y);
	}

	inline Vec2 operator-(const Vec2& other) const {
		return Vec2(x - other.x, y - other.y);
	}

	inline Vec2 operator*(const Vec2& other) const {
		return Vec2(x * other.x, y * other.y);
	}

	inline Vec2 operator*(float val) const {
		return Vec2(x * val, y * val);
	}

	inline Vec2 operator/(float val) const {
		return Vec2(x / val, y / val);
	}

	inline Vec2& operator+=(const Vec2& other) {
		x += other.x;
		y += other.y;

		return *this;
	}

	inline Vec2& operator-=(const Vec2& other) {
		x -= other.x;
		y -= other.y;

		return *this;
	}

	inline Vec2& operator*=(const Vec2& other) {
		x *= other.x;
		y *= other.y;

		return *this;
	}

	inline Vec2& operator*=(float val) {
		x *= val;
		y *= val;

		return *this;
	}

	inline Vec2& operator/=(float val) {
		x /= val;
		y /= val;

		return *this;
	}

	inline float lengthSqr(void) const {
		return x * x + y * y;
	}

	inline float length(void) const {
		return sqrtf(x * x + y * y);
	}

	inline Vec2 normalized(void) const {
		return *this / length();
	}

	inline float dot(const Vec2 &other) const {
		return x * other.x + y * other.y;
	}

	static const Vec2 zero;
};

#endif
