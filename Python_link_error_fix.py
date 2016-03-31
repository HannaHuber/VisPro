# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

import os
replaceActive=False
for root, dirs, files in os.walk("D:/VTK/VTK-7.0.0_VS2013_II/build"):
    for file in files:
        if file.endswith(".vcxproj"):
            fname = os.path.join(root, file)
            content_new = []
            with open(fname) as f:
                content = f.readlines()
                for line in content:
                    if 'Debug|Win32' in line:
                        replaceActive = True
                    elif '</ItemDefinitionGroup>' in line:
                        replaceActive = False
                    elif replaceActive:
                        if 'python35.lib' in line:
                            line = line.replace('python35.lib', 'python35_d.lib')                    
                    content_new.append(line)
                f.close()
                with open(fname,'w+') as f_new:
                    for line_new in content_new:
                        f_new.write(line_new)
                    f_new.close()
                
               