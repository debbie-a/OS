
#include "os.h"

void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn)
{
	uint64_t pt2=pt<<12,offs,vpn2;
	uint64_t *ptr;
	if(ppn==NO_MAPPING)
	{	
		for(int i=4;i>=0;i--)
		{
			vpn2=(vpn>>(i*9));
			offs=(vpn2&0x1ff)*8;
			ptr=(uint64_t*)(phys_to_virt(pt2)+offs);
			pt2=*ptr;
			if(!((*ptr)&0x1))
			{
				return;
			}
		}
		(*ptr)&=~0x1;
	}
	else
	{
		for(int i=4;i>=0;i--)
		{
			vpn2=(vpn>>(i*9));
			offs=(vpn2&0x1ff)*8;
			ptr=(uint64_t*)(phys_to_virt(pt2)+offs);
			if(!((*ptr)&0x1))
			{
				if(i)
				{	
					(*ptr)=(alloc_page_frame()<<12);
					(*ptr)^=0x1;					 
				}
				
			}
			pt2=*ptr; 
			
		}
		(*ptr)=(ppn<<12);
		(*ptr)^=0x1;
	} 
}
uint64_t page_table_query(uint64_t pt, uint64_t vpn)
{
	uint64_t pt2=pt<<12,offs,vpn2;
	uint64_t *ptr;
	for(int i=4;i>=0;i--)
	{
		vpn2=(vpn>>(i*9));
		offs=(vpn2&0x1ff)*8;
		ptr=(uint64_t*)(phys_to_virt(pt2)+offs);
		if(!((*ptr)&0x1))
		{
			return NO_MAPPING;
		}
		pt2=*ptr;
	}
	return ((*ptr)>>12);
}

