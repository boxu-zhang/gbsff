# GBSFF - Generate Byte Stream from Function
This tool is used to generate a byte stream from a predefined function. All bytes in the stream are the values of 'f(x)', where 'f' is a predefined function, and 'x' is the offset of the stream, which start from zero. Please note, the function should produce deterministic byte value rather than random, because random value are hard to verify the correctness.(It could be done through statistic analysis)

## Predefined functions
The tool has some predefined functions listed below.

### Flip Coin Function
This function produces 0 or 1 for each bytes according to the offset's odevity. If offset is odd, it produce 0. Or else, it produces 1.

### Loop increament
This function produces 0x00 ~ 0xFF value. The calculation is simple, use 'offset mod 0x100' directly. 
