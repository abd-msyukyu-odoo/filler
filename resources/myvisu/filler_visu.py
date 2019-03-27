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
COLORS_NEW = ["#000000", "#00ff00", "#ffff00"]

H_PX = 800
W_PX = 1200
W_PX_LOAD = 400
H_PX_LOAD = 20
H_PX_TXT = 20
H_PX_PC = 150
W_PX_PC = 150

class Player:
    def __init__(self, id, line):
        self.id = id % len(IDS)
        self.name = re.match(r"^.*\[(.*)\].*$", line).group(1).split("/")[-1] \
            .replace(".filler", "")
        self.c = COLORS[id]
        self.cn = COLORS_NEW[id]
        self.l = IDS[id] 
        self.ln = IDS_NEW[id]
        self.score = 1

class Map:
    def __init__(self, parent, width, height, w_px, h_px):
        self.frame = tk.Frame(parent)
        self.frame.pack(side = "left")
        self.units = {}
        self.width = width
        self.height = height
        self.cell = min(h_px, w_px) / max(width, height)
        self.canvas = tk.Canvas(self.frame)
        self.canvas.configure(width = (self.cell * self.width),
            height = (self.cell * self.height))
        self.canvas.configure(background = "#000000")
        self.canvas.pack(side = "left")
        for y in range(self.height):
            for x in range(self.width):
                x1 = x * self.cell
                y1 = y * self.cell
                x2 = x1 + self.cell
                y2 = y1 + self.cell
                self.units[y, x] = self.canvas.create_rectangle(x1, y1, x2, y2,
                    fill = "#000000")

class ProgressBar:
    def __init__(self, parent, color_left, color_right):
        self.cl = color_left
        self.cr = color_right
        self.frame = tk.Frame(parent)
        self.frame.configure(background = "#000000")
        self.frame.pack(side = "top")
        self.canvas = tk.Canvas(self.frame, highlightthickness = 0)
        self.canvas.configure(width = W_PX_LOAD, height = H_PX_LOAD)
        self.canvas.configure(background = "#000000")
        self.canvas.pack(side = "top")
        self.lv = 0
        self.rv = 0

    def draw(self):
        self.canvas.delete("all")
        self.canvas.create_rectangle(0, 0, self.lv * W_PX_LOAD / 100, H_PX_LOAD,
            fill = self.cl, width = 0)
        self.canvas.create_rectangle(W_PX_LOAD - self.rv * W_PX_LOAD / 100, 0,
            W_PX_LOAD, H_PX_LOAD, fill = self.cr, width = 0)

    def left(self, value):
        self.lv = value

    def right(self, value):
        self.rv = value

class PVisu:
    def __init__(self, parent, p):
        self.p = p
        self.frame = tk.Frame(parent)
        self.frame.configure(background = "#000000")
        self.frame.pack(side = "left")
        font = Font(family="Helvetica", size = H_PX_TXT, weight = "bold")
        tk.Label(self.frame, text = f"{self.p.name}",
            fg = f"{self.p.c}", font = font, justify = "center",
            bg = "#000000").pack(side = "top")
        self.score = tk.StringVar()
        self.score.set(f'{1}')
        tk.Label(self.frame, textvariable = self.score, fg = f"{self.p.c}",
            font = font, justify = "center", bg = "#000000").pack(side = "top")

class VSLabel:
    def __init__(self, parent):
        self.frame = tk.Frame(parent)
        self.frame.configure(background = "#000000")
        self.frame.pack(side = "left")
        font = Font(family="Helvetica", size = H_PX_TXT, weight = "bold")
        tk.Label(self.frame, text = " VS ", fg = "#fff", font = font,
            justify = "center", bg = "#000000").pack(side = "left")

class Piece:
    def __init__(self, parent, width, height, w_px, h_px):
        self.frame = tk.Frame(parent)
        self.frame.pack(side = "left")
        self.units = {}
        self.w_px = w_px
        self.h_px = h_px
        self.width = width
        self.height = height
        self.cell = min(min(h_px, w_px) / max(width, height), W_PX_PC / 5)
        self.canvas = tk.Canvas(self.frame)
        self.canvas.configure(width = w_px, height = h_px)
        self.canvas.configure(background = "#000000")
        self.canvas.pack(side = "left")

    def fill_canvas(self):
        for y in range(self.height):
            for x in range(self.width):
                x1 = x * self.cell
                y1 = y * self.cell
                x2 = x1 + self.cell
                y2 = y1 + self.cell
                self.units[y, x] = self.canvas.create_rectangle(x1, y1, x2, y2,
                    fill = "#000000")

    def modify_canvas(self, width, height):
        self.canvas.delete("all")
        self.cell = min(min(self.h_px, self.w_px) / max(width, height), W_PX_PC / 5)
        self.width = width
        self.height = height
        self.fill_canvas()

