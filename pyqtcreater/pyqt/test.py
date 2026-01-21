def str_to_hex(s):
    # 文本转16进制
    a = ' '.join([hex(ord(c)).replace('0x', '') for c in s])
    a1 = a[0::3]
    a2 = a[1::3]
    a3 = a[2::3]
    sum = []
    for i in range(len(a1)):
        sum.append(a1[i]+a2[i])
    return sum


a = str_to_hex("hello")
print(a)
