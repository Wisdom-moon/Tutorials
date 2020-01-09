#!/usr/bin/env Rscript
#The args is data input file name
printf <- function(...) cat(sprintf(...))
args <- commandArgs(TRUE)
tmp <- read.table(header = FALSE, file = args[1])
y <- tmp[, 1]
x <- tmp[, 2]
lml <- lm(y ~ x)
summary(lml)
anova(lml)
print(lml)
q()
