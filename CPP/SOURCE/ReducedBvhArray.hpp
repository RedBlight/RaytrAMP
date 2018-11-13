#ifndef REDUCED_BVH_ARRAY_INCLUDED
#define REDUCED_BVH_ARRAY_INCLUDED

#include "TypeDef.hpp"
#include "BvhNodeTypes.hpp"

/*
.rba file: each cell is 8 byte
[0] = tetrahedron count (uint64)
[A ... B-1] = eps mu (double) 8 byte X 2 X [0] -> electromagnetic material properties, eps and mu relative to eps0 and mu0

A = 1
B = A + [0]*2
*/

// always double

template< class T >
class ReducedBvhArray
{
public:
	bool init_;
	U32 nodeCount_;
	std::shared_ptr< ReducedBvhNode< T > > bvhNodeArray_;

public:

	ReducedBvhArray() :
		init_( false ),
		nodeCount_( 0 ),
		bvhNodeArray_()
	{

	}

	~ReducedBvhArray()
	{

	}

	void Reset()
	{
		if( init_ )
		{
			init_ = false;
			nodeCount_ = 0;
			bvhNodeArray_.reset();
		}
	}

	void Initialize( const U32& nodeCount )
	{
		Reset();
		nodeCount_ = nodeCount;
		bvhNodeArray_.reset( new ReducedBvhNode< T >[ nodeCount ], []( ReducedBvhNode< T >* ptr ){ delete[] ptr; } );
		init_ = true;
	}

	void Initialize( const U32& nodeCount, const std::shared_ptr< ReducedBvhNode< T > >& bvhNodeArray )
	{
		Reset();
		nodeCount_ = nodeCount;
		bvhNodeArray_ = bvhNodeArray;
		init_ = true;
	}

	bool Load( const std::string& filePath )
	{
		Reset();

		std::fstream rbaFile( filePath, std::ios::in | std::ios::binary );

		if( !rbaFile.good() )
		{
			rbaFile.close();
			return false;
		}

		rbaFile.read( (char*)( &nodeCount_ ), sizeof( U32 ) );

		bvhNodeArray_.reset( new ReducedBvhNode< T >[ nodeCount_ ], []( ReducedBvhNode< T >* ptr ){ delete[] ptr; } );
		rbaFile.read( (char*)( bvhNodeArray_.get() ), sizeof( ReducedBvhNode< T > ) * nodeCount_ );

		rbaFile.close();

		init_ = true;

		return true;
	}

	bool Save( const std::string& filePath )
	{
		std::fstream rbaFile( filePath, std::ios::trunc | std::ios::out | std::ios::binary );

		if( !rbaFile.good() )
		{
			rbaFile.close();
			return false;
		}

		rbaFile.write( (char*)( &nodeCount_ ), sizeof( U32 ) );
		rbaFile.write( (char*)( bvhNodeArray_.get() ), sizeof( ReducedBvhNode< T > ) * nodeCount_ );

		rbaFile.close();

		return true;
	}

};

#endif