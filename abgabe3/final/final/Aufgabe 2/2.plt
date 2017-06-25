set title "Pendelschwingdauer"

set xlabel "{/Symbol f}_{0}"
set ylabel "T({/Symbol f}_{0}) - T({/Symbol f}_{0}=0)"

set logscale y

plot "data.txt" using 1:2

set terminal png
set output "2.png"
replot
set terminal x11


