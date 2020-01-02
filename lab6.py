import numpy as np
import matplotlib.pyplot as plt
import math


epsilon = 0.07

#print('Write borders for the center of the circle ')
#centre_coordinates = int(input())
centre_coordinates = 2

#print('Write max value of radius ')
#radius_h = int(input())
radius_h = 2

#print('Write amount of points')
#amount = int(input())
amount = 200

#Generate centre and radius
def circle(c, r_h):
    o = (2 * c * np.random.random_sample() - c, 2 * c * np.random.random_sample() - c)
    r = (r_h - 0.7) * np.random.random_sample() + 0.7
    return o, r

print('Centere and radius')
centre, radius = circle(centre_coordinates, radius_h)
print(centre, radius)

#Generate points
def points(m, c = centre_coordinates + radius_h + 1):
    p = np.zeros((m, 2))
    for i in range(m):  
        p[i] = (2 * c * np.random.random_sample() - c, 2 * c * np.random.random_sample() - c)
    return p

print('Points coordinates')
points_coordinates = np.asarray(points(amount))
#print(points_coordinates)

#Division of points into internal and external
def division(c, r, p):
    w_out = np.zeros((p.shape[0], 2))
    w_in = np.zeros((p.shape[0], 2))

    for i in range(p.shape[0]):
        dist = math.sqrt((p[i,0] - c[0])**2 + (p[i,1] - c[1])**2)
        if (dist > r + epsilon):
            #print('OUT')
            plt.scatter(points_coordinates[i,0], points_coordinates[i,1], color = "violet")
            w_out[i] = p[i]
        elif (dist < r - epsilon):
            #print('IN')
            plt.scatter(points_coordinates[i,0], points_coordinates[i,1], color = "blue")
            w_in[i] = p[i]
        else:
            #print('Points on the border')
            plt.scatter(points_coordinates[i,0], points_coordinates[i,1], color = "r")
            
    return w_out, w_in


print('Division of points ')
params_out = division(centre, radius, points_coordinates)
w_out = np.asarray(params_out[0])
w_in =  np.asarray(params_out[1])

w1 = np.all(np.equal(w_out, 0), axis = 1)
w2 = np.all(np.equal(w_in, 0), axis = 1)


#print('Class w1')
w1 = np.delete(w_out, np.where(w1 == True), axis = 0)
#print(w1)
#print('Class w2')
w2 = np.delete(w_in, np.where(w2 == True), axis = 0)
#print(w2)

k_out = w1.shape[0]
k_in = w2.shape[0]

#Transformation w1 and w2
e1 = np.ones((k_out, 1))
n1 = np.ones((1, k_out))
n1[0] = np.transpose(w1)[0] ** 2 + np.transpose(w1)[1] ** 2
e2 = np.ones((k_in, 1))
n2 = np.ones((1, k_in))
n2[0] = np.transpose(w2)[0] ** 2 + np.transpose(w2)[1] ** 2
e1 = np.append(e1, np.transpose(n1), axis = 1)
e2 = np.append(e2, np.transpose(n2), axis = 1)
w1 = np.append(e1, w1, axis = 1)
w1 = np.append(w1, [[0, 10, 0, 0]], axis = 0)
w2 = np.append(e2, w2, axis = 1)

e1 = np.ones((k_out + 1, 1))
w1 = np.append(w1, e1, axis = 1)
e2 = np.ones((k_in, 1))
w2 = (np.append(w2, e2, axis = 1)) * (-1)

#print(w1)
#print(w2)

#Concatenate w1 and w2
x = np.concatenate((w1, w2), axis=0)
#print(x.shape)
print('---------------------------------------------------')


def p(alpha, max_steps = 1000 * amount):
    
    steps = 0
    while (steps < max_steps):
        for i in range(x.shape[0]):   
            steps = steps + 1
            if (np.dot(alpha, x[i]) <= 0):
                alpha = alpha + x[i]
            
    return alpha

alpha = p([0, 0, 0, 0, 0])
print(alpha)
alpha /= alpha[1]
a = alpha[2] / (-2)
b = alpha[3] / (-2)
r = (a ** 2 + b ** 2 - alpha[0]) ** 0.5
print((a, b), r)
#print('trans ---------------------')            
#print(w1, w2)

#Circle plotting
circle_draw = plt.Circle(centre, radius, fill = False)
plt.gcf().gca().add_artist(circle_draw)
plt.show()

print('Tests')
test_amount = 1000
points_coordinates = np.asarray(points(test_amount))

params_out = division(centre, radius, points_coordinates)
w_out = np.asarray(params_out[0])
w_in =  np.asarray(params_out[1])
w1 = np.all(np.equal(w_out, 0), axis = 1)
w2 = np.all(np.equal(w_in, 0), axis = 1)


#print('Class w1')
w1 = np.delete(w_out, np.where(w1 == True), axis = 0)
#print(w1)
#print('Class w2')
w2 = np.delete(w_in, np.where(w2 == True), axis = 0)

k_out = w1.shape[0]
k_in = w2.shape[0]

#Transformation w1 and w2
e1 = np.ones((k_out, 1))
n1 = np.ones((1, k_out))
n1[0] = np.transpose(w1)[0] ** 2 + np.transpose(w1)[1] ** 2
e2 = np.ones((k_in, 1))
n2 = np.ones((1, k_in))
n2[0] = np.transpose(w2)[0] ** 2 + np.transpose(w2)[1] ** 2
e1 = np.append(e1, np.transpose(n1), axis = 1)
e2 = np.append(e2, np.transpose(n2), axis = 1)
w1 = np.append(e1, w1, axis = 1)
w1 = np.append(w1, [[0, 10, 0, 0]], axis = 0)
w2 = np.append(e2, w2, axis = 1)

e1 = np.ones((k_out + 1, 1))
w1 = np.append(w1, e1, axis = 1)
counter = 0
for w in w1:
    if np.dot(w, alpha) >= 0:
        counter += 1
print('First class ', counter /  w1.shape[0] * 100.0)
counter = 0
e2 = np.ones((k_in, 1))
w2 = np.append(w2, e2, axis = 1)
for w in w2:
    if np.dot(w, alpha) <= 0:
        counter += 1
print('Second class ', counter /  w2.shape[0] * 100.0)

