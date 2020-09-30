# The code for changing pages was derived from: http://stackoverflow.com/questions/7546050/switch-between-two-frames-in-tkinter
# License: http://creativecommons.org/licenses/by-sa/3.0/	

import matplotlib
matplotlib.use("TkAgg")
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2Tk
from matplotlib.figure import Figure
import matplotlib.animation as animation
from matplotlib import style
from matplotlib import pyplot as plt
import os

import tkinter as tk
from tkinter import ttk
from tkinter import filedialog


import numpy as np

import time

import threading

from read_binary import read_binary

LARGE_FONT= ("Verdana", 12)
# style.use("ggplot")

f = Figure(figsize=(5,5), dpi=100)
a = f.add_subplot(111)

xList = np.arange(0,10,1)
yList = np.random.randint(0,10,10)

a.clear()
a.plot(xList, yList)
    


class DumpViewer(tk.Tk):

    def __init__(self, dumpfile, *args, **kwargs):
        
        tk.Tk.__init__(self, *args, **kwargs)
        # self.geometry("400x510+100+100")

        tk.Tk.iconbitmap(self, default="D:\\Ashot\\Desktop\\collisionwise-simulation\\python-gui\\logo.ico")
        tk.Tk.wm_title(self, "Dump Viewer")
        self.geometry("120x50+200+150")
        
        
        container = tk.Frame(self)
        container.pack(side="top", fill="both", expand = True)
        container.grid_rowconfigure(0, weight=1)
        container.grid_columnconfigure(0, weight=1)

        self.frames = {}
        # dumpfile = "D:\\Ashot\\Desktop\\collisionwise-simulation\\python-gui\\dump 27-09-2020 19-11-02.dcbf"
        if dumpfile is None:
            dumpfile =  filedialog.askopenfilename(
                initialdir = "/",title = "Select file",
                filetypes = (("dump files","*.dcbf"),("all files","*.*")))
        if not self.check_file(dumpfile):
            exit(0)
        
        self.dumpfile = dumpfile

        for F in (LoadingPage, Plot2DPage):

            frame = F(container, self)
            self.frames[F] = frame
            frame.grid(row=0, column=0, sticky="nsew")

        self.show_frame(LoadingPage)

    def show_frame(self, cont):

        frame = self.frames[cont]
        frame.tkraise()
        return frame

    @staticmethod
    def check_file(filename):
        if filename is None:
            return False
        _, ext = os.path.splitext(filename)
        return (ext == ".dcbf")
        
class LoadingPage(tk.Frame):

    def __init__(self, parent, controller):
        tk.Frame.__init__(self,parent)
        self.pbar = ttk.Progressbar(self, mode='indeterminate')
        self.pbar.pack(side="top", expand=True)
        self.pbar.start(3)

        def run():
            controller.data = read_binary(controller.dumpfile)

            
            self.pbar.stop()
            controller.geometry("400x600+50+50")

            frame = controller.show_frame(Plot2DPage)

            frame.data_available(controller.data)


        threading.Thread(target=run,daemon=True).start()

S_PAUSE = '\u23F8'
S_PLAY = "\u25B6"


