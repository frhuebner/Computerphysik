set title "Kepler-Orbit Energie (Runge-Kutta)"
set xlabel "t"
set ylabel "Energie E in J"

plot "2_runge_kutta.txt" using 1:11 pointtype 1 title ""

set terminal jpeg
set output "2_runge_kutta_energie.jpeg"
replot
set terminal x11


set title "Kepler-Orbit Energie (Leapfrog)"
set xlabel "t"
set ylabel "Energie E in J"

plot "2_leapfrog.txt" using 1:11 pointtype 1 title ""

set terminal jpeg
set output "2_leapfrog_energie.jpeg"
replot
set terminal x11


set title "Kepler-Orbit Laplace-Runge-Lenz-Vektor (Runge-Kutta)"
set xlabel "x"
set ylabel "y"

plot "2_runge_kutta.txt" using 8:9 pointtype 1 title ""

set terminal jpeg
set output "2_runge_kutta_lrl.jpeg"
replot
set terminal x11


set title "Kepler-Orbit Laplace-Runge-Lenz-Vektor (Leapfrog)"
set xlabel "x"
set ylabel "y"

plot "2_leapfrog.txt" using 8:9 pointtype 1 title ""

set terminal jpeg
set output "2_leapfrog_lrl.jpeg"
replot 
set terminal x11


set title "Kepler-Orbit Bewegung (Runge-Kutta)"
set ylabel "y"
set xlabel "x"

plot "2_runge_kutta.txt" using 2:3 pointtype 1 title ""

set terminal jpeg
set output "2_runge_kutta_bewegung.jpeg"
replot
set terminal x11


set title "Kepler-Orbit Bewegung (Leapfrog)"
set xlabel "x"
set ylabel "y"

plot "2_leapfrog.txt" using 2:3 pointtype 1 title ""

set terminal jpeg
set output "2_leapfrog_bewegung.jpeg"
replot
set terminal x11

