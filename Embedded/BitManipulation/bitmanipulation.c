#include <stdint.h>
#include <stdio.h>

#define BITFIELD_ID(val) (((val) >> 24) & 0xF)
#define BITFIELD_NOPKTS(val) (((val) >> 20) & 0xF)
#define BITFIELD_PERM(val) (((val) >> 16) & 0XF)
#define BITFIELD_MSCHAR(val) (((val) >> 8) & 0xFF)
#define BITFIELD_LSCHAR(val) (val & 0xFF)

struct Packet {
    int id;
    int numOfPkts;
    int permission;
    char msc;
    char lsc;
};


int main() {
    int values[] = {0x1216463, 0x1006463};

    for(int i = 0; i < (sizeof(values)/sizeof(values[0])); i++) {
        struct Packet p = {
            .id = BITFIELD_ID(values[i]),
            .numOfPkts = BITFIELD_NOPKTS(values[i]),
            .permission = BITFIELD_PERM(values[i]),
            .msc = BITFIELD_MSCHAR(values[i]),
            .lsc = BITFIELD_LSCHAR(values[i]),
        };
        printf("Bitfields of 0x%x are ID:%d, NoPkts:%d, permission:%d, CHAR1:%c CHAR0:%c \n", values[i], p.id, p.numOfPkts, p.permission, p.msc, p.lsc);
    }

    return 0;
}