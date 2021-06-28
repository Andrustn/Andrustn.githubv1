#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Jul 27 17:53:37 2020

@author: trevorandrus
Test of Logistic Regression on Titanic Data
"""

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import pprint
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import classification_report
from sklearn.metrics import roc_curve
from sklearn.metrics import auc as calc_auc
import sklearn

# Read in cleaned dataset / remove "Nan" age rows
Data = pd.read_csv('../data/Titanic_Cleaned_Data.csv')
Data = Data[(Data['age'] > 0)]
            
            



# ---Input Predictor Variables----
VarOfInterest = ['fare', 'age', 'sex_i']





############## Setting response and explanatory variables ###################
Explanatories = np.array(Data[VarOfInterest])
Survival = np.array(Data['survived'])





# re-shape x variable to conform to the scikitlearn expectations
# requires an explicit value (1) for the number of columns
if len(Explanatories.shape) < 2:
    Explanatories = Explanatories.reshape(-1,1)



# create logistic regression object
logmodel = LogisticRegression()

# run model; results stored in object - using sex as predictor and survival as outcome
logmodel.fit(Explanatories, Survival)
#pprint.pprint(logmodel.get_params())
#print('\ncoefficient =', logmodel.coef_) ---- optional print statements ----
#print('intercept = ', logmodel.intercept_)

# calculate predicted values (probabilities)
predval = logmodel.predict_proba(Explanatories)
predval = predval[:, 1]


#plt.plot(predval, Data['sex_i'], 'o')
#plt.xlabel('Predicted probability of survival')
#plt.ylabel('Sex (0 male, 1 female)')



# set up confusion matrix - Not useful until using a fixed threshold (spits out
# too many numbers)

# label predicted values and actual values
tempdict = {'Actual':Survival, 'Predicted':predval}
# combine them into a confusion matrix
ActualVPredicted = pd.DataFrame(tempdict)

ConfusionMatrix = pd.crosstab(ActualVPredicted['Actual'], ActualVPredicted['Predicted'],
                     rownames=['Actual'], colnames=['Predicted'],
                     margins=True)



# plot
#sns.heatmap(ConfusionMatrix, annot=True, fmt='g', cmap='YlGn')
#plt.title('\nTitanic: Confusion Matrix: Sex --> Survivor\n')
#plt.show()

# confusion matrix statistics
#print('\n', classification_report(Survival, predval))

# calculate ROC and AUC

# re-shape variables to conform to the scikitlearn expectations
# requires an explicit value (1) for the number of columns
Survival = Survival.reshape(-1, 1)
predval = predval.reshape(-1,1)


# Compute FalsePositive, TruePositive, thresholds
FalsePositive, TruePositive, thresholds = roc_curve(Survival, predval)

#print('ROC Curve Data')
#print('\nThresholds = %s' % (thresholds))


# The call to calc_auc fails because the predval variable is not continuous
roc_auc = sklearn.metrics.roc_auc_score(Survival, predval)
# so I will give the roc_auc variable a fake value to test the plot


# Plot ROC curve
plt.plot(FalsePositive, TruePositive, label='ROC curve area = %0.3f' % roc_auc)
plt.plot([0, 1], [0, 1], 'k--')  # random predictions curve
plt.xlim([0.0, 1.0])
plt.ylim([0.0, 1.0])
plt.xlabel('False Positive Rate or (1 - Specificity)')
plt.ylabel('True Positive Rate or (Sensitivity/Recall)')
plt.title('Titanic: ' + ', '.join(VarOfInterest) + ' --> Surviver: ROC')
#plt.annotate('FalsePositive = 0.146\nTruePositive = 0.681', (0.08,0.85))
#plt.arrow(0.12, 0.81, 0.02, -0.09, length_includes_head=True,
          #head_width=0.02, head_length=0.02)
plt.legend(loc="lower right")
plt.show()

print("Area under the curve:", roc_auc)

# end of code
# exit() --  careful, this restarts the kernal

