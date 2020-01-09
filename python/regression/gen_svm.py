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
for timeline in time_data_obj:
    ret = re.findall('(\d+(\.\d+)?)', timeline)
    if len(ret) == 0:
	continue
    ret_time = ret[2:]
    lst_time = list()
    for time_item in ret_time:
	lst_time.append(time_item[0])
    lst_data.append((ret[0][0], ret[1][0], lst_time))

lst_features = list()
for featuresline in features_data_obj:
    ret_features = re.findall('(\d+(\.\d+)?)', featuresline)
    features = list()
    for feature_item in ret_features:
	features.append(feature_item[0])
    lst_features.append(features)

lst_streams_num = ['1', '2', '4', '8', '16', '32', '64', '128', '256', '512', '1024']
#Generate data.group file
group_file_object = open("data.group", 'w')
for eachData in lst_data:
    for i, eachTime in enumerate(eachData[2]):
	group_file_object.write(eachData[0]+'\t'+eachData[1]+'\t'+lst_streams_num[i]+'\n')

#Generate data.svm file
index_features = 0
svm_file_object = open("data.svm", 'w')
for eachData in lst_data:
    tt = list()
    for eachTime in eachData[2]:
	tt.append(float(eachTime))
    min_time = min(tt)
    for eachTime in tt:
	speed_ratio = min_time / eachTime
	svm_file_object.write(str(speed_ratio))
	for i, eachFeature in enumerate(lst_features[index_features]):
	    svm_file_object.write(' '+str(i)+':'+eachFeature)
	svm_file_object.write('\n')
	index_features += 1

time_data_obj.close()
features_data_obj.close()
group_file_object.close()
svm_file_object.close()

quit()
