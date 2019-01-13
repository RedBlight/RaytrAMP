%%% RCS import data from FEKO
% Zac Idriss
% [ filename ] : this must be a .txt file extracted from the FEKO program
% [ numberOfLines ] : this is the stricly to extract the header information
%                     its set to a default but you may have to open your
%                     text file and check the number of header line
%                     manually. 
% [ dataColumns ] :   this value is kinda bad coding, you may need to edit
%                     the if loop, just check how many columns you
%                     downloaded and make sure to enter than number.
%                     You have to edit the textscan variable so that the %f
%                     coincide with the number of columns. There's no way
%                     to automatize this unfortnatly. 
% Start of Function  ########
function [FEKO] = importFEKO(filename, numberOfLines, dataColumns)
%filename = 'IED_1_30_deg_RCS_vs_Freq (2).txt';
%numberOfLines = 1;
fprintf('Opening... %s\n\n', filename);
FEKO.headerFile = [];
FEKO.dataCADFEKO = [];
FEKO.dataLabels = [];
% open file
fileID = fopen(filename);
fprintf('Number of headlines: %2.0f\n',numberOfLines');
% extract header infomration
FEKO.headerFile = cell(numberOfLines,1);
for lineNum = 1:numberOfLines
    headerLine = fgets(fileID);    
    FEKO.headerFile(lineNum) =  {headerLine};
    numberOfLines = numberOfLines+1;
end
% extract data labels 
FEKO.dataLabels = fgets(fileID);    
% extract FEKO CAD data
if(dataColumns == 2)
    dataCADFEKO = textscan(fileID, '%f%f', 'HeaderLines', numberOfLines);
    FEKO.dataCADFEKO = cell2mat(dataCADFEKO);
elseif(dataColumns == 9)
    dataCADFEKO = textscan(fileID, '%f%f%f%f%f%f%f%f%f', 'HeaderLines', numberOfLines);
    FEKO.dataCADFEKO = cell2mat(dataCADFEKO);
end
fprintf('Number of data points:%4.0f x%2.0f \n',size(FEKO.dataCADFEKO,1),size(FEKO.dataCADFEKO,2));
fprintf('Data extracted: %s\n',FEKO.dataLabels);
end
% End of Function ########