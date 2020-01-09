import os
import sys
import numpy as np
import re
from sklearn.datasets import load_svmlight_file
from sklearn import linear_model
from sklearn import preprocessing
from sklearn.model_selection import LeaveOneGroupOut
from sklearn.neural_network import MLPRegressor
from sklearn import tree
from sklearn import ensemble
from sklearn import gaussian_process
from sklearn import neighbors
from sklearn import svm

if len(sys.argv) < 3:
    print 'Usage: ~ libsvm_file groups_file'
    quit()

dat_file_name = sys.argv[1]
dat_groups_name = sys.argv[2]

if not os.path.isfile(dat_file_name):
    print dat_file_name + " not exist"
    quit()
if not os.path.isfile(dat_groups_name):
    print dat_groups_name + " not exist"
    quit()

dat_groups_obj = open(dat_groups_name)
P_I_pairs = list()
for eachLine in dat_groups_obj:
    ret = re.findall('\d+', eachLine)
    if ret is not None:
        P_I_pairs.append((int(ret[0]), int(ret[1])))
P_I_pairs = np.asarray(P_I_pairs)
groups = [a[0] for a in P_I_pairs]

X_sparse,y = load_svmlight_file (dat_file_name)
X_dense = X_sparse.todense()

#remove features in row 6
#x1,x2,x3 = np.hsplit(X_dense, [5,6])
#X = np.hstack((x1,x3))
X = X_dense


logo = LeaveOneGroupOut()

#regr_dct = MLPRegressor(hidden_layer_sizes=(80,60, 30, 30, 20, 20, 20), activation='logistic', solver='adam', learning_rate_init=0.0001, max_iter=500,random_state=1)
regr_dct = MLPRegressor(hidden_layer_sizes=(4), activation='tanh', solver='adam')
#regr_dct = MLPRegressor(hidden_layer_sizes=(100, 80,60), activation='logistic', solver='adam')
#regr_dct = tree.DecisionTreeRegressor()
#regr_dct = ensemble.RandomForestRegressor(n_estimators=20)
#regr_dct = gaussian_process.GaussianProcessRegressor(kernel=None)
#regr_dct = neighbors.RadiusNeighborsRegressor(radius=1.0)
#regr_dct = svm.SVR(kernel='rbf')

scores_dct = list()

P_scores = list()
speedup_arry = np.zeros(100, dtype=[('x', float),('y', float)])
for train, test in logo.split(X, y, groups=groups):
    speedup_arry.fill(-np.inf)
    I_scores = list()
    #scaler = preprocessing.StandardScaler(with_mean=False).fit(X[train])
    #X_train = scaler.transform(X[train])
    #X_test = scaler.transform(X[test])
    X_train = X[train]
    X_test = X[test]

    regr_dct.fit(X_train, y[train])
    y_predict = regr_dct.predict(X_test)
    for p_i, y_p, y_t in zip(P_I_pairs[test], y_predict, y[test]):
	input_id = p_i[1]
	program_id = p_i[0]
	if y_p > speedup_arry['y'][input_id]:
	    speedup_arry['y'][input_id] = y_p
	    speedup_arry['x'][input_id] = y_t
    #print 'Program ' + str(program_id) + ' :'
    for speedup in speedup_arry:
	if speedup['x'] > 0.0:
	    score = speedup['x']
	    I_scores.append(score)
	    #print score
    P_scores.append(np.mean(I_scores))


#Decision Tree Regression
print 'Score for Regression'
print P_scores
print 'mean: ' + str(np.mean(P_scores)) + ' std: '  + str(np.std(P_scores))

quit()
