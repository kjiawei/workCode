/*
 *  Board-specific setup code for the AT91SAM9M10G45 Evaluation Kit family
 *
 *  Covers: * AT91SAM9G45-EKES  board
 *          * AT91SAM9M10G45-EK board
 *
 *  Copyright (C) 2009 Atmel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#include <linux/types.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include <linux/fb.h>
#include <linux/gpio_keys.h>
#include <linux/input.h>
#include <linux/leds.h>
#include <linux/clk.h>
#include <linux/atmel-mci.h>
#include <linux/i2c/at24.h>


#include <mach/hardware.h>
#include <video/atmel_lcdc.h>

#include <asm/setup.h>
#include <asm/mach-types.h>
#include <asm/irq.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>

#include <mach/hardware.h>
#include <mach/board.h>
#include <mach/gpio.h>
#include <mach/at91sam9_smc.h>
#include <mach/at91_shdwc.h>

#if defined(CONFIG_CAN_MCP2515) || defined(CONFIG_CAN_MCP2515_MODULE)
#include <linux/can/can.h>
#include <linux/can/mcp251x.h>
#endif
 #if defined(CONFIG_TOUCHSCREEN_ADS7846) || defined(CONFIG_TOUCHSCREEN_ADS7846_MODULE)
#include <linux/spi/ads7846.h>
 #endif
#include "sam9_smc.h"
#include "generic.h"


static void __init ek_map_io(void)
{
	/* Initialize processor: 12.000 MHz crystal */
	at91sam9g45_initialize(12000000);

	/* DGBU on ttyS0. (Rx & Tx only) */
	at91_register_uart(0, 0, 0);

	/* USART0 on ttyS1. (Rx, Tx) */
	//at91_register_uart(AT91SAM9G45_ID_US0, 1, ATMEL_UART_RTS);
	at91_register_uart(AT91SAM9G45_ID_US0, 1, 0);
	/* USART1 on ttyS2. (Rx, Tx, RTS, CTS) */
	//at91_register_uart(AT91SAM9G45_ID_US1, 2, ATMEL_UART_CTS | ATMEL_UART_RTS);
	at91_register_uart(AT91SAM9G45_ID_US1, 2, 0);
	/* USART2 on ttyS3. (Rx, Tx) */
    //at91_register_uart(AT91SAM9G45_ID_US2, 3, ATMEL_UART_RTS);
    at91_register_uart(AT91SAM9G45_ID_US2, 3, 0);
    
	/* USART3 on ttyS4. (Rx, Tx) */
	at91_register_uart(AT91SAM9G45_ID_US3, 4, 0);

	/* set serial console to ttyS0 (ie, DBGU) */
	at91_set_serial_console(0);
}

static void __init ek_init_irq(void)
{
	at91sam9g45_init_interrupts(NULL);
}


/*
 * USB HS Host port (common to OHCI & EHCI)
 */
static struct at91_usbh_data __initdata ek_usbh_hs_data = {
	.ports		= 2,
	.vbus_pin	= {AT91_PIN_PD1, AT91_PIN_PD3},
};


/*
 * USB HS Device port
 */
static struct usba_platform_data __initdata ek_usba_udc_data = {
	.vbus_pin	= AT91_PIN_PC0,
};

/*
 * SPI devices.
 */

/*
 * ADS7846 Touchscreen
 */
#if defined(CONFIG_TOUCHSCREEN_ADS7846) || defined(CONFIG_TOUCHSCREEN_ADS7846_MODULE)
static int ads7843_pendown_state(void)
{
	return !at91_get_gpio_value(AT91_PIN_PD22);	/* Touchscreen PENIRQ */
}

static struct ads7846_platform_data ads_info = {
	.model			= 7843,
	.x_max			= 1024,
	.y_max			= 768,
	.vref_delay_usecs	= 100,
	.x_plate_ohms		= 450,
	.y_plate_ohms		= 250,
	.pressure_max		= 255,
	.debounce_max		= 10,
	.debounce_rep		= 1,
	.debounce_tol		= 3,
	.get_pendown_state	= ads7843_pendown_state,
};

static void __init ek_add_device_ts(void)
{
	at91_set_gpio_input(AT91_PIN_PD22, 1);	/* EXT PULLUP*/
	//at91_set_gpio_input(AT91_PIN_PD23, 1);	/* Not use Touchscreen BUSY signal */
}
#else
static void __init ek_add_device_ts(void) {}
#endif

