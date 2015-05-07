%
% Process and plot data from the xPC
%

fd1 = readxpcfile( 'ANGLE.DAT' );

fd2 = readxpcfile( 'POS.DAT' );

fd3 = readxpcfile( 'THRUST.DAT' );

fd4 = readxpcfile( 'SECTORS.DAT' );


figure;
plot( fd1.data(:,2), fd1.data(:,1) );
title( 'Measured angle data' );

figure;
plot( fd2.data(:,2), fd2.data(:,1) );
title( 'Measured position data' );

figure;
plot( fd3.data(:,2), fd3.data(:,1) );
title( 'Thrust command data' );

figure;
plot( fd4.data(:,5), fd4.data(:,1:4) );
title( 'Measured sector data' );
