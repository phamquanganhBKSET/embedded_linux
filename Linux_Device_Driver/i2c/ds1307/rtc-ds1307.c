#include <linux/bcd.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/property.h>
#include <linux/rtc/ds1307.h>
#include <linux/rtc.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>
#include <linux/clk-provider.h>
#include <linux/regmap.h>
#include <linux/watchdog.h>
#include <linux/device.h>
#include <rtc-ds1307.h>


// DS1307's charateristics
struct ds1307 {
	struct device     *dev; // from device.h - characteristics of device
	struct regmap     *regmap; // from regmap.h
	const char        *name; // name of device
	struct rtc_device *rtc; // from rtc.h - characteristics of rtc_device
#ifdef CONFIG_COMMON_CLK
	struct clk_hw     clks[2];
#endif
}

// Chip discription
struct chip_desc {
	unsigned                   alarm:1;
	u16                        nvram_offset;
	u16                        nvram_size;
	u8                         offset; //register offset
	u8                         century_reg;
	u8                         century_enable_bit;
	u8                         century_bit;
	u8                         bbsqi_bit;
	irq_handle_t               irq_handler;
	const struct rtc_class_ops *rtc_ops;
	u16                        trickle_charger_reg;
	u8			   (*do_trickle_setup)(struct ds1307 *, u32,
						    bool);
	/* Does the RTC require trickle-resistor-ohms to select the value of
	 * the resistor between Vcc and Vbackup?
	 */
	bool			   requires_trickle_resistor;
	/* Some RTC's batteries and supercaps were charged by default, others
	 * allow charging but were not configured previously to do so.
	 * Remember this behavior to stay backwards compatible.
	 */
	bool			   charge_default;
}

//static const struct chip_desc chips[last_ds_type];

// DS1307's get time function
static int ds1307_get_time(struct device *dev, struct rtc_time *t) {
	struct ds1307 *ds1307 = dev_get_drvdata(dev);
	
}


