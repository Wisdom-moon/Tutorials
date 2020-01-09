x <- c(1,2,3,4,5,5,"a",2.3) #create a vector
x <- read.table(header = FALSE, file = "data") #read data from file.
para_num <- strtoi(args[2])

y <- array(as.numeric(unlist(x)), dim=c(nrow(x),ncol(x))
head(x): displat first 6 rows.
hist(x$V1,breaks=30,xlim=c(0,5000),col="blue",border="black",ylim=c(0,40),xlab="Weight",ylab="Counts",main="Histogram of Car Weights"): Draw histogram picture.
par(): set graphic parameters.
plot(x,y): 
save(vars,file='path/name',ascii=TRUE): save data as .Rdata format.
write(x,file="path/name",ncolumns = 1, sep=" "): write data into file.
mode(x): display its mode.
attributes(x):display its attributes.
length(x):
d<-as.character(x): Transform x into character.
X<-cbind(a1, a2): combine vector a1 and a2 by columns into a matrix, a1 is one columns in x.
X<-[,-4]: Deselect 4th column.
X<-rbind(a1,a2): make vector a1 as a row in matrix x.

 z_score <-function(weight) {
   m<-mean(weight); s<-sd(weight)
   z.weight<-(weight-m)/s
   z.weight
 }

 skew_score <- function(weight) {
   (3*(mean(weight)-median(weight)))/sd(weight)
 }


t(x): transpose matrix.
v_z <- array(0,c(231,19)): create mult-dim array.array
ls(): display all defined variables.
rm(var): delete defined variable.


library(psych)
pcal<-principal(f_z,nfactors=13,rotate="none",scores=TRUE)
pcal$loadings
f_pca_13<-f_z
f_pca_13[,3:15]<-pcal$scores
f_pca_13<-f_pca_13[,1:15]
f_pca_13_t<-t(f_pca_13)
write(f_pca_13_t,file="features_z_pca_13",ncolumns=15)

pcal<-principal(f_z,nfactors=9,rotate="none",scores=TRUE)
f_pca_9<-f_z
f_pca_9[,3:11]<-pcal$scores
f_pca_9<-f_pca_9[,1:11]
f_pca_9_t<-t(f_pca_9)
write(f_pca_9_t,file="features_z_pca_9",ncolumns=11)


pcal<-principal(f_z_1[,-(1:2)],nfactors=9,rotate="none",scores=TRUE)
f_tz1_pca_9<-cbind(f_z_1[,1:2], pcal$scores)
write(t(f_tz1_pca_9),file="features_tz1_pca_9",ncolumns=11)

f_z_n5 <- f_z[,-c(11,12)]
pcal<-principal(f_z_1[,-(1:2)],nfactors=9,rotate="none",scores=TRUE)
f_tz1_pca_9<-cbind(f_z_1[,1:2], pcal$scores)
write(t(f_z_7),file="features_z_7",ncolumns=length(f_z_7[1,]))
