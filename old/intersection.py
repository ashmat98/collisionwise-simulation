import numpy as np

a = np.array([1,2,3])
rho = 0.01

np.random.seed(42)

v = np.random.randn(3)
r0 = np.random.rand(3) * a

r0 = r0 * (v>0) + (a-r0)*(v<0)
v = v * (v>0) + (-v) * (v<0)

dt1 = min((a-r0)/v)
r0 = r0 + v * dt1

print(r0)
print(v)



