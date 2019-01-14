#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstdlib>
#include <dirent.h>

#include "../include/pci.h"

#define DISPLAY_CONTROLLER_CLASS "0x03"
#define INTEL_ID  "0x8086"
#define AMD_ID    "0x1002"
#define NVIDIA_ID "0x10de"

std::string _get_path_last(std::string path) {

    if (path.back() == '/')
        path.erase(path.end()-1, path.end());

    std::size_t pos = path.find_last_of("/");

    if (pos != std::string::npos)
        return path.substr(pos+1);
    else
        return path;
}

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

    	const std::string vendor_id_path = PCI_PATH + "/" + subdir_name + "/vendor";
    	const std::string class_path = PCI_PATH + "/" + subdir_name + "/class";

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

                // Get GPU vendor
    			if(vendor_id == INTEL_ID)
    				gpu_info.vendor = INTEL;
    			else if(vendor_id == AMD_ID)
    				gpu_info.vendor = AMD;
    			else if(vendor_id == NVIDIA_ID)
    				gpu_info.vendor = NVIDIA;
    			else
    				gpu_info.vendor = UNKNOWN;

                // Get driver
                char *driver_path_c_str = realpath((PCI_PATH + "/" + subdir_name + "/driver").c_str(), NULL);

                if (driver_path_c_str == NULL) {

                    std::cout << "WARNING : cannot read driver name of " << subdir_name << "." << std::endl;

                } else {

                    const std::string driver_path(driver_path_c_str);
                    gpu_info.driver = _get_path_last(driver_path);

                    const std::string driver_version_path = PCI_PATH + "/" + subdir_name + "/driver/module/version";
                    std::ifstream driver_version_file(driver_version_path);

                    if(!driver_version_file.is_open()) {

                        std::cout << "WARNING : cannot read driver version of " << subdir_name << "." << std::endl;

                    } else {

                        std::string driver_version;
                        std::getline(driver_version_file, driver_version);

                        gpu_info.driver_version = driver_version;
                    }

                }


    			gpu_list.push_back(gpu_info);
    		}
    	}
    }

    return gpu_list;
}
