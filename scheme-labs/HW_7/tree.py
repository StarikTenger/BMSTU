#!/usr/bin/python3
import sys
import os

path = sys.argv[1]
print(len(sys.argv))

def tree(startpath):
    for root, dirs, files in os.walk(startpath):
        level = root.replace(startpath, '').count(os.sep)
        indent = ' ' * 4 * (level)
        print('{}{}/'.format(indent, os.path.basename(root)))
        subindent = ' ' * 4 * (level) + '├── '
        for f in files:
            print('{}{}'.format(subindent, f))

tree(path)