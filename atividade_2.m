close all;
clear all;
clc;

%%
% Leitura dos dados de medição
arquivo_leitura_y_n = fopen('./y_n.txt', 'r');

y_n = fscanf(arquivo_leitura_y_n, "%f\n");

fclose(arquivo_leitura_y_n);

n = [0: 1: length(y_n) - 1 ];

y_fig = figure();

plot(n, y_n, 'r', 'LineWidth', 1);
hold on;
xlabel('n', 'fontsize', 12);
ylabel('y(n)', 'fontsize', 12);
title({'Resultado da convolução', 'y(n) = x(n) * h(n)'}, 'fontsize', 12);
grid on;
hold off;

print (y_fig, "y_n_plot.pdf", "-dpdflatexstandalone");
system ("pdflatex y_n_plot");
open y_n_plot.pdf
