import numpy as np 
from matplotlib import pyplot as plt 
import os
from tqdm import tqdm
import shutil

from read_binary import read_binary, read_binary_inits

files = sorted(os.listdir("simulation"), key=lambda x:float(x.split()[0]))
paths = [os.path.join("simulation", x) for x in files]


from mpl_toolkits.mplot3d import Axes3D

def plot_shere(ax, x, y, z, r, box_size, resolution=100,**kwargs):
    """ simple function to plot a sphere to a 3d axes """

    u = np.linspace(0, 2 * np.pi, resolution)
    v = np.linspace(0, np.pi, resolution)
    
    bx,by,bz = box_size
    xx = r * np.outer(np.cos(u), np.sin(v)) + x
    yy = r * np.outer(np.sin(u), np.sin(v)) + y
    zz = r * np.outer(np.ones(np.size(u)), np.cos(v)) + z
    
#     xx[xx<0] = 0
#     xx[xx>bx] = bx
    
#     yy[yy<0] = 0
#     yy[yy>by] = by
    
#     zz[zz<0] = 0
#     zz[zz>bz] = bz
    
    ind = (xx>0) & (xx<bx) & (yy>0) & (yy<by) & (zz>0) & (zz<bz)
    zz[~ind] = np.nan
    if ind.any():
        ax.plot_surface(xx, yy, zz,  rstride=4, cstride=4, **kwargs)


def job(path):
    box_size, N, radii, masses = read_binary_inits("model_inits.bin")
    t,rs, vs = read_binary(path)

    colors = plt.rcParams["axes.prop_cycle"][:len(rs)]

    # set up the figure
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    # loop through the data and plot the spheres
    for pos, radius, c in zip(rs,radii, colors):
        for i in range(-1,2):
            for j in range(-1,2):
                for k in range(-1,2):
                    plot_shere(ax, *(pos+box_size*np.array([i,j,k])), 
                               radius, box_size, edgecolor='none', color=c["color"])

    # set the axes limits and show the plot
    ax.set_xlim([0,box_size[0]])
    ax.set_ylim([0,box_size[1]])
    ax.set_zlim([0,box_size[2]])
    ax.set_xlabel("x")
    ax.set_ylabel("y")

    plt.savefig(os.path.join("imgs", f"{t:0.4f}.png"))
    plt.close(fig)

from multiprocessing import Pool
import time
from tqdm import *


if __name__ == "__1main__":
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    plot_shere(ax, 3, 3, 3, 0.1, [10,10,10], resolution=100,)
    plot_shere(ax, 3.2, 3, 3, 0.1, [10,10,10], resolution=100,)
    ax.set_ylim([2,4])
    ax.set_xlim([2,4])
    ax.set_zlim([2,4])
    ax.set_xlabel("x")
    ax.set_ylabel("y")

    plt.show()
    if os.path.exists("imgs"):
        shutil.rmtree("imgs",)
    os.mkdir("imgs")
    
    rss = []
    for path in paths:
        box_size,t,_,rs, vs = read_binary(path)
        rss.append(rs)
    
    rss = np.array(rss)
    print(rss.shape)
    y = np.sqrt(np.square((rss[:,1,:]-rss[:,0,:])).sum(axis=-1))
    plt.plot(y)
    plt.show()



if __name__ == "__main__":
    if os.path.exists("imgs"):
        shutil.rmtree("imgs")
    os.mkdir("imgs")
        
    with Pool(processes=8) as p:
        with tqdm(total=len(paths)) as pbar:
            for i, _ in enumerate(p.imap_unordered(job, paths)):
                pbar.update()
            # for path in tqdm(paths):
            #     job(path)
    shutil.rmtree("simulation/*")

