/*
  NoirVisor - Hardware-Accelerated Hypervisor solution

  Copyright 2018, Zero Tang. All rights reserved.

  This file is the central HyperVisor of NoirVisor.

  This program is distributed in the hope that it will be useful, but 
  without any warranty (no matter implied warranty or merchantability
  or fitness for a particular purpose, etc.).

  File Location: /xpf_core/noirhvm.c
*/

#include <nvdef.h>
#include <nvbdk.h>
#include <nvstatus.h>
#include <noirhvm.h>
#include <intrin.h>

u32 noir_visor_version()
{
	u16 major=1;
	u16 minor=0;
	return major<<16|minor;
}

void noir_get_vendor_string(char* vendor_string)
{
	noir_cpuid(0,0,null,(u32*)&vendor_string[0],(u32*)&vendor_string[8],(u32*)&vendor_string[4]);
}

void noir_get_processor_name(char* processor_name)
{
	noir_cpuid(0x80000002,0,(u32*)&processor_name[0x0],(u32*)&processor_name[0x4],(u32*)&processor_name[0x8],(u32*)&processor_name[0xC]);
	noir_cpuid(0x80000003,0,(u32*)&processor_name[0x10],(u32*)&processor_name[0x14],(u32*)&processor_name[0x18],(u32*)&processor_name[0x1C]);
	noir_cpuid(0x80000004,0,(u32*)&processor_name[0x20],(u32*)&processor_name[0x24],(u32*)&processor_name[0x28],(u32*)&processor_name[0x2C]);
}

bool noir_is_under_hvm()
{
	u32 c=0;
	//cpuid[eax=1].ecx[bit 31] could indicate hypervisor's presence.
	noir_cpuid(1,0,null,null,&c,null);
	//If it is read as one. There must be a hypervisor.
	//Otherwise, it is inconclusive to determine the presence.
	return noir_bt(&c,31);
}

noir_status nvc_build_hypervisor()
{
	hvm_p=noir_alloc_nonpg_memory(sizeof(noir_hypervisor));
	if(hvm_p)
	{
		u32 m=0;
		noir_cpuid(0,0,&m,(u32*)&hvm_p->vendor_string[0],(u32*)&hvm_p->vendor_string[8],(u32*)&hvm_p->vendor_string[4]);
		hvm_p->vendor_string[12]=0;
		if(strcmp(hvm_p->vendor_string,"GenuineIntel")==0)
			hvm_p->cpu_manuf=intel_processor;
		else if(strcmp(hvm_p->vendor_string,"AuthenticAMD")==0)
			hvm_p->cpu_manuf=amd_processor;
		else
			hvm_p->cpu_manuf=unknown_processor;
		nvc_store_image_info(&hvm_p->hv_image.base,&hvm_p->hv_image.size);
		switch(hvm_p->cpu_manuf)
		{
			case intel_processor:
			{
				if(nvc_is_vt_supported())
				{
					nv_dprintf("Starting subversion with VMX Engine!\n");
					return nvc_vt_subvert_system(hvm_p);
				}
				else
				{
					nv_dprintf("Your processor does not support Intel VT-x!\n");
					return noir_vmx_not_supported;
				}
				break;
			}
			case amd_processor:
			{
				if(nvc_is_svm_supported())
				{
					nv_dprintf("Starting subversion with SVM Engine!\n");
					return nvc_svm_subvert_system(hvm_p);
				}
				else
				{
					nv_dprintf("Your processor does not support AMD-V!\n");
					return noir_svm_not_supported;
				}
				break;
			}
			default:
			{
				nv_dprintf("Unknown Processor!\n");
				break;
			}
		}
	}
	return noir_insufficient_resources;
}

void nvc_teardown_hypervisor()
{
	if(hvm_p)
	{
		switch(hvm_p->cpu_manuf)
		{
			case intel_processor:
			{
				nvc_vt_restore_system(hvm_p);
				break;
			}
			case amd_processor:
			{
				nvc_svm_restore_system(hvm_p);
				break;
			}
			default:
			{
				nv_dprintf("Unknown Processor!\n");
				break;
			}
		}
		noir_free_nonpg_memory(hvm_p);
	}
}