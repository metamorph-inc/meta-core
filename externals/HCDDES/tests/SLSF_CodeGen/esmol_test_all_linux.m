function esmol_test_all()
    % Add your working unit tests here as (mdl,directory) pairs
%     tests = {
%         'sl_basic'                      ,'sl_basic';
%         'sl_chart'                      ,'sl_chart';
% 
%         'abs'                           ,'unit\prims\Abs';
%         'busobject'                     ,'unit\prims\busobject';
%         'busselector'                   ,'unit\prims\busselector';
%         'constant'                      ,'unit\prims\constant';
%         'demux'                         ,'unit\prims\basic_demux';
%         'demux_vector_conv'             ,'unit\prims\demux_vector_conv';
%         'embedded'                      ,'unit\prims\embedded\simple_embedded';
%         'embedded_with_delay'           ,'unit\prims\embedded\embedded_with_delay';
%         'gain'                          ,'unit\prims\gain';
%         'genericStructBusObject'        ,'unit\prims\genericStructBusObject';
%         'ground'                        ,'unit\prims\ground';
%         'iftest'                        ,'unit\prims\iftest';
%         'logic'                         ,'unit\prims\logic';
%         'math'                          ,'unit\prims\math';
%         'minmax_basic'                  ,'unit\prims\minmax';
%         'multiportswitch_basic'         ,'unit\prims\multiportswitch';         
%         'mux'                           ,'unit\prims\mux';        
%         'product'                       ,'unit\prims\product';
%         'relationalop'                  ,'unit\prims\relationalop';
%         'saturate'                      ,'unit\prims\saturate';
%         'sf_data_store_memory'          ,'unit\prims\SF_data_store_memory';
%         'sign'                          ,'unit\prims\signum';
%         'sum'                           ,'unit\prims\sum';
%         'switch_basic'                  ,'unit\prims\switch';
%         'switchcase'                    ,'unit\prims\switchcase';
%         'tofrom'                        ,'unit\prims\tofrom';
%         'unitdelay'                     ,'unit\prims\unitdelay';
%         'trig'                          ,'unit\prims\trig';
%         'zt'                            ,'unit\prims\zt';
%         'twelve_oclock'                 ,'unit\stateflow\12oclock';
%         'and_state_interleavings_1_2'   ,'unit\stateflow\and_state_interleavings';
%         'and_state_interleavings_2_1'   ,'unit\stateflow\and_state_interleavings';        
%         'counter'                       ,'unit\stateflow\counter';        
%         'dont_enter_at_initialization'  ,'unit\stateflow\dont_enter_at_initialization';
%         'user_spec_exec_order','unit\stateflow\user_spec_exec_order';
%     };
    tests = esmol_list_tests();
    NUM_TESTS = size(tests,1);
    succeeded = true(1,NUM_TESTS);


    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    
    %% Run the tests
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    tic;
    for i=1:NUM_TESTS
        model = char(tests(i,1));
        [path,model] = fileparts(model);    % the .mdl extension is optional
        assert(length(path)==0);            % not really needed
        dir   = char(tests(i,2));
        disp(['TEST ', num2str(i), ' of ', num2str(NUM_TESTS), ': ', model]);
        disp '============================================================'
        try 
            esmol_test_linux(model,dir);
        catch
            %err = lasterror;
            %error(['FAILED: ', model]); 
            succeeded(i) = false;
        end
        disp ' '
    end
    toc;
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    
    %% Summarize Results
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    
    disp ' '
    green = find(succeeded==true);
    red   = find(succeeded==false);
    num_green = length(green);
    num_red   = length(red);
    if (num_red == 0)
        disp ' '
        disp 'All tests passed'
        return;
    end    
    
    msg = ['SUCCEEDED(', num2str(num_green), '): '];
    for i=1:length(green)
        msg = [msg, ' ', char(tests(green(i),1))];
    end
    disp(msg);
    
    msg = ['FAILED(', num2str(num_red), '): '];
    for i=1:length(red)
        msg = [msg, ' ', char(tests(red(i),1))];
    end
    disp(msg);  
end

    
