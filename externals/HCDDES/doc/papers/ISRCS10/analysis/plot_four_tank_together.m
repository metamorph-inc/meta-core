%
% Script to produce the plots for the four-tank example


simtime = max( x1_x2_dec.time ); 

figure(1);
clf;
hold on;
grid on;
[ax,h1,h2]=plotyy( x1_x2_dec.time, ...
     [x1_x2_dec.signals(1).values x1_x2_ida_pbc.signals(1).values(:,2)], ...
      u1_u2_ida_pbc.time, ...
     [u1_u2_dec.signals.values(:,1) u1_u2_ida_pbc.signals.values(:,1) ...
      u1_u2_ida_pbc.signals.values(:,3)] );
%title( 'Four Tank Minimum Phase Dynamics (Tank 1)', 'FontWeight', 'bold' );
xlabel( 't (s)', 'FontWeight', 'bold' );
ylabel(ax(1), 'x_1 (cm)', 'FontWeight', 'bold' );
ylabel(ax(2), 'u (ml/s)', 'FontWeight', 'bold'  );
set( h1(1), 'LineStyle', '-', 'LineWidth', 2 );
set( h1(2), 'LineStyle', '-.', 'LineWidth', 2 );
set( h1(3), 'LineStyle', '--', 'LineWidth', 2 );
set( h2(1), 'LineStyle', '-', 'LineWidth', 2 );
set( h2(2), 'LineStyle', '--', 'LineWidth', 2 );
set( h2(3), 'LineStyle', '-.', 'LineWidth', 2 );
axis(ax(1), [ 0 simtime 5 40 ] );
axis(ax(2), [ 0 simtime -25 25 ] );
l1 = legend(ax(1), 'x_{1}^*', 'x_1 (DC)', 'x_1 (IDA-PBC)' );
l2 = legend(ax(2), 'u_1 (DC)', 'u_1 (IDA-PBC)', 'u_1^{*} (IDA-PBC)' );
set( l1, 'FontWeight', 'bold' );
set( l2, 'FontWeight', 'bold' );

figure(2);
clf;
hold on;
grid on;
[ax,h1,h2]=plotyy( x1_x2_dec.time, ...
     [x1_x2_dec.signals(2).values x1_x2_ida_pbc.signals(2).values(:,2)], ...
      u1_u2_ida_pbc.time, ...
     [u1_u2_dec.signals.values(:,2) u1_u2_ida_pbc.signals.values(:,2) ...
      u1_u2_ida_pbc.signals.values(:,4)] );
%title( 'Four Tank Minimum Phase Dynamics (Tank 2)', 'FontWeight', 'bold' );
xlabel( 't (s)', 'FontWeight', 'bold' );
ylabel(ax(1), 'x_2 (cm)', 'FontWeight', 'bold' );
ylabel(ax(2), 'u (ml/s)', 'FontWeight', 'bold'  );
set( h1(1), 'LineStyle', '-', 'LineWidth', 2 );
set( h1(2), 'LineStyle', '-.', 'LineWidth', 2 );
set( h1(3), 'LineStyle', '--', 'LineWidth', 2 );
set( h2(1), 'LineStyle', '-', 'LineWidth', 2 );
set( h2(2), 'LineStyle', '--', 'LineWidth', 2 );
set( h2(3), 'LineStyle', '-.', 'LineWidth', 2 );
axis(ax(1), [ 0 simtime 5 40 ] );
axis(ax(2), [ 0 simtime -25 25 ] );
l1 = legend(ax(1), 'x_{2}^*', 'x_2 (DC)', 'x_2 (IDA-PBC)' );
l2 = legend(ax(2), 'u_2 (DC)', 'u_2 (IDA-PBC)', 'u_2^{*} (IDA-PBC)' );
set( l1, 'FontWeight', 'bold' );
set( l2, 'FontWeight', 'bold' );

