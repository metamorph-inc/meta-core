%
% Script to produce the plots for the four-tank example

figure;

rows = 4;
cols = 2;

simtime = max( x1_x2_dec.time ); 

subplot( rows, cols, 2 );
plot( x1_x2_dec.time, x1_x2_dec.signals(1).values );
title( 'x1 dec' );
xlabel( 't' );
ylabel( 'x1' );
grid on;
axis( [ 0 simtime 0 20 ] );
legend( 'x1d', 'x1' );

subplot( rows, cols, 4 );
plot( x1_x2_dec.time, x1_x2_dec.signals(2).values );
title( 'x2 dec' );
xlabel( 't' );
ylabel( 'x2' );
grid on;
axis( [ 0 simtime 0 25 ] );
legend( 'x2d', 'x2' );

subplot( rows, cols, 1 );
plot( x1_x2_ida_pbc.time, x1_x2_ida_pbc.signals(1).values );
title( 'x1 IDA-PBC' );
xlabel( 't' );
ylabel( 'x1' );
grid on;
axis( [ 0 simtime 0 20 ] );
legend( 'x1d', 'x1' );

subplot( rows, cols, 3 );
plot( x1_x2_ida_pbc.time, x1_x2_ida_pbc.signals(2).values );
title( 'x2 IDA-PBC' );
xlabel( 't' );
ylabel( 'x2' );
grid on;
axis( [ 0 simtime 0 25 ] );
legend( 'x2d', 'x2' );

subplot( rows, cols, 6 );
plot( x3_x4_dec.time, x3_x4_dec.signals.values );
title( 'x3 x4 dec' );
xlabel( 't' );
ylabel( 'x3 and x4' );
grid on;
axis( [ 0 simtime 0 10 ] );
legend( 'x3', 'x4' );

subplot( rows, cols, 5 );
plot( x3_x4_ida_pbc.time, x3_x4_ida_pbc.signals.values );
title( 'x3 x4 IDA-PBC' );
xlabel( 't' );
ylabel( 'x3 and x4' );
grid on;
axis( [ 0 simtime 0 10 ] );
legend( 'x3', 'x4' );

subplot( rows, cols, 8 );
plot( u1_u2_dec.time, u1_u2_dec.signals.values );
title( 'u1 u2 dec' );
xlabel( 't' );
ylabel( 'u1 and u2' );
grid on;
axis( [ 0 simtime 0 20 ] );
legend( 'u1', 'u2' );

subplot( rows, cols, 7 );
plot( u1_u2_ida_pbc.time, u1_u2_ida_pbc.signals.values );
title( 'u1 u2 IDA-PBC' );
xlabel( 't' );
ylabel( 'u1 and u2' );
grid on;
axis( [ 0 simtime 0 20 ] );
legend( 'u1', 'u2' );


