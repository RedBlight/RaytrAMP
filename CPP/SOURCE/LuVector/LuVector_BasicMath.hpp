#ifndef LU_VECTOR_BASICMATH_INCLUDED
#define LU_VECTOR_BASICMATH_INCLUDED

#include "LuVector.hpp"

namespace LUV
{

	// ABS
	template< class T >
	inline T _Abs( const T& num ) restrict( cpu )
	{
		return CMM::abs( num );
	}
	template< class T >
	inline T _Abs( const T& num ) restrict( amp )
	{
		return AMM::fabs( num );
	}

	// SIN
	template< class T >
	inline T _Sin( const T& num ) restrict( cpu )
	{
		return CMM::sin( num );
	}
	template< class T >
	inline T _Sin( const T& num ) restrict( amp )
	{
		return AMM::sin( num );
	}

	// COS
	template< class T >
	inline T _Cos( const T& num ) restrict( cpu )
	{
		return CMM::cos( num );
	}
	template< class T >
	inline T _Cos( const T& num ) restrict( amp )
	{
		return AMM::cos( num );
	}

	// TAN
	template< class T >
	inline T _Tan( const T& num ) restrict( cpu )
	{
		return CMM::tan( num );
	}
	template< class T >
	inline T _Tan( const T& num ) restrict( amp )
	{
		return AMM::tan( num );
	}

	// ACOS
	template< class T >
	inline T _Acos( const T& num ) restrict( cpu )
	{
		return CMM::acos( num );
	}
	template< class T >
	inline T _Acos( const T& num ) restrict( amp )
	{
		return AMM::acos( num );
	}

	// ASIN
	template< class T >
	inline T _Asin( const T& num ) restrict( cpu )
	{
		return CMM::asin( num );
	}
	template< class T >
	inline T _Asin( const T& num ) restrict( amp )
	{
		return AMM::asin( num );
	}

	// ATAN
	template< class T >
	inline T _Atan( const T& num ) restrict( cpu )
	{
		return CMM::atan( num );
	}
	template< class T >
	inline T _Atan( const T& num ) restrict( amp )
	{
		return AMM::atan( num );
	}

	// ATAN2
	template< class T >
	inline T _Atan2( const T& y, const T& x ) restrict( cpu )
	{
		return CMM::atan2( y, x );
	}
	template< class T >
	inline T _Atan2( const T& y, const T& x ) restrict( amp )
	{
		return AMM::atan2( y, x );
	}

	// SQRT
	template< class T >
	inline T _Sqrt( const T& num ) restrict( cpu )
	{
		return CMM::sqrt( num );
	}
	template< class T >
	inline T _Sqrt( const T& num ) restrict( amp )
	{
		return AMM::sqrt( num );
	}

};

#endif