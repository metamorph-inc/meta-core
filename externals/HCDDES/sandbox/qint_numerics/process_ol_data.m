%%
%% Process outer loop generated data files.
%%

% Iterate over the recorded output data files

% Frequency range is 1/100 up to 25 Hz (stepping logarithmically)
lowend = 1/100; % Hz
highend = 25;   % Hz
steps = 100;
logfreqs = [log10(lowend): ...
    (log10(highend)-log10(lowend))/steps : log10(highend)];
freqs = 10.^logfreqs;

StopTime = 300;  % Seconds
Ts = 0.02;

modname = 'outerloop';

freqmag = zeros( size( freqs ) );
freqphase = zeros( size( freqs ) );

pffreqmag = zeros( size( freqs ) );
pffreqphase = zeros( size( freqs ) );

for freq_idx = 1:length(freqs)
    freq = freqs(freq_idx);
    
    datafilename = sprintf( 'ol_outputs/rslt%06d.mat', floor( freq * 1000 ));
    pfdatafilename = sprintf( 'ol_outputs_pf/rslt%06d.dat', floor( freq * 1000 ));
    
    clear sigdata_out;
    clear ft ftr ftm ftidx ftang origmag outputmag phase_diff;
    clear pfft pfftr pfftm pfftidx pfftang pforigmag pfoutputmag pf_phase_diff;
    
    load(datafilename);
    
    % Process the Simulink-generated data
    origmag = max( abs( sigdata_out(4, : ) ) );
    outputmag = max( abs( sigdata_out(3, : ) ) );
    freqmag( freq_idx ) = 20 * log10( outputmag / origmag );
    ft = fft( sigdata_out(3, :) );
    ftr = fft( sigdata_out(4, :) );
    [ftm, ftidx] = max( abs( ft ) );
    ftang = angle( ft );
    ftrang = angle( ftr );
    phase_diff = ftang - ftrang;
    if ( phase_diff > pi )
        phase_diff = phase_diff - (2 * pi);
    elseif ( phase_diff < -pi )
        phase_diff = phase_diff + (2 * pi);
    end
    freqphase( freq_idx ) = ( ftang( ftidx ) / (2 * pi) ) * 360;
    
    fd = fopen( pfdatafilename );
    fvals = fread( fd, [3, inf], 'single' );
    fclose( fd );
    
    % Process the platform-generated data
    pforigmag = max( abs( fvals( 3, : ) ) );
    pfoutputmag = max( abs( fvals( 2, : ) ) );
    pffreqmag( freq_idx ) = 20 * log10( pfoutputmag / pforigmag );
    pfft = fft( fvals(2, : ) );
    pfftr = fft( fvals(3, : ) );
    [pfftm, pfftidx] = max( abs( pfft ) );
    if ( pfftidx ~= ftidx )
        disp( 'Freq index mismatch!!!!!!!!!!!!!!!!!!!!!!!' );
    end
    pfftang = angle( pfft );
    pfftrang = angle( pfftr );
    pf_phase_diff = pfftang - pfftrang;
    if ( pf_phase_diff > pi )
        pf_phase_diff = pf_phase_diff - (2 * pi);
    elseif ( pf_phase_diff < -pi )
        pf_phase_diff = pf_phase_diff + (2 * pi);
    end
    pffreqphase( freq_idx ) = ( pfftang( pfftidx ) / (2 * pi) ) * 360;
end

figure;
% Analytical result for comparison
dtf = tf( [-((2*47.8393) + 0.6) -((2*-47.8393) + (0.6*-0.0432))], [1 -0.0432], 0.02 );
[tmag, tphase] = bode( dtf, 2 * pi * freqs );

% Replace values over/under the threshold (saturation)
tmagsat = tmag;
thresh = (pi/4) / 0.50;
tmagidx = find( tmag > thresh );
tmagsat( tmagidx ) = thresh;

% Move the phase to +-180
tpidx = find( tphase > 180 );
tphase(tpidx) = tphase(tpidx) - 360;
tpidx = find( tphase < -180 );
tphase(tpidx) = tphase(tpidx) + 360;

subplot( 3,3,1 );
%plot( freqs, 20 * log10( reshape(tmagsat, 1, size(tmagsat,3)) ) );
semilogx( freqs, 20 * log10( reshape(tmagsat, 1, size(tmagsat,3)) ) );
axis( [0 25 -5 50] );
title( 'Magnitude Response (Analytical)');
xlabel( 'Frequency (Hz)' );
ylabel( 'decibels' );
grid;

subplot( 3,3,2 );
%plot( freqs, 20 * log10( reshape(tmagsat, 1, size(tmagsat,3)) ) );
semilogx( freqs, 20 * log10( reshape(tmagsat, 1, size(tmagsat,3)) ) );
axis( [0 0.5 -5 50] );
title( 'Magnitude Response Detail (Analytical)');
xlabel( 'Frequency (Hz)' );
ylabel( 'decibels' );
grid;

%figure; 
subplot( 3,3,3 );
%plot( freqs, reshape( tphase, 1, size(tphase,3)) );
semilogx( freqs, reshape( tphase, 1, size(tphase,3)) );
axis( [0 25 -180 180] );
title( 'Phase Response (Analytical)');
xlabel( 'Frequency (Hz)' );
ylabel( 'degrees' );
grid;

subplot( 3,3,4 );
%plot( freqs, freqmag );
semilogx( freqs, freqmag );
axis( [0 25 -5 50] );
title( 'Magnitude Response (Simulink)');
xlabel( 'Frequency (Hz)' );
ylabel( 'decibels' );
grid;

subplot( 3,3,5 );
%plot( freqs, freqmag );
semilogx( freqs, freqmag );
axis( [0 0.5 -5 50] );
title( 'Magnitude Response Detail (Simulink)');
xlabel( 'Frequency (Hz)' );
ylabel( 'decibels' );
grid;

subplot( 3,3,6 );
%plot( freqs, freqphase );
semilogx( freqs, freqphase );
axis( [0 25 -180 180] );
title( 'Phase Response (Simulink)');
xlabel( 'Frequency (Hz)' );
ylabel( 'degrees' );
grid;

subplot( 3,3,7 );
%plot( freqs, pffreqmag );
semilogx( freqs, pffreqmag );
axis( [0 25 -5 50] );
title( 'Magnitude Response (Platform)');
xlabel( 'Frequency (Hz)' );
ylabel( 'decibels' );
grid;

subplot( 3,3,8 );
%plot( freqs, pffreqmag );
semilogx( freqs, pffreqmag );
axis( [0 0.5 -5 50] );
title( 'Magnitude Response Detail (Platform)');
xlabel( 'Frequency (Hz)' );
ylabel( 'decibels' );
grid;

subplot( 3,3,9 );
%plot( freqs, pffreqphase );
semilogx( freqs, pffreqphase );
axis( [0 25 -180 180] );
title( 'Phase Response (Platform)');
xlabel( 'Frequency (Hz)' );
ylabel( 'degrees' );
grid;



