# Taken from TiltedPhoques/TiltedEvolution: https://github.com/tiltedphoques/TiltedEvolution

import os
import time
import subprocess

def format_cpp_file(file_path, clang_format_path):
  subprocess.run(["clang-format", "-i", file_path, "-style=file:" + clang_format_path, "-fallback-style=none"])

def recursive_directory_iteration(directory, clang_format_path):
  for filename in os.listdir(directory):
    file_path = os.path.join(directory, filename)
    
    if os.path.isdir(file_path):
      recursive_directory_iteration(file_path, clang_format_path)
    elif file_path.endswith(".cpp") or file_path.endswith(".h"):
      format_cpp_file(file_path, clang_format_path)

print("Formatting C++ files...")

path = os.getcwd() + "/.clang-format"
if not os.path.exists(path):
  print("Could not find clang format config at " + path)
  exit()

print("Found clang format config at " + path)

start_time = time.time()
recursive_directory_iteration(os.getcwd(), path)

print("Done formatting C++ files after " + str(time.time() - start_time) + " seconds")