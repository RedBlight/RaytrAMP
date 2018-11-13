#ifndef LU_VECTOR_UNROLL_INCLUDED
#define LU_VECTOR_UNROLL_INCLUDED

#include "LuVector.hpp"

namespace LUV
{
	//////////////////////////////////////////////////////////////
	// TEMPLATE LOOP UNROLLING
	//////////////////////////////////////////////////////////////

	// VECTOR OP SCALAR
	template< unsigned int I, unsigned int N, class T, class S, template< class, class > class OP >
	inline void UNROLL( LuVector< N, T >& lhs, const S& rhs, OP< T, S > operation, LOOP_INDEX< I > ) restrict( cpu, amp )
	{
		UNROLL( lhs, rhs, operation, LOOP_INDEX< I - 1 >() );
		operation( lhs[ I ], rhs );
	}

	template< unsigned int N, class T, class S, template< class, class > class OP >
	inline void UNROLL( LuVector< N, T >& lhs, const S& rhs, OP< T, S > operation, LOOP_INDEX< 0 > ) restrict( cpu, amp )
	{
		operation( lhs[ 0 ], rhs );
	}

	// SCALAR OP VECTOR
	template< unsigned int I, unsigned int N, class T, class S, template< class, class > class OP >
	inline void UNROLL( S& lhs, const LuVector< N, T >& rhs, OP< T, S > operation, LOOP_INDEX< I > ) restrict( cpu, amp )
	{
		UNROLL( lhs, rhs, operation, LOOP_INDEX< I - 1 >() );
		operation( lhs, rhs[ I ] );
	}

	template< unsigned int N, class T, class S, template< class, class > class OP >
	inline void UNROLL( S& lhs, const LuVector< N, T >& rhs, OP< T, S > operation, LOOP_INDEX< 0 > ) restrict( cpu, amp )
	{
		operation( lhs, rhs[ 0 ] );
	}

	// VECTOR OP VECTOR
	template< unsigned int I, unsigned int N, class T, class S, template< class, class > class OP >
	inline void UNROLL( LuVector< N, T >& lhs, const LuVector< N, S >& rhs, OP< T, S > operation, LOOP_INDEX< I > ) restrict( cpu, amp )
	{
		UNROLL( lhs, rhs, operation, LOOP_INDEX< I - 1 >() );
		operation( lhs[ I ], rhs[ I ] );
	}

	template< unsigned int N, class T, class S, template< class, class > class OP >
	inline void UNROLL( LuVector< N, T >& lhs, const LuVector< N, S >& rhs, OP< T, S > operation, LOOP_INDEX< 0 > ) restrict( cpu, amp )
	{
		operation( lhs[ 0 ], rhs[ 0 ] );
	}

	// SCALAR = VECTOR OP VECTOR (could be cross-element)
	template< unsigned int I, unsigned int N, class T, class S, template< unsigned int, class, class > class OP >
	inline void UNROLL( T& result, const LuVector< N, T >& lhs, const LuVector< N, S >& rhs, OP< N, T, S > operation, LOOP_INDEX< I > ) restrict( cpu, amp )
	{
		UNROLL( result, lhs, rhs, operation, LOOP_INDEX< I - 1 >() );
		operation( result, lhs, rhs, I );
	}

	template< unsigned int N, class T, class S, template< unsigned int, class, class > class OP >
	inline void UNROLL( T& result, const LuVector< N, T >& lhs, const LuVector< N, S >& rhs, OP< N, T, S > operation, LOOP_INDEX< 0 > ) restrict( cpu, amp )
	{
		operation( result, lhs, rhs, 0 );
	}

	// VECTOR = VECTOR OP VECTOR (could be cross-element)
	template< unsigned int I, unsigned int N, class T, class S, template< unsigned int, class, class > class OP >
	inline void UNROLL( LuVector< N, T >& result, const LuVector< N, T >& lhs, const LuVector< N, S >& rhs, OP< N, T, S > operation, LOOP_INDEX< I > ) restrict( cpu, amp )
	{
		UNROLL( result, lhs, rhs, operation, LOOP_INDEX< I - 1 >() );
		operation( result, lhs, rhs, I );
	}

	template< unsigned int N, class T, class S, template< unsigned int, class, class > class OP >
	inline void UNROLL( LuVector< N, T >& result, const LuVector< N, T >& lhs, const LuVector< N, S >& rhs, OP< N, T, S > operation, LOOP_INDEX< 0 > ) restrict( cpu, amp )
	{
		operation( result, lhs, rhs, 0 );
	}

	// OSTREAM OP VECTOR
	template< unsigned int I, unsigned int N, class T, template< class > class OP >
	inline void UNROLL( CMM::ostream& lhs, const LuVector< N, T >& rhs, const CMM::string& delimeter, OP< T > operation, LOOP_INDEX< I > )
	{
		UNROLL( lhs, rhs, delimeter, operation, LOOP_INDEX< I - 1 >() );
		operation( lhs, rhs[ I ], delimeter );
	}

	template< unsigned int N, class T, template< class > class OP >
	inline void UNROLL( CMM::ostream& lhs, const LuVector< N, T >& rhs, const CMM::string& delimeter, OP< T > operation, LOOP_INDEX< 0 > )
	{
		operation( lhs, rhs[ 0 ], delimeter );
	}

};

#endif