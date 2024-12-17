
def rapl(device_number):
    return f"rapl{device_number}"    


def rapl_new(device_number, module = "package"):
    return f"intel-rapl:{device_number}/{module}"

def nvml(device_number):
    return f"nvml{device_number}"

def dummy(device_number):
    return f"dummy{device_number}"

def mic(device_number):
    return f"mic{device_number}"

def sb_pdu(device_number):
    return f"sb_pdu{device_number}"

def odroid(device_number):
    return f"odroid{device_number}"

def labee(device_number):
    return f"labee{device_number}" 

def pmlib(device_number, device_outlet = 0):
    return f"pmlib0{device_number}_outlet{device_outlet}" 

