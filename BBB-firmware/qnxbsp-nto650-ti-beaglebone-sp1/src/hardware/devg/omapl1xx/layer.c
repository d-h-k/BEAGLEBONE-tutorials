/*
 * $QNXLicenseC:
 * Copyright 2010, QNX Software Systems.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You
 * may not reproduce, modify or distribute this software except in
 * compliance with the License. You may obtain a copy of the License
 * at: http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
 *
 * This file may contain contributions from others, either as
 * contributors under the License or as licensors under other terms.
 * Please review this entire file for other proprietary rights or license
 * notices, as well as the QNX Development Suite License Guide at
 * http://licensing.qnx.com/license-guide/ for other information.
 * $
 */

#include <unistd.h>
#include <string.h>

#include "omapl1xx.h"
#include "hdmi.h"

int
omapl1xx_layer_query(disp_adapter_t * adapter, int dispno,
	int layer_idx, int fmt_idx, disp_layer_query_t * info_t)
{
	omapl1xx_context_t	*omapl1xx = adapter->ms_ctx;
	disp_layer_query_t	info;

	if (info_t->size > 0)
		info.size = info_t->size;
	else
		info.size = sizeof(info);

	if (fmt_idx != 0 || layer_idx != 0)
		return -1;

	info.pixel_format = omapl1xx->pixel_format;
	info.dst_max_width = info.src_max_viewport_width = omapl1xx->lcd_xres;
	info.dst_max_height = info.src_max_viewport_height = omapl1xx->lcd_yres;
	info.max_scaledown_x = info.max_scaleup_x = 1;
	info.max_scaledown_y = info.max_scaleup_y = 1;

	if (dispno == 0)
		info.caps = DISP_LAYER_CAP_MAIN_DISPLAY;

	info.dst_min_width = omapl1xx->lcd_xres;
	info.dst_min_height = omapl1xx->lcd_yres;
	info.src_max_width = omapl1xx->lcd_xres;
	info.src_max_height = omapl1xx->lcd_yres;

	info.chromakey_caps = 0;
	info.alpha_valid_flags = 0;
	info.alpha_combinations = 0;
	info.order_mask = 1;

	memcpy(info_t, &info, info.size);

	return 0;
}

int
omapl1xx_layer_enable(disp_adapter_t * adapter, int dispno, int layer_idx)
{
	omapl1xx_context_t *omapl1xx = adapter->ms_ctx;

	switch (omapl1xx->display)
	{
	case AM335X_LCD:
		if (layer_idx == 0) {
			omapl1xx->lcd_enable = 1;
			return 0;
		} else
			return -1;
	case AM335X_HDMI:
#ifdef AM335X_HDMI_DEBUG
		disp_printf(adapter, "%s() HDMI setup", __FUNCTION__);
#endif
		if (layer_idx == 0) {
			omapl1xx->lcd_enable = 1;
			setup_hdmi(adapter, omapl1xx->hdmi_dev);
			return 0;
		} else
			return -1;
	default:
			return -2;
	}
}

int
omapl1xx_layer_disable(disp_adapter_t * adapter, int dispno, int layer_idx)
{
	omapl1xx_context_t *omapl1xx = adapter->ms_ctx;

	if (layer_idx == 0) {
		omapl1xx->lcd_enable = 0;
		return 0;
	} else
		return -1;
}

int
omapl1xx_layer_set_surface(disp_adapter_t * adapter, int dispno,
    int layer_idx, unsigned layer_format, disp_surface_t *surfaces[])
{
	omapl1xx_context_t	*omapl1xx = adapter->ms_ctx;
	disp_surface_t		*surf = surfaces[0];

	if (layer_idx != 0)
		return -1;


	omapl1xx->lcd_surf = surf;
	omapl1xx->lcd_size = surf->stride * (surf->height+1);
	omapl1xx->lcd_swap = 2;  /* 2 buffers to update */

	return 0;
}

int
omapl1xx_layer_set_source_viewport(disp_adapter_t * adapter,
	int dispno, int layer_idx, int x1, int y1, int x2, int y2)
{
	if (layer_idx == 0)
		return 0;
	else
		return -1;
}

int
omapl1xx_layer_set_dest_viewport(disp_adapter_t * adapter,
	int dispno, int layer_idx, int x1, int y1, int x2, int y2)
{
	if (layer_idx == 0)
		return 0;
	else
		return -1;
}

int
omapl1xx_layer_set_blending(disp_adapter_t *adapter, int dispno,
	int layer_idx, unsigned mode, int m1, int m2)
{
	if (layer_idx == 0)
		return 0;
	else
		return -1;
}

