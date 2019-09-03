打印物理地址形如：
    unsigned long phy = 0;
    if (0 == mem_addr_vir2phy ((unsigned long)nodePtr, &phy)) {
        printf ("nodeptr 0x%x, phy 0x%x\n", nodePtr, phy);
    }