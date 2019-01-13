#ifndef MORTON_MANAGER_INCLUDED
#define MORTON_MANAGER_INCLUDED

#include "TypeDef.hpp"
#include "Triangle.hpp"
#include "TriangleMesh.hpp"

class MortonIndexPair
{

public:
	U64 mortonCode_;
	U32 trigIndex_;

public:
	MortonIndexPair()
	{

	}

	MortonIndexPair( const U64& mortonCode, const U32& trigIndex ) :
		mortonCode_( mortonCode ),
		trigIndex_( trigIndex )
	{

	}

};

class MortonIndexPairSorter
{

public:
	bool operator ()( const MortonIndexPair& lhs, const MortonIndexPair& rhs )
	{
		return lhs.mortonCode_ < rhs.mortonCode_;
	}

	bool operator ()( const U64& lhs, const MortonIndexPair& rhs )
	{
		return lhs < rhs.mortonCode_;
	}
	
	bool operator ()( const MortonIndexPair& lhs, const U64& rhs )
	{
		return lhs.mortonCode_ < rhs;
	}
	
	bool operator ()( const U64& lhs, const U64& rhs )
	{
		return lhs < rhs;
	}

};



template< class T >
class MortonManager
{
public:
	const TriangleMesh< T >* mesh_;
	std::vector< MortonIndexPair > mortonArray_;

public:
	MortonManager()
	{

	}

	MortonManager( const TriangleMesh< T >* mesh ) :
		mesh_( mesh ),
		mortonArray_()
	{
		mortonArray_.reserve( mesh_->trigCount_ );
	}

	~MortonManager()
	{

	}

	////////////////////////////////////

	inline U64 GenerateMortonCode( const LUV::Vec3< U32 >& integerCoords ) const
	{
		U64 mortonCode = 0;

		for( U32 idBit = 0; idBit < 21; ++idBit )
		{
			U64 i = 1;
			U32 intCoordShift = 31 - idBit;
			U32 mortonXshift = 63 - 3 * idBit;
			U32 mortonYshift = mortonXshift - 1;
			U32 mortonZshift = mortonYshift - 1;

			mortonCode |= ( ( integerCoords[0] & ( i << intCoordShift ) ) >> intCoordShift ) << mortonXshift;
			mortonCode |= ( ( integerCoords[1] & ( i << intCoordShift ) ) >> intCoordShift ) << mortonYshift;
			mortonCode |= ( ( integerCoords[2] & ( i << intCoordShift ) ) >> intCoordShift ) << mortonZshift;
		}

		return mortonCode;
	}

	inline void GetMortonBounds
	(
		const U64& lowerMortonRange, const U64& upperMortonRange,
		const U32& lowerIdxRange, const U32& upperIdxRange,
		U32& lowerIdx, U32& upperIdx
	) const
	{
	/*	const std::vector< MortonIndexPair >::cos beginIt;
		std::vector< MortonIndexPair >::iterator lowerIt, upperIt;*/

		
		//std::cout << lowerIdxRange << std::endl;
		//std::cout << upperIdxRange << std::endl;

		const std::vector< MortonIndexPair >::const_iterator beginIt = mortonArray_.begin();
		const std::vector< MortonIndexPair >::const_iterator lowerIt = std::lower_bound( beginIt + lowerIdxRange, beginIt + upperIdxRange, lowerMortonRange, MortonIndexPairSorter() );
		const std::vector< MortonIndexPair >::const_iterator upperIt = std::upper_bound( beginIt + lowerIdxRange, beginIt + upperIdxRange, upperMortonRange, MortonIndexPairSorter() );
		lowerIdx = (U32)( lowerIt - beginIt );
		upperIdx = (U32)( upperIt - beginIt );

		//std::cout << (U32) (lowerIt - beginIt ) << std::endl;
		
		//std::cout << upperIdx << std::endl;
	}

	////////////////////////////////////

	void GenerateMortonArray()
	{
		LUV::Vec3< T > minF = mesh_->boundSphere_.center_ - mesh_->boundSphere_.radius_;
		LUV::Vec3< T > maxF = mesh_->boundSphere_.center_ + mesh_->boundSphere_.radius_;
		T rangeF = 2 * mesh_->boundSphere_.radius_;

		LUV::Vec3< U32 > minU( 0, 0, 0 );
		LUV::Vec3< U32 > maxU = minU - 1;
		
		U32 trigCount = mesh_->trigArray_.size();
		for( U32 idTrig = 0; idTrig < trigCount; ++idTrig )
		{
			const Triangle< T >& trig = mesh_->trigArray_[ idTrig ];
			LUV::Vec3< T > normalizedCenter = ( trig.GetCenter() - minF ) / rangeF;
			LUV::Vec3< U32 > integerCenter = maxU * normalizedCenter;
			U64 mortonCode = GenerateMortonCode( integerCenter );
			mortonArray_.push_back( MortonIndexPair( mortonCode, idTrig ) );
		}

		std::sort( mortonArray_.begin(), mortonArray_.end(), MortonIndexPairSorter() );
	}

	/*
		Tüm cismi kapsayan küpün geniþliðini bul
		Bu küp 
	*/

	//void GenerateMortonArray_GPU() restrict( amp )
	//{
	//	using namespace concurrency;

	//	LUV::Vec3< T > minF = mesh_->boundSphere_.center_ - mesh_->boundSphere_.radius_;
	//	LUV::Vec3< T > maxF = mesh_->boundSphere_.center_ + mesh_->boundSphere_.radius_;
	//	T rangeF = 2 * mesh_->boundSphere_.radius_;

	//	LUV::Vec3< U32 > minU( 0, 0, 0 );
	//	LUV::Vec3< U32 > maxU = minU - 1;
	//	
	//	U64 trigCount = mesh_->trigArray_.size();
	//	mortonArray_.resize( trigCount );

	//	array_view< const Triangle< T >, 1 > trigArray_GPU( trigCount, mesh_->trigArray_.begin() );
	//	array_view< MortonIndexPair, 1 > mortonArray_GPU( trigCount, mortonArray_.begin() );
	//	mortonArray_GPU.discard_data();

	//	PARFOR( mortonArray_GPU.extent, [=]( index< 1 > idx ) restrict( amp )
	//	{
	//		LUV::Vec3< T > normalizedCenter = ( trigArray_GPU[ idx ].center_ - minF ) / rangeF;
	//		LUV::Vec3< U32 > integerCenter = maxU * normalizedCenter;
	//		U64 mortonCode = GenerateMortonCode( integerCenter );
	//		mortonArray_GPU[ idx ] = MortonIndexPair( mortonCode, idx[0] );
	//	});

	//	mortonArray_GPU.synchronize();

	//	std::sort( mortonArray_.begin(), mortonArray_.end(), MortonIndexPair() );
	//}

};

#endif