import os
import sys
import numpy as np
from sklearn.datasets import load_svmlight_file
from sklearn import linear_model
from sklearn import preprocessing
from sklearn.tree import DecisionTreeRegressor
from sklearn.ensemble import AdaBoostRegressor
from sklearn.model_selection import cross_val_score
from sklearn.ensemble import RandomForestRegressor

if len(sys.argv) < 2:
    print 'Usage: ~ libsvm_file'
    quit()

dat_file_name = sys.argv[1]

if not os.path.isfile(dat_file_name):
    print dat_file_name + " not exist"
    quit()

x,y = load_svmlight_file (dat_file_name)
np.random.seed(0)
indices = np.random.permutation(len(y))
x_train = x[indices[:-10]]
x_test = x[indices[-10:]]
y_train = y[indices[:-10]]
y_test = y[indices[-10:]]


regr = linear_model.LinearRegression()
regr.fit(x_train,y_train)
regr.predict(x_test)
print 'score for LinearRegression'
print regr.score(x_test, y_test) # 1 is perfect prediction and 0 means that there is no linear relationship between x and y
scores = cross_val_score(regr, x, y)# cross_val_score is not suitable for regression
print scores.mean()

#Ridge regression.
rege = linear_model.Ridge(alpha=.1)
regr.fit(x_train,y_train)
print 'Score for Ridge'
print regr.score(x_test, y_test)

#Lasso regression
regr = linear_model.Lasso()
regr.fit(x_train,y_train)
print 'Score for Lasso'
print regr.score(x_test, y_test)

#Decision Tree Regression
regr = DecisionTreeRegressor(max_depth=4)
regr.fit(x_train,y_train)
print 'Score for Decision Tree'
print regr.score(x_test, y_test)

regr = AdaBoostRegressor(DecisionTreeRegressor(max_depth=4),n_estimators=300)
regr.fit(x_train,y_train)
print 'Score for AdaBoostRegressor'
print regr.score(x_test, y_test)

regr = RandomForestRegressor()
regr.fit(x_train,y_train)
print 'Score for RandomForestRegressor'
print regr.score(x_test, y_test)

quit()
