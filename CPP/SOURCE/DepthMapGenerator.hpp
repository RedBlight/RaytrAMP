//#ifndef DEPTH_MAP_GENERATOR_INCLUDED
//#define DEPTH_MAP_GENERATOR_INCLUDED
//
//#include <complex>
//
//#include "TypeDef.hpp"
//#include "Triangle.hpp"
//#include "TriangleMesh.hpp"
//#include "MortonManager.hpp"
//#include "BvhNodeTypes.hpp"
//#include "ReducedBvhArray.hpp"
//#include "RayPool.hpp"
//
//template< class T >
//class DepthMapGenerator
//{
//
//public:
//	
//public:
//
//	DepthMapGenerator()
//	{
//
//	}
//
//	~DepthMapGenerator()
//	{
//
//	}
//
//	void ProcessRays_BVH( const ReducedBvhArray< T >& bvhArray, RayPool< T >& rayPool )
//	{
//		ReducedBvhNode< T >* bvhPtr = bvhArray.bvhNodeArray_.get();
//		RayTube< T >* rayPtr = rayPool.rayTubeArray_.get();
//
//		for( U32 idRay = 0; idRay < rayPool.rayCount_; ++idRay )
//		{
//			RayTube< T >& ray = rayPtr[ idRay ];
//
//			U32 stackArray[ 128 ];
//			I32 stackIdx = 0;
//			stackArray[ stackIdx ] = 0;
//
//			while( stackIdx >= 0 )
//			{
//				ReducedBvhNode< T >& node = bvhPtr[ stackArray[ stackIdx ] ];
//				
//				bool isHit;
//				T hitDist;
//
//				if( node.status_ == BRANCH || node.status_ == ROOT )
//				{
//																//		std::cout << "AAAAaaa" << std::endl;
//					ray.CollisionWithBoundBox( node.data_.boundBox_, isHit, hitDist );
//					if( isHit && hitDist < ray.dist_ )
//					{
//													// std::cout << "A" << std::endl;
//						stackArray[ stackIdx ] = node.data_.leftChildIdx_;
//						++stackIdx;
//						stackArray[ stackIdx ] = node.data_.rightChildIdx_;
//						++stackIdx;
//					}
//					else
//					{
//						//std::cout << "AAAAaaa" << std::endl;
//					}
//				}
//				else if( node.status_ == LEAF )
//				{
//					ray.CollisionWithTriangle( node.trig_, isHit, hitDist );
//					if( isHit && hitDist < ray.dist_ )
//					{
//						
//						ray.dist_ = hitDist;
//					}
//				}
//				
//				--stackIdx;
//			}
//
//		}
//
//	}
//
//	void ProcessRays_BVH_GPU_1( const ReducedBvhArray< T >& bvhArray, RayPool< T >& rayPool )
//	{
//		using namespace concurrency;
//
//		ReducedBvhNode< T >* bvhPtr = bvhArray.bvhNodeArray_.get();
//		RayTube< T >* rayPtr = rayPool.rayTubeArray_.get();
//		
//		array_view< const ReducedBvhNode< T >, 1 > bvhGpu( bvhArray.nodeCount_, bvhPtr );
//		array_view< RayTube< T >, 1 > rayGpu( rayPool.rayCount_, rayPtr );
//
//		PARFOR( rayGpu.extent, [=]( index< 1 > idRay ) restrict( amp )
//		{
//
//			U32 stackArray[ 32 ];
//			I32 stackIdx = 0;
//			stackArray[ stackIdx ] = 0;
//
//			RayTube< T >& ray = rayGpu[ idRay ];
//
//			while( stackIdx >= 0 )
//			{				
//				
//				bool isHitL = true;
//				bool isHitR = true;
//				T hitDistL = 1E32;
//				T hitDistR = 1E32;
//
//				const ReducedBvhNode< T >& node = bvhGpu[ index< 1 >( stackArray[ stackIdx ] ) ];
//
//				if( node.status_ == BRANCH || node.status_ == ROOT )
//				{
//					const ReducedBvhNode< T >& left = bvhGpu[ index< 1 >( node.data_.leftChildIdx_ ) ];
//					const ReducedBvhNode< T >& right = bvhGpu[ index< 1 >( node.data_.rightChildIdx_ ) ];
//					if( left.status_ != LEAF ) ray.CollisionWithBoundBox( left.data_.boundBox_, isHitL, hitDistL );
//					if( right.status_ != LEAF ) ray.CollisionWithBoundBox( right.data_.boundBox_, isHitR, hitDistR );
//
//					if( left.status_ == LEAF )
//					{
//						if( right.status_ == LEAF )
//						{
//							stackArray[ stackIdx++ ] = node.data_.rightChildIdx_;
//						}
//						else
//						{
//							ray.CollisionWithBoundBox( right.data_.boundBox_, isHitR, hitDistR );
//							if( isHitR ) stackArray[ stackIdx++ ] = node.data_.rightChildIdx_;
//						}
//						stackArray[ stackIdx++ ] = node.data_.leftChildIdx_;
//					}
//					else
//					{
//						if( right.status_ != LEAF )
//						{
//							ray.CollisionWithBoundBox( left.data_.boundBox_, isHitL, hitDistL );
//							ray.CollisionWithBoundBox( right.data_.boundBox_, isHitR, hitDistR );
//							if( hitDistL < hitDistR )
//							{
//								if( isHitR ) stackArray[ stackIdx++ ] = node.data_.rightChildIdx_;
//								if( isHitL ) stackArray[ stackIdx++ ] = node.data_.leftChildIdx_;
//							}
//							else
//							{
//								if( isHitL ) stackArray[ stackIdx++ ] = node.data_.leftChildIdx_;
//								if( isHitR ) stackArray[ stackIdx++ ] = node.data_.rightChildIdx_;
//							}
//						}
//						else
//						{
//							ray.CollisionWithBoundBox( left.data_.boundBox_, isHitL, hitDistL );
//							if( isHitL ) stackArray[ stackIdx++ ] = node.data_.leftChildIdx_;
//							stackArray[ stackIdx++ ] = node.data_.rightChildIdx_;
//						}
//					}
//				}
//				else
//				{
//					ray.CollisionWithTriangle( node.trig_, isHitL, hitDistL );
//					if( isHitL && hitDistL < ray.dist_ )
//					{
//						
//						ray.dist_ = hitDistL;
//					}
//				}
//				
//				--stackIdx;
//			}
//		});
//		
//		rayGpu.synchronize();
//
//	}
//
//	void ProcessRays_BVH_GPU( const ReducedBvhArray< T >& bvhArray, RayPool< T >& rayPool )
//	{
//		using namespace concurrency;
//
//		ReducedBvhNode< T >* bvhPtr = bvhArray.bvhNodeArray_.get();
//		RayTube< T >* rayPtr = rayPool.rayTubeArray_.get();
//		
//		array_view< const ReducedBvhNode< T >, 1 > bvhGpu( bvhArray.nodeCount_, bvhPtr );
//		array_view< RayTube< T >, 1 > rayGpu( rayPool.rayCount_, rayPtr );
//
//		PARFOR( rayGpu.extent, [=]( index< 1 > idRay ) restrict( amp )
//		{
//
//			U32 stackArray[ 32 ];
//			I32 stackIdx = 0;
//			stackArray[ stackIdx ] = 0;
//
//			RayTube< T >& ray = rayGpu[ idRay ];
//
//			while( stackIdx >= 0 )
//			{				
//				bool isHit = false;
//				T hitDist = 0;
//
//				const ReducedBvhNode< T >& node = bvhGpu[ index< 1 >( stackArray[ stackIdx ] ) ];
//
//				if( node.status_ == BRANCH || node.status_ == ROOT )
//				{
//					ray.CollisionWithBoundBox( node.data_.boundBox_, isHit, hitDist );
//					if( isHit && hitDist < ray.dist_ )
//					{
//						stackArray[ stackIdx++ ] = node.data_.leftChildIdx_;
//						stackArray[ stackIdx++ ] = node.data_.rightChildIdx_;
//					}
//				}
//				else
//				{
//					ray.CollisionWithTriangle( node.trig_, isHit, hitDist );
//					if( isHit && hitDist < ray.dist_ )
//					{
//						ray.dist_ = hitDist;
//					}
//				}
//				
//				--stackIdx;
//			}
//		});
//
//		rayGpu.synchronize();
//
//	}
//
//	void ProcessRaysSBR_BVH_GPU( const ReducedBvhArray< T >& bvhArray, RayPool< T >& rayPool )
//	{
//		using namespace concurrency;
//
//		ReducedBvhNode< T >* bvhPtr = bvhArray.bvhNodeArray_.get();
//		RayTube< T >* rayPtr = rayPool.rayTubeArray_.get();
//		
//		array_view< const ReducedBvhNode< T >, 1 > bvhGpu( bvhArray.nodeCount_, bvhPtr );
//		array_view< RayTube< T >, 1 > rayGpu( rayPool.rayCount_, rayPtr );
//
//		PARFOR( rayGpu.extent, [=]( index< 1 > idRay ) restrict( amp )
//		{
//			RayTube< T >& ray = rayGpu[ idRay ];
//			bool isHitAtAll = true;
//
//			ray.dist_ = 0;
//
//			while( isHitAtAll && ray.refCount_ < 1 )
//			{
//				isHitAtAll = false;
//
//				U32 stackArray[ 32 ];
//				I32 stackIdx = 0;
//				stackArray[ stackIdx ] = 0;
//
//				T hitDistMin = 1E32;
//				U32 hitIdx = -1;
//				LUV::Vec3< T > hitPointMin;
//				
//
//				while( stackIdx >= 0 )
//				{
//					bool isHit = false;
//					T hitDist = 0;
//
//					const ReducedBvhNode< T >& node = bvhGpu[ index< 1 >( stackArray[ stackIdx ] ) ];
//
//					if( node.status_ == BRANCH || node.status_ == ROOT )
//					{
//						ray.CollisionWithBoundBox( node.data_.boundBox_, isHit, hitDist );
//						if( isHit && hitDist < hitDistMin )
//						{
//							stackArray[ stackIdx++ ] = node.data_.leftChildIdx_;
//							stackArray[ stackIdx++ ] = node.data_.rightChildIdx_;
//						}
//					}
//					else if( stackArray[ stackIdx ] != ray.lastHitIdx_ )
//					{
//						LUV::Vec3< T > hitPoint;
//						ray.CollisionWithTriangleSbr( node.trig_, isHit, hitDist, hitPoint );
//						if( isHit && hitDist < hitDistMin )
//						{
//							isHitAtAll = true;
//							hitDistMin = hitDist;
//							hitPointMin = hitPoint;
//							hitIdx = stackArray[ stackIdx ];
//						}
//					}
//
//					--stackIdx;
//				}
//
//				//DO ray modification here
//				if( isHitAtAll )
//				{
//					const ReducedBvhNode< T >& node = bvhGpu[ index< 1 >( hitIdx ) ];
//					LUV::Vec3< T > hitNormal = node.trig_.GetNormal();
//					LUV::Vec3< T > dirCrossNormal = LUV::Cross( ray.dir_, hitNormal );
//					LUV::Vec3< T > polU = LUV::Unit( dirCrossNormal );
//					LUV::Vec3< T > polR = LUV::Cross( polU, ray.dir_ );
//					LUV::Vec3< T > refDir = LUV::Unit( ray.dir_ - hitNormal * ( 2.0 * LUV::Dot( ray.dir_, hitNormal ) ) );
//					LUV::Vec3< T > refPolU = polU;
//					LUV::Vec3< T > refPolR = LUV::Cross( refPolU, refDir );
//					T polCompU = LUV::Dot( ray.pol_, polU );
//					T polCompR = LUV::Dot( ray.pol_, polR );
//					ray.pos_ = hitPointMin;
//					ray.dir_ = refDir;
//					ray.pol_ = -polCompR * refPolR + polCompU * refPolU;
//					ray.dist_ += hitDistMin;
//					ray.refNormal_ = hitNormal;
//					ray.refCount_ += 1;
//					ray.lastHitIdx_ = hitIdx;
//				}
//
//			}
//
//		});
//
//		rayGpu.synchronize();
//
//	}
//
//	void ProcessRaysSBR_BVH_GPU_RED( const ReducedBvhArray< T >& bvhArray, RayPool< T >& rayPool, T& rcsVal )
//	{
//		using namespace concurrency;
//
//		ReducedBvhNode< T >* bvhPtr = bvhArray.bvhNodeArray_.get();
//		RayTube< T >* rayPtr = rayPool.rayTubeArray_.get();
//		
//		array_view< const ReducedBvhNode< T >, 1 > bvhGpu( bvhArray.nodeCount_, bvhPtr );
//		array_view< RayTube< T >, 1 > rayGpu( rayPool.rayCount_, rayPtr );
//
//		PARFOR( rayGpu.extent, [=]( index< 1 > idRay ) restrict( amp )
//		{
//			RayTube< T >& ray = rayGpu[ idRay ];
//			bool isHitAtAll = true;
//
//			ray.dist_ = 0;
//
//			while( isHitAtAll && ray.refCount_ < 1 )
//			{
//				isHitAtAll = false;
//
//				U32 stackArray[ 32 ];
//				I32 stackIdx = 0;
//				stackArray[ stackIdx ] = 0;
//
//				T hitDistMin = 1E32;
//				U32 hitIdx = -1;
//				LUV::Vec3< T > hitPointMin;
//				
//
//				while( stackIdx >= 0 )
//				{
//					bool isHit = false;
//					T hitDist = 0;
//
//					const ReducedBvhNode< T >& node = bvhGpu[ index< 1 >( stackArray[ stackIdx ] ) ];
//
//					if( node.status_ == BRANCH || node.status_ == ROOT )
//					{
//						ray.CollisionWithBoundBox( node.data_.boundBox_, isHit, hitDist );
//						if( isHit && hitDist < hitDistMin )
//						{
//							stackArray[ stackIdx++ ] = node.data_.leftChildIdx_;
//							stackArray[ stackIdx++ ] = node.data_.rightChildIdx_;
//						}
//					}
//					else if( stackArray[ stackIdx ] != ray.lastHitIdx_ )
//					{
//						LUV::Vec3< T > hitPoint;
//						ray.CollisionWithTriangleSbr( node.trig_, isHit, hitDist, hitPoint );
//						if( isHit && hitDist < hitDistMin )
//						{
//							isHitAtAll = true;
//							hitDistMin = hitDist;
//							hitPointMin = hitPoint;
//							hitIdx = stackArray[ stackIdx ];
//						}
//					}
//
//					--stackIdx;
//				}
//
//				//DO ray modification here
//				if( isHitAtAll )
//				{
//					const ReducedBvhNode< T >& node = bvhGpu[ index< 1 >( hitIdx ) ];
//					LUV::Vec3< T > hitNormal = node.trig_.GetNormal();
//					LUV::Vec3< T > dirCrossNormal = LUV::Cross( ray.dir_, hitNormal );
//					LUV::Vec3< T > polU = LUV::Unit( dirCrossNormal );
//					LUV::Vec3< T > polR = LUV::Cross( polU, ray.dir_ );
//					LUV::Vec3< T > refDir = LUV::Unit( ray.dir_ - hitNormal * ( 2.0 * LUV::Dot( ray.dir_, hitNormal ) ) );
//					LUV::Vec3< T > refPolU = polU;
//					LUV::Vec3< T > refPolR = LUV::Cross( refPolU, refDir );
//					T polCompU = LUV::Dot( ray.pol_, polU );
//					T polCompR = LUV::Dot( ray.pol_, polR );
//					ray.pos_ = hitPointMin;
//					ray.dir_ = refDir;
//					ray.pol_ = -polCompR * refPolR + polCompU * refPolU;
//					ray.dist_ += hitDistMin;
//					ray.refNormal_ = hitNormal;
//					ray.refCount_ += 1;
//					ray.lastHitIdx_ = hitIdx;
//				}
//
//			}
//
//		});
//
//		rayGpu.synchronize();
//
//	}
//
//	T ReduceRcs( RayPool< T >& rayPool, T the, T phi )
//	{
//		using namespace std;
//		using namespace std::complex_literals;
//
//		T c0 = 299792458.0;
//		T mu0 = 12.566370614E-7;
//		T eps0 = 8.854187817E-12;
//		T z0 = mu0 * c0;
//		T pi = 3.14159265359;
//
//		T freq = 25 * c0;
//
//
//		T angFreq = 2 * pi * freq;
//		T waveLen = c0 / freq;
//		T waveNum = 2 * pi / waveLen;
//
//		RayTube< T >* rayPtr = rayPool.rayTubeArray_.get();
//		U32 rayCount = rayPool.rayCount_;
//		T rayArea = rayPool.rayArea_;
//
//		T cp = cos( phi );
//		T sp = sin( phi );
//		T ct = cos( the );
//		T st = sin( the );
//
//		LUV::Vec3< T > dirX( 1.0, 0.0, 0.0 );
//		LUV::Vec3< T > dirY( 0.0, 1.0, 0.0 );
//		LUV::Vec3< T > dirZ( 0.0, 0.0, 1.0 );
//		LUV::Vec3< T > dirP( -sp, cp, 0.0 );
//		LUV::Vec3< T > dirT( cp * ct, sp * ct, -st );
//
//		LUV::Vec3< T > vecK = waveNum * ( ( dirX * cp + dirY * sp ) * st + dirZ * ct );
//
//		T AUre = 0;
//		T AUim = 0;
//		T ARre = 0;
//		T ARim = 0;
//
//		
//		complex< T > AU = 0;
//		complex< T > AR = 0;
//
//		complex< T > i(0,1.0);
//
//		for( U32 idRay = 0; idRay < rayCount; ++idRay )
//		{
//			RayTube< T >& ray = rayPtr[ idRay ];
//			if( ray.refCount_ > 0 )
//			{
//				T kr = waveNum * ray.dist_;
//				T rc = pow( -1.0, ray.refCount_ );
//
//				LUV::Vec3< complex< T > > apE = exp(  i * kr ) * ray.pol_ * rc;
//				LUV::Vec3< complex< T > > apH = -LUV::Cross( apE, ray.dir_ );
//
//				complex< T > BU = LUV::Dot( -( LUV::Cross( apE, -dirP ) + LUV::Cross( apH, dirT ) ), ray.dir_ );
//				complex< T > BR = LUV::Dot( -( LUV::Cross( apE, dirT ) + LUV::Cross( apH, dirP ) ), ray.dir_ );
//				
//				//complex< T > BU = LUV::Dot( -( LUV::Cross( apE, -dirP ) ) , ray.dir_ );
//				//complex< T > BR = LUV::Dot( -( LUV::Cross( apE, dirT ) ) , ray.dir_ );
//
//				complex< T > factor = ( ( i * waveNum * rayArea ) / ( 4 * pi ) ) * exp( - i * LUV::Dot( vecK, ray.pos_ ));
//
//				AU += BU * factor;
//				AR += BR * factor;
//			}
//
//
//
//			//RayTube< T >& ray = rayPtr[ idRay ];
//			//T kr = waveNum * ray.dist_;
//			//T rc = pow( -1.0, ray.refCount_ );
//
//			//T phaseRe = cos( kr );
//			//T phaseIm = sin( kr );
//
//			//LUV::Vec3< T > eapRe = ray.pol_ * ( rc * phaseRe );
//			//LUV::Vec3< T > eapIm = ray.pol_ * ( rc * phaseIm );
//
//			//LUV::Vec3< T > hapRe = LUV::Cross( ray.dir_, eapRe ) / z0;
//			//LUV::Vec3< T > hapIm = LUV::Cross( ray.dir_, eapIm ) / z0;
//			//
//			//T BUre = LUV::Dot( ray.dir_, 0.5 * ( LUV::Cross( -dirP, eapRe ) + z0 * LUV::Cross( dirT, hapRe ) ) );
//			//T BUim = LUV::Dot( ray.dir_, 0.5 * ( LUV::Cross( -dirP, eapIm ) + z0 * LUV::Cross( dirT, hapIm ) ) );
//			//T BRre = LUV::Dot( ray.dir_, 0.5 * ( LUV::Cross( dirT, eapRe ) + z0 * LUV::Cross( dirP, hapRe ) ) );
//			//T BRim = LUV::Dot( ray.dir_, 0.5 * ( LUV::Cross( dirT, eapIm ) + z0 * LUV::Cross( dirP, hapIm ) ) );
//
//			//T posPhaseRe = cos( LUV::Dot( vecK, ray.pos_ ) );
//			//T posPhaseIm = sin( LUV::Dot( vecK, ray.pos_ ) );
//			//
//			//T factorIm = ( waveNum * rayArea ) / ( 2 * pi );
//
//			//T fRe = - posPhaseIm * factorIm;
//			//T fIm = posPhaseRe * factorIm;
//
//			//AUre += BUre * fRe - BUim * fIm;
//			//AUim += BUre * fIm + BUim * fRe;
//
//			//ARre += BRre * fRe - BRim * fIm;
//			//ARim += BRre * fIm + BRim * fRe;
//
//			// ax + ay + bx + by		RE: ax - by;	IM: ay + bx;
//
//
//			
//
//
//		}
//
//		std::cout << "AU: " << AU.real() << " + i" << AU.imag() << std::endl;
//		std::cout << "AR: " << AR.real() << " + i" << AR.imag() << std::endl;
//
//		return pow( abs( AU ), 2 ) + pow( abs( AR ), 2 );
//
//	}
//
//	void ProcessRays_Mesh( const TriangleMesh< T >& mesh, RayPool< T >& rayPool )
//	{
//		RayTube< T >* rayPtr = rayPool.rayTubeArray_.get();
//
//		for( U32 idRay = 0; idRay < rayPool.rayCount_; ++idRay )
//		{
//			RayTube< T >& ray = rayPtr[ idRay ];
//
//			U32 stackArray[ 128 ];
//			I32 stackIdx = 0;
//			stackArray[ stackIdx ] = 0;
//
//			for( const Triangle< T >& trig : mesh.trigArray_ )
//			{
//				bool isHit;
//				T hitDist;
//				ray.CollisionWithTriangle( trig, isHit, hitDist );
//				if( isHit && hitDist < ray.dist_ )
//				{
//						
//					ray.dist_ = hitDist;
//				}
//			}
//
//
//		}
//
//	}
//
//	void GenerateImage( const ReducedBvhArray< T >& bvhArray, const RayPool< T >& rayPool, const std::string& fileName )
//	{
//		T distMin = bvhArray.bvhNodeArray_.get()[ 0 ].data_.boundBox_.GetRadius();
//		T distMax = distMin * 3;
//		T distRange = distMax - distMin;
//
//
//		int w = rayPool.rayCountSqrt_;
//		int h = rayPool.rayCountSqrt_;
//
//		int x;
//		int y;
//
//
//
//
//
//
//		FILE *f;
//		unsigned char *img = NULL;
//		int filesize = 54 + 3*w*h;  //w is your image width, h is image height, both int
//
//		img = (unsigned char *)malloc(3*w*h);
//		memset(img,0,3*w*h);
//
//		for(int i=0; i<w; i++)
//		{
//			for(int j=0; j<h; j++)
//			{
//				x=i; y=(h-1)-j;
//
//				U32 idRay = i + w * j;
//
//				T dist = rayPool.rayTubeArray_.get()[ idRay ].dist_;
//				dist = (dist - distMin) / distRange;
//				if( dist < 0 ) dist = 0;
//				if( dist > 1 ) dist = 1;
//
//				dist = dist * 254;
//
//				//     std::cout << dist << std::endl;
//
//
//				LUV::Vec3< T > dirr = rayPool.rayTubeArray_.get()[ idRay ].dir_;
//
//				dirr = (dirr + 1.0)/2.0;
//
//				//r = red[i][j]*255;
//				//g = green[i][j]*255;
//				//b = blue[i][j]*255;
//
//				//if (r > 255) r=255;
//				//if (g > 255) g=255;
//				////if (b > 255) b=255;
//				//img[(x+y*w)*3+2] = (unsigned char)(dist);
//				//img[(x+y*w)*3+1] = (unsigned char)(dist);
//				//img[(x+y*w)*3+0] = (unsigned char)(dist);
//
//				img[(x+y*w)*3+2] = (unsigned char)(dirr[0]*254);
//				img[(x+y*w)*3+1] = (unsigned char)(dirr[1]*254);
//				img[(x+y*w)*3+0] = (unsigned char)(dirr[2]*254);
//
//				//if( i==1999 && j == 1999  ) std::cout <<
//
//			}
//		}
//
//		unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
//		unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
//		unsigned char bmppad[3] = {0,0,0};
//
//		bmpfileheader[ 2] = (unsigned char)(filesize    );
//		bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
//		bmpfileheader[ 4] = (unsigned char)(filesize>>16);
//		bmpfileheader[ 5] = (unsigned char)(filesize>>24);
//
//		bmpinfoheader[ 4] = (unsigned char)(       w    );
//		bmpinfoheader[ 5] = (unsigned char)(       w>> 8);
//		bmpinfoheader[ 6] = (unsigned char)(       w>>16);
//		bmpinfoheader[ 7] = (unsigned char)(       w>>24);
//		bmpinfoheader[ 8] = (unsigned char)(       h    );
//		bmpinfoheader[ 9] = (unsigned char)(       h>> 8);
//		bmpinfoheader[10] = (unsigned char)(       h>>16);
//		bmpinfoheader[11] = (unsigned char)(       h>>24);
//
//		fopen_s( &f, ( fileName + ".bmp" ).c_str(), "wb" );
//		fwrite(bmpfileheader,1,14,f);
//		fwrite(bmpinfoheader,1,40,f);
//		for(int i=0; i<h; i++)
//		{
//			fwrite(img+(w*(h-i-1)*3),3,w,f);
//			fwrite(bmppad,1,(4-(w*3)%4)%4,f);
//		}
//
//		free(img);
//		fclose(f);
//	}
//
//
//	//void GenerateBvhArray()
//	//{
//	//	bvhNodeArray_.clear();
//	//	bvhNodeArray_.reserve( 16 * mesh_->trigCount_ );
//
//	//	bvhNodeArray_.push_back( BvhNode< T >(
//	//		(U64)( 0 ),
//	//		(U64)( 0 ) - (U64)( 1 ),
//	//		0,
//	//		mesh_->trigCount_,
//	//		0,
//	//		0,
//	//		ROOT
//	//	) );
//	//	
//	//	U32 nodeIdx = 0;
//	//	U32 nodeCount = 1;
//	//	while( nodeIdx < nodeCount )
//	//	{
//	//		BvhNode< T >* node = &(bvhNodeArray_[ nodeIdx ]);
//
//	//		
//	//			//std::cout << std::bitset< 64 >( node->upperMortonRange_ ) << std::endl;
//
//	//		U32 newLowerIdx;
//	//		U32 newUpperIdx;
//	//		
//	//	//std::cout << node->upperIdxRange_ << std::endl;
//
//	//		
//	//	//std::cout << node->lowerIdxRange_ << std::endl;
//	//	//std::cout << node->upperIdxRange_ << std::endl;
//
//	//		morton_->GetMortonBounds( node->lowerMortonRange_, node->upperMortonRange_, node->lowerIdxRange_, node->upperIdxRange_, newLowerIdx, newUpperIdx );
//	//		node->lowerIdxRange_ = newLowerIdx;
//	//		node->upperIdxRange_ = newUpperIdx;
//	//		I32 elemCount = (I32)newUpperIdx - (I32)newLowerIdx;
//
//	//		
//	//			//std::cout << std::bitset< 64 >( node->upperMortonRange_ ) << std::endl;
//
//	//		if( elemCount == 0 )
//	//		{
//	//			node->status_ = EMPTY;
//	//		}
//	//		else if( elemCount == 1 )
//	//		{
//	//			node->status_ = LEAF;
//	//			node->trigIdx_ = node->lowerIdxRange_;
//	//		}
//	//		else if( elemCount > 1 )
//	//		{
//	//			node->status_ = BRANCH;
//
//	//			node->leftChildIdx_ = nodeCount;
//	//			++nodeCount;
//	//			
//	//			node->rightChildIdx_ = nodeCount;
//	//			++nodeCount;
//	//			
//	//			U64 midMortonRange = node->lowerMortonRange_ + ( ( node->upperMortonRange_ - node->lowerMortonRange_ ) >> 1 );
//	//			
//	//			//std::cout << std::bitset< 64 >( node->upperMortonRange_ ) << std::endl;
//
//	//			bvhNodeArray_.push_back( BvhNode< T >(
//	//				node->lowerMortonRange_,
//	//				midMortonRange,
//	//				node->lowerIdxRange_,
//	//				node->upperIdxRange_,
//	//				nodeIdx,
//	//				node->rightChildIdx_,
//	//				EMPTY
//	//			) );
//
//	//			// BE CAREFUL, PUSH BACK MAY RELOCATE THE MEMORY, UPDATE THE POINTER
//	//			BvhNode< T >* node = &(bvhNodeArray_[ nodeIdx ]);
//
//	//			bvhNodeArray_.push_back( BvhNode< T >(
//	//				midMortonRange + 1,
//	//				node->upperMortonRange_,
//	//				node->lowerIdxRange_,
//	//				node->upperIdxRange_,
//	//				nodeIdx,
//	//				node->leftChildIdx_,
//	//				EMPTY
//	//			) );
//
//	//			
//
//	//		}
//	//		
//	//		//std::cout << std::bitset< 64 >( node->upperMortonRange_ ) << std::endl;
//
//	//		++nodeIdx;
//
//	//		///
//	//		//if( nodeIdx > 1000 )
//	//		//	return;
//	//		///
//	//	}
//
//	//	bvhNodeArray_[0].status_ = ROOT;
//
//	//	bvhNodeArray_.shrink_to_fit();
//
//
//	//}
//
//	//void SqueezeBvhArray()
//	//{
//	//	//2 ) remove empties
//
//	//	//nodeIdx = lastIdx
//
//	//	//while nodeIdx != 0
//	//
//	//	//	if I am not empty
//	//	//		if my sibling is empty
//	//	//			clone me propserties to my parent
//	//	//	
//	//	//			if I em not leaf
//	//	//				update the parent idx of my children
//	//	//			set me empty.
//	//	//		else
//	//	//			if I am leaf
//	//	//				set my aabb to my triangle's aabb
//	//	//			update my parent's aabb with mine
//	//	//	--nodeIdx
//	//	//end
//
//	//	U32 nodeIdx = bvhNodeArray_.size() - 1;
//
//	//	while( nodeIdx != 0 )
//	//	{
//	//		BvhNode< T >* node = &(bvhNodeArray_[ nodeIdx ]);
//	//		BvhNode< T >* sibling = &(bvhNodeArray_[ node->siblingIdx_ ]);
//	//		BvhNode< T >* parent = &(bvhNodeArray_[ node->parentIdx_ ]);
//
//	//		if( node->status_ != EMPTY )
//	//		{
//	//			if( sibling->status_ == EMPTY )
//	//			{
//	//				parent->leftChildIdx_ = node->leftChildIdx_;
//	//				parent->rightChildIdx_ = node->rightChildIdx_;
//	//				parent->status_ = node->status_;
//	//				parent->boundBox_ = node->boundBox_;
//	//				parent->bbInitialized_ = node->bbInitialized_;
//	//				parent->trigIdx_ = node->trigIdx_;
//
//	//				if( node->status_ == BRANCH )
//	//				{
//	//					BvhNode< T >* leftChild = &(bvhNodeArray_[ node->leftChildIdx_ ]);
//	//					BvhNode< T >* rightChild = &(bvhNodeArray_[ node->rightChildIdx_ ]);
//	//					leftChild->parentIdx_ = node->parentIdx_;
//	//					rightChild->parentIdx_ = node->parentIdx_;
//	//				}
//
//	//				node->status_ = EMPTY;
//	//			}
//	//			else
//	//			{
//	//				if( node->status_ == LEAF )
//	//				{
//	//					node->boundBox_ = mesh_->trigArray_[ node->trigIdx_ ].GetBoundBox();
//	//					node->bbInitialized_ = true;
//	//				}
//	//				if( parent->bbInitialized_ )
//	//				{
//	//					parent->boundBox_ = parent->boundBox_.UnionWith( node->boundBox_ );
//	//				}
//	//				else
//	//				{
//	//					parent->boundBox_ = node->boundBox_;
//	//					parent->bbInitialized_ = true;
//	//				}
//	//			}
//
//	//		}
//
//	//		--nodeIdx;
//	//	}
//
//	//}
//
//	//void RemoveEmptyNodes()
//	//{
//	//	U32 nodeCount = bvhNodeArray_.size();
//	//	std::vector< U32 > indexDiff( nodeCount, 0 );
//
//	//	U32 nodeIdx = 1;
//	//	U32 nodeIdxNew = 1;
//
//	//	while( nodeIdx < nodeCount )
//	//	{
//	//		indexDiff[ nodeIdx ] = indexDiff[ nodeIdx - 1 ];
//
//	//		if( bvhNodeArray_[ nodeIdx ].status_ != EMPTY )
//	//		{
//	//			bvhNodeArray_[ nodeIdxNew ] = bvhNodeArray_[ nodeIdx ];
//	//			++nodeIdxNew;
//	//		}
//	//		else
//	//		{
//	//			indexDiff[ nodeIdx ] += 1;
//	//		}
//
//	//		++nodeIdx;
//	//	}
//
//	//	bvhNodeArray_.resize( nodeIdxNew );
//	//	for( BvhNode< T >& node : bvhNodeArray_ )
//	//	{
//	//		node.parentIdx_ = node.parentIdx_ - indexDiff[ node.parentIdx_ ];
//	//		node.siblingIdx_ = node.siblingIdx_ - indexDiff[ node.siblingIdx_ ];
//	//		if( node.status_ != LEAF )
//	//		{
//	//			node.leftChildIdx_ = node.leftChildIdx_ - indexDiff[ node.leftChildIdx_ ];
//	//			node.rightChildIdx_ = node.rightChildIdx_ - indexDiff[ node.rightChildIdx_ ];
//	//		}
//	//	}
//
//	//}
//
//	//void PopulateReducedBvhArray( ReducedBvhArray< T >& reducedBvhArray )
//	//{
//	//	U32 nodeCount = bvhNodeArray_.size();
//	//	reducedBvhArray.Initialize( nodeCount );
//	//	ReducedBvhNode< T >* arrayPtr = reducedBvhArray.bvhNodeArray_.get();
//	//	for( U32 idNode = 0; idNode < nodeCount; ++idNode )
//	//	{
//	//		const BvhNode< T >& node = bvhNodeArray_[ idNode ];
//	//		if( node.status_ == LEAF )
//	//		{
//	//			arrayPtr[ idNode ] = ReducedBvhNode< T >( mesh_->trigArray_[ node.trigIdx_ ] );
//	//		}
//	//		else
//	//		{
//	//			arrayPtr[ idNode ] = ReducedBvhNode< T >( node.status_, node.parentIdx_, node.leftChildIdx_, node.rightChildIdx_, node.boundBox_ );
//	//		}
//	//	}
//	//}
//
//};
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//#endif