class Analysis:
    i_piece = 7

    def __init__(self, parent, p1, p2):
        self.frame = tk.Frame(parent)
        self.frame.configure(background = "#000000")
        self.frame.pack(side = "left")
        self.progress = ProgressBar(self.frame, p1.c, p2.c)
        self.p_visu = tk.Frame(self.frame)
        self.p_visu.configure(background = "#000000")
        self.p_visu.pack(side = "top")
        self.p1 = p1
        self.p2 = p2
        self.p1_visu = PVisu(self.p_visu, p1)
        self.versus_label = VSLabel(self.p_visu)
        self.p2_visu = PVisu(self.p_visu, p2)
        self.piece = {}
        self.vspiece = {}
        for n in range(4):
            self.vspiece[n] = tk.Frame(self.frame)
            self.vspiece[n].configure(background = "#000000")
            self.vspiece[n].pack(side = "top")
        for n in range(8):
            self.piece[n] = Piece(self.vspiece[n // 2], 1, 1, W_PX_PC, H_PX_PC)

    def draw_piece(self, extrema, pby):
        if extrema["ymin"] is None or extrema["xmin"] is None:
            return
        height = extrema["ymax"] - extrema["ymin"] + 1
        width = extrema["xmax"] - extrema["xmin"] + 1
        Analysis.i_piece = (Analysis.i_piece + 1) % 8
        ip = Analysis.i_piece
        self.piece[ip].modify_canvas(width, height)
        for y in range(height):
            for x in range(width):
                if (y + extrema["ymin"] in pby) and (x + extrema["xmin"] in \
                    pby[y + extrema["ymin"]]):
                    self.piece[ip].canvas.itemconfig(
                        self.piece[ip].units[y, x],
                        fill = pby[y + extrema["ymin"]][x + extrema["xmin"]])

class Game:
    def __init__(self, width, height, p1, p2 = None):
        self.p1 = p1
        self.p2 = p2
        self.window = self.create_window()
        self.map = Map(self.window, width, height, W_PX, H_PX)
        self.analysis = Analysis(self.window, p1, p2)
        self.max_score = width * height

    def create_window(self):
        window = tk.Tk()
        window.title('Fillerminator')
        window.geometry(f"{W_PX}x{H_PX}")
        window.configure(background = COLORS[0])
        return window

    def start(self):
        self.window.after(1, self.loop())
        self.window.mainloop()

    def loop(self):
        input()
        self.update_board()
        while (self.next_turn()):
            pass

    def next_turn(self):
        read = ""
        while "Plateau" not in read:
            try:
                read = input()
            except:
                return False
        if "Plateau" in read:
            input()
            self.update_board()
            return True
        return False

    def update_board(self):
        check = False
        extrema = {
            "xmin": None,
            "xmax": None,
            "ymin": None,
            "ymax": None}
        pby = {}
        for y in range(self.map.height):
            line = input()
            line = line.split(" ")[1]
            for x in range(self.map.width):
                if line[x] == 'o':
                    p = self.p1
                    c = self.p1.cn
                elif line[x] == 'x':
                    p = self.p2
                    c = self.p2.cn
                elif line[x] == 'X':
                    p = self.p2
                    c = self.p2.c
                elif line[x] == 'O':
                    p = self.p1
                    c = self.p1.c
                else:
                    continue
                if line[x] in "ox":
                    if (not check):
                        check = True
                    else:
                        p.score += 1
                    if y not in pby:
                        pby[y] = {}
                    pby[y][x] = p.c
                    if extrema["xmin"] is None or x < extrema["xmin"]:
                        extrema["xmin"] = x
                    if extrema["xmax"] is None or x > extrema["xmax"]:
                        extrema["xmax"] = x
                    if extrema["ymin"] is None or y < extrema["ymin"]:
                        extrema["ymin"] = y
                    if extrema["ymax"] is None or y > extrema["ymax"]:
                        extrema["ymax"] = y
                if self.map.canvas.itemcget(self.map.units[y, x], "fill") != c:
                    self.map.canvas.itemconfig(self.map.units[y, x], fill = c)
        self.analysis.draw_piece(extrema, pby)
        self.analysis.progress.left(100 * p1.score / self.max_score)
        self.analysis.progress.right(100 * p2.score / self.max_score)
        self.analysis.progress.draw()
        self.analysis.p1_visu.score.set(f"{p1.score}")
        self.analysis.p2_visu.score.set(f"{p2.score}")
        self.window.update_idletasks()
        self.window.update()
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