#if defined(CONFIG_CAN_MCP2515) || defined(CONFIG_CAN_MCP2515_MODULE)
static struct mcp251x_platform_data mcp251x_data = {
	.f_osc = 16000000UL,
};
#endif
static struct spi_board_info ek_spi_devices[] = 
{
    {	/* DataFlash chip */
	.modalias	= "mtd_dataflash",
	.chip_select	= 0,
        .max_speed_hz	= 15 * 1000 * 1000,
	.bus_num	= 0,
    },
#if defined(CONFIG_CAN_MCP2515) || defined(CONFIG_CAN_MCP2515_MODULE)

#if 0   // 1 for EK board, 0 for our JH5805 mainboard.
	{
        .modalias = "mcp2515",
        .chip_select = 1,
        .irq = AT91_PIN_PD25,
        .platform_data = &mcp251x_data,
        .max_speed_hz = 5 * 1000 * 1000,
        .bus_num = 0,
        .mode = 0,
    },
#endif

    {

        .modalias = "mcp2515",
        .chip_select = 2,
        .irq = AT91_PIN_PD26,
        .platform_data = &mcp251x_data,
        .max_speed_hz = 5 * 1000 * 1000,
	 .bus_num = 1,
	 .mode = 0,

	},

#endif
#if defined(CONFIG_TOUCHSCREEN_ADS7846) || defined(CONFIG_TOUCHSCREEN_ADS7846_MODULE)
    {
        .modalias	= "ads7846",
	.chip_select	= 1,
	//.max_speed_hz	= 125000 * 26,	/* (max sample rate @ 3V) * (cmd + data + overhead) */
	.max_speed_hz	= 1000 * 600,
	.bus_num	= 0,
	.platform_data	= &ads_info,
	//.irq		= AT91SAM9G45_ID_IRQ0,
	.irq		= AT91_PIN_PD22,    //use I/O interrupt
    },
#endif
	

};


/*
 * MCI (SD/MMC)
 */
static struct mci_platform_data __initdata mci0_data = {
	.slot[0] = {
		.bus_width	= 4,
		.detect_pin	= AT91_PIN_PD10,
		.wp_pin		= -1,
	},
};


static struct mci_platform_data __initdata mci1_data = {
	.slot[0] = {
		.bus_width	= 8,
		.detect_pin	= AT91_PIN_PB29,
		.wp_pin		= AT91_PIN_PB30,
	},
};


/*
 * MACB Ethernet device
 */
static struct at91_eth_data __initdata ek_macb_data = {
	.phy_irq_pin	= AT91_PIN_PD5,
	.is_rmii	= 1,
};


/*
 * NAND flash
 */
static struct mtd_partition __initdata ek_nand_partition[] = {
	{
		.name	= "bootstrap",
		.offset	= 0,
		.size	= 0x20000,
	},
	{
		.name	= "u-boot",
		.offset	= 0x20000,
		.size	= 0x40000,
	},
	{
		.name	= "para1",
		.offset	= 0x60000,
		.size	= 0x20000,
	},
	{
		.name	= "para2",
		.offset	= 0x80000,
		.size	= 0x20000,
	},
	{
		.name	= "logo",
		.offset	= 0xa0000,
		.size	= 0x100000,
	},
	{
		.name	= "kernel",
		.offset	= 0x1a0000,
		.size	= 0x300000,
	},
	{
		.name	= "rootfs",
		.offset	= 0x4a0000,
		.size	= 0x1000000,
	},
	
	{
		.name	= "appfs",
		.offset = 0x14a0000,
		.size	= 0x3000000,
	},
	{
		.name	= "data",
		.offset	= MTDPART_OFS_NXTBLK,
		.size	= MTDPART_SIZ_FULL,
	},
};

static struct mtd_partition * __init nand_partitions(int size, int *num_partitions)
{
	*num_partitions = ARRAY_SIZE(ek_nand_partition);
	return ek_nand_partition;
}

/* det_pin is not connected */
static struct atmel_nand_data __initdata ek_nand_data = {
	.ale		= 21,
	.cle		= 22,
	.rdy_pin	= AT91_PIN_PC8,
	.enable_pin	= AT91_PIN_PC14,
	.partition_info	= nand_partitions,
#if defined(CONFIG_MTD_NAND_AT91_BUSWIDTH_16)
	.bus_width_16	= 1,
#else
	.bus_width_16	= 0,
#endif
};

