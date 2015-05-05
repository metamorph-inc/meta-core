% Parameter specs should be outside this file BEGIN
global prevDat;
global wind;
global CValue;
global LValue;
global RValue;
CValue = 0.00068;
LValue = 0.001;
RValue = 2;
prevDat = zeros(1, 1024);
wind = hamming(1024)';
% Parameter specs should be outside this file END

% HBG datastructure printer - test version
clear bg;
global bg;
bg = [];
bg.sampleTime = 0.5;

% Printing bond node #1
bg.node(1).ID = 1;
bg.node(1).name = 'RLC/Components/RLC/BondGraphDynamics/MSe';
bg.node(1).type = 'MSe';
bg.node(1).numBonds = 1;
bg.node(1).bond(1) = 2;

% Printing bond node #2
bg.node(2).ID = 2;
bg.node(2).name = 'RLC/Components/RLC/BondGraphDynamics/zj0';
bg.node(2).type = 'ZeroJunction';
bg.node(2).numBonds = 2;
bg.node(2).bond(1) = 1;
bg.node(2).bond(2) = 3;
bg.node(2).state = 1;
bg.node(2).index = 2;
bg.node(2).hybrid = 0;

% Printing bond node #3
bg.node(3).ID = 3;
bg.node(3).name = 'RLC/Components/RLC/BondGraphDynamics/oj0';
bg.node(3).type = 'OneJunction';
bg.node(3).numBonds = 4;
bg.node(3).bond(1) = 2;
bg.node(3).bond(2) = 4;
bg.node(3).bond(3) = 6;
bg.node(3).bond(4) = 8;
bg.node(3).state = 1;
bg.node(3).index = 1;
bg.node(3).hybrid = 0;

% Printing bond node #4
bg.node(4).ID = 4;
bg.node(4).name = 'RLC/Components/RLC/BondGraphDynamics/zj1';
bg.node(4).type = 'ZeroJunction';
bg.node(4).numBonds = 2;
bg.node(4).bond(1) = 3;
bg.node(4).bond(2) = 5;
bg.node(4).state = 1;
bg.node(4).index = 3;
bg.node(4).hybrid = 0;

% Printing bond node #5
bg.node(5).ID = 5;
bg.node(5).name = 'RLC/Components/RLC/BondGraphDynamics/R';
bg.node(5).type = 'R';
bg.node(5).numBonds = 1;
bg.node(5).bond(1) = 4;

% Printing bond node #6
bg.node(6).ID = 6;
bg.node(6).name = 'RLC/Components/RLC/BondGraphDynamics/zj2';
bg.node(6).type = 'ZeroJunction';
bg.node(6).numBonds = 2;
bg.node(6).bond(1) = 7;
bg.node(6).bond(2) = 3;
bg.node(6).state = 1;
bg.node(6).index = 4;
bg.node(6).hybrid = 0;

% Printing bond node #7
bg.node(7).ID = 7;
bg.node(7).name = 'RLC/Components/RLC/BondGraphDynamics/C';
bg.node(7).type = 'C';
bg.node(7).numBonds = 1;
bg.node(7).bond(1) = 6;

% Printing bond node #8
bg.node(8).ID = 8;
bg.node(8).name = 'RLC/Components/RLC/BondGraphDynamics/zj3';
bg.node(8).type = 'ZeroJunction';
bg.node(8).numBonds = 2;
bg.node(8).bond(1) = 9;
bg.node(8).bond(2) = 3;
bg.node(8).state = 1;
bg.node(8).index = 5;
bg.node(8).hybrid = 0;

% Printing bond node #9
bg.node(9).ID = 9;
bg.node(9).name = 'RLC/Components/RLC/BondGraphDynamics/L';
bg.node(9).type = 'I';
bg.node(9).numBonds = 1;
bg.node(9).bond(1) = 8;

bg.numNodes = 9;
bg.numJunctions = 5;
bg.numHybridJunctions = 0;
bg.db = [ -1 -1 -1 -1 -1 ];
global determiningBonds;
