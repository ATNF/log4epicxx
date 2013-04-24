# DO THIS FIRST to set project name !!!
import askapdev.sphinx
# CAN NOT contain spaces!
askapdev.sphinx.project = u'log4epicxx'

from askapdev.sphinx.conf import *

#version = 'current'
#release = 'current'
extensions += ['askap.epicsdb.sphinxext']
html_sidebars = {'records_*' : ['globaltoc.html', 'sourcelink.html', 'searchbox.html']}
