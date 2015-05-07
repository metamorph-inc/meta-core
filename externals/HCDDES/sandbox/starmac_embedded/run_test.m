function [] = run_test( infilename, modelname )
% run_test( infilename, modelname )
% This is a function, but it's called for side effects.  It reads in the
% data from input filename and then runs the specified simulation on it.
% The results are written back to a filename with _results appended.
%

%clear tout yaw_ref inertial_ref

input = load(infilename);

[t, x, xe_ve, err_xe, err_ve, a_inertia, a_euler] = ...
   sim( modelname, input.tout, [], input.inertial_ref, input.yaw_ref);

outfilename = [infilename '_results'];
save(outfilename, 'input', 'xe_ve', ...
     'err_xe', 'err_ve', 'a_inertia', 'a_euler');

%clear tout yaw_ref inertial_ref xe_ve error_xe error_ve a_euler a_inertia


