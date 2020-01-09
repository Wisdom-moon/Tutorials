#Generate train dataset

import os
import sys
import numpy as np
import re

if len(sys.argv) < 3:
    print 'Usage: ~ time_file features_file'
    quit()

time_file_name = sys.argv[1]
features_file_name = sys.argv[2]

if not os.path.isfile(time_file_name):
    print time_file_name + " not exist"
    quit()
if not os.path.isfile(features_file_name):
    print features_file_name + " not exist"
    quit()

time_data_obj = open(time_file_name)
features_data_obj = open(features_file_name)

lst_data = list()
for timeline,featuresline in zip(time_data_obj, features_data_obj):
    ret = re.findall('(\d+(\.\d+)?)', timeline)
    if len(ret) == 0:
	continue
    ret_time = ret[2:]
    lst_time = list()
    for time_item in ret_time:
	lst_time.append(time_item[0])
    ret_features = re.findall('(\d+(\.\d+)?)', featuresline)
    lst_features = list()
    for features_item in ret_features:
	lst_features.append(features_item[0])
    lst_data.append((ret[0][0], ret[1][0], lst_time, lst_features))

lst_streams_num = ['1', '2', '4', '8', '16', '32', '64', '128', '256', '512', '1024']

#Generate features.raw file which will be processed by R
raw_file_object = open("features.raw", 'w')
for eachData in lst_data:
    for i, eachTime in enumerate(eachData[2]):
	raw_file_object.write(lst_streams_num[i])
	for eachFeature in eachData[3]:
	    raw_file_object.write(' '+eachFeature)
	raw_file_object.write('\n')

time_data_obj.close()
features_data_obj.close()
raw_file_object.close()

quit()
