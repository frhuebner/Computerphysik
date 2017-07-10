set title "Meteorit in der Atmosphäre"
 
set ylabel "Geschwindigkeit v in km/s"
set xlabel "Höhe h in km"

plot "2a.txt" using 2:3 pointtype 1 title "v_0 = 12 km/s", "2b.txt" using 2:3 pointtype 1 title "v_0 = 20 km/s", "2c.txt" using 2:3 pointtype 1 title "v_0 = 35 km/s", "2d.txt" using 2:3 pointtype 1 title "v_0 = 50 km/s"


set terminal jpeg
set output "2.jpeg"
replot
set terminal x11

plot "2a.txt" using 2:3 pointtype 1 title "v_0 = 12 km/s"

set terminal jpeg
set output "2a.jpeg"
replot
set terminal x11

plot "2b.txt" using 2:3 pointtype 1 title "v_0 = 20 km/s"

set terminal jpeg
set output "2b.jpeg"
replot
set terminal x11

plot "2c.txt" using 2:3 pointtype 1 title "v_0 = 35 km/s"

set terminal jpeg
set output "2c.jpeg"
replot
set terminal x11

plot "2d.txt" using 2:3 pointtype 1 title "v_0 = 50 km/s"

set terminal jpeg
set output "2d.jpeg"
replot
set terminal x11
