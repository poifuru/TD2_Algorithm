#pragma once
#include "struct.h"
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

//加算
Vector2<float> Add (const Vector2<float>& v1, const Vector2<float>& v2);

//減算
Vector2<float> Subtract (const Vector2<float>& v1, const Vector2<float>& v2);

//乗算
Vector2<float> Multiply (float scalar, const Vector2<float>& v);

//内積
float Dot (const Vector2<float>& v1, const Vector2<float>& v2);

//長さ(ノルム)
float Length (const Vector2<float>& v);

//正規化
Vector2<float> Normalize (const Vector2<float>& v);

//演算子オーバーロード
Vector2<float> operator+(const Vector2<float>& v1, const Vector2<float>& v2);
Vector2<float> operator-(const Vector2<float>& v1, const Vector2<float>& v2);
Vector2<float> operator*(float s, const Vector2<float>& v);
Vector2<float> operator*(const Vector2<float>& v, float s);
Vector2<float> operator*(const Vector2<float>& v1, const Vector2<float>& v2);
Vector2<float> operator/(const Vector2<float>& v, float s);
/*単項演算子*/
Vector2<float> operator+(const Vector2<float>& v);
Vector2<float> operator-(const Vector2<float>& v);

//正射影ベクトルを求める関数
Vector2<float> Project (const Vector2<float>& v1, const Vector2<float>& v2);

//最近接点を求める関数
Vector2<float> ClosestPoint (const Vector2<float>& point, const Segment& segment);

//反射ベクトルを求める関数
Vector2<float> Reflect (const Vector2<float>& input, Vector2<float>& normal);

//当たり判定の関数群
bool isCollision (const Vector2<float>& pos, const Vector2<float>& radius, const Segment& segment);