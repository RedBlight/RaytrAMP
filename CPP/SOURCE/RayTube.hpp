#ifndef RAY_TUBE_INCLUDED
#define RAY_TUBE_INCLUDED

#include "LuVector.hpp"
#include "TypeDef.hpp"
#include "BoundBox.hpp"
#include "Triangle.hpp"

template< class T >
class RayTube
{
public:
	LUV::Vec3< T > pos_;
	LUV::Vec3< T > dir_;
	LUV::Vec3< T > pol_;
	T dist_;
	LUV::Vec3< T > refNormal_;
	U32 refCount_;
	U32 lastHitIdx_;

public:
	RayTube() restrict( cpu, amp ) 
	{

	}

	RayTube(
		const LUV::Vec3< T >& pos,
		const LUV::Vec3< T >& dir,
		const LUV::Vec3< T >& pol,
		const T& dist,
		const LUV::Vec3< T >& refNormal,
		const U32& refCount,
		const U32& lastHitIdx
	) restrict( cpu, amp ) :
		pos_( pos ),
		dir_( dir ),
		pol_( pol ),
		dist_( dist ),
		refNormal_( refNormal ),
		refCount_( refCount ),
		lastHitIdx_( lastHitIdx )
	{

	}

	~RayTube() restrict( cpu, amp ) 
	{

	}

	inline void CollisionWithBoundBox( const BoundBox< T >& boundBox, bool& isHit, T& hitDist ) restrict( cpu, amp ) 
	{
		T tx0 = (boundBox.min_[0] - pos_[0]) / dir_[0];
		T tx1 = (boundBox.max_[0] - pos_[0]) / dir_[0];

		T tmin = min(tx0, tx1);
		T tmax = max(tx0, tx1);

		T ty0 = (boundBox.min_[1] - pos_[1]) / dir_[1];
		T ty1 = (boundBox.max_[1] - pos_[1]) / dir_[1];

		tmin = max(tmin, min(ty0, ty1));
		tmax = min(tmax, max(ty0, ty1));

		T tz0 = (boundBox.min_[2] - pos_[2]) / dir_[2];
		T tz1 = (boundBox.max_[2] - pos_[2]) / dir_[2];

		tmin = max(tmin, min(tz0, tz1));
		tmax = min(tmax, max(tz0, tz1));

		isHit = tmax >= max(tmin, 0.0);
		hitDist = tmin;
		
	}


	inline void CollisionWithBoundBox__( const BoundBox< T >& boundBox, bool& isHit, T& hitDist ) restrict( cpu, amp ) 
	{
		if( boundBox.Contains( pos_ ) )
		{
			isHit = true;
			hitDist = 0;
			return;
		}
		else
		{
			    LUV::Vec3< T > T_1, T_2; // vectors to hold the T-values for every direction
				T t_near = -DBL_MAX; // maximums defined in float.h
				T t_far = DBL_MAX;

				for (int i = 0; i < 3; i++){ //we test slabs in every direction
					if (dir_[i] == 0){ // ray parallel to planes in this direction
						if ((pos_[i] < boundBox.min_[i]) || (pos_[i] > boundBox.max_[i])) {
							isHit = false; // parallel AND outside box : no intersection possible
						}
					} else { // ray not parallel to planes in this direction
						T_1[i] = (boundBox.min_[i] - pos_[i]) / dir_[i];
						T_2[i] = (boundBox.max_[i] - pos_[i]) / dir_[i];

						if(T_1[i] > T_2[i]){ // we want T_1 to hold values for intersection with near plane
							//std::swap(T_1,T_2);
							LUV::Vec3< T > T_3 = T_1;
							T_1 = T_2;
							T_2 = T_3;
						}
						if (T_1[i] > t_near){
							t_near = T_1[i];
						}
						if (T_2[i] < t_far){
							t_far = T_2[i];
						}
						if( (t_near > t_far) || (t_far < 0) ){
							isHit = false;
						}
					}
				}
				hitDist = t_near; // tfar = t_far; // put return values in place
				isHit = true; // if we made it here, there was an intersection - YAY
		}
	}

