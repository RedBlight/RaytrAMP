#ifndef BVH_GENERATOR_INCLUDED
#define BVH_GENERATOR_INCLUDED

#include "TypeDef.hpp"
#include "Triangle.hpp"
#include "TriangleMesh.hpp"
#include "MortonManager.hpp"
#include "BvhNodeTypes.hpp"
#include "ReducedBvhArray.hpp"

template< class T >
class BvhGenerator
{

public:
	const TriangleMesh< T >* mesh_;
	const MortonManager< T >* morton_;
	std::vector< BvhNode< T > > bvhNodeArray_;
	
public:

	BvhGenerator()
	{

	}

	BvhGenerator( const MortonManager< T >* morton ) :
		mesh_( morton->mesh_ ),
		morton_( morton )
	{

	}

	~BvhGenerator()
	{

	}

	void GenerateBvhArray()
	{
		bvhNodeArray_.clear();
		bvhNodeArray_.reserve( 24 * mesh_->trigCount_ );

		bvhNodeArray_.push_back( BvhNode< T >(
			(U64)( 0 ),
			(U64)( 0 ) - (U64)( 1 ),
			0,
			mesh_->trigCount_,
			0,
			0,
			ROOT
		) );
		
		U32 nodeIdx = 0;
		U32 nodeCount = 1;
		while( nodeIdx < nodeCount )
		{
			BvhNode< T >* node = &(bvhNodeArray_[ nodeIdx ]);

			
				//std::cout << std::bitset< 64 >( node->upperMortonRange_ ) << std::endl;

			U32 newLowerIdx;
			U32 newUpperIdx;
			
		//std::cout << node->upperIdxRange_ << std::endl;

			
		//std::cout << node->lowerIdxRange_ << std::endl;
		//std::cout << node->upperIdxRange_ << std::endl;

			morton_->GetMortonBounds( node->lowerMortonRange_, node->upperMortonRange_, node->lowerIdxRange_, node->upperIdxRange_, newLowerIdx, newUpperIdx );
			node->lowerIdxRange_ = newLowerIdx;
			node->upperIdxRange_ = newUpperIdx;
			I32 elemCount = (I32)newUpperIdx - (I32)newLowerIdx;

			
				//std::cout << std::bitset< 64 >( node->upperMortonRange_ ) << std::endl;

			if( elemCount == 0 )
			{
				node->status_ = EMPTY;
			}
			else if( elemCount == 1 )
			{
				node->status_ = LEAF;
				node->trigIdx_ = node->lowerIdxRange_;
			}
			else if( elemCount > 1 )
			{
				node->status_ = BRANCH;

				node->leftChildIdx_ = nodeCount;
				++nodeCount;
				
				node->rightChildIdx_ = nodeCount;
				++nodeCount;
				
				U64 midMortonRange = node->lowerMortonRange_ + ( ( node->upperMortonRange_ - node->lowerMortonRange_ ) >> 1 );
				
				//std::cout << std::bitset< 64 >( node->upperMortonRange_ ) << std::endl;

				U32 loRangeL = node->lowerIdxRange_;
				U32 hiRangeL = node->upperIdxRange_;
				U32 loRangeR = node->lowerIdxRange_;
				U32 hiRangeR = node->upperIdxRange_;

				///
				// if this is the case, you have a bad mesh with tangled triangles.
				if( midMortonRange <= node->lowerMortonRange_ || midMortonRange >= node->upperMortonRange_  )
				{
					//std::cout << "LO MORTON ERROR!!" << std::endl;
					//std::cout << "UP MORTON ERROR!!" << std::endl;
					//std::cout << node->lowerMortonRange_ << std::endl;
					//std::cout << midMortonRange << std::endl;
					//std::cout << node->upperMortonRange_ << std::endl;
					//std::cout << node->lowerIdxRange_ << std::endl;
					//std::cout << node->upperIdxRange_ << std::endl;
					//system( "pause" );
					loRangeL = node->lowerIdxRange_;
					hiRangeL = loRangeL + elemCount / 2;
					loRangeR = hiRangeL;
					hiRangeR = node->upperIdxRange_;
				}
				///

				bvhNodeArray_.push_back( BvhNode< T >(
					node->lowerMortonRange_,
					midMortonRange,
					loRangeL,
					hiRangeL,
					nodeIdx,
					node->rightChildIdx_,
					EMPTY
				) );

				// BE CAREFUL, PUSH BACK MAY RELOCATE THE MEMORY, UPDATE THE POINTER
				BvhNode< T >* node = &(bvhNodeArray_[ nodeIdx ]);

				bvhNodeArray_.push_back( BvhNode< T >(
					midMortonRange + 1,
					node->upperMortonRange_,
					loRangeR,
					hiRangeR,
					nodeIdx,
					node->leftChildIdx_,
					EMPTY
				) );

				

			}
			
			//std::cout << std::bitset< 64 >( node->upperMortonRange_ ) << std::endl;

			++nodeIdx;

			///
			//if( nodeIdx > 1000 )
			//	return;
			///
		}

		bvhNodeArray_[0].status_ = ROOT;

		bvhNodeArray_.shrink_to_fit();


	}

