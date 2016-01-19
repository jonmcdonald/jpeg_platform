
#pragma once

void pagetable_init();
void enable_mmu();

void page_set_cacheable(unsigned long addr, int enabled);

#endif
