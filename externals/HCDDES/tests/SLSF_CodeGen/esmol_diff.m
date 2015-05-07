%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% This simple function is required by the external build & test
%% infrastructure.  
%%
%% It assumes that you have already run esmol_test(); for the desired test
%% model and thus that the 'simulation_matlab.mat' and the 
%% 'simulation_esmol.csv' files already exist.  
%%
%% Since error()s in invocations of regular .m files can't be surfaced
%% back to the OS' %ERRORLEVEL% system variable at the command prompt, we
%% must compile a .m file instead using the MATLAB C Compiler.  The 
%% esmol_test() function cannot be compiled directly due to its reliance 
%% on Simulink.  Therefore, we have this simple script, which checks 
%% that the result vectors previously created by esmol_test() are
%% equivalent.  
%%
%% USAGE: 
%%      cd svn://HCDDES/tests/unit/prims/mytest/esmol_test
%%      mcc -m ..\..\..\..\esmol_diff.m
%%      .\esmol_diff
%%      echo %ERRORLEVEL%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function esmol_diff
    SHOULD_FAIL = false;
    
    switch SHOULD_FAIL
        case true
            error('fail')
    end

    matlab = load('simulation_matlab.mat');
    esmol  = load('simulation_esmol.csv');
    if (sum(sum(matlab.Y~=esmol)) > 0)
        error('fail')
    end
    %matlab.Y'
    %esmol'
end