#ifndef TRIANGLE_INCLUDED
#define TRIANGLE_INCLUDED

#include "LuVector.hpp"
#include "BoundBox.hpp"
#include "BoundSphere.hpp"

template< class T >
class Triangle
{
public:
	LUV::Vec3< T > v1_;
	LUV::Vec3< T > v2_;
	LUV::Vec3< T > v3_;
	//LUV::Vec3< T > normal_;
	//LUV::Vec3< T > center_;

public:
	Triangle() restrict( cpu, amp )
	{

	}

	Triangle( const LUV::Vec3< T >& v1, const LUV::Vec3< T >& v2, const LUV::Vec3< T >& v3 ) restrict( cpu, amp ) :
		v1_( v1 ),
		v2_( v2 ),
		v3_( v3 )
		//normal_( LUV::PlaneNormal( v1, v2, v3 ) ),
		//center_( ( v1 + v2 + v3 ) / 3 )
	{

	}

	LUV::Vec3< T > GetNormal() const restrict( cpu, amp )
	{
		return LUV::PlaneNormal( v1_, v2_, v3_ );
	}
	
	LUV::Vec3< T > GetCenter() const restrict( cpu, amp )
	{
		return ( v1_ + v2_ + v3_ ) / 3;
	}

	BoundBox< T > GetBoundBox() const restrict( cpu, amp )
	{
		return BoundBox< T >( LUV::Min( LUV::Min( v1_, v2_ ), v3_ ), LUV::Max( LUV::Max( v1_, v2_ ), v3_ ) ); 
	}

	//BoundSphere< T > GetBoundSphere() const restrict( cpu, amp )
	//{
	//	LUV::Vec3< T > bCenter;
	//	T bRadius;

	//	T A = LUV::Length( v1_ - v2_ );
	//	T B = LUV::Length( v2_ - v3_ );
	//	T C = LUV::Length( v3_ - v1_ );

	//	const LUV::Vec3< T >* a = &v3_;
	//	const LUV::Vec3< T >* b = &v1_;
	//	const LUV::Vec3< T >* c = &v2_;

	//	if( B < C )
	//	{
	//		std::swap( B, C );
	//		std::swap( b, c );
	//	}

	//	if( A < B )
	//	{
	//		std::swap( B, C );
	//		std::swap( b, c );
	//	}

	//	T A2 = A * A;
	//	T B2 = B * B;
	//	T C2 = C * C;

	//	if( A2 >= B2 + C2 )
	//	{
	//		bRadius = A / 2;
	//		bCenter = ( *b + *c ) / 2;
	//	}
	//	else
	//	{
	//		T cosa = ( B2 + C2 - A2 ) / ( B * C * 2 );
	//		bRadius = A / ( 2 * std::sqrt( 1 - cosa * cosa ) );

	//		LUV::Vec3< T > alpha = *a - *c;
	//		LUV::Vec3< T > beta = *b - *c;

	//		T alphaD = LUV::Dot( alpha, alpha );
	//		T betaD = LUV::Dot( beta, beta );
	//		LUV::Vec3< T > aCb = LUV::Cross( alpha, beta );

	//		bCenter = *c + LUV::Cross( beta * alphaD - alpha * betaD, aCb ) / ( 2 * LUV::Dot( aCb, aCb ) )
	//	}
	//	
	//	return BoundSphere< T >( bCenter, bRadius );
	//}

};

#endif