	inline void CollisionWithTriangleSbr( const Triangle< T >& trig, bool& isHit, T& hitDist, LUV::Vec3< T >& hitPoint ) restrict( cpu, amp )
	{
		const T eps = 1E-7;
		LUV::Vec3< T > vecEdge1 = trig.v2_ - trig.v1_;
		LUV::Vec3< T > vecEdge2 = trig.v3_ - trig.v1_;
		LUV::Vec3< T > vecPerp = LUV::Cross( dir_, vecEdge2 ); // h
		T magA = LUV::Dot( vecEdge1, vecPerp ); //a
		
		if( magA > -eps && magA < eps )
		{
			isHit = false;
			return;
		}

		T magInvA = 1.0 / magA; //f
		LUV::Vec3< T > vecV1ToPos = pos_ - trig.v1_; //s
		T magU = magInvA * LUV::Dot( vecV1ToPos, vecPerp ); //u

		if( magU < 0.0 || magU > 1.0 )
		{
			isHit = false;
			return;
		}

		LUV::Vec3< T > vecQ = LUV::Cross( vecV1ToPos, vecEdge1 );
		T magV = magInvA * LUV::Dot( dir_, vecQ );

		if( magV < 0.0 || magU + magV > 1.0 )
		{
			isHit = false;
			return;
		}

		T magT = magInvA * LUV::Dot( vecEdge2, vecQ );
		if( magT > eps )
		{
			hitPoint = pos_ + dir_ * magT;
			hitDist = magT;
			isHit = true;
			return;
		}
		isHit = false;

	}

	inline void CollisionWithTriangle( const Triangle< T >& trig, bool& isHit, T& hitDist ) restrict( cpu, amp )
	{
		const T eps = 1E-7;
		LUV::Vec3< T > vecEdge1 = trig.v2_ - trig.v1_;
		LUV::Vec3< T > vecEdge2 = trig.v3_ - trig.v1_;
		LUV::Vec3< T > vecPerp = LUV::Cross( dir_, vecEdge2 ); // h
		T magA = LUV::Dot( vecEdge1, vecPerp ); //a
		T magInvA = 1.0 / magA; //f
		LUV::Vec3< T > vecV1ToPos = pos_ - trig.v1_; //s
		T magU = magInvA * LUV::Dot( vecV1ToPos, vecPerp ); //u
		LUV::Vec3< T > vecQ = LUV::Cross( vecV1ToPos, vecEdge1 );
		T magV = magInvA * LUV::Dot( dir_, vecQ );
		T magT = magInvA * LUV::Dot( vecEdge2, vecQ );

		isHit =
			!( magA > -eps && magA < eps )
			&& !( magU < 0.0 || magU > 1.0 )
			&& !( magV < 0.0 || magU + magV > 1.0 )
			&& ( magT > eps )
		;

		hitDist = magT;

	}

	inline void CollisionWithTriangle_01( const Triangle< T >& trig, bool& isHit, T& hitDist ) restrict( cpu, amp )
	{
		const T eps = 1E-7;
		LUV::Vec3< T > vecEdge1 = trig.v2_ - trig.v1_;
		LUV::Vec3< T > vecEdge2 = trig.v3_ - trig.v1_;
		LUV::Vec3< T > vecPerp = LUV::Cross( dir_, vecEdge2 ); // h
		T magA = LUV::Dot( vecEdge1, vecPerp ); //a
		
		if( magA > -eps && magA < eps )
		{
			isHit = false;
			return;
		}

		T magInvA = 1.0 / magA; //f
		LUV::Vec3< T > vecV1ToPos = pos_ - trig.v1_; //s
		T magU = magInvA * LUV::Dot( vecV1ToPos, vecPerp ); //u

		if( magU < 0.0 || magU > 1.0 )
		{
			isHit = false;
			return;
		}

		LUV::Vec3< T > vecQ = LUV::Cross( vecV1ToPos, vecEdge1 );
		T magV = magInvA * LUV::Dot( dir_, vecQ );

		if( magV < 0.0 || magU + magV > 1.0 )
		{
			isHit = false;
			return;
		}

		T magT = magInvA * LUV::Dot( vecEdge2, vecQ );
		if( magT > eps )
		{
			hitDist = magT;
			isHit = true;
			return;
		}
		isHit = false;

	}

