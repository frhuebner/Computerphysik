set title "Schiessverfahren"
set xlabel "{/Symbol a}"
set ylabel "F({/Symbol a})"

f(x) = a*x + b

fit f(x) "1.txt" using 2:7 via a,b	

plot "1.txt" using 2:7 pointtype 1 title "",f(x)

set terminal jpeg
set output "1.jpeg"
replot
set terminal x11
