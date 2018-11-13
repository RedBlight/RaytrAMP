#ifndef BVH_NODE_TYPES_INCLUDED
#define BVH_NODE_TYPES_INCLUDED

#include "TypeDef.hpp"
#include "Triangle.hpp"

enum BvhNodeStatus : U32
{
	EMPTY = 1,
	LEAF = 2,
	BRANCH = 4,
	ROOT = 8
};

template< class T >
class BvhNode
{

public:
	U64 lowerMortonRange_;
	U64 upperMortonRange_;
	U32 lowerIdxRange_;
	U32 upperIdxRange_;

	U32 parentIdx_;
	U32 siblingIdx_;
	U32 leftChildIdx_;
	U32 rightChildIdx_;

	BvhNodeStatus status_;

	BoundBox< T > boundBox_;
	bool bbInitialized_;

	U32 trigIdx_;

public:
	
	BvhNode()
	{

	}

	BvhNode
	(
		const U64& lowerMortonRange,
		const U64& upperMortonRange,
		const U32& lowerIdxRange,
		const U32& upperIdxRange,
		const U32& parentIdx,
		const U32& siblingIdx,
		const BvhNodeStatus& status
	) :
		lowerMortonRange_( lowerMortonRange ),
		upperMortonRange_( upperMortonRange ),
		lowerIdxRange_( lowerIdxRange ),
		upperIdxRange_( upperIdxRange ),
		parentIdx_( parentIdx ),
		siblingIdx_( siblingIdx ),
		status_( status ),
		boundBox_(),
		bbInitialized_(false),
		trigIdx_()
	{

	}
	
	~BvhNode()
	{

	}

};

template< class T >
class ReducedBvhNodeData
{

public:
	BoundBox< T > boundBox_;
	U32 parentIdx_;
	U32 leftChildIdx_;
	U32 rightChildIdx_;

public:

	ReducedBvhNodeData() restrict( cpu, amp )
	{

	}
	

	ReducedBvhNodeData
	(
		const U32& parentIdx,
		const U32& leftChildIdx,
		const U32& rightChildIdx,
		const BoundBox< T >& boundBox
	) restrict( cpu, amp ) :
		parentIdx_( parentIdx ),
		leftChildIdx_( leftChildIdx ),
		rightChildIdx_( rightChildIdx ),
		boundBox_( boundBox )
	{

	}

	
	~ReducedBvhNodeData() restrict( cpu, amp )
	{

	}

};

template< class T >
class ReducedBvhNode
{

public:
	BvhNodeStatus status_;
	union
	{
		ReducedBvhNodeData< T > data_;
		Triangle< T > trig_;
	};

public:

	ReducedBvhNode() restrict( cpu, amp )
	{

	}
	

	ReducedBvhNode( const Triangle< T >& trig ) restrict( cpu, amp ) :
		status_( LEAF ),
		trig_( trig )
	{

	}

	ReducedBvhNode
	(
		const BvhNodeStatus& status,
		const U32& parentIdx,
		const U32& leftChildIdx,
		const U32& rightChildIdx,
		const BoundBox< T >& boundBox
	) restrict( cpu, amp ) :
		status_( status ),
		data_( parentIdx, leftChildIdx, rightChildIdx, boundBox )
	{

	}

	~ReducedBvhNode() restrict( cpu, amp )
	{

	}	
	
};

#endif