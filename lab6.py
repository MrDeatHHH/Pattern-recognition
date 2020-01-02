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
