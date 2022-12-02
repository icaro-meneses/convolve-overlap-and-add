close all;
clear all;
clc;

%%
% Leitura dos dados de medição
arquivo_leitura_y_n = fopen('./y_n.txt', 'r');
arquivo_leitura_r_n = fopen('./r_n.txt', 'r');

y_n = fscanf(arquivo_leitura_y_n, "%f\n");
r_n = fscanf(arquivo_leitura_r_n, "%f\n");

fclose(arquivo_leitura_y_n);
fclose(arquivo_leitura_r_n);

n = [0: 1: length(y_n) - 1];
m = [0: 1: length(r_n) - 1];
T = 21;

y_fig = figure();

plot(n, y_n, 'r', 'LineWidth', 1);
hold on;
plot(m, r_n, 'b-.', 'LineWidth', 1);
xlabel('n', 'fontsize', 12);
ylabel('y(n)', 'fontsize', 12);
title({'Resultado da convolução', 'y(n) = r(n) * h(n)'}, 'fontsize', 12);
xlim([0, (2 * T)]);
legend({'y(n)', 'r(n)'}, 'fontsize', 10);
grid on;
hold off;

print (y_fig, "y_n_plot.pdf", "-dpdflatexstandalone");
system ("pdflatex y_n_plot");
