#ifndef ADDR2_H
#define ADDR2_H

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <math.h>

#define    page_map_file     "/proc/self/pagemap"
#define    self_map_file	 "/proc/self/maps"
#define    PFN_MASK          ((((uint64_t)1)<<55)-1)
#define    PFN_PRESENT_FLAG  (((uint64_t)1)<<63)


int mem_addr_vir2phy (unsigned long vir, unsigned long *phy);
long hex (long inputhex);
void transaddr (int *a, int arraylength, int typelength, char name[]);


int mem_addr_vir2phy (unsigned long vir, unsigned long *phy)
{
    int fd;
    int page_size = getpagesize ();  //4096
    unsigned long vir_page_idx = vir / page_size;  //page_index    which page vir located on
    unsigned long pfn_item_offset = vir_page_idx*sizeof (uint64_t); //uint64_t ----> 8
    uint64_t pfn_item;


    //	printf("vir = %ld &&&&& page_idx = %ld\t",vir,vir_page_idx);


    fd = open (page_map_file, O_RDONLY);
    if (fd < 0)
    {
        printf ("open %s failed", page_map_file);
        return -1;
    }


    if ((off_t)-1 == lseek (fd, pfn_item_offset, SEEK_SET))
    {
        printf ("lseek %s failed", page_map_file);
        return -1;
    }


    if (sizeof (uint64_t) != read (fd, &pfn_item, sizeof (uint64_t)))
    {
        printf ("read %s failed", page_map_file);
        return -1;
    }


    if (0 == (pfn_item & PFN_PRESENT_FLAG))
    {
        printf ("page is not present\t");
        return -1;
    }

    //print_self_maps();
    *phy = (pfn_item & PFN_MASK)*page_size + vir % page_size;
    close (fd);
    return 0;
}


long hex (long inputhex)
{
    long a = inputhex;
    long sum = 0;
    int str[20] = { 0 };
    int j = 0, i = 0;
    while (a)
    {
        str[j] = a % 10;
        a /= 10;
        j++;
    }
    for (i = j - 1; i > 0; i--)
    {
        sum += str[i] * pow (10, i);
    }
    sum += str[0];
    //printf("sum = %ld\n",sum);
    return sum;
}


void transaddr (int *p, int arraylength, int typelength, char name[])
{
    memset (p, 1, arraylength);
    mlock (p, arraylength);

    if (p)
        printf ("Memory (Vaddr) Allocated at: %lx\n", p);
    else
    {
        printf ("No Input Memory!\n"); return;
    }
    long dp = hex ((unsigned long)p);
    //printf("\ndp = %ld\n",dp);
    unsigned long phy;
    int i;
    //for(i=0;i<(arraylength/4096+1)*4096+1;i+=4096)
    for (i = 0; i < (arraylength / 4096 + 1) * 4096; i += 4096)
    {
        mem_addr_vir2phy (dp + i, &phy);
        if (i == 0 && arraylength <= 4096)
            printf ("%s[%d-%d],Vaddr=%p,phy=0x%lx\n", name, 0, arraylength / typelength - 1, dp + i, phy);
        else if (i == 0 && arraylength>4096)
            printf ("%s[%d-%d],Vaddr=%p,phy=0x%lx\n", name, 0, 4096 / typelength - 1, dp + i, phy);
        else if (arraylength > 2 * 4096)
            printf ("%s[%d-%d],Vaddr=%p,phy=0x%lx\n", name, i / typelength, i / typelength + 511, dp + i, phy);
        else
            printf ("%s[%d-%d],Vaddr=%p,phy=0x%lx\n", name, i / typelength, arraylength / typelength - 1, dp + i, phy);
    }
}


# define M_PCM_CPUFREQ 2400 /* GHz */

#define NS2CYCLE(__ns) ((__ns) * M_PCM_CPUFREQ / 1000)
#define CYCLE2NS(__cycles) ((__cycles) * 1000 / M_PCM_CPUFREQ)

static inline unsigned long long asm_rdtsc (void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((unsigned long long)lo) | (((unsigned long long)hi) << 32);
}

typedef unsigned long pcm_hrtime_t;

#endif /* ADDR2_H */
