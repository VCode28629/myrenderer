#pragma once

#include <cmath>
#include <iostream>

template <typename t>
struct Vec2 {
    union {
        struct {
            t x, y;
        };
        t raw[2];
    };
    Vec2<t>() {
        for (int i = 0; i < 2; ++i) raw[i] = 0;
    }
    Vec2<t>(t x, t y) : x(x), y(y) {}
    Vec2<t> operator+(Vec2<t> v) {
        Vec2<t> res;
        for (int i = 0; i < 2; ++i) res[i] = raw[i] + v[i];
        return res;
    }
    Vec2<t> operator-(Vec2<t> v) {
        Vec2<t> res;
        for (int i = 0; i < 2; ++i) res[i] = raw[i] - v[i];
        return res;
    }
    Vec2<t> operator*(float f) {
        Vec2<t> res;
        for (int i = 0; i < 2; ++i) res[i] = raw[i] * f;
        return res;
    }
    t operator*(Vec2<t> v) {
        t res;
        for (int i = 0; i < 2; ++i) res += raw[i] * v[i];
        return res;
    }
    t norm() {
        t res;
        for (int i = 0; i < 2; ++i) res += raw[i] * raw[i];
        return std::sqrt(res);
    }
    Vec2<t> normalize(t l = 1) {
        *this = (*this) * (l / norm());
        return *this;
    }
    t& operator[](const int idx) { return raw[idx]; }
    friend std::ostream& operator<<(std::ostream& s, const Vec2<t>& m) {
        return s << '(' << m.x << ',' << m.y << ')';
    }
};

template <typename t>
struct Vec3 {
    union {
        struct {
            t x, y, z;
        };
        t raw[3];
    };
    Vec3<t>() {
        for (int i = 0; i < 3; ++i) raw[i] = 0;
    }
    Vec3<t>(t x, t y, t z) : x(x), y(y), z(z) {}
    Vec3<t> operator+(Vec3<t> v) {
        Vec3<t> res;
        for (int i = 0; i < 3; ++i) res[i] = raw[i] + v[i];
        return res;
    }
    Vec3<t> operator-(Vec3<t> v) {
        Vec3<t> res;
        for (int i = 0; i < 3; ++i) res[i] = raw[i] - v[i];
        return res;
    }
    Vec3<t> operator*(float f) {
        Vec3<t> res;
        for (int i = 0; i < 3; ++i) res[i] = raw[i] * f;
        return res;
    }
    t operator*(Vec3<t> v) {
        t res = 0;
        for (int i = 0; i < 3; ++i) res += raw[i] * v[i];
        return res;
    }
    t norm() {
        t res = 0;
        for (int i = 0; i < 3; ++i) res += raw[i] * raw[i];
        return std::sqrt(res);
    }
    Vec3<t> normalize(t l = 1) {
        *this = (*this) * (l / norm());
        return *this;
    }
    t& operator[](const int idx) { return raw[idx]; }
    Vec3<t> operator^(const Vec3<t> v) const {
        return Vec3<t>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
    friend std::ostream& operator<<(std::ostream& s, const Vec3<t>& m) {
        return s << '(' << m.x << ',' << m.y << ',' << m.z << ')';
    }
};

template <typename t>
struct Vec4 {
    union {
        struct {
            t x, y, z, p;
        };
        t raw[4];
    };
    Vec4<t>() {
        for (int i = 0; i < 4; ++i) raw[i] = 0;
    }
    Vec4<t>(t x, t y, t z, t p) : x(x), y(y), z(z), p(p) {}
    Vec4<t> operator+(Vec4<t> v) {
        Vec4<t> res;
        for (int i = 0; i < 4; ++i) res[i] = raw[i] + v[i];
        return res;
    }
    Vec4<t> operator-(Vec4<t> v) {
        Vec4<t> res;
        for (int i = 0; i < 4; ++i) res[i] = raw[i] - v[i];
        return res;
    }
    Vec4<t> operator*(float f) {
        Vec4<t> res;
        for (int i = 0; i < 4; ++i) res[i] = raw[i] * f;
        return res;
    }
    t operator*(Vec4<t> v) {
        t res = 0;
        for (int i = 0; i < 4; ++i) res += raw[i] * v[i];
        return res;
    }
    t norm() {
        t res = 0;
        for (int i = 0; i < 4; ++i) res += raw[i] * raw[i];
        return std::sqrt(res);
    }
    Vec4<t> normalize(t l = 1) {
        *this = (*this) * (l / norm());
        return *this;
    }
    t& operator[](const int idx) { return raw[idx]; }
    friend std::ostream& operator<<(std::ostream& s, const Vec4<t>& m) {
        return s << '(' << m.x << ',' << m.y << ',' << m.z << ',' << m.p << ')';
    }
};

typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;
typedef Vec4<float> Vec4f;
typedef Vec4<int> Vec4i;

template <typename t>
Vec4<t> cross(Vec4<t> a, Vec4<t> b, Vec4<t> c);
class Matrix4f {
    float data[4][4];

