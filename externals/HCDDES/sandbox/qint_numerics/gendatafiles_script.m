%%
%% Generate sample files for use in testing the control components.
%%

% Iterate over a list of frequencies to test
% Open the mdl-file, set the parameters, run it, and save the data.

% Frequency range is 1/100 up to 25 Hz (stepping logarithmically)
lowend = 1/100; % Hz
highend = 25;   % Hz
steps = 100;
logfreqs = [log10(lowend): ...
    (log10(highend)-log10(lowend))/steps : log10(highend)];
freqs = 10.^logfreqs;

StopTime = 300;  % Seconds

modname = 'gendatafiles';

for freq_idx = 1:length(freqs)
    freq = freqs(freq_idx);
    
    outfilename = sprintf( 'inputs/sin%06d.dat', floor( freq * 1000 ) );
    
    clear sigdata;
    
    load_system( modname );
    set_param( sprintf( '%s/SineSig', modname), 'Frequency', num2str(freq*2*pi) );
    
    set_param( modname, 'StopTime', num2str(StopTime) );
    
    sim( modname );
    close_system( modname, 0 );
    
    fd = fopen( outfilename, 'w' );
    fwrite( fd, [sigdata.time sigdata.signals.values]', 'single' );
    size( [sigdata.time sigdata.signals.values]' )
    fclose( fd );
    
    %clear sigdata;
end