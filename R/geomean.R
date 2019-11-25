#!/usr/bin/env Rscript
#The args is data input file name
printf <- function(...) cat(sprintf(...))
args <- commandArgs(TRUE)
x <- read.table(header = FALSE, file = args[1])
id <- unique(x[,1:2])
for (i in 1:nrow(id)) {
  y <- x[x[,1]==id[i,1]&x[,2]==id[i,2], 3:4]
  s_id <- unique(y[,1])
  printf("%d\t%d\t", id[i, 1], id[i, 2])
  for (j in 1:length(s_id)) {
    t <- y[y[,1]==s_id[j], 2]
    result <- summary(t)
    printf("%f\t", result[3])
    }
  printf("\n")
  }
q()
