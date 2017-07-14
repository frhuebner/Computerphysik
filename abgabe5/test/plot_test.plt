reset

set encoding utf8

set bars 0.5
show bars
set pointsize 3

set key right top
set term pngcairo size 1280,960 fontscale 2 linewidth 2

set logscale y

set output "plot_test.png"
plot \
 "test.txt" using 1:2 title "Test" with lines lt rgb "black",\
 exp(x*x)

set output "plot_test_h.png"
plot \
 "test.txt" using 1:4 title "Test" with lines lt rgb "black"
 
set output "plot_test_error.png"
plot \
 "test.txt" using 1:(($2)-exp(($1)**2)) title "Test" with lines lt rgb "black" 

set output "plot_test_error_rel.png"
plot \
 "test.txt" using 1:((($2)/exp(($1)**2))-1) title "Test" with lines lt rgb "black" 
 
 unset logscale
 
set output "plot_test1.png"
plot \
 "test1.txt" using 1:2 title "Test" with lines lt rgb "black",\
 cos(x)
 
  
set output "plot_test1_circ.png"
plot \
 "test1.txt" using 2:3 title "Test" with lines lt rgb "black",\
 
set logscale y
set output "plot_test1_error.png"
plot \
 "test1.txt" using 1:(($2)-cos(($1))) title "Test" with lines lt rgb "black"  

 set output "plot_test1_error_rel.png"
plot \
 "test1.txt" using 1:((($2)/cos(($1)))-1) title "Test" with lines lt rgb "black"  

 unset logscale
 
 set output "plot_test1_h.png"
plot \
 "test1.txt" using 1:5 title "Test" with lines lt rgb "black"
