#ifndef LU_VECTOR_MATH_INCLUDED
#define LU_VECTOR_MATH_INCLUDED

#include "LuVector.hpp"

namespace LUV
{

	template< unsigned int N, class T >
	inline T Sum( const LuVector< N, T >& vec ) restrict( cpu, amp )
	{
		T result = 0;
		UNROLL( result, vec, OP_ADD< T, T >(), LOOP_INDEX< N - 1 >() );
		return result;
	}

	// Abs
	template< unsigned int N, class T >
	inline LuVector< N, T > Abs( const LuVector< N, T >& vec ) restrict( cpu, amp )
	{
		LuVector< N, T > absVec;
		UNROLL( absVec, vec, OP_ABS< T, T >(), LOOP_INDEX< N - 1 >() );
		return absVec;
	}

	// Length
	template< unsigned int N, class T >
	inline T Length( const LuVector< N, T >& vec ) restrict( cpu, amp )
	{
		T result = 0;
		UNROLL( result, vec, vec, OP_DOT< N, T, T >(), LOOP_INDEX< N - 1 >() );
		return _Sqrt( result );
	}

	// Unit
	template< unsigned int N, class T >
	inline LuVector< N, T > Unit( const LuVector< N, T >& vec ) restrict( cpu, amp )
	{
		return vec / Length( vec );
	}

	//////////////////////////////////////////////////////////////

	//// Abs Complex
	//template< unsigned int N, class T >
	//inline LuVector< N, T > Abs( const LuVector< N, complex< T > >& vec )
	//{
	//	LuVector< N, T > absVec;
	//	UNROLL( absVec, vec, OP_ABS< T, complex< T > >(), LOOP_INDEX< N - 1 >() );
	//	return absVec;
	//}

	//// Arg Complex
	//template< unsigned int N, class T >
	//inline LuVector< N, T > Arg( const LuVector< N, complex< T > >& vec )
	//{
	//	LuVector< N, T > argVec;
	//	UNROLL( argVec, vec, OP_ARG< T, complex< T > >(), LOOP_INDEX< N - 1 >() );
	//	return argVec;
	//}

	//// Length Complex
	//template< unsigned int N, class T >
	//inline T Length( const LuVector< N, complex< T > >& vec )
	//{
	//	return Length( Abs( vec ) ); 
	//}

	//// Unit Complex
	//template< unsigned int N, class T >
	//inline LuVector< N, complex< T > > Unit( const LuVector< N, complex< T > >& vec )
	//{
	//	return vec / Length( vec );
	//}

	//////////////////////////////////////////////////////////////

	// Min
	template< unsigned int N, class T, class S >
	inline LuVector< N, T > Min( const LuVector< N, T >& lhs, const LuVector< N, S >& rhs ) restrict( cpu, amp )
	{
		LuVector< N, T > result;
		UNROLL( result, lhs, rhs, OP_MIN< N, T, S >(), LOOP_INDEX< N - 1 >() );
		return result;
	}

	// Max
	template< unsigned int N, class T, class S >
	inline LuVector< N, T > Max( const LuVector< N, T >& lhs, const LuVector< N, S >& rhs ) restrict( cpu, amp )
	{
		LuVector< N, T > result;
		UNROLL( result, lhs, rhs, OP_MAX< N, T, S >(), LOOP_INDEX< N - 1 >() );
		return result;
	}

	// Dot
	template< unsigned int N, class T, class S >
	inline T Dot( const LuVector< N, T >& lhs, const LuVector< N, S >& rhs ) restrict( cpu, amp )
	{
		T result = 0;
		UNROLL( result, lhs, rhs, OP_DOT< N, T, S >(), LOOP_INDEX< N - 1 >() );
		return result;
	}

	// Cross
	template< class T, class S >
	inline LuVector< 3, T > Cross( const LuVector< 3, T >& lhs, const LuVector< 3, S >& rhs ) restrict( cpu, amp )
	{
		return LuVector< 3, T >(
			lhs[1] * rhs[2] - lhs[2] * rhs[1],
			lhs[2] * rhs[0] - lhs[0] * rhs[2],
			lhs[0] * rhs[1] - lhs[1] * rhs[0]
		);
	}

	//////////////////////////////////////////////////////////////

	// Reflect
	template< unsigned int N, class T, class S >
	inline LuVector< N, T > Reflect( const LuVector< N, T >& vec, const LuVector< N, S >& normal ) restrict( cpu, amp )
	{
		return vec - normal * Dot( vec, normal ) * 2;
	}

	// Rotate...

	//////////////////////////////////////////////////////////////

