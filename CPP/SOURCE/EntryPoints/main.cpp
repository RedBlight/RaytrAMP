#include <amp.h>
#include <amp_math.h>
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>

#include <vector>
#include <algorithm>
#include <fstream>
#include <bitset>

typedef std::chrono::high_resolution_clock Clock;

#include "TypeDef.hpp"
#include "Triangle.hpp"
#include "TriangleMesh.hpp"
#include "MortonManager.hpp"
#include "BvhGenerator.hpp"
#include "BvhNodeTypes.hpp"
#include "ReducedBvhArray.hpp"
#include "RayPool.hpp"


#include "DepthMapGenerator.hpp"

#define TT F32


int main()
{
	using namespace concurrency;
	std::vector<accelerator> accs = accelerator::get_all();

	for( const accelerator& acc : accs )
	{
		std::wcout << acc.description << std::endl;
	}

	accelerator::set_default( accs[0].device_path );




	std::string fileName = "sphere1r287580";
	// std::string fileName = "sphere1r11256"; //337ms


	UnvTrigMeshFile< TT > unvTrigMeshFile;
	unvTrigMeshFile.Load( fileName + ".unv" );

	TriangleMesh< TT > trigMesh;
	trigMesh.ImportFromUnvTrigMeshFile( unvTrigMeshFile );

				auto t1 = Clock::now();

	MortonManager< TT > mortonManager( &trigMesh );
	mortonManager.GenerateMortonArray();

	BvhGenerator< TT > bvhGenerator( &mortonManager );
	bvhGenerator.GenerateBvhArray();
	bvhGenerator.SqueezeBvhArray();
	bvhGenerator.RemoveEmptyNodes();

	ReducedBvhArray< TT > reducedBvhArray;
	bvhGenerator.PopulateReducedBvhArray( reducedBvhArray );

				auto t2 = Clock::now();
				std::cout << "RBA GENERATION: " 
					  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
					  << " ms." << std::endl;



	reducedBvhArray.Save( fileName + ".rba" );
	reducedBvhArray.Load( fileName + ".rba" );

	RayPool< TT > rayPool;
	rayPool.Initialize( 3000 );

	DepthMapGenerator< TT > dmGen;

	// CPU
	rayPool.ReGenerateRays( reducedBvhArray.bvhNodeArray_.get()[ 0 ].data_.boundBox_, LUV::Unit( LUV::Vec3< TT >( 1, 1, 1 ) ), LUV::Vec3< TT >( 0, 0, 1 ) );

				auto t3 = Clock::now();

	dmGen.ProcessRaysSBR_BVH_GPU( reducedBvhArray, rayPool );

				auto t4 = Clock::now();
				std::cout << "CPU: " 
					  << std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count()
					  << " ms." << std::endl;
				
	dmGen.GenerateImage( reducedBvhArray, rayPool, fileName + "CPU" );

	// GPU
	rayPool.ReGenerateRays( reducedBvhArray.bvhNodeArray_.get()[ 0 ].data_.boundBox_, LUV::Unit( LUV::Vec3< TT >( 1, 1, 1 ) ), LUV::Unit( LUV::Vec3< TT >( 1, -0.5, -0.5 ) ) );

				auto t5 = Clock::now();

	dmGen.ProcessRaysSBR_BVH_GPU( reducedBvhArray, rayPool );

				auto t6 = Clock::now();
				std::cout << "GPU: " 
					  << std::chrono::duration_cast<std::chrono::milliseconds>(t6 - t5).count()
					  << " ms." << std::endl;


	dmGen.GenerateImage( reducedBvhArray, rayPool, fileName + "GPU" );

	
				std::cout << "GAIN: " 
					  << std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count() / std::chrono::duration_cast<std::chrono::milliseconds>(t6 - t5).count()
					  << "x" << std::endl;

				
				TT pi = 3.14159265359;
				std::cout << "RCS: " << dmGen.ReduceRcs( rayPool, pi/2, 0 );


		//for( U64 idBvh = 0; idBvh < reducedBvhArray.nodeCount_; ++idBvh )
		//{
		//	std::cout << sizeof( ReducedBvhNode<F64> ) << std::endl;
		//	std::cout << idBvh << " " << reducedBvhArray.bvhNodeArray_.get()[ idBvh ].status_ << std::endl;
		//	//<< " " << bvhGenerator.bvhNodeArray_[ idBvh ].parentIdx_ << " " << bvhGenerator.bvhNodeArray_[ idBvh ].lowerIdxRange_ << " " << bvhGenerator.bvhNodeArray_[ idBvh ].upperIdxRange_ << std::endl;
		//	//std::cout << bvhGenerator.bvhNodeArray_[ idBvh ].boundBox_.min_ << std::endl;
		//	//std::cout << bvhGenerator.bvhNodeArray_[ idBvh ].boundBox_.max_ << std::endl;
		//}

	/*
	
	RayPool< F64 > rayPool;
	rayPool.Generate
	
	
	
	*/

	//RayTubeManager< F64 > rayTubeManager;


	//RayShooter< F64 > rayShooter;
	//ShootRays( bvhArray,  )

	//for( U32 idBvh = 0; idBvh < bvhGenerator.bvhNodeArray_.size(); ++idBvh )
	//{
	//	ReducedBvhNode< F64 >* nda = reducedBvhArray.bvhNodeArray_.get();
	//	std::cout << idBvh << " " << nda[idBvh].status_ << " " << bvhGenerator.bvhNodeArray_[ idBvh ].parentIdx_ << " " << bvhGenerator.bvhNodeArray_[ idBvh ].lowerIdxRange_ << " " << bvhGenerator.bvhNodeArray_[ idBvh ].upperIdxRange_ << std::endl;
	//		
	//}

	
		//for( U64 idTrig = 0; idTrig < trigMesh.trigCount_; ++idTrig )
		//{
		//	std::cout << std::bitset< 64 >( mortonManager.mortonArray_[ idTrig ].mortonCode_ ) << std::endl;
		//	std::cout << mortonManager.mortonArray_[ idTrig ].trigIndex_ << std::endl;
		//}

		
		//for( U64 idBvh = 0; idBvh < bvhGenerator.bvhNodeArray_.size(); ++idBvh )
		//{
		//	std::cout << idBvh << " " << bvhGenerator.bvhNodeArray_[ idBvh ].status_ << " " << bvhGenerator.bvhNodeArray_[ idBvh ].parentIdx_ << " " << bvhGenerator.bvhNodeArray_[ idBvh ].lowerIdxRange_ << " " << bvhGenerator.bvhNodeArray_[ idBvh ].upperIdxRange_ << std::endl;
		//	

		//	//if( bvhGenerator.bvhNodeArray_[ idBvh ].status_  == 1  ) {
		//	//std::cout << idBvh << " " << bvhGenerator.bvhNodeArray_[ idBvh ].status_ << " " << bvhGenerator.bvhNodeArray_[ idBvh ].parentIdx_ << " " << bvhGenerator.bvhNodeArray_[ idBvh ].lowerIdxRange_ << " " << bvhGenerator.bvhNodeArray_[ idBvh ].upperIdxRange_ << std::endl;
		//	//std::cout << std::bitset< 64 >( bvhGenerator.bvhNodeArray_[ idBvh ].lowerMortonRange_ ) << std::endl;
		//	//std::cout << std::bitset< 64 >( bvhGenerator.bvhNodeArray_[ idBvh ].upperMortonRange_ ) << std::endl;
		//	//}
		//}


		//for( U64 idBvh = 0; idBvh < bvhGenerator.bvhNodeArray_.size(); ++idBvh )
		//{
		//	if( bvhGenerator.bvhNodeArray_[ idBvh ].status_  == 1  ) {
		//	std::cout << idBvh << " " << bvhGenerator.bvhNodeArray_[ idBvh ].status_ << " " << bvhGenerator.bvhNodeArray_[ idBvh ].parentIdx_ << " " << bvhGenerator.bvhNodeArray_[ idBvh ].lowerIdxRange_ << " " << bvhGenerator.bvhNodeArray_[ idBvh ].upperIdxRange_ << std::endl;
		//	//std::cout << std::bitset< 64 >( bvhGenerator.bvhNodeArray_[ idBvh ].lowerMortonRange_ ) << std::endl;
		//	//std::cout << std::bitset< 64 >( bvhGenerator.bvhNodeArray_[ idBvh ].upperMortonRange_ ) << std::endl;
		//	std::cout << bvhGenerator.bvhNodeArray_[ idBvh ].boundBox_.min_ << std::endl;
		//	}
		//}

		//
		//std::cout << std::endl;

		//for( U64 idBvh = 0; idBvh < bvhGenerator.bvhNodeArray_.size(); ++idBvh )
		//{
		//	std::cout << idBvh << " " << bvhGenerator.bvhNodeArray_[ idBvh ].status_ << " " << bvhGenerator.bvhNodeArray_[ idBvh ].parentIdx_ << " " << bvhGenerator.bvhNodeArray_[ idBvh ].lowerIdxRange_ << " " << bvhGenerator.bvhNodeArray_[ idBvh ].upperIdxRange_ << std::endl;
		//	std::cout << bvhGenerator.bvhNodeArray_[ idBvh ].boundBox_.min_ << std::endl;
		//	std::cout << bvhGenerator.bvhNodeArray_[ idBvh ].boundBox_.max_ << std::endl;
		//}

		//std::cout << std::endl;

		//std::cout << bvhGenerator.bvhNodeArray_[ 0 ].boundBox_.min_ << std::endl;
		//std::cout << bvhGenerator.bvhNodeArray_[ 0 ].boundBox_.max_ << std::endl;

		//std::cout << trigMesh.boundBox_.min_ << std::endl;
		//std::cout << trigMesh.boundBox_.max_ << std::endl;

		//std::cout << LUV::Min( LUV::Vec3< F64 >( -1, -2, -3 ), LUV::Vec3< F64 >( -9, 5, -4 ) ) << std::endl;


	std::system("pause");

	return 0;
}  