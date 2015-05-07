function [] = plot_result( infilename )
% plot_result( infilename )
%
% Plot the results for the specified input file.
%

outfilename = [infilename '_results'];

r = load( outfilename );
last_T = max(r.input.tout);

figure;
grid on;
title 'Position states and errors'
subplot( 2, 1, 1 );
plot( r.input.tout, r.xe_ve(:,1:3) );
ylabel 'Position';
subplot( 2, 1, 2 );
plot( r.input.tout, r.err_xe );
ylabel 'Pos err';

figure;
grid on;
title 'Velocity states and errors'
subplot( 2, 1, 1 );
plot( r.input.tout, r.xe_ve(:,5:6) );
ylabel 'Velocity';
subplot( 2, 1, 2 );
plot( r.input.tout, r.err_ve );
ylabel 'Velocity err';

figure;
grid on;
title 'Sector bounds'
subplot( 2, 1, 1 );
plot( r.input.tout, r.a_euler );
ylabel 'a_euler';
axis([0 last_T -1 0]);
subplot( 2, 1, 2 );
plot( r.input.tout, r.a_inertia );
ylabel 'a_inertia';
axis([0 last_T -1 0]);

