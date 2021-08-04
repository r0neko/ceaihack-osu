#pragma once
// vector2 class done by kat - thank him, not me. i'm still a retard

#include <algorithm>
#include <cmath>
#include <limits>
#include <imgui.h>
#include <numeric>

#define M_PI 3.14159265358979323846

struct vector2_t {
    float x, y;

    vector2_t() = default;
    vector2_t(float x, float y) : x(x), y(y) {};
    vector2_t(float value) : x(value), y(value) {};
    vector2_t(const vector2_t& vector) {
        *this = vector;
    }

    vector2_t(const ImVec2& vector) : x(vector.x), y(vector.y) {};

    inline bool operator==(const vector2_t& rhs) const {
        return this->x == rhs.x && this->y == rhs.y;
    }

    inline vector2_t& operator+=(const vector2_t& rhs) {
        this->x += rhs.x;
        this->y += rhs.y;

        return *this;
    }

    inline vector2_t& operator-=(const vector2_t& rhs) {
        this->x -= rhs.x;
        this->y -= rhs.y;

        return *this;
    }

    inline vector2_t& operator/=(const vector2_t& rhs) {
        this->x /= rhs.x;
        this->y /= rhs.y;

        return *this;
    }

    inline vector2_t& operator*=(const vector2_t& rhs) {
        this->x *= rhs.x;
        this->y *= rhs.y;

        return *this;
    }

    inline vector2_t operator+(const vector2_t& rhs) const {
        return { this->x + rhs.x, this->y + rhs.y };
    }

    inline vector2_t operator-(const vector2_t& rhs) const {
        return { this->x - rhs.x, this->y - rhs.y };
    }

    inline vector2_t operator/(const vector2_t& rhs) const {
        return { this->x / rhs.x, this->y / rhs.y };
    }

    inline vector2_t operator*(const vector2_t& rhs) const {
        return { this->x * rhs.x, this->y * rhs.y };
    }

    inline vector2_t operator<(const vector2_t& rhs) const {
        return this->x < rhs.x&& this->y < rhs.y;
    }

    inline vector2_t operator>(const vector2_t& rhs) const {
        return this->x > rhs.x && this->y > rhs.y;
    }

    inline vector2_t operator<=(const vector2_t& rhs) const {
        return this->x <= rhs.x && this->y <= rhs.y;
    }

    inline vector2_t operator>=(const vector2_t& rhs) const {
        return this->x >= rhs.x && this->y >= rhs.y;
    }

    inline float dot(const vector2_t& rhs) const {
        return this->x * rhs.x + this->y * rhs.y;
    }

    // r0neko lol
    inline double distance(const vector2_t& target) const {
        auto x_sqrd = std::pow(x - target.x, 2);
        auto y_sqrd = std::pow(y - target.y, 2);

        return sqrt(x_sqrd + y_sqrd);
    }

    // r0neko too
    inline vector2_t cos_interpolate_to(const vector2_t& transformation, float transformation_time) {
        auto c = (1 - cosf(transformation_time * M_PI)) / 2;

        this->x = this->x * (1 - c) + (transformation.x * c);
        this->y = this->y * (1 - c) + (transformation.y * c);

        return *this;
    }

    inline float length_squared() const {
        return this->x * this->x + this->y * this->y;
    }

    inline float length() const {
        return this->x + this->y;
    }

    inline vector2_t normalized() const {
        return *this / this->length();
    }

    inline vector2_t delta(const vector2_t& rhs) const {
        return *this - rhs;
    }

    inline vector2_t& interpolate_to(const vector2_t& transformation, float transformation_time) {
        this->x = std::lerp(this->x, transformation.x, transformation_time);
        this->y = std::lerp(this->y, transformation.y, transformation_time);

        return *this;
    }

    inline vector2_t& interpolate_from(const vector2_t& transformation, float transformation_time) {
        this->x = std::lerp(transformation.x, this->x, transformation_time);
        this->y = std::lerp(transformation.y, this->y, transformation_time);

        return *this;
    }

    inline vector2_t mid(const vector2_t& b) const {
        return { std::midpoint(this->x, b.x), std::midpoint(this->y, b.y) };
    }

    inline float relative_angle(const vector2_t& rhs) const {
        return std::acos(std::fmax(-1.f, std::fmin(1.f, normalized().dot(rhs.normalized()))));
    }

    inline vector2_t rotate(float angle) const {
        const auto cos_angle = std::cos(angle);
        const auto sin_angle = std::sin(angle);

        return { cos_angle * this->x + sin_angle * this->y, -sin_angle * this->x + cos_angle * this->y };
    }

    inline bool empty() const {
        return true ? !this->x && !this->y : false;
    }

    inline bool is_finite() const {
        return true ? std::isfinite(this->length()) : false;
    }

    inline vector2_t finite() const {
        if (this->is_finite())
            return *this;

        return {};
    }

    inline ImVec2 imvec() const {
        return ImVec2(this->x, this->y);
    }

    inline vector2_t clamp(float range) {
        return { std::clamp(this->x, -range, range), std::clamp(this->y, -range, range) };
    }

    inline vector2_t& direction_clamp(float range) {
        *this - (this->normalized() * range);

        return *this;
    }
};