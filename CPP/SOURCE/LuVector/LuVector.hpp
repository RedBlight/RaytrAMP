#ifndef LU_VECTOR_INCLUDED
#define LU_VECTOR_INCLUDED

#include <sstream>
#include <string>
#include <complex>
#include <amp.h>
#include <amp_math.h>

#define AMM concurrency::fast_math
//#define AMM concurrency::precise_math
#define CMM std

namespace LUV
{

	template< unsigned int N, class T >
	class LuVector;

	template< class T >
	using LuVector2 = LuVector< 2, T >;

	template< class T >
	using LuVector3 = LuVector< 3, T >;

	template< class T >
	using LuVector3c = LuVector< 3, CMM::complex< T > >;

	using LuVector2f = LuVector< 2, float >;
	using LuVector2d = LuVector< 2, double >;
	using LuVector2cd = LuVector< 2, CMM::complex< double > >;

	using LuVector3f = LuVector< 3, float >;
	using LuVector3d = LuVector< 3, double >;
	using LuVector3cd = LuVector< 3, CMM::complex< double > >;

	//////////////////////

	template< unsigned int N, class T >
	class Vec;

	template< class T >
	using Vec2 = LuVector< 2, T >;

	template< class T >
	using Vec3 = LuVector< 3, T >;

	template< class T >
	using Vec3c = LuVector< 3, CMM::complex< T > >;

	using Vec2f = LuVector< 2, float >;
	using Vec2d = LuVector< 2, double >;
	using Vec2cd = LuVector< 2, CMM::complex< double > >;

	using Vec3f = LuVector< 3, float >;
	using Vec3d = LuVector< 3, double >;
	using Vec3cd = LuVector< 3, CMM::complex< double > >;

};

#include "LuVector_BasicMath.hpp"
#include "LuVector_Op.hpp"
#include "LuVector_Unroll.hpp"
#include "LuVector_Body.hpp"
#include "LuVector_Overload.hpp"
#include "LuVector_Math.hpp"
#include "LuVector_Geometry.hpp"

#endif