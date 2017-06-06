reset

set encoding utf8

set bars 0.5
show bars
set pointsize 3

set title "Gerade Lösungen"
set samples 1000
set ylabel "y"
set xlabel "z"
#set format y "10^{%T}"
#set format z "10^{%T}"
#set logscale xy
set xrange [0:12]
#set xzeroaxis# linetype 3 linewidth 2.5
set yrange [0:10]
set key right top
set term pngcairo size 1280,960 fontscale 2 linewidth 2
set output "plot_tan.png"
plot \
 tan(x) title "tan(z)" lt rgb "black",\
 sqrt(10**2-x**2)/x title "√({/Symbol x}^2-z^2)/z "lt rgb "red"
 
set title "Ungerade Lösungen"
set output "plot_cot.png"
plot \
 -1/tan(x) title "-cot(z)" lt rgb "black",\
 sqrt(10**2-x**2)/x title "√({/Symbol x}^2-z^2)/z "lt rgb "red"