set title "Doppelpendel"
 
set ylabel "x in m"
set xlabel "y in m"

plot "1a.txt" using (0.2*sin($2)):(2*0.2-0.2*cos($2)) pointtype 1 title "Masse m_1 mit {/Symbol f}_1 = {/Symbol p}/4", "1a.txt" using (0.2*sin($2)+0.2*(sin($3))):(2*0.2-0.2*cos($2)-0.2*cos($3)) pointtype 1 title "Masse m_2 mit {/Symbol f}_2=3{/Symbol p}/4"

set terminal jpeg
set output "1a.jpeg"
replot
set terminal x11

plot "1b.txt" using (0.2*sin($2)):(0.2*2-0.2*cos($2)) pointtype 1 title "Masse m_1 mit {/Symbol f}_1={/Symbol p}/18", "1b.txt" using (0.2*sin($2)+0.2*sin($3)):(2*0.2-0.2*cos($2)-0.2*cos($3)) pointtype 1 title "Masse m_2 mit {/Symbol f}_2={/Symbol p}/36"

set terminal jpeg
set output "1b.jpeg"
replot
set terminal x11

set title "Doppelpendel Energiedifferenz"

set ylabel "Energiedifferenz in J"
set xlabel "Zeit t in s"

plot "1a.txt" using 1:6 pointtype 1 title "{/Symbol f}_1={/Symbol p}/4, {/Symbol f}_2=3{/Symbol p}/4"

set terminal jpeg
set output "1ae.jpeg"
replot
set terminal x11

plot "1b.txt" using 1:6 pointtype 1 title "{/Symbol f}_1={/Symbol p}/18, {/Symbol f}_2={/Symbol p}/36"

set terminal jpeg
set output "1be.jpg"
replot
set terminal x11
