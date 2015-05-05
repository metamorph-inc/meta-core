function result = findPeak(inDat)
global prevDat;
global wind;
prevDat = [prevDat inDat];
prevDat = prevDat(2:length(prevDat));
fhist = abs(fft(prevDat.*wind));
%fhist = abs(fft(prevDat));
[val idx] = max(fhist);
if val > 1000 
  val = 0;
 end
 result = [val idx];

