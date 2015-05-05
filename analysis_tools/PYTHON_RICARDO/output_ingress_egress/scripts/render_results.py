"""
Library of code related to plotting figures.
"""
import os, logging
import numpy as np
from matplotlib import pyplot as plt


def save_heatmap(save_folder, base_name, data_array,
                 label=None,
                 tick_labels=None,
                 scale_results=True,
                 ignore_in_scale=np.inf):
    """
    Save a heatmap of results using np.imshow(). Specify optional colorbar label.
    :param save_folder:
    :param base_name: The base name of the file (sans extension or folder path)
    :param data_array:
    :param label: The label for the colorbar
    :param tick_marks: Tick mark labels (numerical); if applicable
    :param scale_results:
    :param ignore_in_scale:
    """
    v_min = np.min(data_array[data_array != ignore_in_scale]) if scale_results else None
    v_max = np.max(data_array[data_array != ignore_in_scale]) if scale_results else None

#    fig = plt.figure()
    cimg = plt.imshow(data_array,
                      vmin=v_min,
                      vmax=v_max,
                      norm=None)

    cbar = plt.colorbar(cimg, ticks=tick_labels)
    if label is not None:
        cbar.set_label(label)

    out_fn = os.path.join(save_folder, base_name + ".png")
    plt.savefig(out_fn)
    plt.close()
    logging.debug("Saved heatmap visualization to {}".format(out_fn))