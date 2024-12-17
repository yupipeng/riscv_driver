#include "fifo1.h"

void fifo_init ( FIFO *pFifo, U8 *pBuf, unsigned long len, int empty)
{
	pFifo->valid = FALSE;
	pFifo->fifo_start_addr = pBuf;
	pFifo->fifo_end_addr = pBuf + len;
	pFifo->fifo_length = len;
	pFifo->pRead = pFifo->fifo_start_addr;
	pFifo->pWrite = pFifo->fifo_start_addr;
	pFifo->wlen_H = 0;
	
	if (empty)
	{
		pFifo->wLen = 0;
		pFifo->rLen = 0;
	}
	else
	{
		pFifo->wLen = pFifo->fifo_length;
		pFifo->rLen = 0;
	}

	pFifo->start_pos = 0;
	pFifo->stream_len = STREAM_LENTH_UNKNOWN;
	pFifo->entry = 0;
	pFifo->stream_margin = 0;
}

void fifo_init_w_data ( FIFO *pFifo, U8 *pBuf, unsigned long len, unsigned long dlen)
{
	if (dlen > len) {
		dlen = 0;
		DBG_Assert (FALSE);
	}
	
	pFifo->valid = FALSE;
	pFifo->fifo_start_addr = pBuf;
	pFifo->fifo_end_addr = pBuf + len;
	pFifo->fifo_length = len;
	pFifo->pRead = pFifo->fifo_start_addr;
	pFifo->pWrite = pFifo->fifo_start_addr;
	pFifo->wlen_H = 0;


	memset (pBuf, 0, dlen);
	
	if (dlen > 0) {
		pFifo->wLen = dlen;
		pFifo->pWrite = pFifo->fifo_start_addr + dlen;
		pFifo->rLen = 0;
	}
	else {
		pFifo->wLen = 0;
		pFifo->rLen = 0;
	}

	pFifo->start_pos = 0;
	pFifo->stream_len = STREAM_LENTH_UNKNOWN;
	pFifo->entry = 0;
	pFifo->stream_margin = 0;
}

long fifo_get_data_len (FIFO *pFifo)
{
	if (pFifo->wLen < pFifo->rLen)
	{
		return ( pFifo->wLen + ((0x100000000 - pFifo->rLen)) );
	}
	else
	{
		return (pFifo->wLen - pFifo->rLen);
	}
}

long fifo_get_free_space (FIFO *pFifo)
{
	int data_len = fifo_get_data_len(pFifo);

	return (pFifo->fifo_length - data_len - pFifo->stream_margin);
}

long fifo_get_fifo_data_wlen (FIFO *pFifo)
{
	if (pFifo->wlen_H > 0x0)
	{
		return 0xFFFFFFFF;
	}
	else 
	{
		return pFifo->wLen;
	}
}	

long fifo_put_data (FIFO *pFifo, U8 *pBuf, unsigned long len)
{
	unsigned long l;
	unsigned long pre_wlen;

	/*
	//def WAV_DEC//for special wave files
	if ( pFifo->wLen > pFifo->stream_len )
	{
		if ( ((global_vari.audio_source == SOURCE_USB_MSC) || 
			(global_vari.audio_source == MEDIA_SDMMC)) && 
			(app_media_data.playing_stream_type == STREAM_WAV) )
		{
			return 0;
		}
	}
	*/



	if (pFifo->stream_len != STREAM_LENTH_UNKNOWN) 
	{
		l = (pFifo->stream_len - pFifo->wLen);
		if (len > l) 
		{
			len = l;
		}
	}

	if (pFifo->wLen < pFifo->rLen)
	{
		l = pFifo->wLen + ((0x100000000 - pFifo->rLen));
	}
	else
	{
		l = pFifo->wLen - pFifo->rLen;
	}
	

	if (len > (pFifo->fifo_length - l - pFifo->stream_margin))
	{
		return 0;//overflow
	}

	
	pre_wlen = pFifo->wLen;

	if (len == 0)
	{
		return 0;
	}

	if ( (pFifo->pWrite + len) > pFifo->fifo_end_addr)
	{	
		l = pFifo->fifo_end_addr - pFifo->pWrite;

		CFasm_memcpy ( (void*) pFifo->pWrite, (void*) pBuf, l);
		
		pFifo->pWrite = pFifo->fifo_start_addr;
		pBuf += l;

		l = len - l;
		CFasm_memcpy ( (void*) pFifo->pWrite, (void*) pBuf, l);

		pFifo->pWrite += l;

		pFifo->wLen += len;
	}
	else
	{
		CFasm_memcpy ( (void*) pFifo->pWrite, (void*) pBuf, len);

		pFifo->pWrite += len;
		
		if (pFifo->pWrite == pFifo->fifo_end_addr)
		{
			pFifo->pWrite = pFifo->fifo_start_addr;
		}

		pFifo->wLen += len;
	}

	if (pFifo->wLen < pre_wlen)
	{
		pFifo->wlen_H = 0x01;
	}

	return len;
}

long fifo_put_data_one_byte (FIFO *pFifo, U8 *pBuf)
{
	U8 len = 1;

	DBG_Assert ( len <= ( pFifo->fifo_length - (pFifo->wLen - pFifo->rLen)) );

	if ( (pFifo->pWrite + len) > pFifo->fifo_end_addr)
	{
		pFifo->pWrite = pFifo->fifo_start_addr;
		*pFifo->pWrite++ = *pBuf;
		pFifo->wLen += 1;
	}
	else
	{
		*pFifo->pWrite++ = *pBuf;

		if (pFifo->pWrite == pFifo->fifo_end_addr)
		{
			pFifo->pWrite = pFifo->fifo_start_addr;
		}

		pFifo->wLen += 1;
	}

	return 1;
}

long fifo_get_data (FIFO *pFifo, U8 *pBuf, unsigned long len)
{
	unsigned long l;
	
	if (pFifo->wLen < pFifo->rLen)
	{
		l = pFifo->wLen + ((0x100000000 - pFifo->rLen));
	}
	else
	{
		l = pFifo->wLen - pFifo->rLen;
	}

	if (len > l)
	{
		// if ( (decode_type_get() == DECODE_OPTICAL) ||
		// 	(decode_type_get() == DECODE_DD_AC3) )
		// {
		// 	len = 0;//buf underflow
		// }
		// else
		// {
		// 	len = l;
		// }
	}

	if (len == 0)
	{
		return 0;
	}

	if ( (pFifo->pRead + len) > pFifo->fifo_end_addr)
	{	
		l = pFifo->fifo_end_addr - pFifo->pRead;

		CFasm_memcpy ((void*) pBuf, (void*) pFifo->pRead, l);
		
		pFifo->pRead = pFifo->fifo_start_addr;
		pBuf += l;

		l = len - l;
		CFasm_memcpy ( (void*) pBuf, (void*) pFifo->pRead, l);

		pFifo->pRead += l;

		pFifo->rLen += len;
	}
	else
	{
		CFasm_memcpy ( (void*) pBuf, (void*) pFifo->pRead, len);

		pFifo->pRead += len;
		
		if (pFifo->pRead == pFifo->fifo_end_addr)
		{
			pFifo->pRead = pFifo->fifo_start_addr;
		}

		pFifo->rLen += len;
	}

	return len;
}
