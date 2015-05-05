"""
Tools to help plot test bench outputs.
"""

from math import pi

import matplotlib.pyplot as plt
import numpy as np

def vehicle_polar_axes():
    """
    Make a polar plot with simplified labels and 0 deg pointing up.
    """
    ax = plt.subplot(111, polar=True)
    ax.set_thetagrids(np.arange(0, 360, 360.0 / 4), ["fore", "left", "aft", "right"])
    def name_gen(count):
        names = iter(("fore", "left", "aft", "right"))
        i = 0
        while i < count:
            if i % (count / 4) == 0:
                yield next(names)
            else:
                yield ""
            i += 1
                
    ax.set_thetagrids(np.arange(0, 360, 360.0 / 12), list(name_gen(12)))
    ax.set_theta_offset(pi / 2)    
    return ax