	void SqueezeBvhArray()
	{
		//2 ) remove empties

		//nodeIdx = lastIdx

		//while nodeIdx != 0
	
		//	if I am not empty
		//		if my sibling is empty
		//			clone me propserties to my parent
		//	
		//			if I em not leaf
		//				update the parent idx of my children
		//			set me empty.
		//		else
		//			if I am leaf
		//				set my aabb to my triangle's aabb
		//			update my parent's aabb with mine
		//	--nodeIdx
		//end

		U32 nodeIdx = bvhNodeArray_.size() - 1;

		while( nodeIdx != 0 )
		{
			BvhNode< T >* node = &(bvhNodeArray_[ nodeIdx ]);
			BvhNode< T >* sibling = &(bvhNodeArray_[ node->siblingIdx_ ]);
			BvhNode< T >* parent = &(bvhNodeArray_[ node->parentIdx_ ]);

			if( node->status_ != EMPTY )
			{
				if( sibling->status_ == EMPTY )
				{
					parent->leftChildIdx_ = node->leftChildIdx_;
					parent->rightChildIdx_ = node->rightChildIdx_;
					parent->status_ = node->status_;
					parent->boundBox_ = node->boundBox_;
					parent->bbInitialized_ = node->bbInitialized_;
					parent->trigIdx_ = node->trigIdx_;

					if( node->status_ == BRANCH )
					{
						BvhNode< T >* leftChild = &(bvhNodeArray_[ node->leftChildIdx_ ]);
						BvhNode< T >* rightChild = &(bvhNodeArray_[ node->rightChildIdx_ ]);
						leftChild->parentIdx_ = node->parentIdx_;
						rightChild->parentIdx_ = node->parentIdx_;
					}

					node->status_ = EMPTY;
				}
				else
				{
					if( node->status_ == LEAF )
					{
						node->boundBox_ = mesh_->trigArray_[ node->trigIdx_ ].GetBoundBox();
						node->bbInitialized_ = true;
					}
					if( parent->bbInitialized_ )
					{
						parent->boundBox_ = parent->boundBox_.UnionWith( node->boundBox_ );
					}
					else
					{
						parent->boundBox_ = node->boundBox_;
						parent->bbInitialized_ = true;
					}
				}

			}

			--nodeIdx;
		}

	}

	void RemoveEmptyNodes()
	{
		U32 nodeCount = bvhNodeArray_.size();
		std::vector< U32 > indexDiff( nodeCount, 0 );

		U32 nodeIdx = 1;
		U32 nodeIdxNew = 1;

		while( nodeIdx < nodeCount )
		{
			indexDiff[ nodeIdx ] = indexDiff[ nodeIdx - 1 ];

			if( bvhNodeArray_[ nodeIdx ].status_ != EMPTY )
			{
				bvhNodeArray_[ nodeIdxNew ] = bvhNodeArray_[ nodeIdx ];
				++nodeIdxNew;
			}
			else
			{
				indexDiff[ nodeIdx ] += 1;
			}

			++nodeIdx;
		}

		bvhNodeArray_.resize( nodeIdxNew );
		for( BvhNode< T >& node : bvhNodeArray_ )
		{
			node.parentIdx_ = node.parentIdx_ - indexDiff[ node.parentIdx_ ];
			node.siblingIdx_ = node.siblingIdx_ - indexDiff[ node.siblingIdx_ ];
			if( node.status_ != LEAF )
			{
				node.leftChildIdx_ = node.leftChildIdx_ - indexDiff[ node.leftChildIdx_ ];
				node.rightChildIdx_ = node.rightChildIdx_ - indexDiff[ node.rightChildIdx_ ];
			}
		}

	}

	void PopulateReducedBvhArray( ReducedBvhArray< T >& reducedBvhArray )
	{
		U32 nodeCount = bvhNodeArray_.size();
		reducedBvhArray.Initialize( nodeCount );
		ReducedBvhNode< T >* arrayPtr = reducedBvhArray.bvhNodeArray_.get();
		for( U32 idNode = 0; idNode < nodeCount; ++idNode )
		{
			const BvhNode< T >& node = bvhNodeArray_[ idNode ];
			if( node.status_ == LEAF )
			{
				arrayPtr[ idNode ] = ReducedBvhNode< T >( mesh_->trigArray_[ node.trigIdx_ ] );
			}
			else
			{
				arrayPtr[ idNode ] = ReducedBvhNode< T >( node.status_, node.parentIdx_, node.leftChildIdx_, node.rightChildIdx_, node.boundBox_ );
			}
		}
	}

};
















#endif