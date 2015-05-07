function [cur] = dm_curve( Tlist, periods )
%
% function [cur] = dm_curve( Tlist, periods )
% 
% Returns a curve representing the bandwidth vs. period curve for
% the specified DM workload.  
%
% Tlist = [ period, execution time, deadline ]
% periods = [ list of desired interface periods ]

% Sort the task list according to deadline
[ntasks, sort_idx] = sort( Tlist( :, 3 ) );
Tlist = Tlist( sort_idx, : );

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

% Build demand-bound functions
rbfi = zeros(length(t), size(Tlist, 1));
rbfA = zeros(length(t), size(Tlist, 1));
for Tidx = 1:size(Tlist, 1)
    rbfi(:,Tidx) = ceil( t / Tlist( Tidx, 1 ) )' * Tlist( Tidx, 2 );
    rbfA(:,Tidx) = sum( rbfi, 2 );    
end

figure; plot( t, rbfi );
figure; plot( t, rbfA );

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

        % Different criterion - for each task, check up to its deadline
        % and see if we have one point in the list that satisfies the
        % criterion
        failure_flag = 0;
        for Tidx = 1:size( Tlist, 1 )
            deadline_idxs = ( t <= Tlist( Tidx, 3 ) );
            comparison_idxs = ( rbfA( :, Tidx ) <= lsbf' );
            result_idxs = deadline_idxs' .* comparison_idxs;
            result_idxs(1) = 0;
            if ( max(result_idxs) == 0 )
                failure_flag = 1;
            end
        end
        if ( failure_flag == 0 )
            cur = [cur; period cap cap/period];
            %figure; hold on; plot( t, sbf ); plot( t, rbfA ); plot( t, lsbf);
            break;
        end
    end
    
end