static struct sam9_smc_config __initdata ek_nand_smc_config = {
	.ncs_read_setup		= 0,
	.nrd_setup		= 2,
	.ncs_write_setup	= 0,
	.nwe_setup		= 2,

	.ncs_read_pulse		= 4,
	.nrd_pulse		= 4,
	.ncs_write_pulse	= 4,
	.nwe_pulse		= 4,

	.read_cycle		= 7,
	.write_cycle		= 7,

	.mode			= AT91_SMC_READMODE | AT91_SMC_WRITEMODE | AT91_SMC_EXNWMODE_DISABLE,
	.tdf_cycles		= 3,
};

static void __init ek_add_device_nand(void)
{
	/* setup bus-width (8 or 16) */
	if (ek_nand_data.bus_width_16)
		ek_nand_smc_config.mode |= AT91_SMC_DBW_16;
	else
		ek_nand_smc_config.mode |= AT91_SMC_DBW_8;

	/* configure chip-select 3 (NAND) */
	sam9_smc_configure(3, &ek_nand_smc_config);

	at91_add_device_nand(&ek_nand_data);
}


/*
 * LCD Controller
 */
#if defined(CONFIG_FB_ATMEL) || defined(CONFIG_FB_ATMEL_MODULE)
static struct fb_videomode at91_tft_vga_modes;

static struct fb_videomode __initdata at91_tft_vga_modes_arrary[] = {
	{
		.name           = "TX09D50VM1CCA @ 70",
		.refresh	= 60,
		.xres		= 480,		.yres		= 272,
		.pixclock	= KHZ2PICOS(10000),

		.left_margin	= 1,		.right_margin	= 1,
		.upper_margin	= 40,		.lower_margin	= 1,
		.hsync_len	= 45,		.vsync_len	= 1,

		.sync		= 0,
		.vmode		= FB_VMODE_NONINTERLACED,
	},
	{
		.name           = "TX09D50VM1CCA @ 70",
		.refresh	= 60,
		.xres		= 800,		.yres		= 480,
		.pixclock	= KHZ2PICOS(34000),

		.left_margin	= 17,		.right_margin	= 11,
		.upper_margin	= 4,		.lower_margin	= 7,
		.hsync_len	= 5,		.vsync_len	= 1,

		.sync		= 0,
		.vmode		= FB_VMODE_NONINTERLACED,
	},
	{
		.name           = "TX09D50VM1CCA @ 70",
		.refresh	= 60,
		.xres		= 800,		.yres		= 600,
		.pixclock	= KHZ2PICOS(40000),

		.left_margin	= 1,		.right_margin	= 1,
		.upper_margin	= 40,		.lower_margin	= 1,
		.hsync_len	= 45,		.vsync_len	= 1,

		.sync		= 0,
		.vmode		= FB_VMODE_NONINTERLACED,
	},
	{
		.name           = "LB150X02-TL01 @ 60",  /* Added by YuYinghui 2012.02.22 */
		.refresh		= 60,
		.xres			= 1024,		
		.yres			= 768,
		.pixclock		= KHZ2PICOS(66000), /* 44000 for 24bit and 66000 for 16bit */

		.left_margin	= 300,		
		.right_margin	= 20,
		.upper_margin	= 30,		
		.lower_margin	= 8,
		.hsync_len		= 40,		
		.vsync_len		= 20,

		.sync			= FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,//0,
		.vmode			= FB_VMODE_NONINTERLACED,
	},
};

static struct fb_monspecs at91fb_default_monspecs = {
	.manufacturer	= "HIT",
	.monitor        = "TX09D70VM1CCA",

	.modedb		=  &at91_tft_vga_modes,
	.modedb_len	=  1,
	.hfmin		= 15000,
	.hfmax		= 17640,
	.vfmin		= 57,
	.vfmax		= 67,
};

#define AT91SAM9G45_DEFAULT_LCDCON2 	(ATMEL_LCDC_MEMOR_LITTLE \
					| ATMEL_LCDC_DISTYPE_TFT \
					| ATMEL_LCDC_IFWIDTH_16 \
					| ATMEL_LCDC_CLKMOD_ALWAYSACTIVE)

