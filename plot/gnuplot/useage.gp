Specify the range on x-axis:
>plot [5:5] sin(x)

Save plot data:
>set table 'data.text'
>plot -x**2
>unset table

Plot data from file:
>plot 'data.text'

Plot multiple line:
>plot sin(x), -x**2

Plot with two different y-axes
>set y2tics -100, 10      -100 is the starting value, 10 is the interval between tics
>set ytics nomirror       means use different axis on the right-hand side.
>plot sin(1/x) axis x1y1, 100*cos(x) axis x1y2

Plot with two different x-axes
>set x2tics -20, 2
>set xtics nomirror
>set xrange [-10:10]
>set x2range [-20:0]
>plot sin(1/x) axis x1y1, 100*cos(x-1) axis x2y2

Plot scatter figure
>plot 'scatter.dat' with points pt 7

Plot boxes
>set style fill pattern
>plot [-3:6] besj0(x) with boxes, sin(x) with boxes

Plot circles
>set key off
>plot "circles.text" with circles

Plot filled curves
>plot [0:50] besy0(x) with filledcurves above y1=0.07, besy0(x)
>set style fill pattern 5
>plot 'intersection' using 1:2:3 with filledcurves,\
    '' using 1:2 lw 3 notitle, '' using 1:3 lw 3 notitle
----using selects the column from the data file;
----lw 3 chooses the line thickness
----notitle tells gnuplot not to add an legend entry
----'' represents previous file name
>plot [0:1] '+' using 1:($1):(sin($1)) with filledcurves,\
    x lw 3 notitle, sin(x) lw 3 notitle
----'+' refers to a fictitious datafile
----$1 means the first column value

Plot histograms
>set style fill solid 1.0 border lt -1
>set style data histograms
>set style histogram rowstacked
>plot [0:40] 'circles.text' using (-$2), '' using (20*$3) notitle
----rowstacked means data will be plotted together in stack, default is multiple bars.
>plot "dist1.dat" using 1:(.0001) smooth kdensity title "1" dt 1 lw 4

Plot errorbars
>set pointsize 3
>set bars 3
>plot [1:3] 'circles.text' using 1:(-$2):3 with errorbars, '' using 1:(-$2):3 pt 7 notitle
>plot [1:3] 'circles.text' using 1:(-$2):3 with boxerrorbars

Plot impulse
>set samples 30
>plot [0:2*pi] sin(x) with impulses lw 2 notitle, sin(x) with points pt 7
----samples will set the number of points used to sample or plot the function.

Plot parametric curves
>set samples 1000
>set parametric
>plot sin(3*t), cos(11*t) notitle
>set trnage  [0:2]
>replot
----parametric changes the way gnuplot interprets plot commands; now the two functions
----  are understood to provide x and y coordinates as the parameter t is varied.

Plot polar
>set xtics axis nomirror
>set ytics axis nomirror
>set zeroaxis
>unset border
>set samples 500
>set polar
>plot [0:12*pi] t
---- set nomirror and zeroaxis to create a pair of axes that intersect at the center.
---- nomirror means the ticks on axis not mirror with axis.
---- unset border to remove the frame which has served up to as axes.

Present all the pointtypes available for the currently selected terminal.
>test
