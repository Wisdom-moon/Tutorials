#include <stdio.h>

#define  BSCR0       0x04001DFF80 
#define  INNER_CORE_ADDR_OFFSET 0x08000000

struct DMA_bscr {
  unsigned long wordNum : 20;   //[0-19] 20 bits for the number of need recived word.
  unsigned long : 12;           //[20-31] 12 bits unused.
  unsigned long coreID : 2;     //[32-33] 2 bits for the Core id in SN.
  unsigned long hostID : 6;     //[34-39] 6 bits for the DSP SN id.
  unsigned long : 23;           //[40-62] 23 bits unused.
  unsigned long ipt : 1;        //[63] 1 bit for enable multicast/broadcast interrupt.
};

int main (int argc, char **argv)
{
  unsigned int core_ID =  atoi (argv[1]);
  unsigned int bscrnum,bscrId,hostId,bscrEnable;
  bscrnum= 16;
  bscrId= 0x00;
  hostId= 0x00;
  bscrEnable= 0x0;

  struct DMA_bscr dma_bscr;
  dma_bscr.wordNum = 16;
  dma_bscr.coreID = 0;
  dma_bscr.hostID = 0;
  dma_bscr.ipt = 0;

  
  unsigned long * bscr_addr = BSCR0 + core_ID * INNER_CORE_ADDR_OFFSET;
  unsigned long bscr = (0x7ffff&bscrnum)|(bscrId<<32)|(bscrEnable<<63)|(hostId<<34);

  printf("Addr: %lx\n", bscr_addr);
  printf("Value: %lx\n", bscr);
  printf("Struct Value: %lx\n", *(unsigned long *)&dma_bscr);
}
