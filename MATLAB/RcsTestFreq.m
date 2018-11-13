%clc;
clear all;
%close all;

%%OBS FREQ of sphere

c0 = 299792458.0;

N = 1000;
fMin = 0.5 * c0;
fMax = 5 * c0;

obsX = repmat( 1, N, 1 );
obsY = repmat( 0, N, 1 );
obsZ = repmat( 0, N, 1 );

polX = repmat( 0, N, 1 );
polY = repmat( 0, N, 1 );
polZ = repmat( 1, N, 1 );

freq = logspace( log10(fMin), log10(fMax), N );
rayPerLam = repmat( 100, N, 1 );

RaytrAMP.GenerateObsFile( "ObsFreq100.obs", N, obsX,obsY,obsZ, polX,polY,polZ, freq, rayPerLam );
RaytrAMP.MonoRCS( "sphere1r287580.rba", "ObsFreq100.obs", "ObsFreq100.rcs" );
[ rcsCount, rcsVector ] = RaytrAMP.LoadRcsFile( "ObsFreq100.rcs" );

figure();
plot( freq, rcsVector );




