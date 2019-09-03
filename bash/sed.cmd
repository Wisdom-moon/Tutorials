
#Translate the newline char from Windows to Linux.
sed -i 's/\r//' main.cu
#delete the matched line
sed -i '/zp/d' 2mm.unopt_log