	inline void CollisionWithTriangle__2( const Triangle< T >& trig, bool& isHit, T& hitDist ) restrict( cpu, amp )
	{
		LUV::Vec3< T > normal = LUV::PlaneNormal( trig.v1_, trig.v2_, trig.v3_ );
		LUV::Vec3< T > proj = LUV::ProjPlane( pos_, trig.v1_, normal );

		T eps = 1E-7;

		LUV::Vec3< T > posToProj = proj - pos_;

		LUV::Vec3< T > posToProjDir = LUV::Unit( posToProj );
		T cosine = LUV::Dot( posToProjDir, dir_ );

		LUV::Vec3< T > hitPos = pos_ + dir_ * ( LUV::Length( posToProj ) / cosine );

		//if(
		//	cosine < eps ||
		//	LUV::Length( posToProj ) < eps ||
		//	LUV::Dot( LUV::LineNormalP( trig.v1_, trig.v2_, trig.v3_ ), hitPos - trig.v2_ ) < -eps ||
		//	LUV::Dot( LUV::LineNormalP( trig.v2_, trig.v3_, trig.v1_ ), hitPos - trig.v3_ ) < -eps ||
		//	LUV::Dot( LUV::LineNormalP( trig.v3_, trig.v1_, trig.v2_ ), hitPos - trig.v1_ ) < -eps
		//)
		//{
		//	isHit = false;
		//	return;
		//}

		isHit =
		!(
			cosine < eps ||
			LUV::Length( posToProj ) < eps
			//||
			//LUV::Dot( LUV::LineNormalP( trig.v1_, trig.v2_, trig.v3_ ), hitPos - trig.v2_ ) < -eps ||
			//LUV::Dot( LUV::LineNormalP( trig.v2_, trig.v3_, trig.v1_ ), hitPos - trig.v3_ ) < -eps ||
			//LUV::Dot( LUV::LineNormalP( trig.v3_, trig.v1_, trig.v2_ ), hitPos - trig.v1_ ) < -eps
		);
		//hitDist =  LUV::Length( pos_ - hitPos );
		hitDist =  LUV::Length( LUV::LineNormalP( trig.v1_, trig.v2_, trig.v3_ ) );


		////trig.v1_;
		////trig.v2_;
		////trig.v3_;
		////LUV::Vec3< T >;
		////T;

		////LUV::Length( aaa );
		////LUV::Dot( aaa, bbb );
		////LUV::Cross( aaa, bbb );

		//T kEpsilon = 1E-6;

		//// compute plane's normal
		//LUV::Vec3< T > v0v1 = trig.v2_ - trig.v1_; 
		//LUV::Vec3< T > v0v2 = trig.v3_ - trig.v1_; 
		//// no need to normalize
		//LUV::Vec3< T > N = LUV::Cross( v0v1, v0v2 ); // N 
		//T area2 = LUV::Length( N ); 
 
		//// Step 1: finding P
 
		//// check if ray and plane are parallel ?
		//T NdotRayDirection = LUV::Dot( N, dir_ );
		//if ( LUV::_Abs(NdotRayDirection) < kEpsilon) // almost 0
		//{
		//	std::cout << "1" << std::endl;
		//	isHit = false; // they are parallel so they don't intersect !
		//	return;
		//}

		//// compute d parameter using equation 2
		//T d = LUV::Dot( N, trig.v1_ );
 
		//// compute t (equation 3)
		//hitDist = ( LUV::Dot( N, pos_ ) + d ) / NdotRayDirection;
		//// check if the triangle is in behind the ray
		//if (hitDist < 0)
		//{
		//	std::cout << "2" << std::endl;
		//	isHit = false; // the triangle is behind 
		//	return;
		//}
		//// compute the intersection point using equation 1
		//LUV::Vec3< T > P = pos_ + hitDist * dir_; 
 
		//// Step 2: inside-outside test
		//LUV::Vec3< T > C; // vector perpendicular to triangle's plane 
 
		//// edge 0
		//LUV::Vec3< T > edge0 = trig.v2_ - trig.v1_; 
		//LUV::Vec3< T > vp0 = P - trig.v1_; 
		//C = LUV::Cross( edge0, vp0 );
		//if ( LUV::Dot( N, C ) < 0 )
		//{
		//	std::cout << "3" << std::endl;
		//	isHit = false; // P is on the right side 
		//	return;
		//}
		//// edge 1
		//LUV::Vec3< T > edge1 = trig.v3_ - trig.v2_; 
		//LUV::Vec3< T > vp1 = P - trig.v2_; 
		//C = LUV::Cross( edge1, vp1 );
		//if ( LUV::Dot( N, C ) < 0 )
		//{
		//	std::cout << "4" << std::endl;
		//	isHit = false; // P is on the right side 
		//	return;
		//}
 
		//// edge 2
		//LUV::Vec3< T > edge2 = trig.v1_ - trig.v3_; 
		//LUV::Vec3< T > vp2 = P - trig.v3_; 
		//C = LUV::Cross( edge2, vp2 );
		//if ( LUV::Dot( N, C ) < 0 )
		//{
		//	std::cout << "5" << std::endl;
		//	isHit = false; // P is on the right side 
		//	return;
		//}
		//
		//								std::cout << "HIT!!!" << std::endl;

		//isHit = true; // this ray hits the triangle 
	}

