#ifndef CHECKLIST_INFO_H
#define CHECKLIST_INFO_H

#include <string>

#include "../include/pci.h"

struct ChecklistInfo
{
	GpuPCIinfo pci_gpu_info;
	std::string mesa_version;
};

std::vector<GpuPCIinfo> get_PCI_GPUs_list();

#endif 
