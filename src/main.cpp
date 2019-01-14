#include <iostream>
#include <string>
#include <map>
#include "../include/pci.h"
#include "../include/gl_info.h"


int main() {

	std::map<GpuVendor, std::string> VENDOR_NAME_MAP = {{INTEL, "Intel"}, {AMD, "AMD"}, {NVIDIA, "Nvidia"}};

	std::vector<GpuPCIinfo> gpu_list = get_PCI_GPUs_list();

	std::cout << std::endl << "Found " << gpu_list.size() << " GPUs." << std::endl << std::endl;

	for(auto const& pci_gpu: gpu_list) {
		const std::string name = VENDOR_NAME_MAP[pci_gpu.vendor];
		std::cout << name << " at " << pci_gpu.bus_id << std::endl
				  << "\t" << "Driver : " << pci_gpu.driver << std::endl
				  << "\t" << "Driver version : " << pci_gpu.driver_version << std::endl << std::endl;
	}

	std::cout << "OpenGL version string : " << get_opengl_info() << std::endl;

	return EXIT_SUCCESS;
} 
