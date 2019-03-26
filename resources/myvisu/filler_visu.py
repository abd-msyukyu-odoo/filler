import tkinter as tk
import re
import sys
import time
from tkinter.font import Font
from tkinter.ttk import Progressbar
from tkinter.ttk import Style

IDS = ".OX"
IDS_NEW = "\0ox"
COLORS = ["#000000", "#8219c4", "#d3168c"]
COLORS_NEW = ["#000000", "#9d00ff", "#ff009d"]

H_PX = 800
W_PX = 1200
W_PX_LOAD = 250
H_PX_LOAD = 20
H_PX_TXT = 20

class Player():
    def __init__(self, id, line):
        self.id = id % len(IDS)
        self.name = re.match(r"^.*\[(.*)\].*$", line).group(1).split("/")[-1] \
            .replace(".filler", "")
        self.c = COLORS[id]
        self.cn = COLORS_NEW[id]
        self.l = IDS[id] 
        self.ln = IDS_NEW[id]
        self.score = 0
        self.capture = 0

class Map():
    def __init__(self, parent, width, height, w_px, h_px):
        self.frame = tk.Frame(parent)
        self.frame.pack(side = "left")
        self.colors = {}
        self.width = width
        self.height = height
        self.cell = min(h_px, w_px) / max(width, height)
        self.canvas = tk.Canvas(self.frame)
        self.canvas.configure(width = (self.cell * self.width),
            height = (self.cell * self.height))
        self.canvas.configure(background = "#000000")
        self.canvas.pack(side = "left")
        for x in range(self.width):
            for y in range(self.height):
                x1 = x * self.cell
                y1 = y * self.cell
                x2 = x1 + self.cell
                y2 = y1 + self.cell
                self.colors[y, x] = self.canvas.create_rectangle(x1, y1, x2, y2,
                    fill="#000000")

class ProgressBar():
    def __init__(self, parent, color):
        self.c = color
        self.frame = tk.Frame(parent)
        self.frame.pack(side = "top")
        self.frame.configure(background = "#000000")
        self.canvas = tk.Canvas(self.frame)
        self.canvas.configure(width = W_PX_LOAD, height = H_PX_LOAD)
        self.canvas.configure(background = "#000000")
        self.canvas.pack(side = "top")
        self.canvas.

class PVisu():
    def __init__(self, parent, p):
        self.p = p
        self.frame = tk.Frame(parent)
        self.frame.pack(side = "top")
        self.frame.configure(background = "#000000")
        font = Font(family="Helvetica", size = H_PX_TXT, weight = "bold")
        tk.Label(self.frame, text = f"{self.p.name}",
            fg = f"{self.p.c}", font = font, justify = "center",
            bg = "#000000").pack(side = "top")
        self.progress = ProgressBar(self.frame, p.c)
        self.score = tk.StringVar()
        self.score.set(f'{0}%')
        tk.Label(self.frame, textvariable = self.score, fg = f"{self.p.c}",
            font = font, justify = "center", bg = "#000000").pack(side = "top")


class Analysis():
    def __init__(self, parent, p1, p2):
        self.frame = tk.Frame(parent)
        self.frame.pack(side = "left")
        self.frame.configure(background = "#000000")
        self.p1 = p1
        self.p2 = p2
        self.p1_visu = PVisu(self.frame, p1)
        self.piece = Map(self.frame, 50, 50, 400, 400)
        self.piece.frame.pack(side = "top")
        self.p2_visu = PVisu(self.frame, p2)


class Game():
    def __init__(self, width, height, p1, p2 = None):
        self.p1 = p1
        self.p2 = p2
        self.window = self.create_window()
        self.map = Map(self.window, width, height, W_PX, H_PX)
        self.analysis = Analysis(self.window, p1, p2)

    def create_window(self):
        window = tk.Tk()
        window.title('Fillerminator')
        window.geometry(f"{W_PX}x{H_PX}")
        window.configure(background = COLORS[0])
        return window

    def start(self):
        self.loop()

    def loop(self):
        self.next_turn()
        self.loop()

    def next_turn(self):
        read = ""
        while "Plateau" not in read:
            try:
                read = input()
            except:
                exit
        if "Plateau" in read:
            input()
            self.update_board()

    def update_board(self):
        return

if __name__ == '__main__':
    read = ""
    p1 = None
    p2 = None
    while "Plateau" not in read:
        read = input()
        if "p1" in read:
            p1 = Player(1, read)
        elif "p2" in read:
            p2 = Player(2, read)
    if "Plateau" not in read:
        exit()
    dimensions = read[:-1].split(" ")
    h = int(dimensions[1])
    w = int(dimensions[2])
    game = Game(w, h, p1, p2)
    game.start()