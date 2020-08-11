import numpy as np
from matplotlib import pyplot as plt
from matplotlib import animation
from tqdm import tqdm
from read_binary import read_binary

fig = plt.figure()
fig.set_dpi(100)


(boxx, boxy,_), radii, masses,tss, rss, vss = read_binary()

print(rss.shape, vss.shape)

fig.set_size_inches(boxx*5, boxy*5)

dirs = []
for i in (-1,0,1):
    for j in (-1,0,1):
        dirs.append((i*boxx, j*boxy))


ax = plt.axes(xlim=(0, boxx), ylim=(0, boxy))
particles = []
for pi, rad in enumerate(radii):
    for dx,dy in dirs:
        particles.append(
            plt.Circle((rss[0,pi,0]+dx, rss[0,pi,1]+dy), rad)
        )
    
def init():
    for p in particles:
        ax.add_patch(p)
    return particles

def animate(i):
    for pi, p in enumerate(particles):
        dx, dy = dirs[pi%len(dirs)]
        p.center = (rss[i,pi//len(dirs),0]+dx, rss[i,pi//len(dirs),1]+dy)
    return particles

anim = animation.FuncAnimation(fig, animate, 
                               init_func=init, 
                               frames=len(tss), 
                               interval=10,
                               blit=True)
# progresbar.close()
# anim.save('animation.mp4', fps=60, 
#           extra_args=['-vcodec', 'h264', 
#                       '-pix_fmt', 'yuv420p'])

plt.show()