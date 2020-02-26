#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
from datetime import datetime

try:
    from urllib.request import urlretrieve, urlopen, Request
except ImportError:
    from urllib2 import urlretrieve, urlopen, Request
try:
    from urllib import error
except ImportError:
    from urllib2 import error

from collections import defaultdict

# The VTKExamples URL.
vtk_examples_url = 'https://raw.githubusercontent.com/lorensen/VTKExamples/master/'


def load_figures(fn):
    """
    Read a text file containing the figure name and example name into a dictionary.
    The dictionary is keyed on chapter and figure number with the example name as the value.

    :param fn: The file.
    :return: The dictionary.
    """
    figs = defaultdict(dict)
    with open(fn, 'r') as f:
        for line in f:
            # Remove trailing whitespace and multiple spaces.
            line = ' '.join(line.rstrip().split())
            if not line:
                continue
            if line[0] == '#':
                continue
            kv = line.split()
            if len(kv) > 2:
                k1 = kv[0]
                k2 = kv[1]
                # There may be spaces in the value.
                v = ' '.join(kv[2:])
                figs[k1][k2] = v
    return figs


def can_open(link):
    """
    Test to see if we can open the URL.

    If urlopen() is successful (resp, None) is returned.
    If a URLError is triggered (None, err) is returned.

    :param link: The URL.
    :return: (resp, err)
    """
    req = Request(link)
    try:
        resp = urlopen(req)
    except error.URLError as e:
        err = None
        if hasattr(e, 'reason'):
            if hasattr(e.reason, 'errno'):
                err = '[Errno {:d}] {:s} {:s}'.format(e.reason.errno, e.reason.strerror, link)
            else:
                if hasattr(e, 'code'):
                    err = '[Errno {:d}] {:s} {:s}'.format(e.code, e.reason, link)
                else:
                    err = '{:s}  {:s}'.format(e.reason, link)
        elif hasattr(e, 'code'):
            err = '[Errno {:d}] {:s}'.format(e.code, link)
        return None, err
    else:
        return resp, None


def main():
    t0 = datetime.now()
    copy_changed, dest_path = get_program_parameters()
    example_url = vtk_examples_url + 'src/Testing/Baseline/Cxx/'
    figure_path = os.path.join(dest_path, 'Figures')
    figs_path = os.path.join(dest_path, 'FigureSource.txt')
    if os.path.isfile(figs_path):
        figs = load_figures(figs_path)
    else:
        print('{:s} not found.'.format(figs_path))
        return
    ext = '.png'
    files_to_copy = list()
    for chap, v in figs.items():
        for num, src in v.items():
            fig_name = 'Figure{}-{}{}'.format(chap, num, ext)
            dest = os.path.join(figure_path, fig_name)
            url = example_url + src + ext
            files_to_copy.append([url, dest])
    files_copied = 0
    new_files = list()
    changed_files = list()
    for f in files_to_copy:
        resp = can_open(f[0])
        if resp[0] is not None:
            try:
                # By default we copy new and changed fies.
                if not os.path.exists(f[1]):
                    urlretrieve(f[0], f[1])
                    files_copied += 1
                    new_files.append(f[1])
                    continue
                if os.path.exists(f[1]) and os.path.isfile(f[1]) and os.stat(f[1]).st_size != resp[0].length:
                    urlretrieve(f[0], f[1])
                    files_copied += 1
                    changed_files.append(f[1])
                    continue
                if not copy_changed:
                    urlretrieve(f[0], f[1])
                    files_copied += 1
            except error.HTTPError as e:
                print('Error: {:s}\n[{:d}] {:s}'.format(e.filename, e.code, e.reason))
                return
        else:
            print(resp[1])

    if new_files:
        print('These new files were added:')
        print('{:s}\n'.format('\n'.join(new_files)))
    if changed_files:
        print('These files were changed:')
        print('{:s}\n'.format('\n'.join(changed_files)))
    print('Number of new files added:  {:6d}'.format(len(new_files)))
    print('Number of files changed:    {:6d}'.format(len(changed_files)))
    print('Number of files copied:     {:6d}'.format(files_copied))
    print('Number of files not copied: {:6d}'.format(len(files_to_copy) - files_copied))
    print('Total number of files:      {:6d}'.format(len(files_to_copy)))
    t1 = datetime.now()
    delta = t1 - t0
    print('Elapsed time: {:s}'.format(str(delta)))


def get_program_parameters():
    import argparse
    description = 'Copy and rename images from VTKExamples.'
    epilogue = '''
The program takes one parameter, the path to the VTKBook folder.
Only new and changed files are copied by default.

It then takes relevant images from
   https://raw.githubusercontent.com/lorensen/VTKExamples/master/src/Testing/Baseline/Cxx/*,
   renames them, and places them into <PATH_TO>/VTKBook/Figures.
   
FigureSource.txt must exist in the VTKBook folder.

If running from the VTKBook folder:
   python ImportFigures.py .

'''
    parser = argparse.ArgumentParser(description=description, epilog=epilogue,
                                     formatter_class=argparse.RawTextHelpFormatter)
    # Use a mutually exclusive group for copying all or changed files.
    label_parser = parser.add_mutually_exclusive_group(required=False)
    label_parser.add_argument('-a', '--all', action='store_false', dest='copy_changed',
                              help='Copy all files.')
    label_parser.add_argument('-c', '--changed', action='store_true', dest='copy_changed',
                              help='Copy changed and new files.')
    parser.set_defaults(copy_changed=True)
    parser.add_argument('dest_path', help='The path to the VTKBook folder.')

    args = parser.parse_args()
    return args.copy_changed, args.dest_path


if __name__ == '__main__':
    main()
