#ifndef TRIANGLE_MESH_INCLUDED
#define TRIANGLE_MESH_INCLUDED

#include <vector>
#include <algorithm>
#include <fstream>

#include "TypeDef.hpp"
#include "BoundBox.hpp"
#include "BoundSphere.hpp"
#include "Triangle.hpp"
#include "UnvTrigMeshFile.hpp"

template< class T >
class TriangleMesh
{
public:
	U32 trigCount_;
	std::vector< Triangle< T > > trigArray_;
	BoundSphere< T > boundSphere_;
	BoundBox< T > boundBox_;

public:
	TriangleMesh()
	{

	}

	TriangleMesh( const U32& reservedTrigCount ) :
		trigCount_( 0 ),
		trigArray_(),
		boundSphere_(),
		boundBox_()
	{
		trigArray_.reserve( reservedTrigCount );
	}

	~TriangleMesh()
	{

	}

	void Reset( const U32& reservedTrigCount )
	{
		trigCount_ = 0;
		trigArray_.clear();
		trigArray_.reserve( reservedTrigCount );
		boundSphere_ = BoundSphere< T >();
		boundBox_ = BoundBox< T >();
	}

	void CalculateBounds()
	{
		boundBox_ = trigArray_[0].GetBoundBox();
		for( const Triangle< T >& trig : trigArray_ )
		{
			boundBox_ = boundBox_.UnionWith( trig.GetBoundBox() );
		}

		boundSphere_.center_ = ( boundBox_.min_ + boundBox_.max_ ) / 2;
		boundSphere_.radius_ = LUV::Length( boundBox_.max_ - boundSphere_.center_ );
	}

	void InsertTrig( const Triangle< T >& trig )
	{
		trigArray_.push_back( trig );
		trigCount_ = trigArray_.size();
	}

	void ImportFromUnvTrigMeshFile( const UnvTrigMeshFile< T >& unvTrigMeshFile )
	{
		Reset( unvTrigMeshFile.trigCount_ );

		T* vertexPtr = unvTrigMeshFile.vertexData_.get();
		U32* indexPtr = unvTrigMeshFile.trigVertexIndex_.get();

		for( U32 trigIndex = 0; trigIndex < unvTrigMeshFile.trigCount_; ++trigIndex )
		{
			U32 indexV1 = trigIndex * 3;
			U32 indexV2 = indexV1 + 1;
			U32 indexV3 = indexV2 + 1;

			U32 indexV1x = indexPtr[ indexV1 ] * 3;
			U32 indexV1y = indexV1x + 1;
			U32 indexV1z = indexV1y + 1;

			U32 indexV2x = indexPtr[ indexV2 ] * 3;
			U32 indexV2y = indexV2x + 1;
			U32 indexV2z = indexV2y + 1;

			U32 indexV3x = indexPtr[ indexV3 ] * 3;
			U32 indexV3y = indexV3x + 1;
			U32 indexV3z = indexV3y + 1;

			LUV::Vec3< T > v1( vertexPtr[ indexV1x ], vertexPtr[ indexV1y ], vertexPtr[ indexV1z ] );
			LUV::Vec3< T > v2( vertexPtr[ indexV2x ], vertexPtr[ indexV2y ], vertexPtr[ indexV2z ] );
			LUV::Vec3< T > v3( vertexPtr[ indexV3x ], vertexPtr[ indexV3y ], vertexPtr[ indexV3z ] );

			trigArray_.push_back( Triangle< T >( v1, v2, v3 ) );
		}
		
		trigCount_ = trigArray_.size();
		CalculateBounds();
	}	

};

#endif