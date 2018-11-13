#ifndef RAY_POOL_INCLUDED
#define RAY_POOL_INCLUDED

#include "TypeDef.hpp"
#include "BoundBox.hpp"
#include "RayTube.hpp"

template< class T >
class RayPool
{
public:
	bool init_;
	U32 rayCountSqrt_;
	U32 rayCount_;
	T rayArea_;
	std::shared_ptr< RayTube< T > > rayTubeArray_;

public:

	RayPool() :
		init_( false ),
		rayCountSqrt_( 0 ),
		rayCount_( 0 ),
		rayArea_( 0 ),
		rayTubeArray_()
	{

	}

	~RayPool()
	{

	}

	void Reset()
	{
		if( init_ )
		{
			init_ = false;
			rayCount_ = 0;
			rayCount_ = 0;
			rayArea_ = 0;
			rayTubeArray_.reset();
		}
	}

	void Initialize( const U32& rayCountSqrt )
	{
		Reset();
		rayCountSqrt_ = rayCountSqrt;
		rayCount_ = rayCountSqrt * rayCountSqrt;
		rayArea_ = 0;
		rayTubeArray_.reset( new RayTube< T >[ rayCount_ ], []( RayTube< T >* ptr ){ delete[] ptr; } );
		init_ = true;
	}

	//void Initialize( const U32& rayCountSqrt, std::shared_ptr< RayTube< T > >& rayTubeArray )
	//{
	//	Reset();
	//	rayCountSqrt_ = rayCountSqrt;
	//	rayCount_ = rayCountSqrt * rayCountSqrt;
	//	rayTubeArray_ = rayTubeArray;
	//	init_ = true;
	//}

	void ReGenerateRays
	(
		const BoundBox< T >& boundBox,
		const LUV::Vec3< T >& incDir,
		const LUV::Vec3< T >& polDir
	)
	{
		LUV::Vec3< T > outDirSph( LUV::CtsToSph( -incDir ) );
		LUV::Vec3< T > dirN; // ray direction
		LUV::Vec3< T > dirU;
		LUV::Vec3< T > dirR;
		LUV::OrthonormalSet( outDirSph[1], outDirSph[2], dirN, dirU, dirR );
		LUV::Orthonormalize( dirN, dirU, dirR );

		LUV::Vec3< T > boundBoxCenter = boundBox.GetCenter();
		T boundBoxRadius = boundBox.GetRadius();
		LUV::Vec3< T > rayPoolCenter = boundBoxCenter - dirN * 2.0 * boundBoxRadius;
		LUV::Vec3< T > rayPoolRectMin = rayPoolCenter - ( dirR + dirU ) * boundBoxRadius;
		LUV::Vec3< T > rayPoolRectMax = rayPoolCenter + ( dirR + dirU ) * boundBoxRadius;

		T rayTubeRadius = boundBoxRadius / (T)rayCountSqrt_;
		T rayTubeDiameter = rayTubeRadius * 2.0;
		LUV::Vec3< T > rayPosStepU = rayTubeDiameter * dirU;
		LUV::Vec3< T > rayPosStepR = rayTubeDiameter * dirR;
		LUV::Vec3< T > rayPosBegin = rayPoolRectMin + ( rayPosStepU + rayPosStepR ) / 2.0;

		rayArea_ = rayTubeDiameter * rayTubeDiameter;

		RayTube< T >* rayArray = rayTubeArray_.get();

		for( U32 idU = 0; idU < rayCountSqrt_; ++idU )
		{
			for( U32 idR = 0; idR < rayCountSqrt_; ++idR )
			{
				U32 idRay = idR + rayCountSqrt_ * idU;
				rayArray[ idRay ].pos_ = rayPosBegin + rayPosStepU * (T)idU + rayPosStepR * (T)idR;
				rayArray[ idRay ].dir_ = dirN;
				rayArray[ idRay ].pol_ = polDir;
				rayArray[ idRay ].dist_ = 1E36;
				rayArray[ idRay ].refCount_ = 0;
				rayArray[ idRay ].lastHitIdx_ = -1;
			}
		}

	}

	void ReGenerateRaysRand
	(
		const BoundBox< T >& boundBox,
		const LUV::Vec3< T >& incDir,
		const LUV::Vec3< T >& polDir
	)
	{
		LUV::Vec3< T > outDirSph( LUV::CtsToSph( -incDir ) );
		LUV::Vec3< T > dirN; // ray direction
		LUV::Vec3< T > dirU;
		LUV::Vec3< T > dirR;
		LUV::OrthonormalSet( outDirSph[1], outDirSph[2], dirN, dirU, dirR );
		LUV::Orthonormalize( dirN, dirU, dirR );

		LUV::Vec3< T > boundBoxCenter = boundBox.GetCenter();
		T boundBoxRadius = boundBox.GetRadius();
		LUV::Vec3< T > rayPoolCenter = boundBoxCenter - dirN * 2.0 * boundBoxRadius;
		LUV::Vec3< T > rayPoolRectMin = rayPoolCenter - ( dirR + dirU ) * boundBoxRadius;
		LUV::Vec3< T > rayPoolRectMax = rayPoolCenter + ( dirR + dirU ) * boundBoxRadius;

		T rayTubeRadius = boundBoxRadius / (T)rayCountSqrt_;
		T rayTubeDiameter = rayTubeRadius * 2.0;
		LUV::Vec3< T > rayPosStepU = rayTubeDiameter * dirU;
		LUV::Vec3< T > rayPosStepR = rayTubeDiameter * dirR;
		LUV::Vec3< T > rayPosBegin = rayPoolRectMin + ( rayPosStepU + rayPosStepR ) / 2.0;

		rayArea_ = rayTubeDiameter * rayTubeDiameter;

		RayTube< T >* rayArray = rayTubeArray_.get();





		std::srand(std::time(0));


		std::vector< U32 > mixIdRay;
		mixIdRay.reserve( rayCount_ );
		for( U32 idx = 0; idx < rayCount_; idx++ )
		{
			mixIdRay.push_back( idx );
		}
		mixIdRay.shrink_to_fit();
		std::random_shuffle( mixIdRay.begin(), mixIdRay.end() );




		for( U32 idU = 0; idU < rayCountSqrt_; ++idU )
		{
			for( U32 idR = 0; idR < rayCountSqrt_; ++idR )
			{
				U32 idRay = idR + rayCountSqrt_ * idU;

				U32 idMix = mixIdRay[ idRay ];
				U32 idMixR = idMix % rayCountSqrt_;
				U32 idMixU = ( idMix - idMixR ) / rayCountSqrt_;

				//std::cout << idMix << std::endl;

				rayArray[ idRay ].pos_ = rayPosBegin + rayPosStepU * (T)idMixU + rayPosStepR * (T)idMixR;
				rayArray[ idRay ].dir_ = dirN;
				rayArray[ idRay ].pol_ = polDir;
				rayArray[ idRay ].dist_ = 1E36;
				rayArray[ idRay ].refCount_ = 0;
				rayArray[ idRay ].lastHitIdx_ = -1;
			}
		}


	}


};

#endif