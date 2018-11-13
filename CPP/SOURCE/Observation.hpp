#ifndef OBSERVATION_INCLUDED
#define OBSERVATION_INCLUDED

#include "LuVector.hpp"
#include "TypeDef.hpp"

template< class T >
class Observation
{
public:
	LUV::Vec3< T > direction_;
	LUV::Vec3< T > polarization_;
	T frequency_;
	U32 rayPerLam_;

public:
	Observation() restrict( cpu, amp )
	{

	}

	Observation( const LUV::Vec3< T >& direction, const LUV::Vec3< T >& polarization, const T& frequency, const U32& rayPerLam ) restrict( cpu, amp ) :
		direction_( direction ),
		polarization_( polarization ),
		frequency_( frequency ),
		rayPerLam_( rayPerLam )
	{

	}

};

#endif