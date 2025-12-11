import sys

filename = sys.argv[1]
data = open(filename, "rb").read()

print("const uint8_t index_html[] = {")
for i, b in enumerate(data):
    end = "," if i < len(data)-1 else ""
    print(f"0x{b:02x}{end}", end="")
    if (i+1) % 20 == 0:
        print()
print("};")
print(f"unsigned int index_html_len = {len(data)};")
