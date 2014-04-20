// ------------------------------------------------------------------
// Copyright (c) 2004-2010 Atheros Corporation.  All rights reserved.
//
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
//
//
// ------------------------------------------------------------------
//===================================================================
// Author(s): ="Atheros"
//===================================================================

#ifndef _RTC_WLAN_REG_REG_H_
#define _RTC_WLAN_REG_REG_H_

#define WLAN_RESET_CONTROL_ADDRESS               0x00000000
#define WLAN_RESET_CONTROL_OFFSET                0x00000000
#define WLAN_RESET_CONTROL_DEBUG_UART_RST_MSB    14
#define WLAN_RESET_CONTROL_DEBUG_UART_RST_LSB    14
#define WLAN_RESET_CONTROL_DEBUG_UART_RST_MASK   0x00004000
#define WLAN_RESET_CONTROL_DEBUG_UART_RST_GET(x) (((x) & WLAN_RESET_CONTROL_DEBUG_UART_RST_MASK) >> WLAN_RESET_CONTROL_DEBUG_UART_RST_LSB)
#define WLAN_RESET_CONTROL_DEBUG_UART_RST_SET(x) (((x) << WLAN_RESET_CONTROL_DEBUG_UART_RST_LSB) & WLAN_RESET_CONTROL_DEBUG_UART_RST_MASK)
#define WLAN_RESET_CONTROL_BB_COLD_RST_MSB       13
#define WLAN_RESET_CONTROL_BB_COLD_RST_LSB       13
#define WLAN_RESET_CONTROL_BB_COLD_RST_MASK      0x00002000
#define WLAN_RESET_CONTROL_BB_COLD_RST_GET(x)    (((x) & WLAN_RESET_CONTROL_BB_COLD_RST_MASK) >> WLAN_RESET_CONTROL_BB_COLD_RST_LSB)
#define WLAN_RESET_CONTROL_BB_COLD_RST_SET(x)    (((x) << WLAN_RESET_CONTROL_BB_COLD_RST_LSB) & WLAN_RESET_CONTROL_BB_COLD_RST_MASK)
#define WLAN_RESET_CONTROL_BB_WARM_RST_MSB       12
#define WLAN_RESET_CONTROL_BB_WARM_RST_LSB       12
#define WLAN_RESET_CONTROL_BB_WARM_RST_MASK      0x00001000
#define WLAN_RESET_CONTROL_BB_WARM_RST_GET(x)    (((x) & WLAN_RESET_CONTROL_BB_WARM_RST_MASK) >> WLAN_RESET_CONTROL_BB_WARM_RST_LSB)
#define WLAN_RESET_CONTROL_BB_WARM_RST_SET(x)    (((x) << WLAN_RESET_CONTROL_BB_WARM_RST_LSB) & WLAN_RESET_CONTROL_BB_WARM_RST_MASK)
#define WLAN_RESET_CONTROL_CPU_INIT_RESET_MSB    11
#define WLAN_RESET_CONTROL_CPU_INIT_RESET_LSB    11
#define WLAN_RESET_CONTROL_CPU_INIT_RESET_MASK   0x00000800
#define WLAN_RESET_CONTROL_CPU_INIT_RESET_GET(x) (((x) & WLAN_RESET_CONTROL_CPU_INIT_RESET_MASK) >> WLAN_RESET_CONTROL_CPU_INIT_RESET_LSB)
#define WLAN_RESET_CONTROL_CPU_INIT_RESET_SET(x) (((x) << WLAN_RESET_CONTROL_CPU_INIT_RESET_LSB) & WLAN_RESET_CONTROL_CPU_INIT_RESET_MASK)
#define WLAN_RESET_CONTROL_VMC_REMAP_RESET_MSB   10
#define WLAN_RESET_CONTROL_VMC_REMAP_RESET_LSB   10
#define WLAN_RESET_CONTROL_VMC_REMAP_RESET_MASK  0x00000400
#define WLAN_RESET_CONTROL_VMC_REMAP_RESET_GET(x) (((x) & WLAN_RESET_CONTROL_VMC_REMAP_RESET_MASK) >> WLAN_RESET_CONTROL_VMC_REMAP_RESET_LSB)
#define WLAN_RESET_CONTROL_VMC_REMAP_RESET_SET(x) (((x) << WLAN_RESET_CONTROL_VMC_REMAP_RESET_LSB) & WLAN_RESET_CONTROL_VMC_REMAP_RESET_MASK)
#define WLAN_RESET_CONTROL_RST_OUT_MSB           9
#define WLAN_RESET_CONTROL_RST_OUT_LSB           9
#define WLAN_RESET_CONTROL_RST_OUT_MASK          0x00000200
#define WLAN_RESET_CONTROL_RST_OUT_GET(x)        (((x) & WLAN_RESET_CONTROL_RST_OUT_MASK) >> WLAN_RESET_CONTROL_RST_OUT_LSB)
#define WLAN_RESET_CONTROL_RST_OUT_SET(x)        (((x) << WLAN_RESET_CONTROL_RST_OUT_LSB) & WLAN_RESET_CONTROL_RST_OUT_MASK)
#define WLAN_RESET_CONTROL_COLD_RST_MSB          8
#define WLAN_RESET_CONTROL_COLD_RST_LSB          8
#define WLAN_RESET_CONTROL_COLD_RST_MASK         0x00000100
#define WLAN_RESET_CONTROL_COLD_RST_GET(x)       (((x) & WLAN_RESET_CONTROL_COLD_RST_MASK) >> WLAN_RESET_CONTROL_COLD_RST_LSB)
#define WLAN_RESET_CONTROL_COLD_RST_SET(x)       (((x) << WLAN_RESET_CONTROL_COLD_RST_LSB) & WLAN_RESET_CONTROL_COLD_RST_MASK)
#define WLAN_RESET_CONTROL_WARM_RST_MSB          7
#define WLAN_RESET_CONTROL_WARM_RST_LSB          7
#define WLAN_RESET_CONTROL_WARM_RST_MASK         0x00000080
#define WLAN_RESET_CONTROL_WARM_RST_GET(x)       (((x) & WLAN_RESET_CONTROL_WARM_RST_MASK) >> WLAN_RESET_CONTROL_WARM_RST_LSB)
#define WLAN_RESET_CONTROL_WARM_RST_SET(x)       (((x) << WLAN_RESET_CONTROL_WARM_RST_LSB) & WLAN_RESET_CONTROL_WARM_RST_MASK)
#define WLAN_RESET_CONTROL_CPU_WARM_RST_MSB      6
#define WLAN_RESET_CONTROL_CPU_WARM_RST_LSB      6
#define WLAN_RESET_CONTROL_CPU_WARM_RST_MASK     0x00000040
#define WLAN_RESET_CONTROL_CPU_WARM_RST_GET(x)   (((x) & WLAN_RESET_CONTROL_CPU_WARM_RST_MASK) >> WLAN_RESET_CONTROL_CPU_WARM_RST_LSB)
#define WLAN_RESET_CONTROL_CPU_WARM_RST_SET(x)   (((x) << WLAN_RESET_CONTROL_CPU_WARM_RST_LSB) & WLAN_RESET_CONTROL_CPU_WARM_RST_MASK)
#define WLAN_RESET_CONTROL_MAC_COLD_RST_MSB      5
#define WLAN_RESET_CONTROL_MAC_COLD_RST_LSB      5
#define WLAN_RESET_CONTROL_MAC_COLD_RST_MASK     0x00000020
#define WLAN_RESET_CONTROL_MAC_COLD_RST_GET(x)   (((x) & WLAN_RESET_CONTROL_MAC_COLD_RST_MASK) >> WLAN_RESET_CONTROL_MAC_COLD_RST_LSB)
#define WLAN_RESET_CONTROL_MAC_COLD_RST_SET(x)   (((x) << WLAN_RESET_CONTROL_MAC_COLD_RST_LSB) & WLAN_RESET_CONTROL_MAC_COLD_RST_MASK)
#define WLAN_RESET_CONTROL_MAC_WARM_RST_MSB      4
#define WLAN_RESET_CONTROL_MAC_WARM_RST_LSB      4
#define WLAN_RESET_CONTROL_MAC_WARM_RST_MASK     0x00000010
#define WLAN_RESET_CONTROL_MAC_WARM_RST_GET(x)   (((x) & WLAN_RESET_CONTROL_MAC_WARM_RST_MASK) >> WLAN_RESET_CONTROL_MAC_WARM_RST_LSB)
#define WLAN_RESET_CONTROL_MAC_WARM_RST_SET(x)   (((x) << WLAN_RESET_CONTROL_MAC_WARM_RST_LSB) & WLAN_RESET_CONTROL_MAC_WARM_RST_MASK)
#define WLAN_RESET_CONTROL_MBOX_RST_MSB          2
#define WLAN_RESET_CONTROL_MBOX_RST_LSB          2
#define WLAN_RESET_CONTROL_MBOX_RST_MASK         0x00000004
#define WLAN_RESET_CONTROL_MBOX_RST_GET(x)       (((x) & WLAN_RESET_CONTROL_MBOX_RST_MASK) >> WLAN_RESET_CONTROL_MBOX_RST_LSB)
#define WLAN_RESET_CONTROL_MBOX_RST_SET(x)       (((x) << WLAN_RESET_CONTROL_MBOX_RST_LSB) & WLAN_RESET_CONTROL_MBOX_RST_MASK)
#define WLAN_RESET_CONTROL_UART_RST_MSB          1
#define WLAN_RESET_CONTROL_UART_RST_LSB          1
#define WLAN_RESET_CONTROL_UART_RST_MASK         0x00000002
#define WLAN_RESET_CONTROL_UART_RST_GET(x)       (((x) & WLAN_RESET_CONTROL_UART_RST_MASK) >> WLAN_RESET_CONTROL_UART_RST_LSB)
#define WLAN_RESET_CONTROL_UART_RST_SET(x)       (((x) << WLAN_RESET_CONTROL_UART_RST_LSB) & WLAN_RESET_CONTROL_UART_RST_MASK)
#define WLAN_RESET_CONTROL_SI0_RST_MSB           0
#define WLAN_RESET_CONTROL_SI0_RST_LSB           0
#define WLAN_RESET_CONTROL_SI0_RST_MASK          0x00000001
#define WLAN_RESET_CONTROL_SI0_RST_GET(x)        (((x) & WLAN_RESET_CONTROL_SI0_RST_MASK) >> WLAN_RESET_CONTROL_SI0_RST_LSB)
#define WLAN_RESET_CONTROL_SI0_RST_SET(x)        (((x) << WLAN_RESET_CONTROL_SI0_RST_LSB) & WLAN_RESET_CONTROL_SI0_RST_MASK)

