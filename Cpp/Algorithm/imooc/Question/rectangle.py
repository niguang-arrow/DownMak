import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle

def isRoad(cell):
    return cell == '[R]'

def drawMaze(mat):
    m, n = len(mat), len(mat[0])
    width, height = 1, 1

    fig = plt.figure()
    ax = fig.add_subplot(111, aspect='equal')
    ax.set_xlim(0, m)
    ax.set_ylim(0, n)
    # ax.set_axis_off()
    ax.tick_params(
        axis='both', # changes apply to both axis
        bottom=False, # bottom ticks off
        left=False,
        labelbottom=False, # bottom label off
        labelleft=False,
    )
    for i, matRow in enumerate(mat):
        for j, cell in enumerate(matRow):
            ax.add_patch(
                Rectangle(
                    (j, m - 1 - i),
                    width,
                    height,
                    edgecolor='k',
                    facecolor='white' if isRoad(cell) else 'gray',
                    linewidth=2.0
                )
            )
    plt.savefig('maze.png', bbox_inches='tight')
    plt.show()

if __name__ == '__main__':
    # from pprint import pprint as pp
    import sys
    mat = list()
    for line in sys.stdin:
        mat.append(line.strip().split())
    # pp(mat)
    drawMaze(mat)
