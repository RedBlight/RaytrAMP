RaytrAMP.GenerateObsFile( "ObsFreq100.obs", N, obsX,obsY,obsZ, polX,polY,polZ, freq, rayPerLam );
RaytrAMP.MakeRBA( "sphere1r287580.unv", "sphere1r287580.rba" );
RaytrAMP.MonoRCS( "sphere1r287580.rba", "ObsFreq100.obs", "ObsFreq100.rcs" );