	inline void CollisionWithTriangle____1( const Triangle< T >& trig, bool& isHit, T& hitDist ) restrict( cpu, amp )
	{
		//trig.v1_;
		//trig.v2_;
		//trig.v3_;
		//LUV::Vec3< T >;
		//T;

		//LUV::Length( aaa );
		//LUV::Dot( aaa, bbb );
		//LUV::Cross( aaa, bbb );

		T kEpsilon = 1E-6;

		// compute plane's normal
		LUV::Vec3< T > v0v1 = trig.v2_ - trig.v1_; 
		LUV::Vec3< T > v0v2 = trig.v3_ - trig.v1_; 
		// no need to normalize
		LUV::Vec3< T > N = LUV::Cross( v0v1, v0v2 ); // N 
		T area2 = LUV::Length( N ); 
 
		// Step 1: finding P
 
		// check if ray and plane are parallel ?
		T NdotRayDirection = LUV::Dot( N, dir_ );
		if ( LUV::_Abs(NdotRayDirection) < kEpsilon) // almost 0
		{
			std::cout << "1" << std::endl;
			isHit = false; // they are parallel so they don't intersect !
			return;
		}

		// compute d parameter using equation 2
		T d = LUV::Dot( N, trig.v1_ );
 
		// compute t (equation 3)
		hitDist = ( LUV::Dot( N, pos_ ) + d ) / NdotRayDirection;
		// check if the triangle is in behind the ray
		if (hitDist < 0)
		{
			std::cout << "2" << std::endl;
			isHit = false; // the triangle is behind 
			return;
		}
		// compute the intersection point using equation 1
		LUV::Vec3< T > P = pos_ + hitDist * dir_; 
 
		// Step 2: inside-outside test
		LUV::Vec3< T > C; // vector perpendicular to triangle's plane 
 
		// edge 0
		LUV::Vec3< T > edge0 = trig.v2_ - trig.v1_; 
		LUV::Vec3< T > vp0 = P - trig.v1_; 
		C = LUV::Cross( edge0, vp0 );
		if ( LUV::Dot( N, C ) < 0 )
		{
			std::cout << "3" << std::endl;
			isHit = false; // P is on the right side 
			return;
		}
		// edge 1
		LUV::Vec3< T > edge1 = trig.v3_ - trig.v2_; 
		LUV::Vec3< T > vp1 = P - trig.v2_; 
		C = LUV::Cross( edge1, vp1 );
		if ( LUV::Dot( N, C ) < 0 )
		{
			std::cout << "4" << std::endl;
			isHit = false; // P is on the right side 
			return;
		}
 
		// edge 2
		LUV::Vec3< T > edge2 = trig.v1_ - trig.v3_; 
		LUV::Vec3< T > vp2 = P - trig.v3_; 
		C = LUV::Cross( edge2, vp2 );
		if ( LUV::Dot( N, C ) < 0 )
		{
			std::cout << "5" << std::endl;
			isHit = false; // P is on the right side 
			return;
		}
		
										std::cout << "HIT!!!" << std::endl;

		isHit = true; // this ray hits the triangle 
	}
	
	//inline BoundBox< T > UnionWith( const BoundBox< T >& other ) restrict( cpu, amp )
	//{
	//	return BoundBox< T >( LUV::Min( min_, other.min_ ), LUV::Max( max_, other.max_ ) );
	//}

};

#endif