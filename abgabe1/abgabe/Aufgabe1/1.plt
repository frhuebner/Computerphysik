set title "Aufgabe 1a"
set encoding iso_8859_1

set xlabel "Anzahl der Päckchen"
set ylabel "Häufigkeit"
plot "1a.txt" title ""

set terminal jpeg
set output "1a.jpeg"
replot

set terminal x11

set title "Aufgabe 1b"
set encoding iso_8859_1

set xlabel "Anzahl der Päckchen"
set ylabel "Häufigkeit"
plot "1b.txt" title ""

set terminal jpeg
set output "1b.jpeg"
replot
