#ifndef RCS_ARRAY_INCLUDED
#define RCS_ARRAY_INCLUDED

#include "TypeDef.hpp"

/*
.rcs file for T=F32:
[0] = rcs count (U32)
[A ... B-1] =  4 byte

A = 1
B = A + [0]*8
*/

// always double

template< class T >
class RcsArray
{
public:
	bool init_;
	U32 rcsCount_;
	std::shared_ptr< T > rcsArray_;

public:

	RcsArray() :
		init_( false ),
		rcsCount_( 0 ),
		rcsArray_()
	{

	}

	~RcsArray()
	{

	}

	void Reset()
	{
		if( init_ )
		{
			init_ = false;
			rcsCount_ = 0;
			rcsArray_.reset();
		}
	}

	void Initialize( const U32& rcsCount )
	{
		Reset();
		rcsCount_ = rcsCount;
		rcsArray_.reset( new T[ rcsCount ], []( T* ptr ){ delete[] ptr; } );
		init_ = true;
	}

	void Initialize( const U32& rcsCount, const std::shared_ptr< T >& rcsArray )
	{
		Reset();
		rcsCount_ = rcsCount;
		rcsArray_ = rcsArray;
		init_ = true;
	}

	bool Load( const std::string& filePath )
	{
		Reset();

		std::fstream rcsFile( filePath, std::ios::in | std::ios::binary );

		if( !rcsFile.good() )
		{
			rcsFile.close();
			return false;
		}

		rcsFile.read( (char*)( &rcsCount_ ), sizeof( U32 ) );

		rcsArray_.reset( new T[ rcsCount_ ], []( T* ptr ){ delete[] ptr; } );
		rcsFile.read( (char*)( rcsArray_.get() ), sizeof( T ) * rcsCount_ );

		rcsFile.close();

		init_ = true;

		return true;
	}

	bool Save( const std::string& filePath )
	{
		std::fstream rcsFile( filePath, std::ios::trunc | std::ios::out | std::ios::binary );

		if( !rcsFile.good() )
		{
			rcsFile.close();
			return false;
		}

		rcsFile.write( ( char* )( &rcsCount_ ), sizeof( U32 ) );
		rcsFile.write( ( char* )( rcsArray_.get() ), sizeof( T ) * rcsCount_ );

		rcsFile.close();

		return true;
	}

};

#endif