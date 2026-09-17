#pragma once
#include "../imgui/imgui.h"

#include <vector>
#include <string>
#include <chrono>

typedef struct Vec4 {
    float r, g, b, a;
};

typedef struct Vec3 {
    float x, y, z;

    // Costruttori
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    // Lunghezza (norma) del vettore
    float length() const {
        return sqrtf(x * x + y * y + z * z);
    }

    // Normalizza il vettore (lo rende unitario)
    Vec3 normalized() const {
        float len = length();
        if (len == 0) return Vec3(0, 0, 0);
        return Vec3(x / len, y / len, z / len);
    }

    // Prodotto scalare (dot product)
    float dot(const Vec3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    // Prodotto vettoriale (cross product)
    Vec3 cross(const Vec3& other) const {
        return Vec3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    // Operatori aritmetici
    Vec3 operator+(const Vec3& other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    Vec3 operator-(const Vec3& other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    Vec3 operator*(float scalar) const {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }

    Vec3 operator/(float scalar) const {
        return Vec3(x / scalar, y / scalar, z / scalar);
    }

    // Operatori aritmetici composti
    Vec3& operator+=(const Vec3& other) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }

    Vec3& operator-=(const Vec3& other) {
        x -= other.x; y -= other.y; z -= other.z;
        return *this;
    }

    Vec3& operator*=(float scalar) {
        x *= scalar; y *= scalar; z *= scalar;
        return *this;
    }

    Vec3& operator/=(float scalar) {
        x /= scalar; y /= scalar; z /= scalar;
        return *this;
    }

    // Operatore uguaglianza
    bool operator==(const Vec3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const Vec3& other) const {
        return !(*this == other);
    }
};

inline Vec3 operator*(float scalar, const Vec3& vec) {
    return vec * scalar;
}

typedef struct esp_entity {
    Vec3 origin;
    Vec3 old_origin;

    uint8_t team;
    int32_t health;
    int32_t armor;

    char name[32];

    Vec3 bones[28];

    ImVec2 boneScreen[28];
    bool boneValid[28];
	bool has_defuser;
} esp_entity;

typedef struct bomb_entity {
    bool arming;
    bool planted;
    bool ticking;
    
    int32_t site;

    bool defusing;
    bool exploded;
    bool defused;

    float timer_length;
    float time_left = 0.0f;
} bomb_entity;