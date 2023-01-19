import os
from surface_io import open_geom, save_binary_stl

def convert():
	ivdir = os.path.join(os.getcwd(), r"INVENTOR")
	stldir = os.path.join(os.getcwd(), r"STL_BINARY")
	if os.path.isdir(ivdir):
		if not os.path.exists(stldir):
			os.mkdir(stldir)
		for iv in os.listdir(ivdir):
			srf = open_geom(os.path.join(ivdir, iv))
			save_binary_stl(os.path.join(stldir, str(os.path.splitext(iv)[0] + ".stl")), srf)
	else:
		print("No inventor file directory found.")

if __name__ == "__main__":
	convert()