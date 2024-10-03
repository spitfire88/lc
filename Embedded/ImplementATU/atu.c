/*
    Design an ATU that translates 64bit input address from a peripheral to 32bit address for the memory.
    It has N channels, each channel can be programmed to translate a range of addresses from the peripheral to the memory.
    The ATU has the following registers:
    32bit base address register (BAR) that holds the base address of the memory
    32bit reserved
    32bit end address register (EAR) that holds the end address of the memory
    32bit reserved
    32bits corresponding to lower 64bits of the start of the peripheral address
    32bits corresponding to upper 64bits of the start of the peripheral address
    32bits corresponding to lower 64bits of the end of the peripheral address
    32bits corresponding to upper 64bits of the end of the peripheral address
    32bits attribute register that holds the attributes of the memory
    32bits reserved

    The ATU has the following operations:
    1. Initialize the ATU with N free channels with the base address of the memory
    2. When request comes from the peripheral, the ATU should translate the 64bit address to the 32bit memory address and assign a channel to the request
    3. When the request is completed, the channel should be freed

    If there are no free channels, the ATU should return an error
    If the peripheral address is not within the range of any channel, the ATU should return an error

    Translation examples:
    FFFF0000 60000000 -> 8008 1000
    FFFF0000 60000001 -> 8008 1001
    FFFF0000 60010000 -> 8009 0000
    FFFF0000 60010001 -> 8009 0001
    ...
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_CHANNELS 3

typedef struct {
    uint32_t base_address;
    uint32_t reserved1;
    uint32_t end_address;
    uint32_t reserved2;
    uint32_t lower_start_address;
    uint32_t upper_start_address;
    uint32_t lower_end_address;
    uint32_t upper_end_address;
    uint32_t attribute;
    uint32_t reserved3;
} ATU;

typedef struct {
    uint64_t start_address;
    uint64_t end_address;
    uint32_t attribute;
    uint32_t channel;
    ATU *atu;
} Channel;

typedef struct {
    Channel channels[MAX_CHANNELS];
    uint32_t free_channels;
} ATU_Controller;

int init_ATU(ATU_Controller *atu_controller) {
    for(int i = 0; i < MAX_CHANNELS; i++) {
        atu_controller->channels[i].start_address = 0;
        atu_controller->channels[i].end_address = 0;
        atu_controller->channels[i].attribute = 0;
        atu_controller->channels[i].channel = 0;
        atu_controller->channels[i].atu = (ATU *)malloc(sizeof(ATU));
        if(atu_controller->channels[i].atu == NULL)
            return -1;
    }
    atu_controller->free_channels = MAX_CHANNELS;
    return 0;
}


uint32_t translate_address(ATU_Controller *atu_controller, uint64_t peripheral_address) {
    return (peripheral_address & 0xFFFF0000) + 0x20080000 + (peripheral_address & 0xFFFF);
}

uint32_t assign_channel(ATU_Controller *atu_controller, uint64_t start_address, uint64_t end_address, uint32_t attribute) {
    
    uint32_t channel = 0;

    if (atu_controller->free_channels == 0) {
        return -1;
    }

    for (int i = 0; i < MAX_CHANNELS; i++) {
        if (atu_controller->channels[i].channel == 0) {
            atu_controller->channels[i].start_address = start_address;
            atu_controller->channels[i].end_address = end_address;
            atu_controller->channels[i].attribute = attribute;
            atu_controller->channels[i].channel = i + 1;
            channel = i + 1;
            atu_controller->free_channels--;
            goto translate;
        }
    }
    return -1;
translate:
    
    atu_controller->channels[channel - 1].atu->base_address = translate_address(atu_controller, start_address);
    atu_controller->channels[channel - 1].atu->end_address = translate_address(atu_controller, end_address);
    atu_controller->channels[channel - 1].atu->lower_start_address = start_address & 0xFFFFFFFF;
    atu_controller->channels[channel - 1].atu->upper_start_address = (start_address >> 32) & 0xFFFFFFFF;
    atu_controller->channels[channel - 1].atu->lower_end_address = end_address & 0xFFFFFFFF;
    atu_controller->channels[channel - 1].atu->upper_end_address = (end_address >> 32) & 0xFFFFFFFF;
    atu_controller->channels[channel - 1].atu->attribute = attribute || 0x1;
    return channel;
}

int free_channel(ATU_Controller *atu_controller, uint32_t channel) {
    if (channel > MAX_CHANNELS) {
        return -1;
    }
    atu_controller->channels[channel - 1].start_address = 0;
    atu_controller->channels[channel - 1].end_address = 0;
    atu_controller->channels[channel - 1].attribute = 0;
    atu_controller->channels[channel - 1].channel = 0;
    free(atu_controller->channels[channel - 1].atu);
    atu_controller->free_channels++;
    return 0;
}

int main() {
    ATU_Controller atu_controller;
    int ret = -1;
    
    ret = init_ATU(&atu_controller);
    if(ret < 0) {
        printf("Failed to initialize ATU\n");
        return -1;
    }

    uint32_t channel;
    assert(assign_channel(&atu_controller, 0xFFFF000060000000, 0xFFFF000060001000, 0) == 1);
    assert(assign_channel(&atu_controller, 0xFFFF000060010000, 0xFFFF000060011000, 0) == 2);
    assert(assign_channel(&atu_controller, 0xFFFF000060020000, 0xFFFF000060021000, 0) == 3);
    assert(assign_channel(&atu_controller, 0xFFFF000060030000, 0xFFFF000060031000, 0) == -1);
    free_channel(&atu_controller, 1);
    assert(assign_channel(&atu_controller, 0xFFFF000060030000, 0xFFFF000060031000, 0) == 1);
    return 0;
}