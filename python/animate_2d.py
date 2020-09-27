import numpy as np
from matplotlib import pyplot as plt
from matplotlib import animation
from tqdm import tqdm
from read_binary import read_binary

colors = plt.rcParams['axes.prop_cycle'].by_key()['color']

plt.figure(figsize=(10,1));plt.scatter(range(len(colors)),np.zeros(len(colors)), c=colors)

fig = plt.figure()
fig.set_dpi(100)


(boxx, boxy,_), radii, masses,tss, rss, vss = read_binary(stamps=-1)

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
        p = plt.Circle((rss[0,pi,0]+dx, rss[0,pi,1]+dy), rad)
        p.set_color(colors[pi % len(colors)])
        particles.append(p)
    
progresbar = tqdm(range(len(tss)+1))
def init():
    global progresbar
    progresbar = tqdm(range(len(tss)+1))
    for ip, p in enumerate(particles):
        ax.add_patch(p)
    return particles

def animate(i):
    if i>0 and i % 100 == 0:
        progresbar.update(100)
    for pi, p in enumerate(particles):
        dx, dy = dirs[pi%len(dirs)]
        p.center = (rss[i,pi//len(dirs),0]+dx, rss[i,pi//len(dirs),1]+dy)
    plt.title(f"frame {i}")
    return particles

anim = animation.FuncAnimation(fig, animate, 
                               init_func=init, 
                               frames=len(tss), 
                               interval=10,
                               blit=True)
print("start")
plt.show();exit(0)

anim.save('animation2.mp4', fps=30, 
          extra_args=['-vcodec', 'h264', 
                      '-pix_fmt', 'yuv420p'])
