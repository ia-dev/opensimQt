# VTK TextBook

## Introduction

These LaTeX files and associated scripts generate the VTK textbook as a PDF. The LaTeX files are divided into chapters for convenience with the master document being `VTKTextBook.tex`. This document collects all the necessary `tex` files needed to produce the final document.

To generate the PDF and to remove temporary files, use these scripts:

- `MakeDocument.bash` and `MakeDocument.cmd`.
- `Clean.bash` and `Clean.cmd`.

Note that `lualatex` and `biber` are used to generate the PDF.

## Editors used

[TexStudio](https://www.texstudio.org/) is used to edit/build the document.

When setting up TexStudio, remember to go to  `Configure TeXstudio|Build` and change the `Default Compiler` to `LuaLaTeX` and the `Default Bibliography Tool` to `Biber`.

[JabRef](http://www.jabref.org/) is used to maintain the bibliography.

## Figures
All the illustrations are found in `Figures`, `Figures/Scraped` and `Figures/Headings` folder. The intent here is that high-quality illustrations will ultimately end up in the `Figures` folder. Currently there is a mix of low/high quality illustrations there.

The Scraped sub-folder contains illustrations directly scraped from the textbook. The Headings sub-folder contains illustrations that are in the chapter headings.

When better illustrations of these are made they go into `Figures` and the corresponding poor quality ones are removed from the `Scraped` sub folder.

Many figures can be synchronized with the corresponding ones in [VTKExamples](https://lorensen.github.io/VTKExamples/site/VTKBookFigures/).

In the folder `VTKBook`there is a file called `FigureSource.txt` this lists all the figure names and the corresponding source names in VTKExamples.

Changes to figures happen in two ways:

- Better ones are introduced
- The figures in VTKExamples are either added to or updated.

When figures in VTKExamples are either added to or updated we need to update the corresponding ones in `Figures`. If the figure does not exist in `Figures`, then a new line needs to be added to `FigureSource.txt` with the figure name and corresponding VTKExample source name. Then either `ImportFigures.py` or `ImportFiguresHTTP.py` need to be run.

- `ImportFigures.py` requires that you have a local checkout of VTKExamples. Make sure that you update this local checkout before running `ImportFigures.py`. To check it out, read: [VTKExamples - Development](https://lorensen.github.io/VTKExamples/site/Instructions/ForDevelopers/)

- `ImportFiguresHTTP.py` is slower as it directly uses the URLs of the on-line master of VTKExamples.


If figures have changes, run one or the other of these scripts before running `MakeDocument`.

## Procedure for adding/editing chapters

1. When starting a chapter, copy the relevant figures across from `VTKExamples/src/VTKBook/Figures` into `VTKBook/Figures`. After that initial copy just add figures into `VTKBook/Figures`. If the figures do not exist then scrape them from the original [PDF](https://www.vtk.org/vtk-textbook/) and place them in `VTKBook/Figures/Scraped`

2. Do the bibliography at the end of the chapter using JabRef (load the existing `Bibliography.bib` first) and then do the Bibliographic Notes section to confirm all references are correct.

3. Where examples exist in [VTKBookFigures](https://lorensen.github.io/VTKExamples/site/VTKBookFigures/), update `FigureSource.txt` with the figure name and and corresponding VTKExample source name, then run either `ImportFigures.py` or `ImportFiguresHTTP.py`. Running either of these scripts will update all the figures that correspond to the test examples.

4. If there is no drawing or figure available then scrape it  from the original [PDF](https://www.vtk.org/vtk-textbook/) and place it in  `VTKBook/Figures/Scraped`. When better images become available they are placed into  `VTKBook/Figures` and the corresponding image in  `VTKBook/Figures/Scraped` is removed.

5. For equations use Bernard's excellent work in `Equations.txt` with one minor change, instead of `\vec{v}` use `\overrightarrow{v\ }` to improve appearances. At the end of each equation you need to add a line describing the equation e.g.:

    ```
    \begin{equation}\label{eq:???}
    x = y
    \end{equation}
    \myequations{Description of the Equation}
    ```
    This means that the equation listings will have a short description for each equation.

6. For code listings, add a short description. See Chapter 12 for examples.

7. In doing the chapter you will find references to pages and sections in other chapters. As these occur, add the relevant labels into the other chapters.

## Indexing
When indexing make sure all sections, subsections and subsubsections when indexed use a page range.

    ```
    \section{some section}
    \index{some section|(}
         ... lots of text ...
    \index{some section|)}
    ```

Where an index is in a listing i.e in `\begin{lstlisting} ... \end{lstlisting}` it must be escaped so add `escapechar=\$` for **C++** or **Python** and `escapechar=\%` for **Tcl**, then escape the `\index{...}` command as follows. For **C++** or **Python**: `$\index{...}$` and for **Tcl**:  `%\index{...}%`

## ToDo

Here we list tasks that need to be done.

1. Improve layout.

2. Replace Tcl examples with Python ones.

3. Remove/replace obsolete sections in the book.

4. Better versions of the figures in `Figures/Scraped` will be needed for publication quality. When a new version is made it should go into `Figures` and the corresponding one in `Figures/Scraped` removed.

5. Better versions of the figures in `Figures/Headings` will be needed for publication quality.

6. Better high quality versions of the line drawings are essential. At present most line drawings are just pngs.

7. Figures will need adjustment, size, layout etc..

8. Figures in `Figures` will need to be reviewed and better versions provided. It may be that the code producing these figures in [VTKBookFigures](https://lorensen.github.io/VTKExamples/site/VTKBookFigures/) will have to be customized.

9. It would be nice to automate the build process with CMake.
