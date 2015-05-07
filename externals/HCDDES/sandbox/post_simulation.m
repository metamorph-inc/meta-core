%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Post sim data processing and display
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% store individual variable names, note these are the "actual" states, as
% opposed to the noisy estimated states used by the controller
t = state.time;
x = state.signals.values(:,1);
y = state.signals.values(:,2);
z = state.signals.values(:,3);
vx = state.signals.values(:,4);
vy = state.signals.values(:,5);
vz = state.signals.values(:,6);
roll = state.signals.values(:,7);
pitch = state.signals.values(:,8);
yaw = state.signals.values(:,9);
ax = state.signals.values(:,10);
ay = state.signals.values(:,11);
az = state.signals.values(:,12);
wx = state.signals.values(:,13);
wy = state.signals.values(:,14);
wz = state.signals.values(:,15);

t_c = state_compare.time;
x_c = state_compare.signals.values(:,1);
y_c = state_compare.signals.values(:,2);
z_c = state_compare.signals.values(:,3);
vx_c = state_compare.signals.values(:,4);
vy_c = state_compare.signals.values(:,5);
vz_c = state_compare.signals.values(:,6);
roll_c = state_compare.signals.values(:,7);
pitch_c = state_compare.signals.values(:,8);
yaw_c = state_compare.signals.values(:,9);
ax_c = state_compare.signals.values(:,10);
ay_c = state_compare.signals.values(:,11);
az_c = state_compare.signals.values(:,12);
wx_c = state_compare.signals.values(:,13);
wy_c = state_compare.signals.values(:,14);
wz_c = state_compare.signals.values(:,15);


rolldeg = 180/pi*roll ;
pitchdeg = 180/pi*pitch ;
yawdeg = 180/pi*yaw;

rolldeg_c = 180/pi*roll_c ;
pitchdeg_c = 180/pi*pitch_c ;
yawdeg_c = 180/pi*yaw_c;


%% Plotting
figure(1);clf;
subplot(2,1,1);
plot(t, roll, t, roll_c, t, pitch, t, pitch_c);  hold on;
plot(att_traj.time, att_traj.signals.values(:,1),'--', att_traj.time, att_traj.signals.values(:,2),'--');
grid on
title('Attitude')
legend('Roll', 'Pitch')

subplot(2,1,2)
plot(t, yaw, t, yaw_c);  hold on;
plot(att_traj.time, att_traj.signals.values(:,3),'--');
grid on
title('Yaw')
legend('Yaw')

figure(2);clf;
subplot(3,1,1);
plot(t, x, t, x_c); hold on;
grid on
title('X')

subplot(3,1,2);
plot( t, y, t, y_c ); hold on;
grid on
title('Y')

subplot(3,1,3);
plot(t, z, t, z_c); hold on;
grid on
title('Z')

figure(3);clf;
plot3(y,x,-z); hold on;
plot3(y(1),x(1),-z(1),'rx')
grid on
title('XYZ Plot')
xlabel('East')
ylabel('North')
zlabel('Altitude')


figure(4);clf;
subplot(3,1,1);
plot(t, vx, t, vx_c); hold on;
grid on
title('Vx')

subplot(3,1,2);
plot( t, vy, t, vy_c); hold on;
grid on
title('Vy')

subplot(3,1,3);
plot(t, vz, t, vz_c); hold on;
grid on
title('Vz')

if (select == 2)
figure(5);clf; % Flapping plot
subplot(3,1,1);
plot(t, pitchdeg, t, pitchdeg_c);  hold all;
plot(att_traj.time, 180/pi*att_traj.signals.values(:,2),'--');
plot([0 12], [0 0] ,'r', 'LineWidth', 2)
grid on
ylabel( 'Pitch')
legend('Actual','Commanded')

subplot(3,1,2);
plot(t, -vx, t, -vx_c);  hold all;
plot([0 12], [0 0] ,'r', 'LineWidth', 2)
grid on
ylabel('Velocity')

subplot(3,1,3);
plot(t, -z, t, -z_c);  hold all;
plot([0 12], [0 0] ,'r', 'LineWidth', 2)
grid on
ylabel('Altitude')
end

if (select == 3)
figure(5);clf; % Popup plot
subplot(3,1,2);
plot(t, rolldeg, t, rolldeg_c);  hold all;
plot(att_traj.time, 180/pi*att_traj.signals.values(:,1),'x--');
plot([0 6], [0 0] ,'r', 'LineWidth', 2)
grid on
ylabel( 'Roll')
legend('Actual','Commanded')

subplot(3,1,1);
plot(t, vy, t, vy_c);  hold all;
plot([0 6], [0 0] ,'r', 'LineWidth', 2)
grid on
ylabel('Velocity')

subplot(3,1,3);
plot(t, -z, t, -z_c);  hold all;
plot([0 6], [0 0] ,'r', 'LineWidth', 2)
grid on
ylabel('Altitude')
end

if (select == 4)
figure(5);clf;  % Att tracking plot
plot(t, rolldeg, t, rolldeg_c);  hold all;
plot(att_traj.time, 180/pi*att_traj.signals.values(:,1),'x--');
plot(t, vy ,'--', t, vy_c , '--' );  hold all;
grid on
axis([0 20 -15 25])
end


if (select == 5)
figure(5);clf;  % Hover plot
plot(x, y );  hold all;
ezplot('x^2+y^2 - 0.4^2');
plot(x(1),y(1),'ro','MarkerSize',8)
plot(x(end),y(end),'rx','MarkerSize',8)
axis equal;
end
