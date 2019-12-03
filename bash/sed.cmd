
#Translate the newline char from Windows to Linux.
sed -i 's/\r//' main.cu
#delete the matched line
sed -i '/zp/d' 2mm.unopt_log

sed  's;../../bin;nvprof --kernels "BFS_kernel" --events divergent_branch,l2_subp0_read_sector_misses,l2_subp1_read_sector_misses,l2_subp0_total_read_sector_queries,l2_subp1_total_read_sector_queries ../../bin;g' ./cmd.sh

sed -i "s/\-m64/\ /g" `grep "\-m64" -rl ./`
sed -i "s/fPIC/fPIC\ \-O0\ \-ggdb\ \-g3\ /g" `find . -name flags.make`