class Plot2DPage(tk.Frame):

    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent, relief = tk.GROOVE)
        self.parent = parent
        
        self.current_frame = 0
        self.current_frame_time = 0
        self.interval = 1
        self.fpi = 1
        self.playing = True
        
        self.canvas_updated = True


        self.dirs = []
        self.dirs_3d = []
        for i in (-1,0,1):
            for j in (-1,0,1):
                # for k in (-1,0,1):
                #     self.dirs_3d.append((i, j,k))
                self.dirs.append((i, j))
        self.colors = plt.rcParams['axes.prop_cycle'].by_key()['color']

        self.fig = Figure(dpi=100)
        self.ax = self.fig.add_subplot(111)
        self.ax.set_aspect("equal")

        lbl_frame = tk.Frame(self)
        lbl_frame.pack(side=tk.TOP,fill="x", padx=30,expand = True)

        lbl_frame.grid_columnconfigure(2, weight=1)


        lbl_id_frame = tk.Frame(lbl_frame)
        lbl_id_frame.grid(row=0, column=1, padx=10)
        lbl_id_0 = ttk.Label(lbl_id_frame, text="Frame: ")
        lbl_id_0.pack(side=tk.LEFT,expand = True)
        self.lbl_id = ttk.Label(lbl_id_frame, text="0")
        self.lbl_id.pack(side=tk.LEFT,expand = True)
 

        lbl_time_frame = tk.Frame(lbl_frame)
        lbl_time_frame.grid(row=0, column=2)
        lbl_time_0 = tk.Label(lbl_time_frame, text="Time: ")
        lbl_time_0.pack(side=tk.LEFT,expand = True)
        self.lbl_time = tk.Label(lbl_time_frame, text="0")
        self.lbl_time.pack(side=tk.LEFT,expand = True)

        lbl_btn_frame = tk.Frame(lbl_frame)
        lbl_btn_frame.grid(row=0, column=0)
        self.lbl_btn = ttk.Button(lbl_btn_frame, text="ids")
        self.lbl_btn.pack(side=tk.LEFT,expand = True)
        self.lbl_btn.bind('<Button-1>', self.onLblBtnClick)
        self.lbl_btn.bind('<ButtonRelease-1>', self.onLblBtnRelease)
        self.lbl_btn.was_playing = self.playing
        self.lbl_btn_txts = []


        canvas = FigureCanvasTkAgg(self.fig, self)
        canvas.draw()
        canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=True)


        self.canvas = canvas
        self.scl = ttk.Scale(self,from_=0,to=100,orient=tk.HORIZONTAL, 
            command=self.onScaleChange)
        self.scl.bind('<Button-1>', self.onScaleClick)
        self.scl.bind('<ButtonRelease-1>', self.onScaleClickRelease)
        
        # self.scl.grid(row=1,column=0)
        self.scl.pack(side=tk.TOP, fill=tk.X,padx=30)
        
        button_frame = tk.Frame(self)
        # button_frame.pack(side=tk.TOP, fill=tk.BOTH)
        button_frame.pack(side=tk.TOP,fill="x", padx=30)
        
        # bt_pause = tk.Button(button_frame,text=,command=self.onPause)
        # bt_pause.grid(row=0,column=2)
        self.bt_prev = tk.Button(button_frame,text="<", command=self.onPrev)
        self.bt_prev.grid(row=0,column=0)
        self.bt_next = tk.Button(button_frame,text=">", command=self.onNext)
        self.bt_next.grid(row=0,column=1)

        self.bt_play_pause = tk.Button(button_frame,text=S_PAUSE,
            command=self.onPlayPause)
        self.bt_play_pause.grid(row=0,column=2)
        self.bt_faster = tk.Button(button_frame,text="\u23E9",state="disabled", command=self.onFaster)
        self.bt_faster.grid(row=0,column=4)
        self.bt_slower = tk.Button(button_frame,text="\u23EA", command=self.onSlower)
        self.bt_slower.grid(row=0,column=3)

        
        self.bt_higher_fps = tk.Button(button_frame,text="\u23ED", command=self.onHigherFps)
        self.bt_higher_fps.grid(row=0,column=6)
        self.bt_lower_fps = tk.Button(button_frame,text="\u23EE", state="disabled", command=self.onLowerFps)
        self.bt_lower_fps.grid(row=0,column=5)


        toolbar_frame = tk.Frame(button_frame)
        toolbar_frame.grid(row=0,column=7)
        toolbar = NavigationToolbar2Tk(canvas, toolbar_frame)

    def onLblBtnClick(self, event):
        self.lbl_btn.was_playing = self.playing
        if self.playing:
            self.onPause()

        self.lbl_btn_txts = []
        for pi, p in enumerate(self.particles):
            txt = self.ax.text(p.center[0], p.center[1], str(pi),ha='center', va='center')
            self.lbl_btn_txts.append(txt)

        self.update_canvas()

    def onLblBtnRelease(self, event):
        for txt in self.lbl_btn_txts:
            txt.remove()
        self.update_canvas()

        if self.lbl_btn.was_playing:
            self.onPlay()


    def onNext(self):
        self.scl.set(self.current_frame+1)
        
    def onPrev(self):
        self.scl.set(self.current_frame-1)
    def onFaster(self):
        self.interval -= 1
        if self.interval == 1:
            self.bt_faster.config(state="disabled")
        self.anim.event_source.interval = self.interval * 10

    def onSlower(self):
        if self.interval == 1:
            self.bt_faster.config(state="active")
        self.interval += 1
        self.anim.event_source.interval = self.interval * 10

        
    def onHigherFps(self):
        if self.fpi == 1:
            self.bt_lower_fps.config(state="active")
        self.fpi += 1

    def onLowerFps(self):
        self.fpi -= 1
        if self.fpi == 1:
            self.bt_lower_fps.config(state="disabled")
    
    def update_labels(self):
        self.lbl_id.config(text=f"{self.current_frame:6d}")
        self.lbl_time.config(text=f"{self.current_frame_time:0.3f}")
        self.bt_next.config(
            state="disabled" if self.current_frame == self.N_frames - 1 
            else "active")
        self.bt_prev.config(
            state="disabled" if self.current_frame == 0 
            else "active")

    def onScaleChange(self, event):
        self.current_frame = round(self.scl.get())
        self.current_frame_time = self.tss[self.current_frame]

        self.update_labels()

        self.update_particles(self.current_frame)
        if not self.playing:
            self.update_canvas()

            
    def onScaleClick(self, event):
        if self.playing:
            self.onPause()
        # self.anim.event_source.stop()

    def onScaleClickRelease(self, event):
        pass

    def onPause(self):
        self.bt_play_pause.config(text=S_PLAY)
        self.playing = False
        self.anim.event_source.stop()
        self.set_animated(False)


    def onPlay(self):
        self.bt_play_pause.config(text=S_PAUSE)

        self.playing = True
        self.set_animated(True)
        self.anim.event_source.start()

    def onPlayPause(self):
        if self.playing:
            self.onPause()
        else:
            self.onPlay()

    def frame_iterator(self):
        while True:
            if self.playing:
                self.current_frame += self.fpi
                if self.current_frame >= self.N_frames:
                    self.current_frame = 0
            yield self.current_frame

    def update_canvas(self):
        def run():
            self.canvas.draw_idle()
            self.canvas_updated = True

        if self.canvas_updated:
            self.canvas_updated = False
            threading.Thread(target=run,daemon=True).start()

    def init_particles(self):
        self.particles = []
        for pi, rad in enumerate(self.radii):
            for _ in self.dirs:
                p = plt.Circle((0,0), rad)
                p.set_color(self.colors[pi % len(self.colors)])
                self.particles.append(p)

    def update_particles(self, frame):
        for pi, p in enumerate(self.particles):
            dx, dy = self.dirs[pi%len(self.dirs)]
            r = self.rss[frame][pi//len(self.dirs)]
            p.center = (r[0]+dx,r[1]+dy)
    def add_patches(self):
        for p in self.particles:
            self.ax.add_patch(p)

    def set_animated(self, animated):
        for p in self.particles:
            p.set_animated(animated)


    def init_animation(self):
        return self.particles
    

    def update_frame(self, i):
        self.scl.set(i)
        return self.particles

    
    def data_available(self, data):
        self.data = data
        (boxx, boxy,_), self.radii, masses,self.tss, self.rss, vss = self.data
        self.N_frames = len(self.rss)
        self.scl.configure(to=self.N_frames-1)


        self.init_particles()
        self.update_particles(self.current_frame)
        self.set_animated(True)
        self.add_patches()

        self.anim = animation.FuncAnimation(self.fig, self.update_frame, 
            frames=self.frame_iterator,
            init_func=self.init_animation,
            interval=1,blit=True)
        
        # self.onPause()


import argparse
  
if __name__ == "__main__":
    
    parser = argparse.ArgumentParser(
        description='Visualize dump file. Provide file path.')
    parser.add_argument('path', metavar='path', type=str, nargs="?",
                    help='path to the *.dcbf file', 
                    default=None)
    args = parser.parse_args()
    print(args.path)
    app = DumpViewer(dumpfile=args.path)
    app.mainloop()