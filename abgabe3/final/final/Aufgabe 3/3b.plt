set title "Planckkurve"
set encoding iso_8859_1

set xlabel "Temperatur T in K"
set ylabel "E_{vis} / E_{ges}"

plot "data.txt" using 1:2 with linespoints title " "

set terminal png
set output "3b.png"
replot
set terminal x11
