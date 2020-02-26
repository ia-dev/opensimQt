#!/bin/bash
lualatex VTKTextBook
biber VTKTextBook
makeglossaries VTKTextBook
lualatex VTKTextBook
makeindex VTKTextBook
# SyncTeX allows an editor or viewer to link between the
#  compiled version of the document (such as a pdf)
#  and the original LATEX source code.
lualatex -synctex=1 VTKTextBook
