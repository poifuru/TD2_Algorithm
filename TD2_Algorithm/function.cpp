#include "function.h"

Vector2<float> Add (const Vector2<float>& v1, const Vector2<float>& v2) {
	return {
		v1.x + v2.x,
		v1.y + v2.y,
	};
}

Vector2<float> Subtract (const Vector2<float>& v1, const Vector2<float>& v2) {
	return {
		v1.x - v2.x,
		v1.y - v2.y,
	};
}

Vector2<float> Multiply (float scalar, const Vector2<float>& v) {
	return {
		scalar * v.x,
		scalar * v.y,
	};
}

float Dot (const Vector2<float>& v1, const Vector2<float>& v2) {
	return {
		(v1.x * v2.x) + (v1.y * v2.y)
	};
}

float Length (const Vector2<float>& v) {
	return {
		sqrtf (powf (v.x, 2) + powf (v.y, 2))
	};
}

Vector2<float> Normalize (const Vector2<float>& v) {
	float nor = sqrtf (powf (v.x, 2) + powf (v.y, 2));

	return {
		v.x / nor,
		v.y / nor,
	};
}

//演算子オーバーロード
Vector2<float> operator+(const Vector2<float>& v1, const Vector2<float>& v2) { return Add (v1, v2); }
Vector2<float> operator-(const Vector2<float>& v1, const Vector2<float>& v2) { return Subtract (v1, v2); }
Vector2<float> operator*(float s, const Vector2<float>& v) { return Multiply (s, v); }
Vector2<float> operator*(const Vector2<float>& v, float s) { return s * v; }
Vector2<float> operator/(const Vector2<float>& v, float s) { return Multiply (1.0f / s, v); }
/*単項演算子*/
Vector2<float> operator+(const Vector2<float>& v) { return { -v.x, -v.y, }; }
Vector2<float> operator-(const Vector2<float>& v) { return v; }

Vector2<float> Project (const Vector2<float>& v1, const Vector2<float>& v2) {
	Vector2<float> result{};

	float dot = Dot (v1, v2);
	float length = Length (v2);

	if (length != 0.0f) {
		result.x = (dot / powf (length, 2)) * v2.x;
		result.y = (dot / powf (length, 2)) * v2.y;
	};

	return result;
}

Vector2<float> ClosestPoint (const Vector2<float>& point, const Segment& segment) {
	Vector2<float> ab = Subtract (segment.diff, segment.origin);
	Vector2<float> ap = Subtract (point, segment.origin);

	float t = Dot (ap, ab) / (powf (ab.x, 2) + powf (ab.y, 2));

	if (t < 0) {
		return segment.origin;
	}
	if (t > 1) {
		return segment.diff;
	}
	else {
		Vector2<float> projection = Add (segment.origin, Multiply (t, ab));
		return projection;
	}
}