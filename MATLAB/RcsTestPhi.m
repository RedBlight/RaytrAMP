%clc;
clear all;
%close all;

%%OBS FREQ of sphere

c0 = 299792458.0;

N = 361;
f = c0 * 10;

phi = linspace( 0, 2 * pi, N );

obsX = cos( phi );
obsY = sin( phi );
obsZ = repmat( 0, N, 1 );

polX = repmat( 0, N, 1 );
polY = repmat( 0, N, 1 );
polZ = repmat( 1, N, 1 );

freq = repmat( f, N, 1 );
rayPerLam = repmat( 30, N, 1 );

RaytrAMP.GenerateObsFile( "ObsPhi256.obs", N, obsX,obsY,obsZ, polX,polY,polZ, freq, rayPerLam );
RaytrAMP.MonoRCS( "dihedral.rba", "ObsPhi256.obs", "ObsPhi256.rcs" );
[ rcsCount, rcsVector ] = RaytrAMP.LoadRcsFile( "ObsPhi256.rcs" );

figure();
plot( phi, 10 * log10( rcsVector ) );




