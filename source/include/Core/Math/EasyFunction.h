
#pragma once
#include "ToolBoxMathHeaders.h"

namespace Math
{
	template<class T>
	[[nodiscard]]
	constexpr T Lerp(const T& t0, const T& t2, float t)
	{
		return t0 * (1 - t) + t2 * t;
	}

	template<class T>
	[[nodiscard]] static inline T Slerp(const T& t1, const T& t2, float t)
	{
		float dotAB = T::DotProduct(t1, t2);
		float theta = std::acos(dotAB);
		float sinTheta = std::sin(theta);
		float af = std::sin((1.0f - t) * theta) / sinTheta;
		float bf = std::sin(t * theta) / sinTheta;
		return (t1 * af + t2 * bf);

	}

	
}