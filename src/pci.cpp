#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <dirent.h>

#include "../include/pci.h"

#define DISPLAY_CONTROLLER_CLASS "0x03"
#define INTEL_ID  "0x8086"
#define AMD_ID    "0x1002"
#define NVIDIA_ID "0x10de"

std::vector<GpuPCIinfo> get_PCI_GPUs_list() {

	const std::string PCI_PATH = "/sys/bus/pci/devices/";

	auto dir = opendir(PCI_PATH.c_str());

	if(dir == NULL)
		throw std::runtime_error(("Cannot open " + PCI_PATH).c_str());

	std::vector<GpuPCIinfo> gpu_list;

	// Skipping . and ..
	auto subdir = readdir(dir);
	readdir(dir);

    while((subdir = readdir(dir)) != NULL)
    {
    	std::string subdir_name = std::string(subdir->d_name);

    	std::string vendor_id_path = PCI_PATH + "/" + subdir_name + "/vendor";
    	std::string class_path = PCI_PATH + "/" + subdir_name + "/class";

    	std::ifstream vendor_file(vendor_id_path);
    	std::ifstream class_file(class_path);

    	if(!vendor_file.is_open() || !class_file.is_open()) {
    		std::cout << "WARNING : cannot read vendor ID or class of " << subdir_name << ". Skipping this device." << std::endl;
    	} else {

    		std::string vendor_id;
    		std::string device_class;

    		std::getline(vendor_file, vendor_id);
    		std::getline(class_file, device_class);

    		if(device_class.substr(0, 4) == DISPLAY_CONTROLLER_CLASS) {

    			GpuPCIinfo gpu_info;

    			gpu_info.bus_id = subdir_name;

    			if(vendor_id == INTEL_ID)
    				gpu_info.vendor = INTEL;
    			else if(vendor_id == AMD_ID)
    				gpu_info.vendor = AMD;
    			else if(vendor_id == NVIDIA_ID)
    				gpu_info.vendor = NVIDIA;
    			else
    				gpu_info.vendor = UNKNOWN;

    			gpu_list.push_back(gpu_info);
    		}
    	}
    }

    return gpu_list;
}
