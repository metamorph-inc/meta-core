function mypoissrnd(block)
% Level-2 M file S-Function for inherited sample time demo.
%   Copyright 1990-2009 The MathWorks, Inc.
%   $Revision: 1.1.6.1 $ 

  setup(block);
  
%endfunction

function setup(block)
  block.NumDialogPrms = 3;      % sample period (tick), faults per second, mean delay ticks
  
  %% Register number of input and output ports
  block.NumInputPorts  = 0;
  block.NumOutputPorts = 1;

  %% Setup functional port properties to dynamically
  %% inherited.
  
  block.SetPreCompPortInfoToDefaults;
  block.OutputPort(1).Dimensions       = 1;
  
  %% User sets block sample time
  block.SampleTimes = [block.DialogPrm(1).Data 0];
  
  %% Set the block simStateCompliance to default (i.e., same as a built-in block)
  block.SimStateCompliance = 'DefaultSimState';

  %% Register methods
  block.RegBlockMethod('PostPropagationSetup',    @DoPostPropSetup);
  block.RegBlockMethod('InitializeConditions',    @InitConditions);  
  block.RegBlockMethod('Outputs',                 @Output);  
  block.RegBlockMethod('Update',                  @Update);  
  
%endfunction

function DoPostPropSetup(block)

  %% Setup Dwork
  block.NumDworks = 1;
  block.Dwork(1).Name = 'nticks'; 
  block.Dwork(1).Dimensions      = 1;
  block.Dwork(1).DatatypeID      = 0;
  block.Dwork(1).Complexity      = 'Real';
  block.Dwork(1).UsedAsDiscState = true;

%endfunction

function InitConditions(block)

  %% Initialize Dwork
  block.Dwork(1).Data = 0;
  
%endfunction

function Output(block)

  % (faults per second) * (clock tick)
  lambda = block.DialogPrm(2).Data * block.DialogPrm(1).Data;
  numevents = poissrnd(lambda);
  
  % generate random delay
  if (numevents > 0), 
      block.Dwork(1).Data = block.Dwork(1).Data + numevents*(1 + poissrnd(block.DialogPrm(3).Data));
  end;
  
%   if ( numevents > 0 && block.Dwork(1).Data < 1 )
%      block.Dwork(1).Data =  randi( [1, block.DialogPrm(3).Data], 1, 1 );
%   end
  
  block.OutputPort(1).Data =  block.Dwork(1).Data;
  
%endfunction

function Update(block)

    % update delay counter
    if ( block.Dwork(1).Data > 0 )
        block.Dwork(1).Data = block.Dwork(1).Data - 1.0;
    end
    
%endfunction
