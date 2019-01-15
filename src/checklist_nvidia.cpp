#include <iostream>
#include <string>
#include <regex>

#include "../include/checklist_nvidia.h"
#include "../include/gl_info.h"

ChecklistInfo _check_opengl_version_string(ChecklistInfo checklist_info, std::vector<ChecklistItem> checklist_items) {

	std::vector<std::string> gl_info_strings = get_opengl_info();

	if(gl_info_strings.empty())
		checklist_items.push_back({ITEM_CRITICAL, "Failed to run OpenGL test"});
	else {

		const std::string vendor = gl_info_strings[1];

		if (checklist_info.pci_gpu_info.driver == "nvidia") {
			if (vendor != "NVIDIA")
				checklist_items.push_back({ITEM_CRITICAL, "OpenGL is not using Nvidia"});
		} else {

			if (vendor != "Mesa")
				checklist_items.push_back({ITEM_CRITICAL, "Using nouveau but OpenGL vendor is not Mesa"});
			else
				checklist_info.mesa_version = gl_info_strings[2];
		}
	}

	return checklist_info;
}


void run_checklist_nvidia(ChecklistInfo checklist_info, std::vector<ChecklistItem> checklist_items) {

	if(checklist_info.pci_gpu_info.driver == "nvidia")
		checklist_items.push_back({ITEM_OK, "Using the proprietary \"nvidia\" driver"});
	else if(checklist_info.pci_gpu_info.driver == "nouveau")
		checklist_items.push_back({ITEM_CRITICAL, "Using the open-source \"nouveau\" driver"});

	checklist_info = _check_opengl_version_string(checklist_info, checklist_items);

}