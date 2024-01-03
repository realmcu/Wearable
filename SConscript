# for module compiling
import os
from building import *

cwd  = GetCurrentDir()
objs = []
list = os.listdir(cwd)

for d in list:
    path = os.path.join(cwd, d)
    if GetDepend(['CONFIG_REALTEK_WITHOUT_WEARABLE']):
        Return('objs')
    else:
        if os.path.isfile(os.path.join(path, 'SConscript')):
            objs = objs + SConscript(os.path.join(d, 'SConscript'))

Return('objs')
