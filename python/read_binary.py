import numpy as np 

def read_binary(f):
    box_size = np.fromfile(f, np.float64, count=3, offset=0)
    time_ = np.fromfile(f, np.float64, count=1, offset=3*8)[0]
    N = np.fromfile(f, np.int32, count=1, offset=3*8+8)[0]

    rs = np.fromfile(f, np.float64, count=N*3, offset=3*8+8+4)
    rs = rs.reshape(N,3)
    vs = np.fromfile(f, np.float64, count=N*3, offset=3*8+8+4+8*N*3)
    vs = vs.reshape(N,3)
    return box_size, time_, N, rs, vs
