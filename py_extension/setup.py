import os
import sys
import glob
from distutils.core import setup, Extension

#os.environ["CPPFLAGS"] = "-Wnostrict-prototypes"

#pyext_path=os.path.abspath(os.path.dirname(__file__))
#nessdb_path=os.path.dirname(os.path.dirname(pyext_path))
#cfiles=glob.glob(os.path.join(nessdb_path,"ul_sign/*.cpp"))
#cfiles=glob.glob("./ul_sign/*.cpp")

os.environ['CC'] = 'g++'
ext = Extension('ul_sign',sources = ['./ul_sign_binding.c','./ul_sign/ul_prime.cpp', './ul_sign/ul_sign.cpp'])

setup(name = 'ul_sign',
      version = '0.01',
      description = 'Python binding of ul_sign',
      ext_modules = [ext])
