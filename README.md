# linux_rdmsr_perf

# do microcode patch by write MSR, or by bios when reset, or by fuse
## Update a block of microcode instructions at one time
# real mode
EAX=Physical address of the update block
ECX=0x79 (IA32_BIOS_UPDT_TRIG)
# 32-bit protect mode
EAX=Linear address of the update block
ECX=0x79 (IA32_BIOS_UPDT_TRIG)
# 64-bit protect mode
EDX:EAX=Linear address of the update block
ECX=0x79 (IA32_BIOS_UPDT_TRIG)

(1)get a kernel va by buffer = __get_free_pages(GFA_DMA|GFP_ATOMC, order) by microcode patch size in the driver
(2)set kernel va page reserved
cnt = 1<<order;
for(page = vir_to_page(buffer), i = 0; i < cnt; page++, i++)
{
  SetPageReserves(page);
}
(3) define driver mmap, use the kernel va about pa, remap userspace vma, so kernel va and userspace va direct same pa
remap_pfn_range(vma, vma->vm_start,__pa(buffer)>>PAGE_SHIFT,vma->vm_end - vma->vm_start, vma->vm_page_port)
(4) userspace mmap driver file and get userspace va, write userspace va microcode data, so kernel space buffer is fill microcode data
(5)kernel space wrmsrl(0x79, buffer)

## Update only one microcode instruction each time
The patch data is not encrypted. Following registers should be configured before executing WRMSR instruction.
ECX: should be 0x317D
EDX:EAX: contains the microcode instruction in specific format.
