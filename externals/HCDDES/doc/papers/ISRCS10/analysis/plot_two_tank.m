%
% Script to produce the plots for the two-tank example


simtime = max( x1_x2_orig.time ); 

figure(1);
clf;
hold on;
grid on;
h = plot( x1_x2_orig.time, ...
      x1_x2_orig.signals(1).values(:,1), '-', ...
      x1_x2_orig.time, ...
      x1_x2_orig.signals(1).values(:,2), '-.', ...
      x1_x2_new.time, x1_x2_new.signals(1).values(:,2), '--' );
%title( 'Two Tank Dynamics (Tank 1)', 'FontWeight', 'bold' );
xlabel( 't (s)', 'FontWeight', 'bold' );
ylabel('x_1 (cm)', 'FontWeight', 'bold' );
set( h(1), 'LineWidth', 2 );
set( h(2), 'LineWidth', 3 );
set( h(3), 'LineWidth', 3 );
axis( [ 0 simtime 0 35 ] );
l1 = legend( 'x_{1}^*', 'x_1 (without integrator)', 'x_1 (with integrator)' );
set( l1, 'FontWeight', 'bold' );

figure(2);
clf;
hold on;
grid on;
[ax,h1,h2] = plotyy( x1_x2_orig.time,[x1_x2_orig.signals(2).values(:,1:2),... 
      x1_x2_new.signals(2).values(:,2)],...
      u_orig.time, [u_orig.signals.values(:,1:2), u_new.signals.values(:,1)]);
%title( 'Two Tank Dynamics (Tank 2)', 'FontWeight', 'bold' );
xlabel( 't (s)', 'FontWeight', 'bold' );
set( h1(1), 'LineStyle', '-', 'LineWidth', 2 );
set( h1(2), 'LineStyle', '-.', 'LineWidth', 3 );
set( h1(3), 'LineStyle', '--', 'LineWidth', 3 );
set( h2(2), 'LineStyle', '-', 'LineWidth', 1 );
set( h2(3), 'LineStyle', '--', 'LineWidth', 2 );
set( h2(1), 'LineStyle', '-.', 'LineWidth', 2 );
axis(ax(1), [ 0 simtime 0 100 ] );
axis(ax(2), [ 0 simtime -100 100 ] );
ylabel(ax(1),'x_2 (cm)', 'FontWeight', 'bold' );
ylabel(ax(2),'u (ml/s)', 'FontWeight', 'bold'  );
l1 = legend(ax(1), 'x_2^*', 'x_2 (without integrator)', 'x_2 (with integrator)' );
l2 = legend(ax(2), 'u (without integrator)', 'u^{*}', 'u (with integrator)' );
set( l1, 'FontWeight', 'bold' );
set( l2, 'FontWeight', 'bold' );

%figure(3);
%clf;
%hold on;
%grid on;
%h = plot( u_orig.time, ...
%      u_orig.signals.values(:,1), '-.', ...
%      u_orig.time, ...
%      u_orig.signals.values(:,2), '-', ...
%      u_new.time, u_new.signals.values(:,1), '--' );
%title( 'Two Tank Dynamics (Input)', 'FontWeight', 'bold' );
%xlabel( 't (s)', 'FontWeight', 'bold' );
%ylabel('u (ml/s)', 'FontWeight', 'bold' );
%set( h(1), 'LineWidth', 3 );
%set( h(2), 'LineWidth', 2 );
%set( h(3), 'LineWidth', 3 );
%axis([ 0 simtime 0 100 ] );
%l1 = legend( 'u (without integrator)', 'u^{*}', 'u (with integrator)' );
%set( l1, 'FontWeight', 'bold' );

