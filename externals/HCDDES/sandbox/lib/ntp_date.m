function [s,r] = ntp_date(ntp_server,file_name)
% [s,r] = ntp_date(ntp_server,file_name)
% To store the return value from ntpdate at run-time:
% [s,r] = system(['ntpdate -q ',ntp_server]);
% if ~s
%     save(file_name,'r');
% end
% To load result from file_name do the following:
% file_name='ntp_date'
% fh = load(file_name);
% display(fh.r);
[s,r] = system(['ntpdate -q ',ntp_server]);%'nist1.columbiacountyga.gov');
if ~s
    save(file_name,'r');%'ntp_date','r');
end
