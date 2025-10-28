#!/bin/python3

from sys import argv
import os

expanded_files = set()

def is_local_include(line: str):
    if line.startswith('#include <gempro/') and line.endswith('>\n'):
        return True
    if line.startswith('#include "gempro/') and line.endswith('"\n'):
        return True
    return False

def get_local_include_file(line: str):
    filename = line[len('#include <gempro/'):-2]
    print(f'Expanding {filename}')
    filepath = os.path.join("./include", 'gempro', filename)
    content = open(filepath).readlines()
    return content

def mapper(line: str):
    if not is_local_include(line):
        return line.rstrip()
    if line in expanded_files:
        return ''
    expanded_files.add(line)
    lines = get_local_include_file(line)
    lines = list(map(mapper, lines))
    return '\n'.join(lines)

def main(filename):
    lines = open(filename).readlines()
    lines = list(map(mapper, lines))
    print('Writing to combined.cpp')
    open('combined.cpp', 'w').write('\n'.join(lines))

if __name__ == '__main__':
    if len(argv) < 2:
        print(f"Usage: {argv[0]} <filename>")
        exit(1)
    main(argv[1])
