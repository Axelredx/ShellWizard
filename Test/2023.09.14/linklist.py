#!/usr/bin/env python3

import os
import subprocess
import sys
from collections import defaultdict

# check recursively for symb. link
def find_symlinks(directory):
    symlink_map = defaultdict(list)

    for root, dirs, files in os.walk(directory):
        for name in files + dirs:
            full_path = os.path.join(root, name)
            if os.path.islink(full_path):
                try:
                    target = os.path.realpath(full_path)
                    symlink_map[target].append(full_path)
                except OSError as e:
                    print(f"Error reading symlink {full_path}: {e}", file=sys.stderr)

    return symlink_map

def main():
    # check if correct usage
    if len(sys.argv) != 2:
        print("Usage: tcmp <directory>", file=sys.stderr)
        sys.exit(1)

    directory = sys.argv[1]
    # check if correct input
    if not os.path.isdir(directory):
        print(f"Error: {directory} is not a directory", file=sys.stderr)
        sys.exit(1)

    symlink_map = find_symlinks(directory)

    for target, links in symlink_map.items():
        if len(links) > 1:
            print(f"The following links point to {target}:")
            for link in links:
                print(f"  {link}")
            print()

if __name__ == "__main__":
    main()