int
omapl1xx_layer_set_chromakey(disp_adapter_t *adapter, int dispno,
	int layer_idx, unsigned chroma_mode,
	disp_color_t color0, disp_color_t color1, disp_color_t mask)
{
	if (layer_idx == 0)
		return 0;
	else
		return -1;
}

int
omapl1xx_layer_set_flags(disp_adapter_t * adapter, int dispno,
	int layer_idx, unsigned flag_mask, unsigned flag_values)
{
	return -1;
}

void
omapl1xx_layer_update_begin(disp_adapter_t *adapter,
    int dispno, uint64_t layer_mask)
{
	return;
}

void
omapl1xx_wait_eof(disp_adapter_t *adapter, int lock, uint32_t trigger) {
	omapl1xx_context_t	*omapl1xx = adapter->ms_ctx;
	struct _pulse   	pulse;
	iov_t           	iov;
	uint64_t        	halfsecond = 500*1000*1000;

	if (lock) {
		if (adapter->callback) {
			adapter->callback(adapter->callback_handle,
				DISP_CALLBACK_UNLOCK, NULL);
		}
	}

	SETIOV(&iov, &pulse, sizeof (pulse));

	/* Disable interrupts */
	*IRQENABLE_CLEAR |= (EOF0_RAW_SET | EOF1_RAW_SET | FUF_RAW_SET);
	/* Acknowledge interrupts (defensively) */
	*IRQSTATUS       |= (EOF0_RAW_SET | EOF1_RAW_SET);
	/* Enable interrupt we're interested in */
	*IRQENABLE_SET   |= trigger;

	while (1) {
		TimerTimeout(CLOCK_REALTIME,
			_NTO_TIMEOUT_RECEIVE, NULL, &halfsecond, NULL);

		if (MsgReceivev(omapl1xx->lcd_chid, &iov, 1, NULL) == -1) {
			disp_perror(adapter, "MsgReceive EOF");
			break;
		}
		if (pulse.code == OMAPL1xx_LCD_INTERRUPT_PULSE) {
			break;
		}
	}

	if (lock) {
		if (adapter->callback) {
			adapter->callback(adapter->callback_handle,
				DISP_CALLBACK_LOCK, NULL);
		}
	}
}


void
omapl1xx_layer_update_end(disp_adapter_t *adapter,
    int dispno, uint64_t layer_mask, int wait_vsync)
{
	omapl1xx_context_t	*omapl1xx = adapter->ms_ctx;

	if (omapl1xx->lcd_swap) {
		if (omapl1xx->lcd_dma_on) {
			/* Set frame buffer ptr (in isr) for buffer 0 */
			omapl1xx_wait_eof(adapter,0, EOF0_RAW_SET);
			/* Set frame buffer ptr (in isr) for buffer 1 */
			omapl1xx_wait_eof(adapter,0, EOF1_RAW_SET);
		}
		else {
			/* setup initial base address */
			*LCDDMA_FB0_BASE = omapl1xx->lcd_surf->paddr;
			*LCDDMA_FB0_CEILING = omapl1xx->lcd_surf->paddr + omapl1xx->lcd_size - 1;
			*LCDDMA_FB1_BASE = omapl1xx->lcd_surf->paddr;
			*LCDDMA_FB1_CEILING = omapl1xx->lcd_surf->paddr + omapl1xx->lcd_size - 1;
		}
	}

	/* enabled Raster Controller */
	if (omapl1xx->lcd_enable) {
		*RASTER_CTRL |=1;
		omapl1xx->lcd_dma_on = 1;
	}
	else {
		if (omapl1xx->lcd_dma_on) {
			omapl1xx_wait_eof(adapter,0, 0x300);
			*RASTER_CTRL &= ~1;
			wait_vsync = 0; /* we're already off so we won't get a vsync*/
		}
		omapl1xx->lcd_dma_on = 0;
	}

	if (*IRQSTATUS & 0x4) {
		disp_printf(adapter,"TI OMAPL1xx Internal LCD Raster Controller frame synchronization lost");
	}	

	if (wait_vsync && adapter->callback) {
		adapter->callback(adapter->callback_handle,
		    DISP_CALLBACK_WAIT_VSYNC, &dispno);
	}
}

void
omapl1xx_layer_reset(disp_adapter_t * adapter, int dispno, int layer_idx)
{
	return;
}


int
omapl1xx_layer_set_alpha_map(disp_adapter_t *adapter,
    int dispno, int layer_idx, disp_surface_t *map)
{
	return -1;
}
