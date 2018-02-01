% asdf
%{
this is a
multiline
comment
%}
function [m,s,w,strarra] = stat2(a, b, c)
m = a * b * c
s = a ^ b
w = struct('m', m, 's', s)
strarra = {'Mercury', 'Venus', 'Earth'}
end

function m = avg(x,n)
m = sum(x)/n;
end
