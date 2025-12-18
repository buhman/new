import sys
import struct

with open(sys.argv[1], 'rb') as f:
    buf = memoryview(f.read())

print("P2")
print("800 600")
print("65535")

for i in range(len(buf) // 4):
    b = buf[i*4:i*4+4]
    v, = struct.unpack("<f", b)
    assert v >= 0.0 and v <= 1.0

    depth16 = v * 65535

    print(depth16, end='')
    if i % 128 == 127:
        print("", end='\n')
    else:
        print("", end=" ")
