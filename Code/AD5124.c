#include "AD5124.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      AD5124≥ı ºªØ
//  @param
//  @return     void
//  @since
//-------------------------------------------------------------------------------------------------------------------

void AD5124_init(uint8 RDAC0, uint8 RDAC1, uint8 RDAC2, uint8 RDAC3)
{

    simiic_init();
    simiic_write_reg(0x2F, 0x10, RDAC0);
    simiic_write_reg(0x2F, 0x11, RDAC1);
    simiic_write_reg(0x2F, 0x12, RDAC2);
    simiic_write_reg(0x2F, 0x13, RDAC3);
}


