
import sys
import os

from inspect import currentframe

import __main__ as main_module

from typing import Callable, Iterable, Dict, Union

import atexit

from pyeml.units import j

from timeit import default_timer as timer

#sudo cat /sys/class/powercap/intel-rapl/intel-rapl:0/energy_uj

EML_LIB_PATH = "/usr/local/lib"

#Appending eml in LD_LIBRARY_PATH (if its needed)
if EML_LIB_PATH not in os.getenv('LD_LIBRARY_PATH',""):
    os.environ["LD_LIBRARY_PATH"] = os.getenv('LD_LIBRARY_PATH',"") + ":" + EML_LIB_PATH
    print(f"Adding '{EML_LIB_PATH}' to LD_LIBRARY_PATH. You should consider permanently adding it.")
    try:
        sys.stdout.flush()

        if(sys.argv[0] == ''): #interpreter
            os.execl(sys.executable," ")
        else:
            os.execl(sys.executable,sys.executable, *sys.argv)

    except Exception as e:
        print(f'Failed re-exec:', e)
        sys.exit(1)


sys.path.append(os.path.join(os.path.dirname(__file__),"lib"))
from eml import measureCode, measureCodeInDevices, getDevices, shutdown
import eml
atexit.register(shutdown)


def start(devices : Union[Iterable,None] = None, unit : Dict = j):
    if(devices == None):
        eml.start(conversion_factor = unit["conv"])
    else:
        if(isinstance(devices,str)):
            devices = (devices,)
        eml.start(devices = set(devices),conversion_factor = unit["conv"])

def stop():
    return eml.stop()

def get_devices():
    return  getDevices()

def measure_code(code : str , devices : Union[Iterable,None] = None, unit : Union[Dict, None] = None):

    if(unit):
        conversion_factor = unit["conv"]
    else:
        conversion_factor = 1

    if(devices is not None):
        return measureCodeInDevices(code, devices=set(devices), conversion_factor = conversion_factor)
    else:
        return measureCode(code, conversion_factor = conversion_factor)



def measure_function(function : Callable, devices : Union[Iterable,None] = None, args : Iterable = (), kwargs : Dict = {}, scope : any = None,  unit : Union[Dict, None] = None):

    if scope is None: scope = currentframe().f_back.f_globals

    main_module.__dict__["__pyeml__"] = {}
    main_module.__dict__["__pyeml__"]["__pyemlscope__"] = scope
    main_module.__dict__["__pyeml__"]["__pyemlargs__"] = args
    main_module.__dict__["__pyeml__"]["__pyemlkwargs__"] = kwargs
    main_module.__dict__["__pyeml__"]["__pyemlfunc__"] = function

    start_time = timer()
    energy = measure_code(f"""
exec('__pyeml__["__pyemloutput__"] = __pyeml__["__pyemlfunc__"](*__pyeml__["__pyemlargs__"],**__pyeml__["__pyemlkwargs__"])', __pyeml__["__pyemlscope__"])
""", devices = devices, unit = unit)
    end_time = timer()
    output = main_module.__dict__["__pyeml__"]["__pyemloutput__"]

    if("pyeml" in main_module.__dict__):
        del main_module.__dict__["__pyeml__"]

    return output, energy, end_time-start_time


def measure_decorator( function = None, devices : Union[Iterable,None] = None, unit : Union[Dict, None] = None):
    def decorator(function : Callable ):
        def wrap(*args, **kwargs):
            return measure_function(function, devices,  args = args, kwargs = kwargs, scope = currentframe().f_back.f_globals, unit = unit)

        return wrap

    if(function is not None):
        return decorator(function)
    else:
        return decorator


def measure_energy(function = None, devices : Union[Iterable,None] = None, unit : Dict = j):
    def decorator(function : Callable ):
        def wrap(*args, **kwargs):
            start(devices, unit = unit)
            start_time = timer()
            output = function(*args,**kwargs)
            energy = stop()
            end_time = timer()

            return output, energy, end_time-start_time

        return wrap

    if(function is not None):
        return decorator(function)
    else:
        return decorator
