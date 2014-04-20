//------------------------------------------------------------------------------
// <copyright file="roaming.h" company="Atheros">
//    Copyright (c) 2004-2010 Atheros Corporation.  All rights reserved.
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
//------------------------------------------------------------------------------
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================

#ifndef _ROAMING_H_
#define _ROAMING_H_

/*
 * The signal quality could be in terms of either snr or rssi. We should
 * have an enum for both of them. For the time being, we are going to move
 * it to wmi.h that is shared by both host and the target, since we are
 * repartitioning the code to the host
 */
#define SIGNAL_QUALITY_NOISE_FLOOR        -96
#define SIGNAL_QUALITY_METRICS_NUM_MAX    2
typedef enum {
    SIGNAL_QUALITY_METRICS_SNR = 0,
    SIGNAL_QUALITY_METRICS_RSSI,
    SIGNAL_QUALITY_METRICS_ALL,
} SIGNAL_QUALITY_METRICS_TYPE;

#endif  /* _ROAMING_H_ */
