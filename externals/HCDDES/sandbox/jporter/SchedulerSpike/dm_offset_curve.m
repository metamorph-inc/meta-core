function [cur] = dm_offset_curve( Tlist, periods )
%
% function [cur] = dm_offset_curve( Tlist, periods )
% 
% Returns a curve representing the bandwidth vs. period curve for
% the specified DM workload.  
%
% Tlist = [ period, execution time, deadline, offset, jitter ]
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
t_x = [];
t_x_idx = [];
for Tidx = 1:size(Tlist, 1)
    L = lcm_val - Tlist( Tidx, 3 ) + Tlist( Tidx, 4 ) + Tlist( Tidx, 5)
    t = [ (Tlist( Tidx, 4 ) + Tlist( Tidx, 5 )):Tlist( Tidx, 1 ):L ];
    t_x_idx = [t_all_idx length(t_all) + 1];
    t_x = [t_all t];
end

t_x_idx = [t_x_idx length(t_x)];

t = unique(t_all);

% Request function ideas: could have a two-dimensional request function,
%   cons: lots of space, little bit of wasted computation  
%   pros: easily handle summation of the priorities
% or evaluate it dynamically as we visit each of the tasks
%   cons: tricky to repeat sums for priorities, avoid a small amount of
%          wasted computation
%   pros: less space

% Build request functions
rfi = zeros(length(t), length(t), size(Tlist, 1));
rfA = zeros(length(t), length(t), size(Tlist, 1));
for Tidx = 1:size(Tlist, 1)
    pd = Tlist( Tidx, 1 );
    wcet = Tlist( Tidx, 2 );
    offset = Tlist( Tidx, 4 );
    jitter = Tlist( Tidx, 5 );
    
    for idx1 = 1:length(t)
        for idx2 = 1:length(t)
            t1 = t(idx1);
            t2 = t(idx2);
            rfi( idx1, idx2, Tidx ) = ( ceil( ( t2 - offset ) / pd ) - ...
                ceil( ( t1 - offset - jitter ) / pd ) ) * wcet;
        end
    end
    rfA(:,:,Tidx) = sum( rfi, 3 );    
end

% What's the best way to visualize these functions?
%figure; plot( t, rfi );
%figure; plot( t, rfA );

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

        % Criterion - for each task, iterate over its tx points
        %  For each tx interval see if we have one point in the list 
        %   that satisfies both schedulability criteria
        failure_flag = 0;
        for Tidx = 1:size( Tlist, 1 )
            
            t_x_list = t_x(t_x_idx(Tidx):t_x_idx(Tidx+1));
            
            % TODO: This is where we left off
                
                deadline_idxs = ( t <= Tlist( Tidx, 3 ) );
                comparison_idxs = ( rbfA( :, Tidx ) <= lsbf' );
                result_idxs = deadline_idxs' .* comparison_idxs;
                result_idxs(1) = 0;
                if ( max(result_idxs) == 0 )
                    failure_flag = 1;
                end
            end
        end
        if ( failure_flag == 0 )
            cur = [cur; period cap cap/period];
            %figure; hold on; plot( t, sbf ); plot( t, rbfA ); plot( t, lsbf);
            break;
        end
    end
    
end