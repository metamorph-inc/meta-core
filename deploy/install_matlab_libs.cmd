
set META_MATLAB_LIB=%~dp0..\runtime\MATLAB

matlab -nosplash -nodesktop -wait -r cd('%META_MATLAB_LIB%\HBG-matlab-library');installHBGSimulink;exit;
matlab -nosplash -nodesktop -wait -r cd('%META_MATLAB_LIB%\MDAO-matlab-library');installMDAOSimulink;exit;

exit