---
layout: post
title: Interpreting linear regression summary from statsmodels
date: Fri, 16 Jul 2021 11:54:36 -0400
tags: math code
---

The python package statsmodels has OLS functions to fit a linear regression
problem. How well the linear regression is fitted, or whether the data fits a
linear model, is often a question to be asked. The way to tell is to use some
statistics, which by default the OLS module produces a few in its summary.

This is an example of using statsmodels to fit a linear regression:

```python
import statsmodels.api as sm
import numpy as np
import pandas as pd

X1 = np.random.rand(200)*3.1
X2 = np.random.rand(200)*4.1
X3 = np.random.rand(200)*5.9
X4 = np.random.rand(200)*2.6
X5 = np.random.rand(200)*5.3
Y0 = 0.58*X1 - 0.97*X2 + 0.93*X3 - 2.3
err = np.random.randn(200)

df = pd.DataFrame(dict(X1=X1, X2=X2, X3=X3, X4=X4, X5=X5, Y=Y0+err))
model = sm.OLS(df["Y"], sm.add_constant(df[["X1","X2","X3","X4","X5"]]), missing="drop").fit()
print(model.summary2())
```

We print the summary using `summary2()` function instead of `summary()`
function because it looks more compact, but the result should be the same. This is how the above looks like:

```text
                 Results: Ordinary least squares
=================================================================
Model:              OLS              Adj. R-squared:     0.799   
Dependent Variable: Y                AIC:                572.1603
Date:               2021-07-16 11:49 BIC:                591.9502
No. Observations:   200              Log-Likelihood:     -280.08 
Df Model:           5                F-statistic:        159.0   
Df Residuals:       194              Prob (F-statistic): 1.27e-66
R-squared:          0.804            Scale:              0.99341 
-------------------------------------------------------------------
          Coef.    Std.Err.      t       P>|t|     [0.025    0.975]
-------------------------------------------------------------------
const    -2.2590     0.2889    -7.8187   0.0000   -2.8288   -1.6892
X1        0.6440     0.0848     7.5968   0.0000    0.4768    0.8112
X2       -0.9834     0.0595   -16.5186   0.0000   -1.1009   -0.8660
X3        0.8920     0.0445    20.0478   0.0000    0.8043    0.9798
X4       -0.0200     0.0921    -0.2167   0.8287   -0.2015    0.1616
X5       -0.0209     0.0465    -0.4486   0.6542   -0.1126    0.0709
-----------------------------------------------------------------
Omnibus:              0.319        Durbin-Watson:           1.825
Prob(Omnibus):        0.853        Jarque-Bera (JB):        0.471
Skew:                 0.030        Prob(JB):                0.790
Kurtosis:             2.770        Condition No.:           22   
=================================================================
```

Showing the names of the dependent and independent variables are supported if
the data are provided as pandas dataframe. We can see that the summary screen
above has three sections, and the elements in each are explained as follows:

First section: The statistics of the overall linear model. In a linear
regression of fitting $$y = \beta^T X + \epsilon$$ using $$N$$ data points with
$$p$$ regressor and one regressand, the value $$\hat{y}_i$$ as predicted by the
model, we have the RSS (residual sum of square) defined as
$$RSS=\sum_i (y_i-\hat{y}_i)^2$$ and the ESS (explained sum of square) defined
as $$ESS = \sum_i (\hat{y}_i - \bar{y})^2$$, and the total sum of square is
$$TSS=ESS+RSS=\sum_i(y_i-\bar{y})^2$$. The items on the first section of the
summary are:

- No. Observations: The number of data points $$N$$
- Df model: Number of parameters in the model $$p$$
  - statsmodels can take string-typed categorical variables in regression. In
    that case, one-hot encoding would be used and the number of parameters will
    be expanded by the number of categories in such variables
