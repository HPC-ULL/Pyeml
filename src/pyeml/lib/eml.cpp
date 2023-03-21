#include <pybind11/pybind11.h>
#include <eml.h>
#include <iostream>
#include <pybind11/stl.h>
#include <pybind11/eval.h>
#include <stdexcept>

namespace py = pybind11;

static std::vector<emlDevice_t*> eml_devices;
static double _conversion_factor = 1;

static bool initialized = false;

void start(const std::set<std::string> devices, const double conversion_factor){

    if(initialized){
        throw std::runtime_error( "Pyeml already started" );
    }

    emlInit();
    initialized = true;

    _conversion_factor = conversion_factor;

    if(devices.size() > 0){
        size_t count;
        emlDeviceGetCount(&count);
        emlDevice_t* dev;
        const char* devname; 

        for (size_t i = 0; i < count; i++) {
            emlDeviceByIndex(i, &dev);
            emlDeviceGetName(dev, &devname);
            if(devices.find(devname) != devices.end()){
                eml_devices.push_back(dev);
                emlDeviceStart(dev);
            }
        }

    }else{
        emlStart();
    }

    py::gil_scoped_release release;
}

std::unordered_map<std::string, double> stop(){

    size_t count;

    if(eml_devices.empty()){
        emlDeviceGetCount(&count);
    }else{
        count = eml_devices.size();
    }

    emlData_t* data[count];

    if(eml_devices.empty()){
        emlStop(data);
    }else{
        for(unsigned i = 0; i < eml_devices.size(); i++){
            emlData_t* d[1];
            emlDeviceStop(eml_devices[i],d);
            data[i] = d[0];
        }

    }

    emlDevice_t* dev;
    const char* devname; 
    
    double consumed, elapsed;
    std::unordered_map<std::string, double> output;

    for(unsigned i = 0; i < count; i++){
        
        emlDataGetConsumed(data[i], &consumed);
        emlDataGetElapsed(data[i], &elapsed);
        emlDataFree(data[i]);

        if(eml_devices.empty()){
            emlDeviceByIndex(i, &dev);
        }else{
            dev = eml_devices[i];
        }
        emlDeviceGetName(dev, &devname);

        output[devname] = consumed/_conversion_factor;
    }

    eml_devices.clear();
    initialized = false;
    py::gil_scoped_acquire acquire;


    return output;
}


std::unordered_map<std::string, double> measureCodeInDevices(const char* code, const std::set<std::string>* devices, const double conversion_factor) {

    emlInit();

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

    py::exec(code , py::module_::import("__main__").attr("__dict__"));

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

std::unordered_map<std::string, double> measureCode(const char* code,  const double conversion_factor) {

    emlInit();

    size_t count;

    emlDeviceGetCount(&count);

    emlDevice_t* dev;
    const char* devname; 
    
    double consumed, elapsed;
    std::unordered_map<std::string, double> output;
  
    emlData_t* data[count];

    emlStart();

    py::exec(code , py::module_::import("__main__").attr("__dict__"));

    emlStop(data);

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

    // m.def("init", &start, "init eml", py::arg("devices") = static_cast<std::set<std::string> *>(nullptr), py::arg("conversion_factor") = 1);

    m.def("start", &start, "start eml", py::arg("devices") =  *(new std::set<std::string>), py::arg("conversion_factor") = 1);

    m.def("stop", &stop, "stop eml");

    m.def("measureCode", &measureCode, "A function to measure code energy consumption", py::arg("code"), py::arg("conversion_factor") = 1);
    m.def("measureCodeInDevices", &measureCodeInDevices, "A function to measure code energy consumption", py::arg("code"), py::arg("devices"), py::arg("conversion_factor") = 1);

    m.def("measureCodeAndTime", &measureCodeAndTime, "A function to measure code energy consumption", py::arg("code"), py::arg("preparationCode") = "");
}