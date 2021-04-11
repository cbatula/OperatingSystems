#include "Page.h"

Page::Page()
{
}

Page::Page(ProcPtr owner, size_t owner_page_id, size_t page_id)
	: owner(owner), owner_page_id(owner_page_id), page_id(page_id)
{
}

bool Page::present() const
{
	return owner != nullptr;
}