- Df residuals: Degree of freedom of the residuals, equals to $$N-p-1$$
- R-squared: $$R^2 = 1-\dfrac{RSS}{TSS} = 1-\dfrac{\sum_i (y_i-\hat{y}_i)^2}{\sum_i (y_i-\bar{y})^2}$$ as the coefficient of determination
- adjusted R-squared: $$\bar{R}^2 = 1-\dfrac{RSS/df_e}{TSS/df_t}=1-(1-R^2)\dfrac{n-1}{n-p-1}$$ where
  $$df_t=N-1$$ is the degrees of freedom of the estimate of the population
  variance of the dependent variable, and $$df_e = n-k-1$$ is the degrees of
  freedom of the estimate of the underlying population error variance
- Log-Likelihood: $$\log p(X|\mu,\Sigma)=\sum_{i=1}^N\log\mathcal{N}(e_i|\mu_i,\Sigma_i)$$. Assumed
  the model is correct, the log of the probability that the set of data is produced by the model
- AIC:  Akaike Information Criterion, $$-2\log L + kp$$ with $$k=2$$. It depends on
  the log-likelihood $$\log L$$ and estimates the relative distance between the
  unknown true likelihood and the fitted likelihood. The lower the AIC means
  the closer to the truth
- BIC: Bayesian Information Criterion, $$-2\log L + kp$$ with $$k=\log(N)$$. Based
  on a Bayesian set up and meansures the posterior probability of a model being
  true. The lower the BIC means the closer to the truth
  - BIC penalizes the model complexity more heavily (usually $$\log N>2$$) than
    AIC, hence AIC may prefer a bigger model compared to BIC
  - AIC is better in situations when false negatives are more misleading than a
    false positive; BIC is better in situations when false positive is more
    misleading than a false negative
- F-statistic and Prob (F-statsitic): The null hypothesis that all the
  coefficients of regressors are zero, hence a high p-value means the model is
  more significant
- Scale: The scale factor of the covariance matrix, $$\dfrac{RSS}{N-p}$$

The second section: Coefficients determined by the regression.

- Coef: Coefficient determined by OLS regression, it is solved analytically with $$\beta=(X^TX)^{-1}X^Ty$$
- Std Err: Estimate of the standard deviation of the coefficient,
  $$\hat\sigma^2_j = \hat\sigma^2[Q_{xx}^{-1}]_{jj}$$ with $$Q_{xx}=X^TX$$ and
  $$\hat\sigma^2=\dfrac{\epsilon^T\epsilon}{N}$$
- t: the t statistic, with the null hypothesis that this particular coefficient
  is zero. It is used as a measurement of whether theh coefficient is
  significant. A coefficient is significant if its magnitude is large with
  small standard error
- P>|t|: the p-value of the t test, i.e., the probability that the variable has
  no effect on the dependent variable as the null hypothesis is true
- 0.025 and 0.975: The two boundaries of the coefficient at 95% confidence
  interval, approximately mean value of the coefficient ±2 standard error

The third section: Normality of the residuals. Linear regression is built based
on the assumption that $\epsilon$ is normally distributed with zero mean.

- Omnibus: D'Agostino's $$K^2$$ test, based on skew and kurtosis. Perfect normality will produce 0
- Prob(Ominbus): Probability that the residuals are normally distributed according to omnibus statistic
- Skew: Skewness (symmetry) of the residual, 0 if perfect symmetry
- Kurtosis: Peakiness of the residual (concentration around 0), higher kurtosis means fewer outliers. Normal distribution will gives 3 here
- Durbin-Watson: Test for autocorrelation in the residuals or the homoscedasticity, i.e., whether the error are independent of each other and even throughout the data
  - if relative error is higher when the data points are higher, then the error is not even
  - ideal measure is between 1 and 2
- Jarque-Bera (JB) and Prob(JB): also a normality test using skewness and kurtosis, as an alternative way to omnibus statistic
  - we need JB and Omnibus mutually confirm with each other
- Condition no.: Measurement of sensitivity of the model compared to the size of changes in the data
  - multicollinearity (i.e., two independent variables are linearly related) has high condition number

Knowing what each of these elements measures, we can see how well the model
fits. Here we try to change the code to give a different summary:

