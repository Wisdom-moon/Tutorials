for i in {1..224}; do str=$str" "tmp_$i; echo $str;done
for i in {1..224}; do cat log_1 | sort -k 12 -n | grep "core $i " | awk '{print $14}' > tmp_$i;done
