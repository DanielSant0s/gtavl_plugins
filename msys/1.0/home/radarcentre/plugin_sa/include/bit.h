#define set_bit(val, bit) val |= 1 << bit
#define clear_bit(val, bit) val &= ~(1 << bit)
#define test_bit(val, bit) (val & (1 << bit)) != 0
