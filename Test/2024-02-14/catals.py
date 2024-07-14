#!/usr/bin/env python3
import sys
import os
import subprocess
from collections import defaultdict

""" Py prgram that create a calogue of dir passed as paramater 
(or current dir. if no parameter)"""

def get_file_type(filepath):
    """Returns the file type string using the 'file' command."""
    result = subprocess.run(['file', '--brief', filepath], stdout=subprocess.PIPE, text=True)
    return result.stdout.strip()

def catalog_files(directory):
    """Catalogs files in the given directory by their file type."""
    file_catalog = defaultdict(list)

    for root, dirs, files in os.walk(directory):
        for name in files:
            filepath = os.path.join(root, name)
            file_type = get_file_type(filepath)
            file_catalog[file_type].append(filepath)
        
        # Catalog directories
        for name in dirs:
            dirpath = os.path.join(root, name)
            file_type = 'directory'
            file_catalog[file_type].append(dirpath)
        
        # Only catalog the top-level directory (do not recurse)
        break

    return file_catalog

def print_catalog(file_catalog):
    """Prints the catalog of files organized by their file type."""
    for file_type, paths in sorted(file_catalog.items()):
        print(f"{file_type}:")
        for path in sorted(paths):
            print(f"   {path}")
        print()

def main():
    if len(sys.argv) > 2:
        print(f" Error, correct usage: {sys.argv[0]} [directory]", file=sys.stderr)
        sys.exit(1)

    directory = sys.argv[1] if len(sys.argv) == 2 else '.'

    if not os.path.isdir(directory):
        print(f"Error: {directory} is not a directory", file=sys.stderr)
        sys.exit(1)

    file_catalog = catalog_files(directory)
    print_catalog(file_catalog)

if __name__ == '__main__':
    main()