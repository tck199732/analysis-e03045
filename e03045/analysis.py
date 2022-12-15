

import pathlib
import os
import glob

# p = pathlib.Path(os.environ['CONDA_ENV'])

datadir = pathlib.Path('/data/kin/e03045')
folders = ['40Ca', '40Ca_One', '48Ca', '48Ca_40t_One', '48Ca_One']


def RDataFrame_HiRA(self):
    files = dict()
    for folder in folders:
        sub_dir = pathlib.Path(datadir, folder)
        files[folder] = glob.glob(f'{str(sub_dir)}/*.root')
        files[folder] = [pathlib.Path(p) for p in sorted(files[folder])]

    self.files = files

# def rdf(self, reac)
