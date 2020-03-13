h = figure;
plot(d1_mat(:,1:2), '.');
legend('\theta_1','\theta_2','x','y')
set(h,'Units','Inches');
pos = get(h,'Position');
set(h,'PaperPositionMode','Auto','PaperUnits','Inches','PaperSize',[pos(3), pos(4)])
print(h,'d1_angle','-dpdf','-r0')