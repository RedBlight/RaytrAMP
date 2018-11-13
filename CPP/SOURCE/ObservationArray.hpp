#ifndef OBSERVATION_ARRAY_INCLUDED
#define OBSERVATION_ARRAY_INCLUDED

#include "TypeDef.hpp"
#include "Observation.hpp"

/*
.obs file for T=F32:
[0] = obs count (U32)
[A ... B-1] = Observation ( 8 * 4 byte )

A = 1
B = A + [0]*8
*/

// always double

template< class T >
class ObservationArray
{
public:
	bool init_;
	U32 obsCount_;
	std::shared_ptr< Observation< T > > observationArray_;

public:

	ObservationArray() :
		init_( false ),
		obsCount_( 0 ),
		observationArray_()
	{

	}

	~ObservationArray()
	{

	}

	void Reset()
	{
		if( init_ )
		{
			init_ = false;
			obsCount_ = 0;
			observationArray_.reset();
		}
	}

	void Initialize( const U32& obsCount )
	{
		Reset();
		obsCount_ = obsCount;
		observationArray_.reset( new Observation< T >[ obsCount ], []( Observation< T >* ptr ){ delete[] ptr; } );
		init_ = true;
	}

	void Initialize( const U32& obsCount, const std::shared_ptr< Observation< T > >& observationArray )
	{
		Reset();
		obsCount_ = obsCount;
		observationArray_ = observationArray;
		init_ = true;
	}

	bool Load( const std::string& filePath )
	{
		Reset();

		std::fstream obsFile( filePath, std::ios::in | std::ios::binary );

		if( !obsFile.good() )
		{
			obsFile.close();
			return false;
		}

		obsFile.read( (char*)( &obsCount_ ), sizeof( U32 ) );

		observationArray_.reset( new Observation< T >[ obsCount_ ], []( Observation< T >* ptr ){ delete[] ptr; } );
		obsFile.read( (char*)( observationArray_.get() ), sizeof( Observation< T > ) * obsCount_ );

		obsFile.close();

		init_ = true;

		return true;
	}

	bool Save( const std::string& filePath )
	{
		std::fstream obsFile( filePath, std::ios::trunc | std::ios::out | std::ios::binary );

		if( !obsFile.good() )
		{
			obsFile.close();
			return false;
		}

		obsFile.write( (char*)( &obsCount_ ), sizeof( U32 ) );
		obsFile.write( (char*)( observationArray_.get() ), sizeof( Observation< T > ) * obsCount_ );

		obsFile.close();

		return true;
	}

};

#endif