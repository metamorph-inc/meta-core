%%

% Alternate plots for outer loop data

figure;
%plot( freqs, 20 * log10( reshape(tmagsat, 1, size(tmagsat,3)) ) );
semilogx( freqs, 20 * log10( reshape(tmagsat, 1, size(tmagsat,3)) ) );
axis( [0 25 -5 50] );
title( 'Magnitude Response (Analytical)');
xlabel( 'Frequency (Hz)' );
ylabel( 'decibels' );
grid;

figure;
%plot( freqs, pffreqmag );
semilogx( freqs, pffreqmag );
axis( [0 25 -5 50] );
title( 'Magnitude Response (Platform)');
xlabel( 'Frequency (Hz)' );
ylabel( 'decibels' );
grid;


