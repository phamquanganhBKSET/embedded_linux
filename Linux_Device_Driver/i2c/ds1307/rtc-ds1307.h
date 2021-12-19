/* RTC registers don't differ much, except for the century flag */

// Secs register
#define DS1307_REG_SECS		0x00	/* 00-59 */

// Min register
#define DS1307_REG_MIN          0x01    /* 00-59 */

// Hour register
// The DS1307 can be run in either 12-hour or 24-hour mode
// Bit 6 of the hours register is defined as the 12- or 24-hour mode select bit
// When high, the 12-hour mode is selected
// In the 12-hour mode, bit 5 is the AM/PM bit with logic high being PM
// In the 24-hour mode, bit 5 is the second 10 hour bit (20-23 hours)
#define DS1307_REG_HOUR         0x02    /* 00-23, or 1-12{am,pm} */
#define DS1307_BIT_12HR         0x40    /* in REG_HOUR */ // Bit 6
#define DS1307_BIT_PM           0x20    /* in REG_HOUR */ // Bit 5

// The contents of the time and calendar registers are in the BCD format
// Bit 7 of register 0 is the clock halt (CH) bit
// When this bit is set to a 1, the oscillator is disabled. When cleared to a 0, the oscillator is enabled
#define DS1307_BIT_CH		0x80 // = 1 0 0 0 0 0 0 0
                                     //   |

// Day register: Day in week
#define DS1307_REG_WDAY		0x03	/* 01-07 */

// Date register
#define DS1307_REG_MDAY		0x04	/* 01-31 */

// Month register
#define DS1307_REG_MONTH	0x05	/* 01-12 */

// Year register
#define DS1307_REG_YEAR		0x06	/* 00-99 */

// Control register
// Structure:
// BIT7      BIT6      BIT5      BIT4      BIT3      BIT2      BIT1      BIT0
// OUT        0         0        SQWE       0         0        RS1       RS0
// OUT (Output control): This bit controls the output level of the SQW/OUT pin when the square wave output is disabled.

// SQWE (Square Wave Enable): This bit, when set to a logic 1, will enable the oscillator output.
// The frequency of the square wave output depends upon the value of the RS0 and RS1 bits.
// With the square wave output set to 1Hz, the clock registers update on the falling edge of the square wave

// RS (Rate Select): These bits control the frequency of the square wave output when the square wave output has been enabled
#define DS1307_REG_CONTROL	0x07

// Bit out register
#define DS1307_BIT_OUT		0x80 // = 1 0 0 0 0 0 0 0
				     //   |

// Bit SQWE of Control register: SQWE = 0 -> the logic level on the SQW/OUT pin is 1 if OUT = 1 and is 0 if OUT = 0
#define DS1307_BIT_SQWE		0x10 // = 0 0 0 1 0 0 0 0
				     //         |

// Bit RS1 of Control register
#define DS1307_BIT_RS1		0x02 // = 0 0 0 0 0 0 1 0
				     //               |

// Bit RS0 of Control register
#define DS1307_BIT_RS0		0x01 // = 0 0 0 0 0 0 0 1
				     //                 |

#define DS13XX_TRICKLE_CHARGER_MAGIC	0xa0
