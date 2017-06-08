reset

set encoding utf8

set bars 0.5
show bars
set pointsize 3


set ylabel "y"
set xlabel "x"
set key right top
set term pngcairo size 1280,960 fontscale 2 linewidth 2
set title "Bessel J_1(x)"
set output "plot_bessel_1.png"
plot \
 "bessel_1.txt" using 1:2 title "J_1(x)" with lines lt rgb "black"
set title "Bessel J_2(x)"
set output "plot_bessel_2.png"
plot \
 "bessel_2.txt" using 1:2 title "J_2(x)" with lines lt rgb "black"
set title "Bessel J_3(x)"
 set output "plot_bessel_3.png"
plot \
 "bessel_3.txt" using 1:2 title "J_3(x)" with lines lt rgb "black"
set title "Bessel J_4(x)"
 set output "plot_bessel_4.png"
plot \
 "bessel_4.txt" using 1:2 title "J_4(x)" with lines lt rgb "black"
 set title "Bessel J_5(x)"
 set output "plot_bessel_5.png"
plot \
 "bessel_5.txt" using 1:2 title "J_5(x)" with lines lt rgb "black"
 set title "Bessel J_6(x)"
 set output "plot_bessel_6.png"
plot \
 "bessel_6.txt" using 1:2 title "J_6(x)" with lines lt rgb "black"
 set title "Bessel J_7(x)"
 set output "plot_bessel_7.png"
plot \
 "bessel_7.txt" using 1:2 title "J_7(x)" with lines lt rgb "black"
  set title "Bessel J_7(x)"
 set output "plot_bessel_0.png"
plot \
 "bessel_0.txt" using 1:2 title "J_0(x)" with lines lt rgb "black"