#ifndef PCI_H
#define PCI_H

#include <string>
#include <vector>

enum GpuVendor {INTEL, AMD, NVIDIA, UNKNOWN};

struct GpuPCIinfo
{
	GpuVendor vendor;
	std::string bus_id;
	std::string driver;
	std::string driver_version;
};

std::vector<GpuPCIinfo> get_PCI_GPUs_list();

#endif
