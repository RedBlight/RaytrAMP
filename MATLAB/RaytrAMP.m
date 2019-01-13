classdef RaytrAMP
	%RAYTRAMP Summary of this class goes here
	%   Detailed explanation goes here
	
	properties( Constant )
		exeFolder = "\Executables\";
		makeRbaExe = "MakeRBA.exe";
		monoRcsExe = "MonoRCS.exe";
		
		unvFolder = "\DataFiles\Unv\";
		rbaFolder = "\DataFiles\Rba\";
		obsFolder = "\DataFiles\Obs\";
		rcsFolder = "\DataFiles\Rcs\";
		fekoFolder = "\Datafiles\Feko\";
	end
	
	methods( Static )
		
		% inputs are col vector
		function GenerateObsFile( ...
			fileName, ...
			obsCount, ...
			obsX, obsY, obsZ, ...
			polX, polY, polZ, ...
			freq, rayPerLam ...
		)
% 			dataArr = [ ...
% 				obsX.'; obsY.'; obsZ.'; ...
% 				polX.'; polY.'; polZ.'; ...
% 				freq.'; rayPerLam.' ...
% 			];
			fileId = fopen( pwd + RaytrAMP.obsFolder + fileName, 'w' );
			fwrite( fileId, obsCount, 'uint32' );
			for k = 1 : obsCount
				fwrite( fileId, single( obsX( k ) ), 'single' );
				fwrite( fileId, single( obsY( k ) ), 'single' );
				fwrite( fileId, single( obsZ( k ) ), 'single' );
				fwrite( fileId, single( polX( k ) ), 'single' );
				fwrite( fileId, single( polY( k ) ), 'single' );
				fwrite( fileId, single( polZ( k ) ), 'single' );
				fwrite( fileId, single( freq( k ) ), 'single' );
				fwrite( fileId, uint32( rayPerLam( k ) ), 'uint32' );
			end
			
			%fwrite( fileId, single( reshape( dataArr, [obsCount * 8, 1] ) ), 'single' );
			fclose( fileId );
		end
		
		function MakeRBA( unvFileName, rbaFileName )
			system( ...
				pwd + RaytrAMP.exeFolder + RaytrAMP.makeRbaExe + " " + ...
				pwd + RaytrAMP.unvFolder + unvFileName + " " + ...
				pwd + RaytrAMP.rbaFolder + rbaFileName ...
			);
		end
		
		function MonoRCS( rbaFileName, obsFileName, rcsFileName )
			system( ...
				pwd + RaytrAMP.exeFolder + RaytrAMP.monoRcsExe + " " + ...
				pwd + RaytrAMP.rbaFolder + rbaFileName + " " + ...
				pwd + RaytrAMP.obsFolder + obsFileName + " " + ...
				pwd + RaytrAMP.rcsFolder + rcsFileName ...
			);
		end
		
		function [ rcsCount, rcsVector ] = LoadRcsFile( fileName )
			fileId = fopen( pwd + RaytrAMP.rcsFolder + fileName );
			rcsCount = fread( fileId, 1, 'uint32' );
			rcsVector = fread( fileId, rcsCount, 'single' );
			fclose( fileId );
		end
		
		function SaveAsFekoData( fileName, angleData, rcsData )
			fileId = fopen( pwd + RaytrAMP.fekoFolder + fileName, 'w' );
			for idx = 1 : numel( angleData )
				fprintf( fileId, '%1.16E', angleData( idx ) );
				fprintf( fileId, ',' );
				fprintf( fileId, '%1.16E', rcsData( idx ) );
				fprintf( fileId, '\n' );
			end
			fclose( fileId );
		end
		
	end
	
end

