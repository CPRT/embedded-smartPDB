# embedded-smartPDB

I2C Address: 0x21

Multi-byte writes are supported -- e.g. writing 0x000123 will set SERVO1_HIGH to 0x01 and SERVO1_LOW to 0x23

NOTE: Servo 4 is disabled on rev. 1 due to hardware issues.

NOTE: Rev. 1 servo headers do not follow standard servo pinout, be careful when plugging in.

Register Map:
| Address | Name | Description |
| ---------- | ---------- | ---------- |
| 0x00 | SERVO1_HIGH | Higher 8 bits of SERVO1 output value (us) |
| 0x01 | SERVO1_LOW  | Lower 8 bits of SERVO1 output value (us) |
| 0x02 | SERVO2_HIGH | Higher 8 bits of SERVO2 output value (us) |
| 0x03 | SERVO2_LOW  | Lower 8 bits of SERVO2 output value (us) |
| 0x04 | SERVO3_HIGH | Higher 8 bits of SERVO3 output value (us) |
| 0x05 | SERVO3_LOW  | Lower 8 bits of SERVO3 output value (us) |
| 0x06 | SERVO4_HIGH | Higher 8 bits of SERVO4 output value (us) |
| 0x07 | SERVO4_LOW  | Lower 8 bits of SERVO4 output value (us) |
| 0x08 | TOGGLE      | Efuse toggle pin output, active low (high turns off) |
| 0x09 | REGPG       | Efuse power good signal, active high |

### TOGGLE and REGPG bit mapping

| Bit 8 | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 |
| -- | -- | -- | -- | -- | -- | -- | -- |
| NC | 5V Out 3 | 5V Out 2 | 5V Out 1 | 12V Out 4 | 12V Out 3 | 12V Out 2 | 12V Out 1 |
