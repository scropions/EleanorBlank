import os
from shutil import rmtree

HOME_DIR = os.getcwd()

def generate_doxygen(doxyfile):
    from doxygen import ConfigParser
    from doxygen import Generator
    config_parser = ConfigParser()
    doxyfile = HOME_DIR + "/" + doxyfile
    configuration = config_parser.load_configuration(doxyfile)
    output_dir = configuration['OUTPUT_DIRECTORY']
    doxy_builder = Generator(doxyfile)
    if os.path.exists(output_dir):
        if os.listdir(output_dir):
            rmtree(output_dir)
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    doxy_builder.build(clean=False,generate_zip=False)

generate_doxygen("scripts/doxygen/Doxyfile.doxygen")