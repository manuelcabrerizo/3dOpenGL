#ifndef MATH_H
#define MATH_H

#define PI 3.14159265359

struct Vec2
{
    float x, y;
};

struct Vec3
{
    float x, y, z;
};

struct Vec4
{
    float x, y, z, w;
};

struct Matrix
{
    float m[4][4];
};

float to_radiant(float v);

Vec3 vec3_from_vec4(Vec4 v);
Vec4 vec4_from_vec3(Vec3 v);

Vec2 operator+(const Vec2& v0, const Vec2& v1);
Vec2 operator-(const Vec2& v0, const Vec2& v1);
Vec2 operator+(const Vec2& v0, const float& s);
Vec2 operator-(const Vec2& v0, const float& s);
Vec2 operator*(const Vec2& v0, const float& s);
Vec2 operator/(const Vec2& v0, const float& s);
float vec2_length(Vec2 v);
float vec2_dot(Vec2 v0, Vec2 v1);

Vec3 operator+(const Vec3& v0, const Vec3& v1);
Vec3 operator-(const Vec3& v0, const Vec3& v1);
Vec3 operator-(const Vec3& v);
Vec3 operator+(const Vec3& v, const float& s);
Vec3 operator-(const Vec3& v, const float& s);
Vec3 operator*(const Vec3& v, const float& s);
Vec3 operator/(const Vec3& v, const float& s);
void operator+=(Vec3& v0, const Vec3& v1);
float vec3_length(Vec3 v);
float vec3_dot(Vec3 v0, Vec3 v1);
Vec3 vec3_cross(Vec3 a, Vec3 b);
Vec3 normaliza_vec3(Vec3 v);

Matrix get_identity_matrix();
Matrix matrix_transpose(Matrix m);
Matrix get_scale_matrix(Vec3 v);
Matrix get_translation_matrix(Vec3 v);
Matrix get_rotation_x_matrix(float angle);
Matrix get_rotation_y_matrix(float angle);
Matrix get_rotation_z_matrix(float angle);
Matrix get_view_matrix(Vec3 eye, Vec3 target, Vec3 up);
Matrix get_projection_perspective_matrix(float fov, float aspect, float znear, float zfar);
Vec4 operator*(const Matrix& m, const Vec4& v);
Matrix operator*(const Matrix& a, const Matrix& b);

float Lerp(float a, float b, float t);

#endif
