#include <stdio.h>
void main()
{ 
  FILE* gnuplot;
  gnuplot = popen("gnuplot -persist", "w");
  if (gnuplot != NULL)
    fprintf(gnuplot, "set sample 1000\nplot sin(1/x) notitle lw 4, cos(x) notitle lw 4\n");
}
