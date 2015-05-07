%
% Copyright (c) 2008, Institute for Software Integrated Systems, Vanderbilt University
% All rights reserved.
%
% Permission to use, copy, modify, and distribute this software and its
% documentation for any purpose, without fee, and without written agreement is
% hereby granted, provided that the above copyright notice, the following
% two paragraphs and the author appear in all copies of this software.
%
% IN NO EVENT SHALL THE VANDERBILT UNIVERSITY BE LIABLE TO ANY PARTY FOR
% DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
% OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE VANDERBILT
% UNIVERSITY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
%
% THE VANDERBILT UNIVERSITY SPECIFICALLY DISCLAIMS ANY WARRANTIES,
% INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
% AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
% ON AN "AS IS" BASIS, AND THE VANDERBILT UNIVERSITY HAS NO OBLIGATION TO
% PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
%

function [f,v] = fifo(c,f,v)
% [f,v]=fifo(c,f,v)
% returns:
% f fifo structure
% v matrix popped/pushed off/on structure
% takes:
% c command (FIFO_PUSH, FIFO_POP, FIFO_NEW)
% f fifo structure
% v value to push on fifo or max size of fifo else ignored
% usage:
% N = 8;
% dim = [2,2];
% Ndim = [N dim];
% [f,v] = fifo([],[],Ndim); to create a new FIFO of length N for
%                           dim matrices
% [f,v] = fifo(f.PUSH,f,rand(dim))
% [f,v] = fifo(f.PUSH,f,rand(dim))
% [f,v] = fifo(f.POP,f,[])
% [f,v] = fifo(f.POP,f,[])
  s.FIFO_PUSH=0;
  s.FIFO_POP=1;
  s.FIFO_NEW=2;
  if isempty(c) || isempty(f)
    c = s.FIFO_NEW;
  end
  switch c
   case s.FIFO_PUSH, %Push v onto the stack
    [f,v]=fifo_push(f,v);
   case s.FIFO_POP,  %Pop v from the stack
    [f,v]=fifo_pop(f,v);
   case s.FIFO_NEW,  %Create new FIFO
    [f,v]=fifo_new(f,v,s);
   otherwise,
    warning('unrecognized switch')
    v=[];
  end
  
function [f,v]=fifo_push(f,v)
  if ~isstruct(f)
    v=[];
    warning('f is not a fifo');
    warning('try [f,v] = fifo([],[],N), to create a new FIFO of length N');
    return
  end
  if f.h < f.N
    f.s{f.h+1}=v;
    f.h = f.h + 1;
    v=[];
  else
      v=[];
  end
  
function [f,v]=fifo_pop(f,v)
  if ~isstruct(f)
    v=[];
    warning('f is not a fifo');
    warning('try [f,v]=fifo(FIFO_NEW,[],[]) to create fifo');
    return
  end
  if f.h > 0
    v = f.s{f.h};
    f.h=f.h-1;
  else
    v = [];
  end
  
function [f,v]=fifo_new(f,v,s)
  if isempty(v) || (length(v) > 3)
    f.N=8;
    dim=[1,1];
  elseif length(v)==1
    f.N=v;
    dim=[1,1];
  elseif length(v) == 2
    f.N=v(1);
    dim=[1,v(2)];
  else
    f.N=v(1);
    dim=v(2:3);
  end
  f.N=max(ceil(f.N),1);
  f.s=cell(1,f.N);
  dim=max(ceil(dim),1);
  for i = 1 : f.N
    f.s{i}=zeros(dim);
  end
  f.h=0;
  f.NEW=s.FIFO_NEW;
  f.POP=s.FIFO_POP;
  f.PUSH=s.FIFO_PUSH;
  v=[];