figure
fd4 = readxpcfile( 'data/SECTORS.DAT' );
plot( fd4.data(:,5), fd4.data(:,1), 'b:', ...
      fd4.data(:,5), fd4.data(:,2), 'k-.', ...
      fd4.data(:,5), fd4.data(:,3), 'g-', ...
      fd4.data(:,5), fd4.data(:,4), 'k--', 'LineWidth', 2);
grid;
axis([0 60 -4 0] );
title( 'Measured sector values', 'FontWeight', 'bold' );
xlabel( 'Time (s)', 'FontWeight', 'bold' );
ylabel( 'Sector correlation values', 'FontWeight', 'bold' );
l2 = legend( 'Pos', 'Vel', 'Ang', 'AngVel' );
set( l2, 'FontWeight', 'bold' );



figure
fd2 = readxpcfile( 'data/POS.DAT' );
plot( fd2.data(:,2), fd2.data(:,1) );

figure
fd3 = readxpcfile( 'data/THRUST.DAT' );
plot( fd3.data(:,10), fd3.data(:,1:9));

figure
fd1 = readxpcfile( 'data/ANGLE.DAT' );
plot( fd1.data(:,2), fd1.data(:,1) );

sigs.time = fd3.data(:,10);
sigs.signals.values = fd3.data(:,1:9);
sigs.signals.dimensions = 9;

% Plot with reference
figure
fd2b = readxpcfile( 'data/POS.DAT' );
fd3b = readxpcfile( 'data/THRUST.DAT' );
plot( fd2b.data(:,2), fd2b.data(:,1), 'k-', ...
    fd3b.data(:,10), ((1/(0.3*0.5))*fd3b.data(:,2))+fd3b.data(:,3), 'b-.', ...
    'LineWidth', 2);
grid;
axis([87.5 147.5 -0.6 0.6] );
title( 'Measured control system tracking', 'FontWeight', 'bold' );
xlabel( 'Time (s)', 'FontWeight', 'bold' );
ylabel( 'Position (m)', 'FontWeight', 'bold' );
l1 = legend( 'Ref', 'Actual' );
set( l1, 'FontWeight', 'bold' );

