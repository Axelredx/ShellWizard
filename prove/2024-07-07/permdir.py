#!/usr/bin/env python3

import os
import stat
import sys

def permdir(directory):
    # check if dir parameter ok
    if not os.path.isdir(directory):
        print(f"Errore: {directory} non è una directory valida")
        return
    
    # iteration on file in dir
    for filename in os.listdir(directory):
        filepath = os.path.join(directory, filename)
        
        # if not plain file ignore
        if not os.path.isfile(filepath):
            continue
        
        # obtain privilegies (str format)
        mode = os.stat(filepath).st_mode
        permissions = stat.filemode(mode)
        
        # mkdir permission dir if not exist
        if not os.path.exists(permissions):
            os.mkdir(permissions)
        
        # creation symb.link path
        symlink_path = os.path.join(permissions, filename)
        
        # delete if symb.link path already exist
        if os.path.exists(symlink_path):
            os.remove(symlink_path)
        
        # crwation symb.link
        os.symlink(filepath, symlink_path)
        print(f"Creato link: {symlink_path} -> {filepath}")

if __name__ == "__main__":
    # check minimum arg
    if len(sys.argv) != 2:
        print(f"Uso: {sys.argv[0]} <directory>")
        sys.exit(1)
    
    directory_path = sys.argv[1]
    permdir(directory_path)
