
#define BigtoLittle16(A)                 ((((uint16_t)(A) & 0xff00) >> 8) | \
                                         (((uint16_t)(A) & 0x00ff) << 8))


#define BigtoLittle32(A)                 ((((uint32_t)(A) & 0xff000000) >> 24) | \
                                         (((uint32_t)(A) & 0x00ff0000) >> 8) | \
                                         (((uint32_t)(A) & 0x0000ff00) << 8) | \
                                         (((uint32_t)(A) & 0x000000ff) << 24))

