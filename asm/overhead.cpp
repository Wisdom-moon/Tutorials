#define rdpmc(counter,low,high) \
     __asm__ __volatile__("rdpmc" \
        : "=a" (low), "=d" (high) \
        : "c" (counter))

    for (j=0; j<NSAMPLES; j++) values64[j] = 0;  // make sure array is in cache

    for (j=0; j<NSAMPLES; j+=8) {
          rdpmc(fixed0, low, high);
          values64[j] = ((unsigned long) high << 32) + (unsigned long) low;
          rdpmc(fixed0, low, high);
          values64[j+1] = ((unsigned long) high << 32) + (unsigned long) low;
          rdpmc(fixed0, low, high);
          values64[j+2] = ((unsigned long) high << 32) + (unsigned long) low;
          rdpmc(fixed0, low, high);
          values64[j+3] = ((unsigned long) high << 32) + (unsigned long) low;
          rdpmc(fixed0, low, high);
          values64[j+4] = ((unsigned long) high << 32) + (unsigned long) low;
          rdpmc(fixed0, low, high);
          values64[j+5] = ((unsigned long) high << 32) + (unsigned long) low;
          rdpmc(fixed0, low, high);
          values64[j+6] = ((unsigned long) high << 32) + (unsigned long) low;
          rdpmc(fixed0, low, high);
          values64[j+7] = ((unsigned long) high << 32) + (unsigned long) low;
      }
