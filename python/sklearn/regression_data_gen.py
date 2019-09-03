import os
import sys
import sqlite3

if len(sys.argv) < 4:
    print 'Usage: ~ database_name program_id input_id'
    quit()

try:
    program_id = int(sys.argv[2])
    input_id = int(sys.argv[3])
except (ValueError, TypeError):
    print 'program/input id must be a number'
    quit()

db_file_name = sys.argv[1]
if not os.path.isfile(db_file_name):
    print db_file_name + " not exist"
    quit()

conn = sqlite3.connect(db_file_name)
c = conn.cursor()
c.execute("select * from perf where program=? AND input=?", 
					(program_id, input_id))
ret_dat = c.fetchall()
if len(ret_dat) == 0:
    print 'The data queried is not presented in database'
    c.execute("select DISTINCT input from perf where program=?", 
					(program_id,))
    ret_dat = c.fetchall()
    if len(ret_dat) == 0:
        print 'The program_id queried is not presented in database'
        c.execute("select DISTINCT program from perf")
        ret_dat = c.fetchall()
	print 'Legal program id is:'
    else:
	print 'Legal input id is:'
    print ret_dat
    quit()
    
#generate libsvm format file for regression analysis
out_name = str(program_id) + '_' + str(input_id) +'.svm'
out_obj = open(out_name, 'w')

for eachData in ret_dat:
    instance_str = str(eachData[4]) + ' 1:' + str(eachData[2]) +\
		   ' 2:' + str(eachData[3]) +'\n'
    out_obj.write(instance_str)

#Do clean works
out_obj.close()
quit()
