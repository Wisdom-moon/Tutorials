set term pdfcairo font "Times, 14" enhanced
unset key
set out 'p0.pdf'
set label 1 'x' at first .5, .5 + .1
plot [0:1] x
set out 'p1.pdf'
set label 1 'x^2' at first .5, .5**2 + .1
plot [0:1] x**2
set out 'p2.pdf'
set label 1 'x^3' at first .5, .5**3 + .1
plot [0:1] x**3
set out 'p3.pdf'
set label 1 'x^4' at first .5, .5**4 + .1
plot [0:1] x**4
set out 'p4.pdf'
set label 1 'x^5' at first .5, .5**5 + .1
plot [0:1] x**5
