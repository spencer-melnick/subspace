#pragma once

// STL includes
#include <cmath>
#include <type_traits>

// Library includes
#include <fmt/ostream.h>

namespace subspace {
	template <typename T>
	struct Simple_Vec2 {
		// Members
		T x, y;

		// Operators
		Simple_Vec2<T>& operator+=(const Simple_Vec2<T>& rhs) {
			x += rhs.x;
			y += rhs.y;

			return *this;
		}

		Simple_Vec2<T>& operator-=(const Simple_Vec2<T>& rhs) {
			x -= rhs.x;
			y -= rhs.y;

			return *this;
		}

		Simple_Vec2<T>& operator*=(const T& rhs) {
			x *= rhs;
			y *= rhs;

			return *this;
		}

		Simple_Vec2<T>& operator/=(const T& rhs) {
			x /= rhs;
			y /= rhs;

			return *this;
		}

		T magnitudeSquared() {
			return x * x + y * y;
		}

		// Magnitude and normalize are only enabled for floating point types
		template <typename = std::enable_if<std::is_floating_point<T>::value>::type>
		T magnitude() {
			return sqrt(magnitudeSquared());
		}

		template <typename = std::enable_if<std::is_floating_point<T>::value>::type>
		Simple_Vec2<T>& normalize() {
			T m = magnitude();

			x /= m;
			y /= m;

			return *this;
		}

		template <typename = std::enable_if<std::is_floating_point<T>::value>::type>
		Simple_Vec2<T> normalized() {
			Simple_Vec2<T> vec(*this);
			return vec.normalize();
		}

		T dot(const Simple_Vec2<T>& rhs) {
			return x * rhs.x + y * rhs.y;
		}

		// fmt support
		friend std::ostream &operator<<(std::ostream &os, const Simple_Vec2<T> &vec) {
			return os << vec.x << ", " << vec.y;
		}
	};

	// Binary operators
	template <typename T>
	inline Simple_Vec2<T> operator+(Simple_Vec2<T> lhs, const Simple_Vec2<T>& rhs) {
		lhs += rhs;
		return lhs;
	}

	template <typename T>
	inline Simple_Vec2<T> operator-(Simple_Vec2<T> lhs, const Simple_Vec2<T>& rhs) {
		lhs -= rhs;
		return lhs;
	}

	template <typename T>
	inline Simple_Vec2<T> operator*(Simple_Vec2<T> lhs, const T& rhs) {
		lhs *= rhs;
		return lhs;
	}

	template <typename T>
	inline Simple_Vec2<T> operator/(Simple_Vec2<T> lhs, const T& rhs) {
		lhs /= rhs;
		return lhs;
	}

	using Vec2 = Simple_Vec2<float>;
}
