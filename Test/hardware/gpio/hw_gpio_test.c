
void ctest_delay(uint32_t ms);
//simple test,only output test
void hw_gpio_test1(void)
{
	int i;
    int32_t returnCode;

	REG_MISC_SW_VER = TEST_SW_VERSION;

#if 0
    // inital timer interrupt as non-vector interrupt
    returnCode = ECLIC_Register_IRQ(SOC_INT19_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 0, NULL);

    returnCode = ECLIC_Register_IRQ(SOC_INT26_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 0, NULL);


    REG_MISC_STATUS = returnCode;

    // Enable interrupts in general.
    __enable_irq();
#endif

#if 1//output
    REG_GPIO0_DOUT = 0;
	REG_GPIO0_DOE = 0xfffffffe;
	REG_GPIO0_IE  = 0x00000000;

    while(1)
    {
        REG_GPIO0_DOUT = 0;
        //delaynop(1000);
        ctest_delay(40);
        REG_GPIO0_DOUT = 0xffffffff;
        //delaynop(1000);
        ctest_delay(100);
    }

    // LPclose
	// for (i=0; i<32; i++)
	// {
    //     REG_MISC_STATUS = i<<16;
	// 	REG_GPIO0_DOUT = (0x1<<i);
	// 	delaynop(1);
	// }
	// REG_MISC_STATUS = TEST_SUCCESFUL;
    // while(1);

	// while(i++){
    //     REG_GPIO0_DOUT = (0x1<<2);
    //     delaynop(1000);
    //     REG_GPIO0_DOUT = 0;
    //     delaynop(10);
    //     REG_MISC_STATUS = i<<16;
    // }
#else
    REG_GPIO0_DOE = (0x1<<2);
    REG_GPIO0_DOE |= (0x1<<4);
#if 0
    REG_GPIO0_IE  = (0x1<<3);

    REG_GPIO0_INTTP0 = 0;//edge
    REG_GPIO0_INTTP1 = 1<<3;//up
    REG_GPIO0_INTST |= (1<<3);//clear
    REG_GPIO0_INTEN |= (1<<3);//ena
#endif

    uint32_t val;

    REG_CORE_2_INT_MASK = 0xffffffff;
    while(1)
    {
        /*
        if (REG_GPIO0_DIN & (0x1<<3))
        {
            REG_GPIO0_DOUT = (1<<2);
        }
        else
        {
            REG_GPIO0_DOUT = 0;
        }
        */
       /*
        val = DMA_GCNTR;
        REG_MISC_STATUS = val;
        if ((val & (0x1<<4)) == (0x1<<4))
        {
            DMA_GCNTR = (0x1000 | (0x1<<4));
            REG_GPIO0_DOUT = (1<<2);
            delaynop(10);
            REG_GPIO0_DOUT = 0;
        }
        */
/*
        val = REG_GPIO0_INTST;
        REG_MISC_STATUS = val;
        if ((val & (0x1<<3)) == (0x1<<3))
        {
            REG_GPIO0_INTST |= (0x1<<3);
            REG_GPIO0_DOUT = (1<<2);
            delaynop(1);
            REG_GPIO0_DOUT = 0;
        }*/

        REG_MISC_STATUS = gpio_int_cnt;
    }
#endif
}

void hw_gpio_test2(void) {

#if 0
    uint32_t rval, seed;
    unsigned long hartid;
    rv_csr_t misa;

    hartid = __RV_CSR_READ(CSR_MHARTID);
    misa = __RV_CSR_READ(CSR_MISA);

    printf("Hart %d, MISA: 0x%lx\r\n", hartid, misa);
    print_misa();

    // Generate random value with seed
    seed = (uint32_t)(__get_rv_cycle()  | __get_rv_instret() | __RV_CSR_READ(CSR_MCYCLE));
    srand(seed);#define	REG(addr)				(*(volatile U32 *)(addr))
    printf("Got rand integer %d using seed %d.\r\n", seed, rval);

    for (unsigned long i = 0; i < RUN_LOOPS; i ++) {
        printf("%d: Hello World From Nuclei RISC-V Processor!\r\n", i);
    }

#else

    volatile uint32_t loopi, outdata, flashdata;
    uint32_t val = 0;

    REG_GPIO0_DOE = 0xffffffff;
    REG_GPIO0_DOUT = 0;

    uint32_t loop = 10;
    while (loop--)
    {
#if 1//gpio test
        REG_GPIO0_DOUT = 0x1<<0;
        delaynop(1);
        REG_GPIO0_DOUT = 0;
        delaynop(1);

        REG_MISC_STATUS = loop;
#endif
#if 0
    	uint8_t err = 0;
    	for (loopi=0; loopi<32; loopi++)
    	{
#if 0
    		*(pwdata+loopi) = sampledata[loopi];

    		if (sampledata[loopi] != *(pwdata+loopi))
    				err = 1;
#endif
    		//printf("Hello World %d\r\n", outdata);
            REG_MISC_STATUS = loopi;
    	}

    	if (!err)
    		printf("successed!!\r\n", outdata);
    	else
    		printf("failed\r\n");

    	//read flash data incrementally.
    	flashdata = *(prflashdata++);
#endif
    }
#endif
}