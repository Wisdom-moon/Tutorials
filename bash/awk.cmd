awk '{if ($1== 2 && $2==12) print $3; if ($1== 14 && $2==42) print"\t"$3; if ($1== 55 && $2==220) print "\t\t"$3}' log-n4096-e4096

for i in {1..224}; do cat log_112 | sort -k 12 -n | grep "core $i " | awk 'BEGIN{sum=0}{sum+=$14}END{print sum}';done | awk 'BEGIN{ret=0} {if ($1 > ret) ret=$1} END{print ret}'

cat log_112 | sort -k 9 -n | awk 'BEGIN{sum=0;fir=0;pre=0} {if ($9 != pre) {pre = $9; print sum,fir; sum=0;fir=0;sum+=$14;fir=$14} else sum+=$14}' | awk 'BEGIN{ret1=0;ret2=0} {if ($1 > ret1) ret1=$1; if ($2 > ret2) ret2 = $2} END{print ret1,ret2}'

cat log-n8192-e4096 | awk 'BEGIN{p=0} {if ($1 > p) {p = $1; printf "\n"$3"\t"} else printf $3"\t"} END{print "\n"}'

echo '[INFO] Kernel: 0.043 s' | awk '/INFO/ {print $3}'
echo '[INFO] Kernel: 0.043 s' | awk 'BEGIN {FS=" "; OFS="\t"} /INFO/ {print $3,$2}'
echo '[INFO] Kernel: 0.043 s' | awk '$1 == "[INFO]" {print $3}'
echo '[INFO] Kernel: 0.043 s' | awk '$1 == "[INFO]" {print $3}; /Kernel/ {print $2}'
echo '[INFO] Kernel: 0.043 s' | awk '$1 == "[INFO]",/Kernel/ {print $3}'
echo '[INFO] Kernel: 0.043 s' | awk '$1 == "[INFO]" && /Kernel/ {print $3}'
echo '[INFO] Kernel: 0.043 s' | awk '$1 == "[INFO]" || /Kernel/ {print $3}'

awk 'BEGIN{srand(); print rand()}'
awk 'BEGIN{srand()} / i/{print $1, $2, rand()}'  parabola.text > circles.text

//getline: get the next line
for i in 1 2;do echo $i; awk '$1 == "2mm" {getline; print $0}' ./raw | awk -v threads=$i '$3 == threads {print $4}';done
for i in 1 2 3 4 5 6 7 8 12 16 32 64 96 128 256;do awk '$1 == "2mm" {getline; print $0}' ./raw | awk -v threads=$i '$2 == threads {printf $4"\t"} END{printf "\n"}';done | xsel -b -i
