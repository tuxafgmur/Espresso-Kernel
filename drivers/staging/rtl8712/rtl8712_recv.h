#ifndef _RTL8712_RECV_H_
#define _RTL8712_RECV_H_

#include "osdep_service.h"
#include "drv_types.h"

#define NR_RECVBUFF (8)
#define NR_PREALLOC_RECV_SKB (8)
#define RXDESC_SIZE	24
#define RXDESC_OFFSET RXDESC_SIZE
#define RECV_BLK_SZ 512
#define RECV_BLK_CNT 16
#define RECV_BLK_TH RECV_BLK_CNT
#define MAX_RECVBUF_SZ (30720) /* 30K */
#define RECVBUFF_ALIGN_SZ 512
#define RSVD_ROOM_SZ (0)
/*These definition is used for Rx packet reordering.*/
#define SN_LESS(a, b)		(((a-b) & 0x800) != 0)
#define SN_EQUAL(a, b)	(a == b)
#define REORDER_WAIT_TIME	30 /* (ms)*/

struct recv_stat {
	unsigned int rxdw0;
	unsigned int rxdw1;
	unsigned int rxdw2;
	unsigned int rxdw3;
	unsigned int rxdw4;
	unsigned int rxdw5;
};

struct phy_cck_rx_status {
	/* For CCK rate descriptor. This is a unsigned 8:1 variable.
	 * LSB bit present 0.5. And MSB 7 bts present a signed value.
	 * Range from -64~+63.5. */
	u8	adc_pwdb_X[4];
	u8	sq_rpt;
	u8	cck_agc_rpt;
};

struct phy_stat {
	unsigned int phydw0;
	unsigned int phydw1;
	unsigned int phydw2;
	unsigned int phydw3;
	unsigned int phydw4;
	unsigned int phydw5;
	unsigned int phydw6;
	unsigned int phydw7;
};
#define PHY_STAT_GAIN_TRSW_SHT 0
#define PHY_STAT_PWDB_ALL_SHT 4
#define PHY_STAT_CFOSHO_SHT 5
#define PHY_STAT_CCK_AGC_RPT_SHT 5
#define PHY_STAT_CFOTAIL_SHT 9
#define PHY_STAT_RXEVM_SHT 13
#define PHY_STAT_RXSNR_SHT 15
#define PHY_STAT_PDSNR_SHT 19
#define PHY_STAT_CSI_CURRENT_SHT 21
#define PHY_STAT_CSI_TARGET_SHT 23
#define PHY_STAT_SIGEVM_SHT 25
#define PHY_STAT_MAX_EX_PWR_SHT 26

union recvstat {
	struct recv_stat recv_stat;
	unsigned int value[RXDESC_SIZE>>2];
};

struct recv_buf {
	struct list_head list;
	spinlock_t recvbuf_lock;
	u32	ref_cnt;
	struct _adapter  *adapter;
	struct urb *purb;
	_pkt *pskb;
	u8 reuse;
	u8  irp_pending;
	u32  transfer_len;
	uint  len;
	u8 *phead;
	u8 *pdata;
	u8 *ptail;
	u8 *pend;
	u8 *pbuf;
	u8 *pallocated_buf;
};

/*
	head  ----->
		data  ----->
			payload
		tail  ----->
	end   ----->
	len = (unsigned int )(tail - data);
*/
struct recv_frame_hdr {
	struct list_head list;
	_pkt	*pkt;
	_pkt *pkt_newalloc;
	struct _adapter  *adapter;
	u8 fragcnt;
	struct rx_pkt_attrib attrib;
	uint  len;
	u8 *rx_head;
	u8 *rx_data;
	u8 *rx_tail;
	u8 *rx_end;
	void *precvbuf;
	struct sta_info *psta;
	/*for A-MPDU Rx reordering buffer control*/
	struct recv_reorder_ctrl *preorder_ctrl;
};

union recv_frame {
	union {
		struct list_head list;
		struct recv_frame_hdr hdr;
		addr_t mem[RECVFRAME_HDR_ALIGN>>2];
	} u;
};

int r8712_init_recvbuf(struct _adapter *padapter, struct recv_buf *precvbuf);
void r8712_rxcmd_event_hdl(struct _adapter *padapter, void *prxcmdbuf);
s32 r8712_signal_scale_mapping(s32 cur_sig);
void r8712_reordering_ctrl_timeout_handler(void *pcontext);

#endif
