import time, os, sys, re
import subprocess
import argparse
from shutil import copyfile, rmtree
from time import sleep

def build_command(args = ""):
        is_mkdir = False
        os.chdir(HOME_DIR)
        if not os.path.exists('build'):
                os.mkdir('build')
                is_mkdir = True
        os.chdir('build')
        visual_studio_proccess = None
        if (args == "") or (args != "" and is_mkdir == True):
                visual_studio_proccess = subprocess.Popen('cmake -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Debug ..')
                visual_studio_proccess.wait()
        if (args == "package"):
                visual_studio_proccess = subprocess.Popen('cmake --build . --config Debug -- /m')
                visual_studio_proccess.wait()
        os.chdir(HOME_DIR) 
        if(visual_studio_proccess.returncode != 0):
                sys.exit(visual_studio_proccess.returncode)
                return version_next

parser = argparse.ArgumentParser()
parser.add_argument('-b','--build',action="store_true", help="Make build. Example: build_project.py -b")
parser.add_argument('-s','--solution', action="store_true",help="Generate solution. Example: build_project.py -s")
args = parser.parse_args()
HOME_DIR = os.getcwd() + "/../"
if(len(sys.argv) == 1):
        parser.print_help()
if(args.solution):
         build_command()
if(args.build):
	build_command('package')   
sys.exit(0)
