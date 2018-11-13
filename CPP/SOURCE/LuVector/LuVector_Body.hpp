#ifndef LU_VECTOR_BODY_INCLUDED
#define LU_VECTOR_BODY_INCLUDED

#include "LuVector.hpp"

namespace LUV
{

	template< unsigned int N, class T >
	class LuVector
	{
	private:
		T _[ N ];

	public:
		LuVector( ) restrict( cpu, amp )
		{

		}

		~LuVector() restrict( cpu, amp )
		{

		}

		template< class... S > 
		LuVector( const S&... elem ) restrict( cpu, amp ) :
			_{ static_cast< T >(elem) ...}
		{

		}

		LuVector( const LuVector< N, const T >& other ) restrict( cpu, amp ) :
			_{ other._ }
		{

		}

		template< class S > 
		LuVector( const LuVector< N, S >& other ) restrict( cpu, amp )
		{
			UNROLL( *this, other, OP_COPY< T, S >(), LOOP_INDEX< N - 1 >() );
		}

		template< class S > 
		LuVector( const S& scalar ) restrict( cpu, amp )
		{
			UNROLL( *this, scalar, OP_COPY< T, S >(), LOOP_INDEX< N - 1 >() );
		}

		template< class S >
		inline LuVector< N, T >& operator =( const LuVector< N, S >& other ) restrict( cpu, amp )
		{
			UNROLL( *this, other, OP_COPY< T, S >(), LOOP_INDEX< N - 1 >() );
			return *this;
		}

		inline const T& operator []( const unsigned int& idx ) const restrict( cpu, amp )
		{
			return _[ idx ];
		}

		inline T& operator []( const unsigned int& idx ) restrict( cpu, amp )
		{
			return _[ idx ];
		}
		
		inline CMM::string ToString() const
		{
			CMM::stringstream ss;
			ss << "(";
			UNROLL( ss, *this, ",", OP_OSTREAM< T >(), LOOP_INDEX< N - 2 >() );
			ss << _[ N - 1 ] << ")";
			return CMM::string( ss.str() );
		}

	};
};

#endif