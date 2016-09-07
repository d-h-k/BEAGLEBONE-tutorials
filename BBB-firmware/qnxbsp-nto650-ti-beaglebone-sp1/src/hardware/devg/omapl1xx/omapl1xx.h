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

#ifndef __OMAPL1xx_H_INCLUDED
#define __OMAPL1xx_H_INCLUDED

#include <graphics/display.h>
#include <graphics/disputil.h>
#include <graphics/ffb.h>

#define lptr(x) (uint32_t volatile *)(((unsigned char volatile *)omapl1xx->lregptr)+x)

//#define LCDDBG							1

#define AM335X_LCD						0
#define AM335X_HDMI						1

#define OMAPL1xx_LCD_INTERRUPT_PULSE	0x5c

#define OMAPL1xx_LCD_BASE				0x4830E000
#define OMAPL1xx_LCD_SIZE				0x1000

#define OMAPL1xx_LCD_IRQ				36

#define OMAPL1xx_LCD_XRES				800
#define OMAPL1xx_LCD_YRES				480

/* LCD controller registers */
#define LCD_REV							lptr(0x00)
#define LCD_CTRL						lptr(0x04)
	#define CLKDIV					0xFF00		// Clock divisor mask.
	#define CLKDIV_SHIFT			 8			// Clock divisor bit shift.
	#define AUTO_UFLOW_RESTART_HW	(1<<1)		// 1 = On an underflow, the hardware will restart on the next frame
	#define AUTO_UFLOW_RESTART_SW	(0<<1)		// 0 = On an underflow, the software has to restart the module
	#define MODESEL_RASTER			(1<<0)		// LCD Mode select: Raster
	#define MODESEL_LIDD			(0<<0)		// LCD Mode select: LIDD
#define LIDD_CTRL						lptr(0x0C)
#define LIDD_CS0_CONF					lptr(0x10)
#define LIDD_CS0_ADDR					lptr(0x14)
#define LIDD_CS0_DATA					lptr(0x18)
#define LIDD_CS1_CONF					lptr(0x1C)
#define LIDD_CS1_ADDR					lptr(0x20)
#define LIDD_CS1_DATA					lptr(0x24)
#define RASTER_CTRL						lptr(0x28)
#define RASTER_TIMING_0					lptr(0x2C)
#define RASTER_TIMING_1					lptr(0x30)
#define RASTER_TIMING_2					lptr(0x34)
#define RASTER_SUBPANEL					lptr(0x38)
#define LCDDMA_CTRL						lptr(0x40)
	#define LCDDMA_SINGLE_BUFFER	0x0000		// 0 = one frame buffer (FB0 only) used
	#define LCDDMA_DOUBLE_BUFFER	0x0001		// 1 = two frame buffers used DMA ping-pongs between FB0 and FB1 in this mode
	#define LCDDMA_BIGENDIAN		0x0002		// Big Endian Enable. Use this bit when the processor is operating in Big Endian mode AND writes to the frame buffer(s) are less than 32 bits wide.
	#define LCDDMA_BYTESWAP			0x0008		// This bit controls the bytelane ordering of the data on the output of the DMA module.
	#define LCDDMA_BURST1			0x0000		// burst size of 1
	#define LCDDMA_BURST2			0x0010		// burst size of 2
	#define LCDDMA_BURST4			0x0020		// burst size of 4
	#define LCDDMA_BURST8			0x0030		// burst size of 8
	#define LCDDMA_BURST16			0x0040		// burst size of 16
	#define LCDDMA_TRESH8			0x0000
	#define LCDDMA_TRESH16			0x0100
	#define LCDDMA_TRESH32			0x0200
	#define LCDDMA_TRESH64			0x0300
	#define LCDDMA_TRESH128			0x0400
	#define LCDDMA_TRESH256			0x0500
	#define LCDDMA_TRESH512			0x0600
	#define LCDDMA_PRIO0			(0<<16)
	#define LCDDMA_PRIO1			(1<<16)
	#define LCDDMA_PRIO2			(2<<16)
	#define LCDDMA_PRIO3			(3<<16)
	#define LCDDMA_PRIO4			(4<<16)
	#define LCDDMA_PRIO5			(5<<16)
	#define LCDDMA_PRIO6			(6<<16)
	#define LCDDMA_PRIO7			(7<<16)