   public:
    Matrix4f() {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                data[i][j] = 0;
            }
        }
    }
    static Matrix4f identity() {
        Matrix4f res;
        for (int i = 0; i < 4; ++i) res[i][i] = 1.f;
        return res;
    }
    Matrix4f transpose() {
        Matrix4f res;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                res[i][j] = data[j][i];
            }
        }
        return res;
    }
    Matrix4f operator*(Matrix4f& m) {
        Matrix4f res;
        for (int i = 0; i < 4; ++i) {
            for (int k = 0; k < 4; ++k) {
                float temp = data[i][k];
                for (int j = 0; j < 4; ++j) {
                    res[i][j] += temp * m[k][j];
                }
            }
        }
        return res;
    }
    Vec4f operator*(Vec4f v) {
        Vec4f res;
        for (int i = 0; i < 4; ++i) {
            float temp = 0;
            for (int j = 0; j < 4; ++j) {
                temp += data[i][j] * v[j];
            }
            res[i] = temp;
        }
        return res;
    }
    float* operator[](int idx) { return data[idx]; }
    friend std::ostream& operator<<(std::ostream& s, const Matrix4f& m) {
        s << '[';
        for (int i = 0; i < 4; ++i) {
            s << '[';
            for (int j = 0; j < 4; ++j) {
                s << m.data[i][j];
                if (j != 3) s << ',';
            }
            s << "]";
            if (i != 3) s << ",\n";
        }
        return s << ']';
    }
};

template <typename t>
Vec4<t> toVec4(Vec3<t> v) {
    return Vec4<t>(v.x, v.y, v.z, 1);
}
template <typename t>
Vec3<t> toVec3(Vec4<t> v) {
    return Vec3<t>(v.x / v.p, v.y / v.p, v.z / v.p);
}
template <typename t>
Vec4<t> cross(Vec4<t> a, Vec4<t> b, Vec4<t> c) {
    a.normalize();
    b.normalize();
    c.normalize();
    return Vec4<t>((Vec3<t>(a.y, a.z, a.p) ^ Vec3<t>(b.y, b.z, b.p)) *
                       Vec3<t>(c.y, c.z, c.p) * -1,
                   (Vec3<t>(a.x, a.z, a.p) ^ Vec3<t>(b.x, b.z, b.p)) *
                       Vec3<t>(c.x, c.z, c.p),
                   (Vec3<t>(a.x, a.y, a.p) ^ Vec3<t>(b.x, b.y, b.p)) *
                       Vec3<t>(c.x, c.y, c.p) * -1,
                   (Vec3<t>(a.x, a.y, a.z) ^ Vec3<t>(b.x, b.y, b.z)) *
                       Vec3<t>(c.x, c.y, c.z));
}