If we use fewer regressor in the input, we should see a lowered AIC and BIC:

```python
model = sm.OLS(df["Y"], sm.add_constant(df[["X1","X2","X3"]]), missing="drop").fit()
print(model.summary2())
```

Result as follows, which the AIC and BIC are lowered a bit due to lowered df
model (simpler model), but the $$R^2$$ has not changed:

```text
                 Results: Ordinary least squares
=================================================================
Model:              OLS              Adj. R-squared:     0.801   
Dependent Variable: Y                AIC:                568.4052
Date:               2021-07-16 11:51 BIC:                581.5985
No. Observations:   200              Log-Likelihood:     -280.20 
Df Model:           3                F-statistic:        267.3   
Df Residuals:       196              Prob (F-statistic): 5.35e-69
R-squared:          0.804            Scale:              0.98447 
-------------------------------------------------------------------
          Coef.    Std.Err.      t       P>|t|     [0.025    0.975]
-------------------------------------------------------------------
const    -2.3391     0.2294   -10.1962   0.0000   -2.7915   -1.8867
X1        0.6385     0.0836     7.6355   0.0000    0.4735    0.8034
X2       -0.9812     0.0591   -16.6130   0.0000   -1.0977   -0.8647
X3        0.8921     0.0443    20.1416   0.0000    0.8048    0.9795
-----------------------------------------------------------------
Omnibus:              0.378        Durbin-Watson:           1.826
Prob(Omnibus):        0.828        Jarque-Bera (JB):        0.526
Skew:                 0.029        Prob(JB):                0.769
Kurtosis:             2.755        Condition No.:           14   
=================================================================
```

Indeed if we check the p-value of the t test in the previous output, we can see
that they are high and the null hypothesis is not rejected for X4 and X5,
hinting that these two regressors should not be included in the model.

If we skew the error by taking its absolute value, the error distribution is no
longer normal:

```python
df = pd.DataFrame(dict(X1=X1, X2=X2, X3=X3, X4=X4, X5=X5, Y=Y0+np.abs(err)))
model = sm.OLS(df["Y"], sm.add_constant(df[["X1","X2","X3","X4","X5"]]), missing="drop").fit()
print(model.summary2())
```

Result as follows. We see that the $$R^2$$ is higher (because the range of error is smaller now)
but the test of normality in the residual has low p-value in both the omnibus
test and the Jarque-Bera statistic. Hence we concluded that the residual is not
normal. This is why the coefficients found deviated from the truth.

```text
                 Results: Ordinary least squares
==================================================================
Model:              OLS              Adj. R-squared:     0.922    
Dependent Variable: Y                AIC:                359.9204 
Date:               2021-07-16 11:52 BIC:                379.7103 
No. Observations:   200              Log-Likelihood:     -173.96  
Df Model:           5                F-statistic:        474.7    
Df Residuals:       194              Prob (F-statistic): 1.02e-106
R-squared:          0.924            Scale:              0.34376  
--------------------------------------------------------------------
           Coef.    Std.Err.      t       P>|t|     [0.025    0.975]
--------------------------------------------------------------------
const     -1.2735     0.1700    -7.4931   0.0000   -1.6087   -0.9383
X1         0.4774     0.0499     9.5733   0.0000    0.3790    0.5757
X2        -1.0152     0.0350   -28.9883   0.0000   -1.0843   -0.9461
X3         0.9284     0.0262    35.4709   0.0000    0.8768    0.9801
X4        -0.0195     0.0542    -0.3606   0.7188   -0.1264    0.0873
X5         0.0183     0.0274     0.6691   0.5042   -0.0357    0.0723
------------------------------------------------------------------
Omnibus:               21.305       Durbin-Watson:          2.091 
Prob(Omnibus):         0.000        Jarque-Bera (JB):       24.991
Skew:                  0.854        Prob(JB):               0.000 
Kurtosis:              3.291        Condition No.:          22    
==================================================================
```

If we introduce multilinearity, statsmodels will produce a vastly large
conditon number and warn us about the result:

```python
df = pd.DataFrame(dict(X1=X1, X2=X2, X3=X3, X4=X2-2*X3, X5=X1+0.5*X2, Y=Y0+(Y0**2)*err))
model = sm.OLS(df["Y"], sm.add_constant(df[["X1","X2","X3","X4","X5"]]), missing="drop").fit()
print(model.summary2())
```

with the result as follows, we can see that all coefficients are significant
according to the p-value of t test but indeed only the first 3 are independent.
The condition number suggested that these set of coefficient is not stable.

```text
                 Results: Ordinary least squares
=================================================================
Model:              OLS              Adj. R-squared:     0.801   
Dependent Variable: Y                AIC:                568.4052
Date:               2021-07-16 13:07 BIC:                581.5985
No. Observations:   200              Log-Likelihood:     -280.20 
Df Model:           3                F-statistic:        267.3   
Df Residuals:       196              Prob (F-statistic): 5.35e-69
R-squared:          0.804            Scale:              0.98447 
-------------------------------------------------------------------
          Coef.    Std.Err.      t       P>|t|     [0.025    0.975]
-------------------------------------------------------------------
const    -2.3391     0.2294   -10.1962   0.0000   -2.7915   -1.8867
X1        0.4671     0.0473     9.8842   0.0000    0.3739    0.5603
X2       -0.5917     0.0498   -11.8909   0.0000   -0.6898   -0.4935
X3       -0.0582     0.0243    -2.3936   0.0176   -0.1062   -0.0103
X4       -0.4752     0.0172   -27.6363   0.0000   -0.5091   -0.4413
X5        0.1713     0.0396     4.3213   0.0000    0.0931    0.2495
-----------------------------------------------------------------
Omnibus:          0.378    Durbin-Watson:       1.826            
Prob(Omnibus):    0.828    Jarque-Bera (JB):    0.526            
Skew:             0.029    Prob(JB):            0.769            
Kurtosis:         2.755    Condition No.:       24475138936904036
=================================================================
* The condition number is large (2e+16). This might indicate
strong multicollinearity or other numerical problems.
```

We can also create heteroscedasticity by making residual larger when the regressand is small:

```python
df = pd.DataFrame(dict(X1=X1, X2=X2, X3=X3, X4=X4, X5=X5, Y=Y0+err/Y0))
model = sm.OLS(df["Y"], sm.add_constant(df[["X1","X2","X3","X4","X5"]]), missing="drop").fit()
print(model.summary2())
```

The result as follows, which we can see the Durbin-Watson statistic is larger
than 2, and as a result, the residual is not normally distributed as well:

```text
                 Results: Ordinary least squares
==================================================================
Model:              OLS              Adj. R-squared:     0.074    
Dependent Variable: Y                AIC:                1330.7666
Date:               2021-07-16 13:16 BIC:                1350.5565
No. Observations:   200              Log-Likelihood:     -659.38  
Df Model:           5                F-statistic:        4.177    
Df Residuals:       194              Prob (F-statistic): 0.00126  
R-squared:          0.097            Scale:              44.098   
--------------------------------------------------------------------
            Coef.    Std.Err.      t      P>|t|     [0.025    0.975]
--------------------------------------------------------------------
const      -1.5268     1.9250   -0.7932   0.4287   -5.3235    2.2698
X1          1.2981     0.5648    2.2983   0.0226    0.1841    2.4120
X2         -1.0072     0.3967   -2.5393   0.0119   -1.7896   -0.2249
X3          0.7941     0.2965    2.6786   0.0080    0.2094    1.3788
X4         -0.3668     0.6134   -0.5979   0.5506   -1.5766    0.8431
X5         -0.2874     0.3100   -0.9271   0.3550   -0.8987    0.3240
------------------------------------------------------------------
Omnibus:              147.586      Durbin-Watson:         2.232   
Prob(Omnibus):        0.000        Jarque-Bera (JB):      9060.224
Skew:                 2.033        Prob(JB):              0.000   
Kurtosis:             35.721       Condition No.:         22      
==================================================================
```