#define LCDDMA_FB0_BASE					lptr(0x44)
#define LCDDMA_FB0_CEILING				lptr(0x48)
#define LCDDMA_FB1_BASE					lptr(0x4C)
#define LCDDMA_FB1_CEILING				lptr(0x50)
#define SYSCONFIG						lptr(0x54)
#define IRQSTATUS_RAW					lptr(0x58)
	#define EOF1_RAW_SET	(1<<9)		// DMA End-of-Frame 1 Raw Interrupt Status and Set
	#define EOF0_RAW_SET	(1<<8)		// DMA End-of-Frame 0 Raw Interrupt Status and Set
	#define PL_RAW_SET		(1<<6)		// DMA Palette Loaded Raw Interrupt Status and Set
	#define FUF_RAW_SET		(1<<5)		// DMA FIFO Underflow Raw Interrupt Status and Set
	#define ACB_RAW_SET		(1<<3)		// For Passive Matrix Panels Only
	#define SYNC_RAW_SET 	(1<<2)		// Frame Synchronization Lost Raw Interrupt Status and Set
	#define DONE_RAW_SET 	(1<<0)		// Raster or LIDD Frame Done (shared, depends on whether Raster or
#define IRQSTATUS						lptr(0x5C)
#define IRQENABLE_SET					lptr(0x60)
#define IRQENABLE_CLEAR					lptr(0x64)
#define IRQEOI_VECTOR					lptr(0x68)
#define CLKC_ENABLE						lptr(0x6C)
	#define CLKC_EN_DMA_CLK_EN 				2 	// Software Clock Enable for the DMA submodule. The DMA submodule runs on the L3 Clock domain.
	#define CLKC_EN_LIDD_CLK_EN				1	// Software Clock Enable for the LIDD submodule (character displays). The LIDD submodule runs on the System Clock (lcd_clk) domain
	#define CLKC_EN_CORE_CLK_EN				0	// Software Clock Enable for the Core, which encompasses the Raster Active Matrix and Passive Matrix logic. The Core runs on the System Clock (lcd_clk) domain.
#define CLKC_RESET						lptr(0x70)

/* LCD Control bits */
#define RASTER_CTRL_LCDEN         0x00000001  /* LCD enable */
#define RASTER_CTRL_LCDBW         0x00000002  /* color/monochrome select */
#define RASTER_CTRL_DMASK         0x00000008  /* mask LCD disable done */
#define RASTER_CTRL_LMASK         0x00000010  /* mask start of frame interrupt */
#define RASTER_CTRL_TFT           0x00000080  /* active/passive select */
#define RASTER_CTRL_CB0           0x00000100  /* control bit 0 */
#define RASTER_CTRL_M8B           0x00000200  /* double-pixel data */
#define RASTER_CTRL_PLOAD         0x00100000  /* Palette loading */
#define RASTER_CTRL_DLOAD         0x00200000  /* Data loading */
#define RASTER_CTRL_CB1           0x00400000  /* control bit 1 */
#define RASTER_CTRL_TFTMAP        0x00800000  /* TFT alternate signal mapping */
#define RASTER_CTRL_565STN        0x01000000  /* 12bpp mode */
#define RASTER_CTRL_FDD(x)        (((x) & 0xff) << 12)

/* This is AM335x EVM specific. The EVM has a TLC59108 that is used to control */
/* 'manually' a small number of signals into the LCD panel, as defined below:  */
#define AM335XEVM_LCD_AVDDEN			0x02	/* To TPS65105 'en' pin */
#define AM335XEVM_LCD_BACKLIGHTEN		0x04	/* NC */
#define AM335XEVM_STBYB					0x08	/* Standby mode. STBYB="1" : normal operation. STBYB="0" : timing control, source driver will turn off, all output are high-Z */
#define AM335XEVM_UPDN					0x10	/* Swap left-right. Gate up or down scan control. UPDN="L" : down shift : G1->G2�->G480. UPDN="H" : up shift: G1<-G2�<-G480 */
#define AM335XEVM_DITH					0x20	/* Dither setting DITH="H" 6bit resolution (last 2 bits of input data truncated) DITH="L" 8bit resolution (default setting)*/
#define AM335XEVM_MODE3					0x40	/* DE/SYNC mode select. Normally pull high. H:DE mode. L:HSD/VSD mode */
#define AM335XEVM_SHLR					0x80	/* Swap up-down. Source right or left sequence control.SHLR="L" : shift left:last data=S1<-S2�S1200=first data. SHLR="H" : shift right:first data=S1->SS2�S1200=last data */
/* end */

