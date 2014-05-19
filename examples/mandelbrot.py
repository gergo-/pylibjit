# This larger example shows off various features: machine integers, machine
# floats, arrays, math intrinsics. It also shows that you need not compile
# all the functions in your application (such as the code for displaying a
# GUI), a small computational core often suffices.
# Execute this program with the -c or --compile command line flag to enable
# compilation; be sure to compare interpreted and compiled speeds.
# This program requires PyQt4, which may not be installed everywhere.

import argparse

compile = False
parser = argparse.ArgumentParser(description='Display the Mandelbrot set.')
parser.add_argument('-c', '--compile', action='store_true',
                    help='compile the hot parts of the computation')
args = parser.parse_args()
compile = args.compile

from array import array
import math

def mandel(N):
    lines = []
    zero_line = [0] * N
    for i in range(N):
        line = array('d', zero_line)
        mandel_line(i, N, line)
        lines.append(line)
    return lines

import jit, pylibjit

# A pylibjit "compiler" is a high-level object that can be used as a
# decorator or as a callable (see invocations below). It does not actually
# do anything until applied to a function definition.
compile_mandel_point = \
    pylibjit.compile(return_type=jit.Type.float64,
                     argument_types=[jit.Type.int] * 3,
                     variables={'cx': jit.Type.float64, 'cy': jit.Type.float64,
                                'x': jit.Type.float64, 'y': jit.Type.float64,
                                'iteration': jit.Type.int,
                                'max_iteration':jit.Type.int},
                     intrinsics={'math.log'},
                     dump_code=False)

def mandel_point(i, j, N):
    cx = 2*i / N - 1.5
    cy = 2*j / N - 1
    x, y = 0, 0
    iteration = 0
    max_iteration = 255
    while x*x + y*y < 4 and iteration < max_iteration:
        x, y = x*x - y*y + cx, 2*x*y + cy
        iteration += 1
    if iteration == max_iteration:
        return 1
    else:
        return math.log(iteration) / math.log(2) / 8

compile_mandel_line = \
    pylibjit.compile(return_type=jit.Type.void,
                     argument_types=[jit.Type.int, jit.Type.int,
                                     jit.Type.array_t(jit.Type.float64)],
                     variables={'j': jit.Type.int, 'p': jit.Type.float64},
                     constant_size={'results'},
                     dump_code=False)

def mandel_line(i, N, results):
    for j in range(N):
        p = mandel_point(j, i, N)
        results[j] = p

# Actual compilation takes place here, if -c/--compile was passed on the
# command line.
if compile:
    print('running in compiled mode')
    mandel_point = compile_mandel_point(mandel_point)
    mandel_line = compile_mandel_line(mandel_line)
else:
    print('running in interpreted mode')

import sys, subprocess
from PyQt4 import QtGui, QtCore
from threading import Thread, Lock
from queue import Queue

N = 500
q = Queue()
done = Queue()

block_size = 5

def mandel_worker(app, w):
    zero_line = [0] * N
    start = time.time()
    for i in range(0, N, block_size):
        for ii in range(block_size):
            line = array('d', zero_line)
            mandel_line(i + ii, N, line)
            q.put(line)
        w.processBlock()
        w.update()
    end = time.time()
    print()
    print('compiled:' if compile else 'interpreted:',
          end - start, 'seconds')
    done.put('done!')

class Mandelbrot(QtGui.QWidget):
    def __init__(self, app):
        super().__init__()
        self.app = app
        self.lines = []
        self.j = 0

    def initUI(self):
        self.resize(N, N)
        # Check whether we are compiling, and place the window elsewhere if
        # not. This way we can show the interpreted and compiled versions
        # running side-by-side.
        if not hasattr(mandel_point, '__original_function__'):
            self.move(0, 0)
        else:
            self.move(N+2, 0)
        self.setWindowTitle('Mandelbrot [' +
                            ('compiled' if compile else 'interpreted')
                            + ']')
        self.image = QtGui.QImage(N, N, QtGui.QImage.Format_RGB32)
        self.image.fill(QtCore.Qt.gray)
        self.show()

    def processBlock(self):
        black = QtCore.Qt.black
        rgb = lambda r, g, b: QtGui.qRgb(r*255, g*255, b*255)
        while True:
            try:
                line = q.get(block=False)
                for i, p in enumerate(line):
                    color = black if p == 1.0 else rgb(p, p, 1-p)
                    self.image.setPixel(i, self.j, color)
                self.j += 1
            except:
                break

    def paintEvent(self, e):
        qp = QtGui.QPainter()
        qp.begin(self)
        qp.drawImage(0, 0, self.image)
        qp.end()

    def keyPressEvent(self, e):
        if e.key() == QtCore.Qt.Key_Q:
            self.close()

    def closeEvent(self, e):
        sys.exit(0)

import time

def main(args):
    app = QtGui.QApplication(args)
    w = Mandelbrot(app)
    w.initUI()
    worker_thread = Thread(target=mandel_worker, args=(app, w))
    w.worker_thread = worker_thread
    worker_thread.start()
    sys.exit(app.exec_())

if __name__ == '__main__':
    main(sys.argv)
