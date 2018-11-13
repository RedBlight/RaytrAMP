#ifndef BOUND_SPHERE_INCLUDED
#define BOUND_SPHERE_INCLUDED

#include "LuVector.hpp"

template< class T >
class BoundSphere
{
public:
	LUV::Vec3< T > center_;
	T radius_;

public:
	BoundSphere() restrict( cpu, amp )
	{

	}

	BoundSphere( const LUV::Vec3< T >& center, const T& radius ) restrict( cpu, amp ) :
		center_( center ),
		radius_( radius )
	{

	}

	~BoundSphere() restrict( cpu, amp ) 
	{

	}

	inline BoundSphere< T > UnionWith( const BoundSphere< T >& other ) restrict( cpu, amp )
	{
		if( center_ == other.center_ )
		{
			return BoundSphere< T >( center_, max( radius_, other.radius_ ) );
		}

		LUV::Vec3< T > centerToOther = other.center_ - center_;
		T distance = LUV::Length( centerToOther );
		LUV::Vec3< T > dir = LUV::Unit( centerToOther );

		T r1a = -radius_;
		T r1b = radius_;
		T r2a = distance - other.radius_;
		T r2b = distance + other.radius_;
		T rMin = min( min( min( r1a, r1b ), r2a ), r2b );
		T rMax = max( max( max( r1a, r1b ), r2a ), r2b );
		T rCenter = ( rMin + rMax ) / 2;

		return BoundSphere< T >( center_ + rCenter * dir, rMax - rCenter );
	}

};

#endif