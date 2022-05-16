def install_package(package):
    pip._internal.main(['install', package])
    
try:
    import pip._internal
except ImportError:
    p = subprocess.Popen('python -m pip install --upgrade pip')
    p.wait()
    import pip._internal

install_package('doxygen-interface')