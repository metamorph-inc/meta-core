clear peak;
peak = max(PeakVals.signals.values(1:end));
clear index;
index = find(peak == PeakVals.signals.values(1:end));
clear f_peak;
clc
RValue
LValue
CValue
f_peak = (PeakVals.signals.values(index, 2)-1)*32