#define WLAN_CPU_CLOCK_ADDRESS                   0x00000020
#define WLAN_CPU_CLOCK_OFFSET                    0x00000020
#define WLAN_CPU_CLOCK_STANDARD_MSB              1
#define WLAN_CPU_CLOCK_STANDARD_LSB              0
#define WLAN_CPU_CLOCK_STANDARD_MASK             0x00000003
#define WLAN_CPU_CLOCK_STANDARD_GET(x)           (((x) & WLAN_CPU_CLOCK_STANDARD_MASK) >> WLAN_CPU_CLOCK_STANDARD_LSB)
#define WLAN_CPU_CLOCK_STANDARD_SET(x)           (((x) << WLAN_CPU_CLOCK_STANDARD_LSB) & WLAN_CPU_CLOCK_STANDARD_MASK)

#define WLAN_CLOCK_CONTROL_ADDRESS               0x00000028
#define WLAN_CLOCK_CONTROL_OFFSET                0x00000028
#define WLAN_CLOCK_CONTROL_LF_CLK32_MSB          2
#define WLAN_CLOCK_CONTROL_LF_CLK32_LSB          2
#define WLAN_CLOCK_CONTROL_LF_CLK32_MASK         0x00000004
#define WLAN_CLOCK_CONTROL_LF_CLK32_GET(x)       (((x) & WLAN_CLOCK_CONTROL_LF_CLK32_MASK) >> WLAN_CLOCK_CONTROL_LF_CLK32_LSB)
#define WLAN_CLOCK_CONTROL_LF_CLK32_SET(x)       (((x) << WLAN_CLOCK_CONTROL_LF_CLK32_LSB) & WLAN_CLOCK_CONTROL_LF_CLK32_MASK)
#define WLAN_CLOCK_CONTROL_SI0_CLK_MSB           0
#define WLAN_CLOCK_CONTROL_SI0_CLK_LSB           0
#define WLAN_CLOCK_CONTROL_SI0_CLK_MASK          0x00000001
#define WLAN_CLOCK_CONTROL_SI0_CLK_GET(x)        (((x) & WLAN_CLOCK_CONTROL_SI0_CLK_MASK) >> WLAN_CLOCK_CONTROL_SI0_CLK_LSB)
#define WLAN_CLOCK_CONTROL_SI0_CLK_SET(x)        (((x) << WLAN_CLOCK_CONTROL_SI0_CLK_LSB) & WLAN_CLOCK_CONTROL_SI0_CLK_MASK)

