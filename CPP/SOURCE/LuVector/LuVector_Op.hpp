#ifndef LU_VECTOR_OP_INCLUDED
#define LU_VECTOR_OP_INCLUDED

#include "LuVector.hpp"

namespace LUV
{
	//////////////////////////////////////////////////////////////
	// TEMPLATE LOOP INDEX VARIABLE
	//////////////////////////////////////////////////////////////
	template< unsigned int N >
	struct LOOP_INDEX
	{

	};

	//////////////////////////////////////////////////////////////
	// TEMPLATE LOOP UNROLLING OPERATION FUNCTORS
	//////////////////////////////////////////////////////////////

	template< class T, class S >
	struct OP_COPY
	{
		inline void operator ()( T& lhs, const S& rhs ) restrict( cpu, amp )
		{
			lhs = rhs;
		}
	};

	//////////////////////////////////////////////////////////////

	template< class T, class S >
	struct OP_ADD
	{
		inline void operator ()( T& lhs, const S& rhs ) restrict( cpu, amp )
		{
			lhs += rhs;
		}
	};

	template< class T, class S >
	struct OP_SUB
	{
		inline void operator ()( T& lhs, const S& rhs ) restrict( cpu, amp )
		{
			lhs -= rhs;
		}
	};

	template< class T, class S >
	struct OP_MUL
	{
		inline void operator ()( T& lhs, const S& rhs ) restrict( cpu, amp )
		{
			lhs *= rhs;
		}
	};

	template< class T, class S >
	struct OP_DIV
	{
		inline void operator ()( T& lhs, const S& rhs ) restrict( cpu, amp )
		{
			lhs /= rhs;
		}
	};

	//////////////////////////////////////////////////////////////

	template< class T, class S >
	struct OP_ABS
	{
		inline void operator ()( T& lhs, const S& rhs ) restrict( cpu, amp )
		{
			lhs = _Abs( rhs );
		}
	};

	template< class T, class S >
	struct OP_ARG
	{
		inline void operator ()( T& lhs, const S& rhs )
		{
			lhs = CMM::arg( rhs );
		}
	};

	template< unsigned int N, class T, class S >
	struct OP_MIN
	{
		inline void operator ()( LuVector< N, T >& result, const LuVector< N, T >& lhs, const LuVector< N, S >& rhs, const unsigned int& I ) restrict( cpu, amp )
		{
			result[ I ] = lhs[ I ] < rhs[ I ] ? lhs[ I ] : rhs[ I ];
		}
	};

	template< unsigned int N, class T, class S >
	struct OP_MAX
	{
		inline void operator ()( LuVector< N, T >& result, const LuVector< N, T >& lhs, const LuVector< N, S >& rhs, const unsigned int& I ) restrict( cpu, amp )
		{
			result[ I ] = lhs[ I ] > rhs[ I ] ? lhs[ I ] : rhs[ I ];
		}
	};

	template< unsigned int N, class T, class S >
	struct OP_DOT
	{
		inline void operator ()( T& result, const LuVector< N, T >& lhs, const LuVector< N, S >& rhs, const unsigned int& I ) restrict( cpu, amp )
		{
			result += lhs[ I ] * rhs[ I ];
		}
	};

	//////////////////////////////////////////////////////////////

	template< class T >
	struct OP_OSTREAM
	{
		inline void operator ()( CMM::ostream& lhs, const T& rhs, const CMM::string& delimeter )
		{
			lhs << rhs << delimeter;
		}
	};

};

#endif