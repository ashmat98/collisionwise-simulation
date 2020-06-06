import numpy as np 
from matplotlib import pyplot as plt 
import os
from tqdm import tqdm

from read_binary import read_binary

paths = [os.path.join("simulation", x) for x in os.listdir("simulation")]

ts, rss, vss = [], [], []
for p in tqdm(paths):
    _,t,_,rs, vs = read_binary(p)
    ts.append(t)
    rss.append(rs)
    vss.append(vs)

    