/* Driver datas */
static struct atmel_lcdfb_info __initdata ek_lcdc_data = {
	.lcdcon_is_backlight	= true,
	.default_bpp			= 16,
	.default_dmacon			= ATMEL_LCDC_DMAEN,
	.default_lcdcon2		= AT91SAM9G45_DEFAULT_LCDCON2,
	.default_monspecs		= &at91fb_default_monspecs,
	.guard_time				= 9,
	.lcd_wiring_mode		= ATMEL_LCDC_WIRING_RGB,
};

void __init ek_fb_set_platdata(int default_display)
{
	memcpy(&at91_tft_vga_modes, &at91_tft_vga_modes_arrary[default_display], sizeof(at91_tft_vga_modes));
}

#else
static struct atmel_lcdfb_info __initdata ek_lcdc_data;
void __init ek_fb_set_platdata(int default_display){}
#endif

/*
 * I2C devices
 */
static struct at24_platform_data at24c16 = {
    .byte_len   = 2 * SZ_1K / 8,
    .page_size  = 256,
    .flags      = 0,
};

struct sc16is752_platform_data {
    int fifosize;
    int clk;
    int irq;
} sc16is752_data = {
	.fifosize = 64,
	.clk = 18432000,
	.irq = AT91_PIN_PC1,
};


static struct i2c_board_info __initdata ek_i2c_devices[] = {
    {
        I2C_BOARD_INFO("24c02", 0x50),
        .platform_data = &at24c16,
    },
    {
        I2C_BOARD_INFO("ds3231", 0xd0 >> 1),
    },
};


static struct i2c_board_info __initdata i2c_devices_1[] = {
    {
        I2C_BOARD_INFO("jmy680ai", 0x50),
        .addr = 0x50,
    },
};


/*
 * Touchscreen
 */
 #if defined(CONFIG_TOUCHSCREEN_ATMEL_TSADCC) || defined(CONFIG_TOUCHSCREEN_ATMEL_TSADCC_MODULE)
static struct at91_tsadcc_data ek_tsadcc_data = {
    .adc_clock              = 350000,
    .pendet_debounce        = 0x0d,
    .ts_sample_hold_time    = 0x0a,
};
#endif
/*
 * GPIO Buttons
 */
#if defined(CONFIG_KEYBOARD_GPIO) || defined(CONFIG_KEYBOARD_GPIO_MODULE)
static struct gpio_keys_button ek_buttons[] = {
};

static struct gpio_keys_platform_data ek_button_data = {
	.buttons	= ek_buttons,
	.nbuttons	= ARRAY_SIZE(ek_buttons),
};

static struct platform_device ek_button_device = {
	.name		= "gpio-keys",
	.id		= -1,
	.num_resources	= 0,
	.dev		= {
		.platform_data	= &ek_button_data,
	}
};

static void __init ek_add_device_buttons(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(ek_buttons); i++) {
		at91_set_GPIO_periph(ek_buttons[i].gpio, 1);
		at91_set_deglitch(ek_buttons[i].gpio, 1);
	}

	platform_device_register(&ek_button_device);
}
#else
static void __init ek_add_device_buttons(void) {}
#endif


/*
 * AC97
 * reset_pin is not connected: NRST
 */
static struct ac97c_platform_data ek_ac97_data = {
	.reset_pin	 = AT91_PIN_PD27, 
};


/*
 * LEDs ... these could all be PWM-driven, for variable brightness
 */
static struct gpio_led ek_leds[] = {
	{	/* "top" led, red, powerled */
		.name			= "d4",
		.gpio			= AT91_PIN_PD0,
		.default_trigger	= "heartbeat",
	},
	{	/* "left" led, green, userled2, pwm3 */
		.name			= "d7",
		.gpio			= AT91_PIN_PB21,
		.active_low		= 1,
		.default_trigger	= "none",
	},
	{	/* "right" led, green, userled1, pwm1 */
		.name			= "d8",
		.gpio			= AT91_PIN_PB22,
		.active_low		= 1,
		.default_trigger	= "none",
	},
	{	/* "right" led, green, userled1, pwm1 */
		.name			= "beep",
		.gpio			= AT91_PIN_PB20,
		.active_low		= 1,
		.default_trigger	= "none",
	},

	{	/* "right" led, green, userled1, pwm1 */
		.name			= "gprs_reset",
		.gpio			= AT91_PIN_PB23,
		.active_low		= 1,
		.default_trigger	= "none",
	},

