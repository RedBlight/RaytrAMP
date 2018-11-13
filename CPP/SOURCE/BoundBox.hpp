#ifndef BOUND_BOX_INCLUDED
#define BOUND_BOX_INCLUDED

#include "LuVector.hpp"

template< class T >
class BoundBox
{
public:
	LUV::Vec3< T > min_;
	LUV::Vec3< T > max_;

public:
	BoundBox() restrict( cpu, amp ) 
	{

	}

	BoundBox( const LUV::Vec3< T >& minVec, const LUV::Vec3< T >& maxVec ) restrict( cpu, amp ) :
		min_( minVec ),
		max_( maxVec )
	{

	}

	~BoundBox() restrict( cpu, amp ) 
	{

	}
	
	inline BoundBox< T > UnionWith( const BoundBox< T >& other ) const restrict( cpu, amp )
	{
		return BoundBox< T >( LUV::Min( min_, other.min_ ), LUV::Max( max_, other.max_ ) );
	}
	
	inline LUV::Vec3< T > GetCenter() const restrict( cpu, amp )
	{
		return ( min_ + max_ ) / 2.0;
	}
	
	inline T GetRadius() const restrict( cpu, amp )
	{
		return LUV::Length( max_ - min_ ) / 2.0;
	}

	inline bool Contains( const LUV::Vec3< T > vec ) const restrict( cpu, amp )
	{
		return
			( vec[0] >= min_[0] ) && ( vec[0] < max_[0] ) && 
			( vec[1] >= min_[1] ) && ( vec[1] < max_[1] ) &&
			( vec[2] >= min_[2] ) && ( vec[2] < max_[2] )
		;
	}

};

#endif