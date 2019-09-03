#!/usr/bin/python
import os
import sys
import re
import sqlite3

if len(sys.argv) < 2:
    print 'Usage: ~ dir_name'
    quit()

base_dir = sys.argv[1]
if not os.path.isdir(base_dir):
    print base_dir +' is not a legal dir name!'
    quit()

input_id_table=[
	[2048,2048],
	[4096,2048],
	[4096,4096],
	[6144,4096],
	[8192,4096],
	[8192,5120],
	[8192,6144],
	[8192,7168],
	[8192,8192],
	[9216,8192],
	[10240,8192],
	[11264,8192],
	[12288,8192],
	[13312,8192],
	[14336,8192],
	[15360,8192],
	[16384,8192],
	[4096,1],
	[6144,1],
	[8192,1],
	[9216,1],
	[10240,1],
	[11264,1],
	[12288,1],
	[13312,1],
	[14336,1],
	[15360,1],
	[16384,1],
	[8388608,1],
	[16777216,1],
	[33554432,1],
	[67108864,1],
	[134217728,1],
	[8192,1024],
	[8192,2048],
	[12288,2048],
	[16384,2048],
	[16384,2560],
	[16384,3072],
	[16384,3584],
	[16384,4096],
	[18432,4096],
	[20480,4096],
	[22528,4096],
	[24576,4096],
	[26624,4096],
	[28672,4096],
	[30720,4096],
	[32768,4096] 
		]

conn = sqlite3.connect('example.db')
c = conn.cursor()

c.execute("select count(*) from sqlite_master where type = 'table' and name = 'perf'")
if c.fetchone()[0] == 1:
    c.execute("DROP TABLE perf")

c.execute('''CREATE TABLE perf (program INT, input INT, 
	partition INT, block INT, time REAL DEFAULT 0.0, 
	PRIMARY KEY (program, input, partition, block))''')

for program_dir in os.listdir(base_dir):
    program_id = re.match('^\d+$',program_dir)
    if program_id is not None:
	program_id = int(program_dir)
	print "Handling program " + str(program_id)
	for log_file in os.listdir(base_dir+'/'+program_dir):
	    if re.match('^log', log_file) is not None:
		input_id = [1,]
		for id_var in re.findall('\d+',log_file):
		    input_id.append(int(id_var))
		input_id.sort(reverse=True)
		input_id = input_id[:2]
		for id_index in range(len(input_id_table)):
		    if cmp(input_id, input_id_table[id_index]) == 0:
			#print input_id_table[id_index]
			break
		if cmp(input_id, input_id_table[id_index]) != 0:
		    print str(input_id[0]) + " "+ str(input_id[1]) +" is not in table"
		    continue
		items = list()
		f_log = open(base_dir+'/'+program_dir+'/'+log_file)
		print "\tinput id " + str(id_index)
		#print "\tinput id " + str(id_index) + " " + str(input_id[0])+ " " + str(input_id[1])
		for eachLine in f_log:
		    l_line = re.findall('-?\d+\.\d+|\d+', eachLine)
		    item = (program_id, id_index, int(l_line[0]), 
			    int(l_line[1]), float(l_line[2]))
		    items.append(item)
		c.executemany('INSERT INTO perf VALUES(?,?,?,?,?)', items)
		conn.commit()
c.execute("select * from perf where program=2 and input=4 ORDER BY time")
print c.fetchall()
conn.close()
