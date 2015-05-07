%%
%% Process sample files using the outer loop component.
%%

% Iterate over the recorded input data files
% Open the mdl-file, set the parameters, run it, and save the data.

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

for freq_idx = 1:length(freqs)
    freq = freqs(freq_idx);
    
    infilename = sprintf( 'inputs/sin%06d.dat', floor( freq * 1000 ) );
    outfilename = sprintf( 'ol_outputs/rslt%06d.mat', floor( freq * 1000 ));
    
    clear sigdata_in;
    
    load_system( modname );
    fd = fopen( infilename, 'r' );
    fvals = fread( fd, [2 inf], 'single' );
    fclose( fd );
    
    sigdata_in.time = fvals( 1, : )';
    sigdata_in.signals.dimensions = 1;
    sigdata_in.signals.values = fvals( 2, :)';
    
    set_param( sprintf( '%s/SaveData', modname), 'Filename', outfilename );
    
    set_param( modname, 'StopTime', num2str(StopTime) );
    
    sim( modname );
    close_system( modname, 0 );
    
end