	{	/* "right" led, green, userled1, pwm1 */
		.name			= "gprs_onoff",
		.gpio			= AT91_PIN_PB24,
		.active_low		= 1,
		.default_trigger	= "none",
	},
	{	/* 3G module */
		.name           = "3g_reset",
		.gpio           = AT91_PIN_PB25,
		.default_trigger    = "none",
	},
	{	/* 2017.11.13 add jkCodi */
		.name           = "lvds_onoff",
		.gpio           = AT91_PIN_PB28,
		.default_trigger    = "none",
		.active_low          = 0,
	},
};


/*
 * PWM Leds
 */
static struct gpio_led ek_pwm_led[] = {
#if defined(CONFIG_LEDS_ATMEL_PWM) || defined(CONFIG_LEDS_ATMEL_PWM_MODULE)
	{	/* "right" led, green, userled1, pwm1 */
		.name			= "d9",
		.gpio			= 1,	/* is PWM channel number */
		.active_low		= 1,
		.default_trigger	= "none",
	},
#endif
};

static char tft_type = 's';

static int __init ek_tft_setup(char *str)
{
    tft_type = str[0];
    return 1;
}

__setup("tft=", ek_tft_setup);


static void __init ek_board_init(void)
{
	int default_display = 0;
	switch (tft_type) {
    case 's': /* small or production */
        default_display = 0;
        break;
    case 'm': /* middle */
        default_display = 1;
        break;
	case 'L': /* for 1024*768, Added by YuYinghui 2012.02.22 */
		default_display = 3;
		break;
    case 'b': /* big */
    default:
        default_display = 2;
        break;
    }
    ek_fb_set_platdata(default_display);

	/* Serial */
	at91_add_device_serial();
	/* USB HS Host */
	at91_add_device_usbh_ohci(&ek_usbh_hs_data);
	at91_add_device_usbh_ehci(&ek_usbh_hs_data);
	/* USB HS Device */
	at91_add_device_usba(&ek_usba_udc_data);
	//add by siwei 20120925
	/* Touch Screen */
	#if defined(CONFIG_TOUCHSCREEN_ADS7846) || defined(CONFIG_TOUCHSCREEN_ADS7846_MODULE)	
	ek_add_device_ts();
	#endif
	#if defined(CONFIG_TOUCHSCREEN_ATMEL_TSADCC) || defined(CONFIG_TOUCHSCREEN_ATMEL_TSADCC_MODULE)
	at91_add_device_tsadcc(&ek_tsadcc_data);
	#endif
	/* SPI */
	at91_add_device_spi(ek_spi_devices, ARRAY_SIZE(ek_spi_devices));
	
	/* MMC0 */
	at91_add_device_mci(0, &mci0_data);
	/* MMC1 */
	at91_add_device_mci(1, &mci1_data);
	/* Ethernet */
	at91_add_device_eth(&ek_macb_data);
	/* NAND */
	ek_add_device_nand();
	/* I2C */
	at91_add_device_i2c(0, ek_i2c_devices, ARRAY_SIZE(ek_i2c_devices));
	at91_add_device_i2c(1, i2c_devices_1, ARRAY_SIZE(i2c_devices_1));
	/* LCD Controller */
	at91_add_device_lcdc(&ek_lcdc_data);
	/* Push Buttons */
	ek_add_device_buttons();
	/* AC97 */
	at91_add_device_ac97(&ek_ac97_data);
	/* LEDs */
	at91_gpio_leds(ek_leds, ARRAY_SIZE(ek_leds));
	at91_pwm_leds(ek_pwm_led, ARRAY_SIZE(ek_pwm_led));

        //init jmy680ai icc pin 
        at91_set_gpio_input(AT91_PIN_PA26, 1);	/* EXT PULLUP*/
}


#if defined(CONFIG_MACH_AT91SAM9G45EKES)
MACHINE_START(AT91SAM9G45EKES, "Atmel AT91SAM9G45-EKES")
#else
MACHINE_START(AT91SAM9M10EKES, "Atmel AT91SAM9M10-EKES")
#endif
	/* Maintainer: Atmel */
	.phys_io	= AT91_BASE_SYS,
	.io_pg_offst	= (AT91_VA_BASE_SYS >> 18) & 0xfffc,
	.boot_params	= AT91_SDRAM_BASE + 0x100,
	.timer		= &at91sam926x_timer,
	.map_io		= ek_map_io,
	.init_irq	= ek_init_irq,
	.init_machine	= ek_board_init,
MACHINE_END
