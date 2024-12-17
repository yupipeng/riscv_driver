/*
 *
 * Copyright (c) [2016] by InvenSense, Inc.
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#ifndef _DYN_PRO_TRANSPORT_H_
#define _DYN_PRO_TRANSPORT_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum DynProTransportEvent {
	DYN_PRO_TRANSPORT_EVENT_ERROR,
	DYN_PRO_TRANSPORT_EVENT_PKT_SIZE,
	DYN_PRO_TRANSPORT_EVENT_PKT_BYTE,
	DYN_PRO_TRANSPORT_EVENT_PKT_END,
	DYN_PRO_TRANSPORT_EVENT_TX_START,
	DYN_PRO_TRANSPORT_EVENT_TX_BYTE,
	DYN_PRO_TRANSPORT_EVENT_TX_END,
	DYN_PRO_TRANSPORT_EVENT_TX_START_DMA,
};

union DynProTransportEventData {
	int      error;
	uint16_t pkt_size;
	uint8_t  pkt_byte;
	uint32_t tx_start;
	uint8_t  tx_byte;
	void *   frame;
};

typedef void (*DynProTransportEvent_cb)(enum DynProTransportEvent      e,
                                        union DynProTransportEventData data, void *cookie);

#ifdef __cplusplus
}
#endif

#endif /* _DYN_PRO_TRANSPORT_H_ */
