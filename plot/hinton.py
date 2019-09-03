"""
Create Hinton diagrams from a matrix file.

Hinton diagrams are useful for visualizing the values of a 2D array (e.g.
a weight matrix): Positive and negative values are represented by white and
black squares, respectively, and the size of each square represents the
magnitude of each value.

Initial idea from David Warde-Farley on the SciPy Cookbook
"""
import os
import sys
import re
import numpy as np
import matplotlib.pyplot as plt

yticklabels_str = ["loop nest", "loop count", "#xfer_mem", "data transfer size",
		"redun. trans. size", "max blocks", "min task unit",
		"#instructions", "branch miss", "L1 DCR"]
xticklabels_str = ["convsepr1", "convsepr8", "dotprod", "fftx1y1", "fftx4y3",
		"fwt", "montecarlo", "mvmult", "scalarprod", "transpose",
		"vecadd", "binomial", "blackscholes", "dct", "prefix",
		"bfs", "histo", "lbm", "mir-q", "mri-gridding",
		"sad", "sgemm", "spmv", "stencil", "2mm",
		"3mm", "adi", "correlation", "covariance", "deriche",
		"gemm", "gemver", "gesummv", "heat-3d", "jacobi-1d",
		"jacobi-2d", "mvt", "syr2k", "syrk"]
#xticklabels_str = ["NVIDIA SDK", "AMD SDK", "PARBOIL", "POLYBENCH"]

def hinton(matrix, max_weight=None, ax=None):
    """Draw Hinton diagram for visualizing a weight matrix."""
    #plt.gca() return Axes instance on the current figure.
    ax = ax if ax is not None else plt.gca()
    ax.figure.set_size_inches(11.5,4.3)

    if not max_weight:
        max_weight = 2 ** np.ceil(np.log(np.abs(matrix).max()) / np.log(2))

    #axes background color
    ax.patch.set_facecolor('white')
    ax.set_aspect('equal', 'box')
    #Not display ticks.
    #ax.xaxis.set_major_locator(plt.NullLocator())
    #ax.yaxis.set_major_locator(plt.NullLocator())

    for (x, y), w in np.ndenumerate(matrix):
        color = 'black'
        size = np.sqrt(np.abs(w) / max_weight)
        rect = plt.Rectangle([x - size / 2 , y - size / 2], size, size,
                             facecolor=color, edgecolor=color)
        ax.add_patch(rect)

    ax.autoscale_view()
    ax.invert_yaxis()
    ax.tick_params(bottom=1, top=1, right=1, left=1, pad=1, direction='in', length=2)
    ax.set_xlim(-0.5,len(xticklabels_str)-0.5)
    ax.set_ylim(-0.5,len(yticklabels_str)-0.5)
    plt.xticks(np.arange(0,len(xticklabels_str),1))
    plt.yticks(np.arange(0,len(yticklabels_str),1))
    x_t_labels = ax.set_xticklabels(xticklabels_str, fontsize=14)
    y_t_labels = ax.set_yticklabels(yticklabels_str,fontsize=14)
    plt.setp(x_t_labels, rotation=45, horizontalalignment='right')


if __name__ == '__main__':
    if len(sys.argv) < 2:
	print 'Usage: ~ file_name'
	quit()
    f_name = sys.argv[1]
    if not os.path.isfile(f_name):
	print f_name + ' is not a legal file name'
	quit()
    m_data = np.loadtxt(f_name)
    plt.rcParams.update({'figure.autolayout':True})
    plt.style.use('my_paper')
    hinton(m_data, 1.0)
    #plt.show()
    plt.savefig('tt.pdf',bbox_inches='tight')
