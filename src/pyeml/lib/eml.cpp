#include <pybind11/pybind11.h>
#include <eml.h>
#include <iostream>
#include <Python.h>
#include <pybind11/stl.h>

namespace py = pybind11;

std::unordered_map<std::string, double> measureCodeInDevices(const char* code, const char* preparationCode, const std::set<std::string>* devices, const double conversion_factor) {
    wchar_t *program = Py_DecodeLocale("measure", NULL);

    Py_SetProgramName(program);
    Py_Initialize();

    emlInit();

    if ((preparationCode != NULL) && (preparationCode[0] != '\0')) {
            PyRun_SimpleString(preparationCode);
    }

    size_t count;

    emlDeviceGetCount(&count);

    emlDevice_t* dev;
    const char* devname; 
    
    double consumed, elapsed;
    std::unordered_map<std::string, double> output;

    std::vector<emlDevice_t*> eml_devices;

    for (size_t i = 0; i < count; i++) {
        emlDeviceByIndex(i, &dev);
        emlDeviceGetName(dev, &devname);
        if(devices->find(devname) != devices->end()){
            eml_devices.push_back(dev);
        }
    }

    emlData_t* data[eml_devices.size()];

    //Measure python execution
    for(unsigned i = 0; i < eml_devices.size(); i++){
        emlDeviceStart(eml_devices[i]);
    }

    PyRun_SimpleString(code);

    for(unsigned i = 0; i < eml_devices.size(); i++){
        emlData_t* d[1];
        emlDeviceStop(eml_devices[i],d);
        data[i] = d[0];
    }

    for(unsigned i = 0; i < eml_devices.size(); i++){
        
        emlDataGetConsumed(data[i], &consumed);
        emlDataGetElapsed(data[i], &elapsed);
        emlDataFree(data[i]);

        emlDeviceGetName(eml_devices[i], &devname);

        output[devname] = consumed/conversion_factor;
    }


    return output;
}

std::unordered_map<std::string, double> measureCode(const char* code, const char* preparationCode, const double conversion_factor) {
    wchar_t *program = Py_DecodeLocale("measure", NULL);

    Py_SetProgramName(program);
    Py_Initialize();

    emlInit();

    if ((preparationCode != NULL) && (preparationCode[0] != '\0')) {
            PyRun_SimpleString(preparationCode);
    }

    size_t count;

    emlDeviceGetCount(&count);

    emlDevice_t* dev;
    const char* devname; 
    
    double consumed, elapsed;
    std::unordered_map<std::string, double> output;
  
    emlData_t* data[count];

    emlStart();

    PyRun_SimpleString(code);

    emlStop(data);

    // PyMem_RawFree(program);

    for(unsigned i = 0; i < count; i++){
        
        emlDataGetConsumed(data[i], &consumed);
        emlDataGetElapsed(data[i], &elapsed);
        emlDataFree(data[i]);

        emlDeviceByIndex(i, &dev);
        emlDeviceGetName(dev, &devname);

        output[devname] = consumed/conversion_factor;
    }

    return output;

}


std::unordered_map<std::string, std::unordered_map<std::string,double>> measureCodeAndTime(const char* code, const char* preparationCode) {
    wchar_t *program = Py_DecodeLocale("measure", NULL);

    Py_SetProgramName(program);
    Py_Initialize();

    emlInit();

    if ((preparationCode != NULL) && (preparationCode[0] != '\0')) {
            PyRun_SimpleString(preparationCode);
    }


    //allocate space for results
    size_t count;
    emlDeviceGetCount(&count);
    emlData_t* data[count];
    //start measuring energy consumption
    emlStart();

    PyRun_SimpleString(code);

    emlStop(data);

    PyMem_RawFree(program);

    emlDevice_t* dev;
    const char* devname;
    double consumed, elapsed;
    std::unordered_map<std::string, std::unordered_map<std::string,double>> output;


    for(unsigned i = 0; i < count; i++) {
        
        emlDataGetConsumed(data[i], &consumed);
        emlDataGetElapsed(data[i], &elapsed);
        emlDataFree(data[i]);

        emlDeviceByIndex(i, &dev);
        emlDeviceGetName(dev, &devname);

        std::unordered_map<std::string,double> deviceData;
        deviceData["consumed"] = consumed;
        deviceData["elapsed"] = elapsed;

        output[devname] = deviceData;
        // std::cout<< devname << " " <<output[devname].first << " " << output[devname].second  <<"\n";
        // printf("The device '%s' consumed %g J in %g s\n", devname , consumed, elapsed);
    }

    return output;

}


std::vector<std::string> getDevices(){
    emlInit();
    size_t count;
    emlDeviceGetCount(&count);
    emlDevice_t* dev;
    const char* devname;

    std::vector<std::string> output;
    for (size_t i = 0; i < count; i++) {
        emlDeviceByIndex(i, &dev);
        emlDeviceGetName(dev, &devname);
        output.push_back(devname);
    }

    return output;
}


void shutdown(){
    emlShutdown();
}

PYBIND11_MODULE(eml, m) {
    m.doc() = "pybind11 eml plugin"; // optional module docstring
    m.def("getDevices", &getDevices, "Get devices names");
    m.def("shutdown", &shutdown, "shutdown eml");

    m.def("measureCode", &measureCode, "A function to measure code energy consumption", py::arg("code"), py::arg("preparationCode") = "", py::arg("conversion_factor") = 1);
    m.def("measureCodeInDevices", &measureCodeInDevices, "A function to measure code energy consumption", py::arg("code"), py::arg("preparationCode") = "", py::arg("devices"), py::arg("conversion_factor") = 1);

    m.def("measureCodeAndTime", &measureCodeAndTime, "A function to measure code energy consumption", py::arg("code"), py::arg("preparationCode") = "");
}