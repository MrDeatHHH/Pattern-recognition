from keras.datasets import mnist
import numpy as np
import matplotlib.pyplot as plt

(x_train, y_train), (x_test, y_test) = mnist.load_data()

amount0 = np.count_nonzero(y_train == 0)
amount1 = np.count_nonzero(y_train == 1)

x01_train = x_train[np.logical_or((y_train == 0), (y_train == 1))]
y01_train = y_train[np.logical_or((y_train == 0), (y_train == 1))]

x01_test = x_test[np.logical_or((y_test == 0), (y_test == 1))]
y01_test = y_test[np.logical_or((y_test == 0), (y_test == 1))]

x01_train = (x01_train > 128) * 1
x01_test = (x01_test > 128) * 1

K = 2
X = x01_train.shape[0]

p_k_x = np.random.sample((X, K))

for i in range(X):
    
    norm = np.sum(p_k_x[i])       
    p_k_x[i] /= norm

p_k_x = np.transpose(p_k_x)

p_k = np.zeros(K, float)
p_x_k = np.zeros((X, K), float)
p_k_i_j = np.zeros((K, x01_train.shape[1], x01_train.shape[2]), float)

swap = 0

max_iter = 5
L = 0
# EM algorithm
print('Starting algorithm')
for it in range(max_iter):

    # 1
    for m in range(K):    
        p_k[m] = np.sum(p_k_x[m]) / float(X)

    # 2
    for n in range(K):
        for i in range(x01_train.shape[1]):
            for j in range(x01_train.shape[2]):
                p_k_i_j[n][i][j] = np.sum(np.moveaxis(x01_train, 0, -1)[i][j] * p_k_x[n]) / np.sum(p_k_x[n])

    for n in range(X):
        for m in range(K):
            p_x_k[n][m] = np.prod((p_k_i_j[m] ** x01_train[n]) *
                                  ((1 - p_k_i_j[m]) ** (1 - x01_train[n])))

    # 3
    for n in range(X):
        for m in range(K):
            p_k_x[m][n] = p_x_k[n][m] * p_k[m] / np.sum(p_x_k[n] * p_k)

    if it == 0:
        L = np.prod(np.sum(p_x_k, axis = 1))
    else:
        Ln = np.prod(np.sum(p_x_k, axis = 1))
        assert (L <= Ln), 'Not passed'
        L = Ln
print("Test passed")
# Results
print('Classes')
for i in range(2):
    plt.imshow(p_k_i_j[i])
    plt.show()

for i in range(K):
    
    norm = np.sum(p_k_i_j[i])       
    p_k_i_j[i] /= norm
# Determining which class is which number
if (np.prod((p_k_i_j[0] ** x01_train[0]) *
            ((1 - p_k_i_j[0]) ** (1 - x01_train[0]))) >
    np.prod((p_k_i_j[1] ** x01_train[0]) *
            ((1 - p_k_i_j[1]) ** (1 - x01_train[0])))) and y01_train[0] == 1:
    swap = 1

if (np.prod((p_k_i_j[0] ** x01_train[0]) *
            ((1 - p_k_i_j[0]) ** (1 - x01_train[0]))) <
    np.prod((p_k_i_j[1] ** x01_train[0]) *
            ((1 - p_k_i_j[1]) ** (1 - x01_train[0])))) and y01_train[0] == 0:
    swap = 1

print('Swap = ', swap)
# Tests
print('Tests')
answers = np.zeros(x01_test.shape[0], y01_test.dtype)
for i in range(x01_test.shape[0]):
    a1 = np.logical_and((p_k_i_j[0] == 0), (x01_test[i] == 1))
    a2 = np.sum(a1)
    b1 = np.logical_and((p_k_i_j[1] == 0), (x01_test[i] == 1))
    b2 = np.sum(b1)
    if (a2 > 0):
        if (a2 > b2):
            answers[i] = swap
        else:
            answers[i] = abs(swap - 1)
    else:
        if (b2 > a2):
            answers[i] = abs(swap - 1)
        else:
            a = np.sum((p_k_i_j[0] ** x01_test[i]) * ((1 - p_k_i_j[0]) ** (1 - x01_test[i])))
            b = np.sum((p_k_i_j[1] ** x01_test[i]) * ((1 - p_k_i_j[1]) ** (1 - x01_test[i])))
            if (a > b):
                answers[i] = swap                
            else:
                answers[i] = abs(swap - 1)

mask = (answers == y01_test)
        
unique, counts = np.unique(mask, return_counts = True)

perc = (counts[0] if unique[0] else counts[1]) / np.sum(counts)
print('Accuracy = ', perc)
