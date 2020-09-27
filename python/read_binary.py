import numpy as np 
import os


def recent_dump():
    return "../dumps/"+sorted(os.listdir("../dumps/"))[-1]

def read_binary(path=None, stamps=-1):
    if path is None:
        path = recent_dump()

    box_size = np.fromfile(path, np.float64, count=3, offset=0)
    N = np.fromfile(path, np.int32, count=1, offset=3*8)[0]

    radii = np.fromfile(path, np.float64, count=N, offset=3*8+4)
    masses = np.fromfile(path, np.float64, count=N, offset=3*8+4+8*N)
    # print(N, radii, masses)

    offset = 3*8+4+8*N + 8*N


    def read_stamp(offset):
        # box_size = np.fromfile(path, np.float64, count=3, offset=0)
        time_ = np.fromfile(path, np.float64, count=1, offset=offset+0)[0]
        _n = np.fromfile(path, np.int32, count=1, offset=offset+8)[0]
        # print(time_)
        rs = np.fromfile(path, np.float64, count=_n*3, offset=offset+8+4)
        rs = rs.reshape(_n,3)
        # print(rs)
        vs = np.fromfile(path, np.float64, count=_n*3, offset=offset+8+4+8*_n*3)
        vs = vs.reshape(_n,3)
        # print(vs)
        return offset+8+4+8*_n*3+8*_n*3,time_, rs, vs
    
    ts, rs, vs = [], [], []

    while True:
        try:
            offset, time_, rs_, vs_ = read_stamp(offset)
        except Exception as e:
            break
        if stamps>0 and len(ts) == stamps:
            break
        ts.append(time_)
        rs.append(rs_)
        vs.append(vs_)
    return box_size, radii, masses, np.array(ts), np.array(rs), np.array(vs)