#define WLAN_SYSTEM_SLEEP_ADDRESS                0x000000c4
#define WLAN_SYSTEM_SLEEP_OFFSET                 0x000000c4
#define WLAN_SYSTEM_SLEEP_HOST_IF_MSB            4
#define WLAN_SYSTEM_SLEEP_HOST_IF_LSB            4
#define WLAN_SYSTEM_SLEEP_HOST_IF_MASK           0x00000010
#define WLAN_SYSTEM_SLEEP_HOST_IF_GET(x)         (((x) & WLAN_SYSTEM_SLEEP_HOST_IF_MASK) >> WLAN_SYSTEM_SLEEP_HOST_IF_LSB)
#define WLAN_SYSTEM_SLEEP_HOST_IF_SET(x)         (((x) << WLAN_SYSTEM_SLEEP_HOST_IF_LSB) & WLAN_SYSTEM_SLEEP_HOST_IF_MASK)
#define WLAN_SYSTEM_SLEEP_MBOX_MSB               3
#define WLAN_SYSTEM_SLEEP_MBOX_LSB               3
#define WLAN_SYSTEM_SLEEP_MBOX_MASK              0x00000008
#define WLAN_SYSTEM_SLEEP_MBOX_GET(x)            (((x) & WLAN_SYSTEM_SLEEP_MBOX_MASK) >> WLAN_SYSTEM_SLEEP_MBOX_LSB)
#define WLAN_SYSTEM_SLEEP_MBOX_SET(x)            (((x) << WLAN_SYSTEM_SLEEP_MBOX_LSB) & WLAN_SYSTEM_SLEEP_MBOX_MASK)
#define WLAN_SYSTEM_SLEEP_MAC_IF_MSB             2
#define WLAN_SYSTEM_SLEEP_MAC_IF_LSB             2
#define WLAN_SYSTEM_SLEEP_MAC_IF_MASK            0x00000004
#define WLAN_SYSTEM_SLEEP_MAC_IF_GET(x)          (((x) & WLAN_SYSTEM_SLEEP_MAC_IF_MASK) >> WLAN_SYSTEM_SLEEP_MAC_IF_LSB)
#define WLAN_SYSTEM_SLEEP_MAC_IF_SET(x)          (((x) << WLAN_SYSTEM_SLEEP_MAC_IF_LSB) & WLAN_SYSTEM_SLEEP_MAC_IF_MASK)
#define WLAN_SYSTEM_SLEEP_LIGHT_MSB              1
#define WLAN_SYSTEM_SLEEP_LIGHT_LSB              1
#define WLAN_SYSTEM_SLEEP_LIGHT_MASK             0x00000002
#define WLAN_SYSTEM_SLEEP_LIGHT_GET(x)           (((x) & WLAN_SYSTEM_SLEEP_LIGHT_MASK) >> WLAN_SYSTEM_SLEEP_LIGHT_LSB)
#define WLAN_SYSTEM_SLEEP_LIGHT_SET(x)           (((x) << WLAN_SYSTEM_SLEEP_LIGHT_LSB) & WLAN_SYSTEM_SLEEP_LIGHT_MASK)
#define WLAN_SYSTEM_SLEEP_DISABLE_MSB            0
#define WLAN_SYSTEM_SLEEP_DISABLE_LSB            0
#define WLAN_SYSTEM_SLEEP_DISABLE_MASK           0x00000001
#define WLAN_SYSTEM_SLEEP_DISABLE_GET(x)         (((x) & WLAN_SYSTEM_SLEEP_DISABLE_MASK) >> WLAN_SYSTEM_SLEEP_DISABLE_LSB)
#define WLAN_SYSTEM_SLEEP_DISABLE_SET(x)         (((x) << WLAN_SYSTEM_SLEEP_DISABLE_LSB) & WLAN_SYSTEM_SLEEP_DISABLE_MASK)

