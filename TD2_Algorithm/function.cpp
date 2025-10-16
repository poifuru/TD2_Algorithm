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
Vector2<float> operator*(const Vector2<float>& v1, const Vector2<float>& v2) {
	return { v1.x * v2.x, v1.y * v2.y };
}
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

Vector2<float> Reflect (const Vector2<float>& input, Vector2<float>& normal) {
	Vector2<float> reflect;

	normal = Normalize (normal);

	float dot = Dot (input, normal);

	reflect = input - normal * (2.0f * dot);
	return reflect;
}

CollisionResult isCollision (const Vector2<float>& pos, const Vector2<float>& radius, const Segment& segment) {
	CollisionResult result;
	//ぷれいやーと地面の最近接点を求める
	result.closest = ClosestPoint (pos, segment);

	//プレイヤーから最近接点までのベクトルを出す
	Vector2<float> diff = Subtract (pos, result.closest);
	float distance = Length (diff);

	//距離よりプレイヤーの位置+半径が小さいか
	if (distance < radius.x) {
		result.isColliding = true;
	}
	else {
		result.isColliding = false;
	}

	//線分の方向ベクトルから法線を作る
	Vector2<float> ab = Subtract (segment.diff, segment.origin);
	//求めたベクトルを回転させて法線にする
	result.normal = Normalize ({ -ab.y, ab.x });
	//内積をチェックして向きを調整
	if (Dot (result.normal, diff) < 0.0f) {
		result.normal = Multiply (-1.0f, result.normal);
	}

	//押し戻す量を求める(半径-距離)
	result.penetration = radius.x - distance;

	return result;
}


