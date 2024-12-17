
// #define UART_TEST
#ifdef UART_TEST

#define UART0_TEST
//#define UART1_TEST
//#define UART2_TEST

const char uart_testString[] = "riscv uart0 test...\n";
// const char uart_testString[] = "UART\r\ntest123456789wtergfbrth24698yuudzcbreyhrfhfdgjtrgnbfadfhxfgjgkrtyjdghnsfghjdfgjkdrtwerysfghdjdjtjdfgjdyaertsryrtufyoiitjdhsdrtsryhrrjtdttruw54y7wrhw46373563w47387U";


void hw_uart_test (void)
{
	U32 i;
	volatile U32 val;
	U8 flag;
	volatile U32 tmp,tmp1,tmp2,tmp3,temp;
	int	ret;
	char *ptx,*prx;
	volatile U8 temp_u8;
    U32 test_err = 0;
    U32 debug_k;
	MISC_CLKGATE_CONTROLs *pGate;
	volatile UART_STATUSs *pUartStatus;


	//REG_MISC_SW_VER = TEST_SW_VERSION;

hw_uart_setup();

#ifdef 	UART0_TEST

	REG_MISC_STATUS = 0x00000000;

	uart_init( UART0,1,1 );
	uart_setParity(UART0, UART_PARITY_NON);
	uart_setBaudRate(UART0, DEFAULT_BAUD, PeriClock);
	uart_open( UART0,1,1 );

#if 1
	char str[32];
	ptx = uart_testString;
	uart_write(UART0, uart_testString, sizeof(uart_testString));
	// uart_read(UART0, str, sizeof(str));
	// uart_write(UART0, str, sizeof(str));

	return;
	while (1) {
		;
	}
#endif


	ptx = uart_testString;
	prx = uart_testString;
	REG_MISC_STATUS = 1;
	while(1)
	{
		val = REG_UART0_STATUS;
		if (pUartStatus->txBufFull)
			break;
		REG_UART0_TX_DATA = *ptx++;
	}
	REG_MISC_STATUS = 2;

	while (1)
	{
		val = REG_UART0_STATUS;
		if (pUartStatus->rxBufEmpty)
			break;

		temp_u8 = REG_UART0_RX_DATA;
		if (temp_u8 != *prx++) {
			test_err = 1;
			break;
		}
	}

	if (test_err == 0)
	{
		REG_MISC_STATUS = 3;
		while(1)
		{
			if (ptx < &uart_testString[sizeof(uart_testString)])
			{
				val = REG_UART0_STATUS;
				if (pUartStatus->txBufTFull_n)
					REG_UART0_TX_DATA = *ptx++;
			}
			else
			{
				val = REG_UART0_STATUS;
				if (pUartStatus->rxBufEmpty && prx >= &uart_testString[sizeof(uart_testString)])
				{
					REG_MISC_STATUS = temp_u8;
					break;
				}
			}

			val = REG_UART0_STATUS;
			if (pUartStatus->rxBufEmpty_n)
			{
				temp_u8 = REG_UART0_RX_DATA;
				if (temp_u8 != *prx++) {
					test_err = 1;
					break;
				}
			}
		}
	}

	if (test_err == 0)
	{
		REG_MISC_STATUS = 0x00000000;
	}
#endif

#ifdef 	UART1_TEST
	if (test_err == 0)
	{
		REG_MISC_STATUS = 0x11111111;

		uart_init(UART1,1,1 );
		uart_setParity(UART1, UART_PARITY_NON);
		uart_setBaudRate(UART1, 115200, PeriClock);
		uart_open( UART1,1,1 );

		#if 1
		ptx = uart_testString;
		pUartStatus = &REG_UART1_STATUS;
		while(1)
		{
			//val = REG_UART1_STATUS;
			while(pUartStatus->txBufFull);
			REG_UART1_TX_DATA = *ptx++;

			if 	(*ptx == 0)
				ptx = uart_testString;
		}
		#endif


		ptx = uart_testString;
		prx = uart_testString;
		REG_MISC_STATUS = 1;


		#if 0//aon_test


#if 1
	uart_init( UART0,1,1 );
	uart_setParity(UART0, UART_PARITY_NON);
	uart_setBaudRate(UART0, 115200, PeriClock);
	uart_open( UART0,1,1 );
	REG_UART0_TX_DATA = 0x55;

	REG_UART1_TX_DATA = 0x55;
	REG_MISC_STATUS = 0x1<<31|0x55;
	delayus(100);

	REG_UART0_TX_DATA = 0xff;
	REG_UART1_TX_DATA = 0xff;
	REG_MISC_STATUS = 0x1<<31|0xff;

	delayus(100);

	uart_setBaudRate(UART0, 38400, PeriClock);
	uart_setBaudRate(UART1, 38400, PeriClock);
	delayus(20);

	REG_UART0_TX_DATA = 0x55;

	REG_UART1_TX_DATA = 0x55;
	REG_MISC_STATUS = 0x3<<30|0x55;

	delayus(100);

	uart_setBaudRate(UART0, 19200, PeriClock);
	uart_setBaudRate(UART1, 19200, PeriClock);
	delayus(20);

	REG_UART0_TX_DATA = 0x55;

	REG_UART1_TX_DATA = 0x55;
	REG_MISC_STATUS = 0x2<<30|0x55;

#endif

#if 0
		//val = REG_UART1_STATUS;
		//while(!pUartStatus->txBufFull);
		REG_UART1_TX_DATA = 0xff;
		REG_MISC_STATUS = 0x1<<31|0xff;
		delayus(100);
		//val = REG_UART1_STATUS;
		//while(!pUartStatus->txBufFull);
		REG_UART1_TX_DATA = 0x55;
		REG_MISC_STATUS = 0x1<<31|0x55;

		delayus(500);

		uart_setBaudRate(UART1, 11520, PeriClock);

		delayus(20);

		//val = REG_UART1_STATUS;
		//while(!pUartStatus->txBufFull);
		REG_UART1_TX_DATA = 0x0;
		REG_MISC_STATUS = 0x3<<30|0x0;
		delayus(1000);
		//val = REG_UART1_STATUS;
		//while(!pUartStatus->txBufFull);
		REG_UART1_TX_DATA = 0x50;
		REG_MISC_STATUS = 0x3<<30|0x50;
#endif
		while(1)
		{
			while (1)
			{
				val = REG_UART1_STATUS;
				if (pUartStatus->rxBufEmpty_n)
					break;
			}


			temp_u8 = REG_UART1_RX_DATA;

			REG_MISC_STATUS = temp_u8;
		}

		asm ("waiti 1");
		while(1);
		#endif


		while(1)
		{
			val = REG_UART1_STATUS;
			if (pUartStatus->txBufFull)
				break;
			REG_UART1_TX_DATA = *ptx++;
		}
		REG_MISC_STATUS = 2;

		while (1)
		{
			val = REG_UART1_STATUS;
			if (pUartStatus->rxBufEmpty)
				break;

			temp_u8 = REG_UART1_RX_DATA;
			if (temp_u8 != *prx++) {
				test_err = 1;
				break;
			}
		}

		if (test_err == 0)
		{
			REG_MISC_STATUS = 3;
			while(1)
			{
			if (ptx < &uart_testString[sizeof(uart_testString)])
			{
				val = REG_UART1_STATUS;
				if (pUartStatus->txBufTFull_n)
					REG_UART1_TX_DATA = *ptx++;
			}
			else
			{
				val = REG_UART1_STATUS;
				if (pUartStatus->rxBufEmpty && prx >= &uart_testString[sizeof(uart_testString)])
				{
					REG_MISC_STATUS = temp_u8;
					break;
				}
			}

				val = REG_UART1_STATUS;
				if (pUartStatus->rxBufEmpty_n)
				{
					temp_u8 = REG_UART1_RX_DATA;
					if (temp_u8 != *prx++) {
						test_err = 1;
						break;
					}
				}
			}
		}
	}
	if (test_err == 0)
	{
		REG_MISC_STATUS = 0x11111111;
	}
#endif

#ifdef 	UART2_TEST
	if (test_err == 0)
	{
		REG_MISC_STATUS = 0x22222222;

		uart_init(UART2,1,1 );
		uart_setParity(UART2, UART_PARITY_NON);
		uart_setBaudRate(UART2, DEFAULT_BAUD, PeriClock);
		uart_open( UART2,1,1 );

		ptx = uart_testString;
		prx = uart_testString;
		REG_MISC_STATUS = 1;
		while(1)
		{
			val = REG_UART2_STATUS;
			if (pUartStatus->txBufFull)
				break;
			REG_UART2_TX_DATA = *ptx++;
		}
		REG_MISC_STATUS = 2;

		while (1)
		{
			val = REG_UART2_STATUS;
			if (pUartStatus->rxBufEmpty)
				break;

			temp_u8 = REG_UART2_RX_DATA;
			if (temp_u8 != *prx++) {
				test_err = 1;
				break;
			}
		}

		if (test_err == 0)
		{
			REG_MISC_STATUS = 3;
			while(1)
			{
				if (ptx < &uart_testString[sizeof(uart_testString)])
				{
					val = REG_UART2_STATUS;
					if (pUartStatus->txBufTFull_n)
						REG_UART2_TX_DATA = *ptx++;
				}
				else
				{
					val = REG_UART2_STATUS;
					if (pUartStatus->rxBufEmpty && prx >= &uart_testString[sizeof(uart_testString)])
					{
						REG_MISC_STATUS = temp_u8;
						break;
					}
				}

				val = REG_UART2_STATUS;
				if (pUartStatus->rxBufEmpty_n)
				{
					temp_u8 = REG_UART2_RX_DATA;
					if (temp_u8 != *prx++) {
						test_err = 1;
						break;
					}
				}
			}
		}
	}
	if (test_err == 0)
	{
		REG_MISC_STATUS = 0x22222222;
	}
#endif

    if (test_err == 0)
		REG_MISC_STATUS = TEST_SUCCESFUL;
	else
		REG_MISC_STATUS = TEST_FAILED;

	asm ("WFI");
	while(1);

}


#endif
