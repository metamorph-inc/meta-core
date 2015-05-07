VCP_DIR = getenv('VCP_PATH');
if strcmp(VCP_DIR, '')
  VCP_DIR = winqueryreg('HKEY_LOCAL_MACHINE', 'Software\ESMoL', 'VCP_PATH');
end

addpath( [VCP_DIR,'\bin'] );


TTK_DIR = getenv('TTKERNEL');
if strcmp(TTK_DIR, '')
  TTK_DIR = winqueryreg('HKEY_LOCAL_MACHINE', 'Software\ESMoL', 'TTKERNEL');
end

addpath( [TTK_DIR,'\matlab\help'    ] );
addpath( [TTK_DIR,'\matlab'         ] );
addpath( [TTK_DIR                   ] );