We can also do a nonlinear model:

```python
Y0 = 0.58*X1 - 0.97*X2 + 0.93*X3**2 - 2.3
df = pd.DataFrame(dict(X1=X1, X2=X2, X3=X3, X4=X4, X5=X5, Y=Y0+err))
model = sm.OLS(df["Y"], sm.add_constant(df[["X1","X2","X3","X4","X5"]]), missing="drop").fit()
print(model.summary2())
```

which we take the squared of X3 and the result is as follows. Because of the
nonlinear model, the residual is no longer normally distributed. The $$R^2$$
here is larger than before. Hence we should be cautious not to merely select
a model based on the coefficient of determination.

```text
                 Results: Ordinary least squares
==================================================================
Model:              OLS              Adj. R-squared:     0.930    
Dependent Variable: Y                AIC:                926.7164 
Date:               2021-07-16 13:31 BIC:                946.5063 
No. Observations:   200              Log-Likelihood:     -457.36  
Df Model:           5                F-statistic:        532.4    
Df Residuals:       194              Prob (F-statistic): 3.37e-111
R-squared:          0.932            Scale:              5.8484   
--------------------------------------------------------------------
           Coef.    Std.Err.      t       P>|t|     [0.025    0.975]
--------------------------------------------------------------------
const     -7.9247     0.7010   -11.3043   0.0000   -9.3074   -6.5421
X1         0.5560     0.2057     2.7031   0.0075    0.1503    0.9616
X2        -1.0398     0.1445    -7.1978   0.0000   -1.3247   -0.7549
X3         5.4317     0.1080    50.3107   0.0000    5.2187    5.6446
X4         0.2395     0.2234     1.0720   0.2850   -0.2011    0.6801
X5        -0.0700     0.1129    -0.6198   0.5361   -0.2926    0.1527
------------------------------------------------------------------
Omnibus:               12.714       Durbin-Watson:          1.895 
Prob(Omnibus):         0.002        Jarque-Bera (JB):       13.907
Skew:                  0.631        Prob(JB):               0.001 
Kurtosis:              2.727        Condition No.:          22    
==================================================================
```

Finally, we can try to use the error as the regressand and see the F statistic
became low (or its p-value became high):

```python
df = pd.DataFrame(dict(X1=X1, X2=X2, X3=X3, X4=X4, X5=X5, Y=err))
model = sm.OLS(df["Y"], sm.add_constant(df[["X1","X2","X3","X4","X5"]]), missing="drop").fit()
print(model.summary2())
```

result:

```text
                 Results: Ordinary least squares
=================================================================
Model:              OLS              Adj. R-squared:     -0.018  
Dependent Variable: Y                AIC:                572.1603
Date:               2021-07-16 13:36 BIC:                591.9502
No. Observations:   200              Log-Likelihood:     -280.08 
Df Model:           5                F-statistic:        0.2807  
Df Residuals:       194              Prob (F-statistic): 0.923   
R-squared:          0.007            Scale:              0.99341 
-------------------------------------------------------------------
            Coef.    Std.Err.      t      P>|t|     [0.025   0.975]
-------------------------------------------------------------------
const       0.0410     0.2889    0.1419   0.8873   -0.5288   0.6108
X1          0.0640     0.0848    0.7547   0.4513   -0.1032   0.2312
X2         -0.0134     0.0595   -0.2257   0.8217   -0.1309   0.1040
X3         -0.0380     0.0445   -0.8531   0.3947   -0.1257   0.0498
X4         -0.0200     0.0921   -0.2167   0.8287   -0.2015   0.1616
X5         -0.0209     0.0465   -0.4486   0.6542   -0.1126   0.0709
-----------------------------------------------------------------
Omnibus:              0.319        Durbin-Watson:           1.825
Prob(Omnibus):        0.853        Jarque-Bera (JB):        0.471
Skew:                 0.030        Prob(JB):                0.790
Kurtosis:             2.770        Condition No.:           22   
=================================================================
```
