set title "Planckkurve"

set xlabel "Wellenlänge {/Symbol w} in m"
set ylabel "spektrale Flächenleistung S({/Symbol w}) in W/m^{3}"

f(x)=(2*pi*6.626*10**(-34)*(2.997*10**8)**2)/(x**5) * 1/(exp((6.626*10**(-34)*2.997*10**8)/(x*1.381*10**(-23)*100)) - 1)

g(x)=(2*pi*6.626*10**(-34)*(2.997*10**8)**2)/(x**5) * 1/(exp((6.626*10**(-34)*2.997*10**8)/(x*1.381*10**(-23)*500)) - 1)

h(x)=(2*pi*6.626*10**(-34)*(2.997*10**8)**2)/(x**5) * 1/(exp((6.626*10**(-34)*2.997*10**8)/(x*1.381*10**(-23)*1000)) - 1)

set yrange[0:1000000]
set xrange[0:0.0001]

plot f(x) title "T=100K", g(x) title "T=500K", h(x) title "T=1000K"

set terminal jpeg
set output "3a_100.jpeg"
replot
set terminal x11


