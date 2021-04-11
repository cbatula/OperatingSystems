#pragma once

#include "Process.h"

class Page
{
public:
	ProcPtr owner;
	size_t owner_page_id;
	size_t page_id;

	Page();
	Page(ProcPtr owner, size_t owner_page_id, size_t page_id);

	bool present() const;
};

typedef std::shared_ptr<Page> PagePtr;
