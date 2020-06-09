import numpy as np 

def read_binary(f):
    # box_size = np.fromfile(f, np.float64, count=3, offset=0)
    time_ = np.fromfile(f, np.float64, count=1, offset=0)[0]
    _n = np.fromfile(f, np.int32, count=1, offset=8)[0]

    rs = np.fromfile(f, np.float64, count=_n*3, offset=8+4)
    rs = rs.reshape(_n,3)
    vs = np.fromfile(f, np.float64, count=_n*3, offset=8+4+8*_n*3)
    vs = vs.reshape(_n,3)
    return time_, rs, vs


def read_binary_inits(f):
    box_size = np.fromfile(f, np.float64, count=3, offset=0)
    N = np.fromfile(f, np.int32, count=1, offset=3*8)[0]

    radii = np.fromfile(f, np.float64, count=N, offset=3*8+4)
    masses = np.fromfile(f, np.float64, count=N, offset=3*8+4+8*N)
    return box_size, N, radii, masses
