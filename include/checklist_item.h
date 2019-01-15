#ifndef CHECKLIST_ITEM_H
#define CHECKLIST_ITEM_H

#include <string>

enum ItemSeverity {ITEM_OK, ITEM_INFO, ITEM_WARNING, ITEM_CRITICAL};

struct ChecklistItem
{
	ItemSeverity severity;
	std::string text;
};

#endif  