typedef struct {
	uint32_t		offset;
	int				stride;
	unsigned		bpp;
	unsigned		layer_format;	
	uint32_t		mode;
	disp_point_t	src1;
	disp_point_t	src2;
	disp_point_t	dst1;
	disp_point_t	dst2;
	disp_color_t	transp;
} layer_info;

#define MAX_STRLEN 50

#define HDMI_CABLE_CONNECTED		1
#define HDMI_CABLE_NOTCONNECTED		0

typedef struct _am335x_hdmi {
	unsigned			reglen;
	uintptr_t			regbase;
	unsigned			physbase;

	char				i2c_dev[MAX_STRLEN];
	int					i2c_fd;
	int					cable_state;
	int					hdmi_cap;

	int					chid;
	int					tid;
	int					coid;

	unsigned char		devid[4];

} am335x_hdmi_t;

/*
 * Omapl1xx private state
 */
typedef struct {
	disp_adapter_t	*adapter;
	uint32_t		*lregptr;
	int				free_context;
	unsigned		vsync_counter;
	unsigned		want_lcd_sync;

	/* LCD Interrupt handling */
	int				lcd_chid;
	int				lcd_coid;
	int				lcd_iid;
	unsigned		irq;
	struct sigevent	lcd_event;

	/* Internal LCD controller */
	disp_surface_t	*lcd_surf;
	unsigned		lcd_size;
	int				lcd_swap;
	int				lcd_update_dma;
	int				lcd_dma_on;
	int				lcd_enable;
	int				lcd_allocated;
	unsigned		lcd_status;

	/* Internal lcd controller */
	int				hfp, hbp, hsw, ppl;
	int				vfp, vbp, vsw, lpp;
	int				sync_ctrl, sync_edge;
	int				ivs;
	int				ihs;
	int				ipc;
	int				bias;
	int				acb;
	int				acbi;
	int				pcd;
	int				type;
	int				lr;
	int				tb;
	int				mode3;
	int				dith;
	int				display;
	int				lcd_xres;
	int				lcd_yres;

	/* i2c tlc59108 */
	int				fd;
	int				i2c_address;

	/* HDMI interface context */
	am335x_hdmi_t	*hdmi_dev;

	int				 err;
	int				 refresh;
	int				 pixel_format;

} omapl1xx_context_t;

void omapl1xx_module_info(disp_adapter_t *adp, disp_module_info_t *info);
int get_config_data(omapl1xx_context_t *p_ctx, const char *filename);

/* Mode switcher (layer control) */
int omapl1xx_layer_query(disp_adapter_t *adapter, int dispno, int layer_idx, int fmt_index, disp_layer_query_t * info);
int omapl1xx_layer_enable(disp_adapter_t *adapter, int dispno, int layer_idx);
int omapl1xx_layer_disable(disp_adapter_t *adapter, int dispno, int layer_idx);
int omapl1xx_layer_set_surface(disp_adapter_t *adapter, int dispno, int layer_idx, unsigned layer_format, disp_surface_t *surf[]);
int omapl1xx_layer_set_source_viewport(disp_adapter_t *adapter, int dispno, int layer_idx, int x1, int y1, int x2, int y2);
int omapl1xx_layer_set_dest_viewport(disp_adapter_t *adapter, int dispno, int layer_idx, int x1, int y1, int x2, int y2);
int omapl1xx_layer_set_blending(disp_adapter_t *adapter, int dispno, int layer_idx, unsigned alpha_mode, int m1, int m2);
int omapl1xx_layer_set_chromakey(disp_adapter_t *adapter, int dispno, int layer_idx, unsigned chroma_mode,
	disp_color_t color0, disp_color_t color1, disp_color_t mask);
int omapl1xx_layer_set_flags(disp_adapter_t *adapter, int dispno, int layer_idx, unsigned flag_mask, unsigned flag_values);
void omapl1xx_layer_update_begin(disp_adapter_t *adapter, int dispno, uint64_t layer_mask);
void omapl1xx_layer_update_end(disp_adapter_t *adapter, int dispno, uint64_t layer_mask, int wait_vsync);
void omapl1xx_layer_reset(disp_adapter_t *adapter, int dispno, int layer_idx);
int omapl1xx_layer_set_alpha_map(disp_adapter_t *adapter, int dispno, int layer_idx, disp_surface_t *map);

/* ISR functions */
inline int isr_setup(omapl1xx_context_t *omapl1xx);
inline void isr_cleanup(omapl1xx_context_t *omapl1xx);

#endif /* _OMAPL1xx_H_ */
