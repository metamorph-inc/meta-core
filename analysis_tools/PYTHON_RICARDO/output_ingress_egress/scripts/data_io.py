"""
Various functions useful for saving and loading arrays of results
"""
import os, json, logging
import numpy as np


def save_json(folder_name, base_name, output_data):
    """
    Serialize a structure of output data to JSON file in specified location

    :param folder_name:
    :param base_name:
    :param output_data: Nested dictionary structure that can be serialized with JSON
    """
    # TODO: most notable required error handling would be if incompatible data struc or non-ASCII
    #  chars specified.
    if not os.path.isdir(folder_name):
        os.makedirs(folder_name)

    out_fn = os.path.join(folder_name, base_name + ".json")
    with open(out_fn, 'w') as f:
        json.dump(output_data, f)
    logging.debug("Saved JSON results to {}".format(out_fn))


def save_array(folder_name, base_name, data_array):
    """
    Save numpy array data
    """
    if not os.path.isdir(folder_name):
        os.makedirs(folder_name)
    out_fn = os.path.join(folder_name, base_name + ".npy")
    np.save(out_fn, data_array)
    logging.debug("Saved array data to {}".format(out_fn))


def save_multi_array(folder_name, base_name, dict_of_arrays):
    """
    Use np.savez to store multiple arrays in a single file.
    :param folder_name:
    :param base_name:
    :param dict_of_arrays:
    """
    if not os.path.isdir(folder_name):
        os.makedirs(folder_name)
    out_fn = os.path.join(folder_name, base_name + ".npz")
    np.savez(out_fn, **dict_of_arrays)
    logging.debug("Saved array data to {}".format(out_fn))


def load_array(folder_name, base_name, multi_array=False):
    """
    Load in an array with results. Sets the extension based on whether the file contains one
    array (from np.save) or multiple arrays (from np.savez).
    """
    ext = ".npz" if multi_array else ".npy"

    inp_fn = os.path.join(folder_name, base_name + ext)
    d = np.load(inp_fn)
    logging.debug("Loaded array data from {}".format(inp_fn))

    if multi_array:
        # np.load seems to return a file object for npz files; i/o can be slow as specific
        # desired arrays are only called in sequence. Get all the data and ensure the file is
        # correctly closed.
        arr_dict = {k: v.copy() for k, v in d.items()}
        d.close()
        return arr_dict
    else:
        return d