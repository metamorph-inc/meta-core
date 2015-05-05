from numpy import *

def SRC_regress(X, Y, otpt, N):
    # Add a constant term.
    X = insert(X,0,1,1) #insert a column of 1's in the 0th column, axis 1
    # Find the least squares solution by the use of Matlab backslash operator.
    # b is the vector of regression coefficients.
    r2=[]
    b=zeros((X.shape[1],otpt))
    r=zeros((X.shape[1],otpt))
    for p in range(otpt):
        b[:,p], resid = linalg.lstsq(X, Y[:, p])[:2]
        r2.append((1 - resid / (Y[:, p].size * Y[:, p].var()))[0])
        r[:, p] = b[:, p] * asarray((std(X,0).T / std(Y[:, p]) ).T)

#    [b(:,p),~,~,~,stats] = regress(Y(:,p),X)
#    r(:,p) = b(:,p).*std(X)'/std(Y(:,p))
#    stat(p)=stats(1)

    # "Standardize" the regression coefficients.
    # Remove the constant term.
    
    return r[1:], r2
