#include <stdio.h>
struct DMA_cfg_opt1 {
  unsigned long link : 1;       //[0] 1 bit for the link parameters enable.
  unsigned long ts : 1;         //[1] 1 bit for the block synchronization. 
                                        //1 means transfer the data block. 
                                        //0 means transfer the data row.
  unsigned long tcint : 1;      //[2] 1 bit for the transfer end interrupt enable.
                                        //1 means generate interrupt after the end of transfer.
  unsigned long tcc : 5;        //[3-7] 5 bits for the transfer end interrupt code.
  unsigned long tmode : 2;      //[8-9] 2 bits for the transfer mode.
                                        //00 means point to point transfer.
                                        //01 reserved.
                                        //10 means subsection transfer.
                                        //11 means broadcast.
  unsigned  long : 2;            //[10-11] 2 bits unused.
  unsigned  long samode : 2;     //[12-13] 2 bits for the src addr modified mode.
                                        //00 means not change.
                                        //01 means use the srcIndex as the addr step value.
                                                //If the srcIndex == 0, the addr step value = 1.
  unsigned long damode : 2;     //[14-15] 2 bits for the dst addr modified mode.
  unsigned long semaphore : 1;   //[16] 1 bit for the synchronization enable between cores.
  unsigned long : 3;             //[17-19] 3 bits unused.
  unsigned long ptpmask : 24;    //[20-43] 24 bits for the synchronization cores mask.
  unsigned long : 4;             //[44-47] 4 bits unused.
  unsigned long : 4;            //[48-51] 4 bits unused.
  unsigned long light: 1;       //[52] 1 bit for the p2p transfer before an block subsection.
                                        //In the p2p transfer before the block subsection, need set this bit to 1.
  unsigned long : 3;            //[53-55] 3 bits unused.
  unsigned long link_addr : 6;  //[56-61] 6 bits for the link parameters addr [6-11] bit.
                                        //This link parameter can be the logical channelNo or parameters group ID.
  unsigned long : 2;            //[62-63] 2 bits unused.
};

struct DMA_cfg_opt2 {
  unsigned long bcnt : 16;      //[0-15] The data granularity of subsection, the unit is 64 bit.
  unsigned long bshift : 5;     //[16-20] 5 bits for the shift times.
  unsigned long : 3;            //[21-23] 3 bits unused.
  unsigned long bmode : 24;      //[24-47] 24 bits for the core id which will accept the first subsection.
  unsigned long : 8;             //[48-55] 8 bits unused.
  unsigned long bstep : 5;      //[56-60] 5 bits for the core id shift step.
  unsigned long : 2;            //[61-62] 2 bits unused.
  unsigned long blocking_mode : 1; //[63] 1 bit for the blocking subsection enable.
};


struct test {
  int a = 1;
  int b = 2;
} tt;

static struct test tt2 = { .b = 0};
//static struct test tt3 = { b = 0}; Compile Error!

void t_add () {
  tt.b = tt.a + tt.b;
}

int main (int argc, char ** argv) {
  tt.a = 3;
  tt.b = tt.a;
  t_add();
  printf("Result = %d\n", tt.b);
  printf("Result = %d\n", tt2.b);


        struct DMA_cfg_opt1 dma_opt1;
        struct DMA_cfg_opt2 dma_opt2;
        struct DMA_cfg_opt2 tmp;
        //Do not nedd to change.
        dma_opt1.link = 0;
        dma_opt1.ts = 1;
        dma_opt1.tcc = 0;
        dma_opt1.semaphore  = 0;
        dma_opt1.ptpmask  = 0;
        dma_opt1.light = 0;
        dma_opt1.link_addr  = 0;
          dma_opt1.tcint = 0;
          dma_opt1.tmode = 2;
          dma_opt1.samode = 1;
          dma_opt1.damode = 1;

          dma_opt2.bcnt = argc;
          dma_opt2.bshift  = 24 - 1;
          dma_opt2.bmode  = 1;
          dma_opt2.bstep  = 1;
          dma_opt2.blocking_mode  = 0;

 	tmp = dma_opt2;
	printf("opt1: %d\n opt2:%d\n",sizeof (struct DMA_cfg_opt1), sizeof (struct DMA_cfg_opt2));
	printf("%ld\n%x\n", *(unsigned long *)&dma_opt1, *((unsigned long *)&dma_opt2));

  	printf("%ld tmp.bshift = %d\n", *(unsigned long *)&tmp, tmp.bshift);

  return 0;
}
