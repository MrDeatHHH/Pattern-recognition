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