#define WLAN_LPO_CAL_ADDRESS                     0x000000e0
#define WLAN_LPO_CAL_OFFSET                      0x000000e0
#define WLAN_LPO_CAL_ENABLE_MSB                  20
#define WLAN_LPO_CAL_ENABLE_LSB                  20
#define WLAN_LPO_CAL_ENABLE_MASK                 0x00100000
#define WLAN_LPO_CAL_ENABLE_GET(x)               (((x) & WLAN_LPO_CAL_ENABLE_MASK) >> WLAN_LPO_CAL_ENABLE_LSB)
#define WLAN_LPO_CAL_ENABLE_SET(x)               (((x) << WLAN_LPO_CAL_ENABLE_LSB) & WLAN_LPO_CAL_ENABLE_MASK)
#define WLAN_LPO_CAL_COUNT_MSB                   19
#define WLAN_LPO_CAL_COUNT_LSB                   0
#define WLAN_LPO_CAL_COUNT_MASK                  0x000fffff
#define WLAN_LPO_CAL_COUNT_GET(x)                (((x) & WLAN_LPO_CAL_COUNT_MASK) >> WLAN_LPO_CAL_COUNT_LSB)
#define WLAN_LPO_CAL_COUNT_SET(x)                (((x) << WLAN_LPO_CAL_COUNT_LSB) & WLAN_LPO_CAL_COUNT_MASK)

#endif /* _RTC_WLAN_REG_H_ */
