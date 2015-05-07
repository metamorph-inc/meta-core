% this automatically adds the truetime path and starts it up:
addpath(getenv('TTKERNEL'))
w = cd;
addpath(w);
init_truetime;
