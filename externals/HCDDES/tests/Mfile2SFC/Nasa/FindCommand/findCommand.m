double
double 3 2

function MyCmdType = FindCommand(CmdID,CmdTypeSpec)

% This block supports an embeddable subset of the MATLAB language.
% See the help menu for details. 

[m n]=size(CmdTypeSpec);

MyCmdType=uint16(0);

for i=1:m

	if CmdTypeSpec(i,1)==CmdID

		MyCmdType=CmdTypeSpec(i,2);
		break

	end

end


