#ifndef UNV_TRIG_MESH_FILE_INCLUDED
#define UNV_TRIG_MESH_FILE_INCLUDED

#include <fstream>
#include <string>
#include <algorithm> 
#include <cctype>
#include <locale>
#include <vector>
#include <sstream>
#include <utility>
#include <cstdint>
#include <memory>

#include "TypeDef.hpp"
#include "StringFuncs.hpp"

/*

.unv file for triangle mesh:
### FILE START ###		-> ASCII file.
***
vertexStartStr			-> Coordinates of vertices.
	//useless line
	{}v1x{}v1y{}v2z{}
	//useless line
	{}v2x{}v2y{}v2z{}	-> repeat for the number of vertices.
vertexEndStr
***
trigStartStr			-> Indices of triangle vertices, starting from 1
	//useless line
	{}t1v1{}t1v2{}t1v3{}
	//useless line
	{}t2v1{}t2v2{}t2v3{}	-> Repeat for the number of triangle.
trigEndStr
***
### FILE END ###

{}	-> denotes varying amounts of whitespace
***	-> denotes various amount of useless line

string vertexStartStr = "  2411";
string vertexEndStr = "    -1";
string trigStartStr = "  2412";
string trigEndStr = "    -1";

*/

// always double
template< class T >
class UnvTrigMeshFile
{
public:
	bool init_;
	U32 vertexCount_;
	U32 trigCount_;
	std::shared_ptr< T > vertexData_;
	std::shared_ptr< U32 > trigVertexIndex_;

	static std::string vertexStartStr_;
	static std::string vertexEndStr_;
	static std::string trigStartStr_;
	static std::string trigEndStr_;

public:

	UnvTrigMeshFile() :
		init_( false ),
		vertexCount_( 0 ),
		trigCount_( 0 ),
		vertexData_(),
		trigVertexIndex_()
	{

	}

	~UnvTrigMeshFile()
	{

	}

	void Reset()
	{
		if( init_ )
		{
			init_ = false;
			vertexCount_ = 0;
			trigCount_ = 0;
			vertexData_.reset();
			trigVertexIndex_.reset();
		}
	}

	bool Load( const std::string& filePath )
	{
		using namespace StringFuncs;

		Reset();

		std::fstream unvFile( filePath, std::ios::in );
		
		/*
			Perhaps do some sanity checks here etc.
		*/

		//string vertexStartStr = "  2411";
		//string vertexEndStr = "    -1";
		//string trigStartStr = "  2412";
		//string trigEndStr = "    -1";

		std::string lineStr = "";

		while( lineStr.compare( vertexStartStr_ ) != 0 )
		{
			std::getline( unvFile, lineStr );
		}

		while( lineStr.compare( vertexEndStr_ ) != 0 )
		{
			std::getline( unvFile, lineStr );
			++vertexCount_;
		}

		vertexCount_ = ( vertexCount_ - 1 ) / 2;

		while( lineStr.compare( trigStartStr_ ) != 0 )
		{
			std::getline( unvFile, lineStr );
		}

		while( lineStr.compare( trigEndStr_ ) != 0 )
		{
			std::getline( unvFile, lineStr );
			++trigCount_;
		}

		trigCount_ = ( trigCount_ - 1 ) / 2;

		vertexData_.reset( new T[ 3 * vertexCount_ ], []( T* ptr ){ delete[] ptr; } );
		trigVertexIndex_.reset( new U32[ 3 * trigCount_ ], []( U32* ptr ){ delete[] ptr; } );

		T* vertexDataPtr = vertexData_.get();
		U32* trigVertexIndexPtr = trigVertexIndex_.get();

		unvFile.clear();
		unvFile.seekg( 0, ios::beg );

		while( lineStr.compare( vertexStartStr_ ) != 0 )
		{
			std::getline( unvFile, lineStr );
		}

		for( U32 idv = 0; idv < vertexCount_; ++idv )
		{
			std::getline( unvFile, lineStr );
			std::getline( unvFile, lineStr );
			Trim( lineStr );
			SearchAndReplace( lineStr, "    ", " " );
			SearchAndReplace( lineStr, "   ", " " );
			SearchAndReplace( lineStr, "  ", " " );
			SearchAndReplace( lineStr, "  ", " " );
			SearchAndReplace( lineStr, "  ", " " );
			SearchAndReplace( lineStr, "  ", " " );
			SearchAndReplace( lineStr, "  ", " " );
			SearchAndReplace( lineStr, "D", "E" );
			vector< string > vCoords = Explode( lineStr, ' ' );

			U32 idx = 3 * idv;
			U32 idy = idx + 1;
			U32 idz = idy + 1;
			vertexDataPtr[ idx ] = ( T )( stod( vCoords[ 0 ] ) );
			vertexDataPtr[ idy ] = ( T )( stod( vCoords[ 1 ] ) );
			vertexDataPtr[ idz ] = ( T )( stod( vCoords[ 2 ] ) );
		}

		while( lineStr.compare( trigStartStr_ ) != 0 )
		{
			std::getline( unvFile, lineStr );
		}

		for( U32 idt = 0; idt < trigCount_; ++idt )
		{
			std::getline( unvFile, lineStr );
			std::getline( unvFile, lineStr );
			Trim( lineStr );
			SearchAndReplace( lineStr, "    ", " " );
			SearchAndReplace( lineStr, "   ", " " );
			SearchAndReplace( lineStr, "  ", " " );
			SearchAndReplace( lineStr, "  ", " " );
			SearchAndReplace( lineStr, "  ", " " );
			SearchAndReplace( lineStr, "  ", " " );
			SearchAndReplace( lineStr, "  ", " " );
			vector< string > vIndices = Explode( lineStr, ' ' );

			U32 idv1 = 3 * idt;
			U32 idv2 = idv1 + 1;
			U32 idv3 = idv2 + 1;
			trigVertexIndexPtr[ idv1 ] = ( U32 )( std::stoull( vIndices[ 0 ] ) ) - 1; // make sure that indices start with 0
			trigVertexIndexPtr[ idv2 ] = ( U32 )( std::stoull( vIndices[ 1 ] ) ) - 1;
			trigVertexIndexPtr[ idv3 ] = ( U32 )( std::stoull( vIndices[ 2 ] ) ) - 1;
			///
			if( trigVertexIndexPtr[ idv1 ] >= vertexCount_ ) std::cout << "ERROR in TrigMeshFile!" << std::endl;
			if( trigVertexIndexPtr[ idv2 ] >= vertexCount_ ) std::cout << "ERROR in TrigMeshFile!" << std::endl;
			if( trigVertexIndexPtr[ idv3 ] >= vertexCount_ ) std::cout << "ERROR in TrigMeshFile!" << std::endl; 
			///
		}

		unvFile.close();

		init_ = true;

		return true;
	}

};

template< class T >
std::string UnvTrigMeshFile< T >::vertexStartStr_ = "  2411";

template< class T >
std::string UnvTrigMeshFile< T >::vertexEndStr_ = "    -1";

template< class T >
std::string UnvTrigMeshFile< T >::trigStartStr_ = "  2412";

template< class T >
std::string UnvTrigMeshFile< T >::trigEndStr_ = "    -1";

#endif