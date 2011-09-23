#ifndef __EEPROM_H
#define __EEPROM_H

#include "typedef.h"
void SectorErase(uint sector_addr);
uchar byte_read(uint byte_addr);
void byte_write(uint byte_addr, uchar original_data);
void LoadFromEEPROM();
void SaveToEEPROM();
void LoadProbeData();
void SaveProbeData();
#endif