%clc;
clear all;
%close all;

%%OBS FREQ of sphere

c0 = 299792458.0;

N = 90 + 1;
f = c0 * 50;

phi = linspace( 0, 90, N );
phi = pi .* phi ./ 180;

A = cos( phi );
B = sin( phi );
C = repmat( 0, N, 1 );
D = repmat( 1, N, 1 );


obsX = C;
obsY = B;
obsZ = A;

polX = D;
polY = C;
polZ = C;
% 
% polX = -sin( phi );
% polY = cos( phi );
% polZ = repmat( 0, N, 1 );

freq = repmat( f, N, 1 );
rayPerLam = repmat( 10, N, 1 );

RaytrAMP.GenerateObsFile( "ObsPhi256.obs", N, obsX,obsY,obsZ, polX,polY,polZ, freq, rayPerLam );
RaytrAMP.MonoRCS( "AudiR8_20000.rba", "ObsPhi256.obs", "ObsPhi256.rcs" );
[ rcsCount, rcsVector ] = RaytrAMP.LoadRcsFile( "ObsPhi256.rcs" );

%figure();
%plot( 180*phi./pi, 10 .* log10( rcsVector ) );
%figure();plot( 180*phi./pi, rcsVector );

RaytrAMP.SaveAsFekoData( "data.feko", 180*phi./pi, rcsVector );


