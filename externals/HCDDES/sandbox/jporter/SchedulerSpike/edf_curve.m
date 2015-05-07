function [cur] = edf_curve( Tlist, periods )
%
% function [cur] = edf_curve( Tlist, periods )
% 
% Returns a curve representing the bandwidth vs. period curve for
% the specified EDF workload.  
%
% Tlist = [ period, execution time, deadline ]
% periods = [ list of desired interface periods ]

% Get all of the task ensemble numbers we'll use later
lcm_val = 1;
max_deadline = 0;
max_leftover = 0;
total_util = 0;
for idx = 1:size( Tlist, 1 )
    lcm_val = lcm( lcm_val, Tlist(idx, 1) );
    max_deadline = max( max_deadline, Tlist( idx, 3 ) );
    max_leftover = max( max_leftover, Tlist( idx, 1 ) - Tlist( idx, 3 ) );
    total_util = total_util + ( Tlist( idx, 2 ) / Tlist( idx, 3 ) );
end

lcm_val
max_deadline
max_leftover
total_util

% Accumulate task demand evaluation points
L = min( lcm_val + max_deadline, 1000 ) % ( total_util * max_leftover ) / ( 1 - total_util ) )
t = [0 L];
for Tidx = 1:size(Tlist, 1)
    t = [t 0:Tlist(Tidx, 2):L];
end

t = unique(t);

% Build demand-bound function
dbfC = zeros(size(t));
for Tidx = 1:size(Tlist, 1)
    dbfi = floor( (t + Tlist( Tidx, 1 ) - Tlist( Tidx, 3 ))/Tlist( Tidx, 1 ) ) * Tlist( Tidx, 2 );
    dbfi = max( 0, dbfi );
    dbfC = dbfC + dbfi;
end

figure; plot( t, dbfC )

cur = [0 0 0];

for idx = 1:length( periods )
    
    period = periods(idx);
    
    for cap = period/1000.0:period/1000.0:period
        
        sbf_p1 = floor( (t - (period - cap)) / period ) * cap;
        sbf_p2 = max( 0, t - 2*(period - cap) - floor( (t - (period-cap))/period) * period );
        sbf = sbf_p1 + sbf_p2;
        
        sbf(  t < (period - cap)  ) = 0;
        
        lsbf = (cap/period) * (t - 2*(period-cap));
        lsbf( t < (2*(period - cap) ) ) = 0;
        
        if ( dbfC <= lsbf )
            cur = [cur; period cap cap/period];
            figure; hold on; plot( t, sbf ); plot( t, dbfC ); plot( t, lsbf);
            break;
        end
    end
    
end