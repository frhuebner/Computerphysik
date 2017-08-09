set title "Gammalinie"
set xlabel "Energie in keV"
set ylabel "Impulsrate"

plot "profile.dat" using 2:3 pointtype 1 title ""

set terminal jpeg
set output "a.jpeg"
replot
set terminal x11

set title "Gammalinie und CMB"
set xlabel "Energie in keV"
set ylabel "Impulsrate"

plot "profile.dat" using 2:3 pointtype 1 title "Gammalinie", "cmb.dat" using 1:2 pointtype 1 title "CMB"

set terminal jpeg 
set output "b.jpeg"
replot
set terminal x11

set title "Gammalinie ohne CMB"
set xlabel "Energie in keV"
set ylabel "Impulsrate"

plot "profile_cmb.dat" using 1:2 pointtype 1 title ""

set terminal jpeg
set output "c.jpeg"
replot
set terminal x11