	// CtsToSph
	// Physics convention
	// X Y Z --> R Phi(x-y angle, 0 to 2pi) The(z-xy angle, 0 to pi)
	template< class T >
	inline LuVector< 3, T > CtsToSph( const LuVector< 3, T >& cts ) restrict( cpu, amp )
	{
		T len = Length( cts );
		return LuVector< 3, T >(
			len,
			cts[0] == 0 && cts[1] == 0 ? 0 : _Atan2( cts[1], cts[0] ),
			_Acos( cts[2] / len )
		);
	}

	// SphToCts
	// Physics convention
	// X Y Z --> R Phi(x-y angle, 0 to 2pi) The(z-xy angle, 0 to pi)
	template< class T >
	inline LuVector< 3, T > SphToCts( const LuVector< 3, T >& sph ) restrict( cpu, amp )
	{
		T planarVal = sph[0] * _Sin( sph[2] );
		return LuVector< 3, T >(
			planarVal * _Cos( sph[1] ),
			planarVal * _Sin( sph[1] ),
			sph[0] * _Cos( sph[2] )
		);
	}

	// CtsToPol
	template< class T >
	inline LuVector< 2, T > CtsToPol( const LuVector< 2, T >& cts ) restrict( cpu, amp )
	{
		T len = Length( cts );
		return LuVector< 2, T >(
			len,
			cts[0] == 0 && cts[1] == 0 ? 0 : _Atan2( cts[1], cts[0] )
		);
	}

	// PolToCts
	template< class T >
	inline LuVector< 2, T > PolToCts( const LuVector< 2, T >& pol ) restrict( cpu, amp )
	{
		//T planarVal = pol[0] * _Sin( sph[2] );
		return LuVector< 2, T >(
			pol[0] * _Cos( pol[1] ),
			pol[0] * _Sin( pol[1] )
		);
	}

	//////////////////////////////////////////////////////////////

	// OrthonormalSet N=3
	// N: looking outside the sphere at given angles
	// U: up vector
	// R: right vector, parallel to XY plane
	// N = U X R 
	template< class T >
	inline void OrthonormalSet( const T& angP, const T& angT, LuVector< 3, T >& dirN, LuVector< 3, T >& dirU, LuVector< 3, T >& dirR ) restrict( cpu, amp )
	{
		T cp = _Cos( angP );
		T sp = _Sin( angP );
		T ct = _Cos( angT );
		T st = _Sin( angT );

		dirN[0] = st * cp;
		dirN[1] = st * sp;
		dirN[2] = ct;

		dirR[0] = sp;
		dirR[1] = - cp;
		dirR[2] = 0;

		dirU = Cross( dirR, dirN );
	}

	// OrthonormalSet N=2
	template< class T >
	inline void OrthonormalSet( const T& ang, LuVector< 2, T >& dirN, LuVector< 2, T >& dirR ) restrict( cpu, amp )
	{
		T c = _Cos( ang );
		T s = _Sin( ang );

		dirN[0] = c;
		dirN[1] = s;

		dirR[0] = s;
		dirR[1] = - c;
	}

	// OrthogonalR N=3
	template< class T >
	inline LuVector< 3, T > OrthonormalR( const LuVector< 3, T >& dirN ) restrict( cpu, amp )
	{
		T ang = dirN[0] == 0 && dirN[1] == 0 ? 0 : _Atan2( dirN[1], dirN[0] );
		T c = _Cos( ang );
		T s = _Sin( ang );

		return LuVector< 3, T >( s, -c, 0 ); 
	}

	// OrthogonalR N=2
	template< class T >
	inline LuVector< 2, T > OrthonormalR( const LuVector< 2, T >& dirN ) restrict( cpu, amp )
	{
		return LuVector< 2, T >( dirN[1], -dirN[0] ); 
	}

	// OrthogonalU N=3
	template< class T >
	inline LuVector< 3, T > OrthonormalU( const LuVector< 3, T >& dirN ) restrict( cpu, amp )
	{
		return Cross( OrthogonalR( dirN ), dirN ); 
	}

	// OrthonormalU N=3
	template< class T >
	inline LuVector< 3, T > OrthonormalR( const LuVector< 3, T >& dirN, const LuVector< 3, T >& dirU ) restrict( cpu, amp )
	{
		return Cross( dirN, dirU ); 
	}

	// Orthonormalize N=3
	// Makes sure that given vectors are absolutely othogonal and unit length.
	template< class T >
	inline void Orthonormalize( LuVector< 3, T >& dirN, LuVector< 3, T >& dirU, LuVector< 3, T >& dirR ) restrict( cpu, amp )
	{
		dirN = Unit( dirN );
		dirU = Unit( dirU );
		dirR = Unit( dirR );

		dirU = Unit( dirU - Dot( dirU, dirN ) * dirN );

		dirR = Unit( dirR - Dot( dirR, dirN ) * dirN );
		dirR = Unit( dirR - Dot( dirR, dirU ) * dirU );
	}

};

#endif