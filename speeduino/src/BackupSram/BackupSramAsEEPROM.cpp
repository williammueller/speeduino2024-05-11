#if defined(ARDUINO_BLACK_F407VE) 
#include "BackupSramAsEEPROM.h"

    BackupSramAsEEPROM::BackupSramAsEEPROM(){}
    int8_t BackupSramAsEEPROM::write_byte( uint8_t *data, uint16_t bytes, uint16_t offset ) {
        uint8_t* base_addr = (uint8_t *) BKPSRAM_BASE;
        uint16_t i;
          if( bytes + offset >= backup_size ) {
            /* ERROR : the last byte is outside the backup SRAM region */
            return -1;
          }
        
          //Enable the power interface clock
          RCC->APB1ENR |= RCC_APB1ENR_PWREN;
        
          //Enable the backup SRAM clock by setting BKPSRAMEN bit i
          RCC->AHB1ENR |= RCC_AHB1ENR_BKPSRAMEN; 
        
          /* disable backup domain write protection */
          //Set the Disable Backup Domain write protection (DBP) bit in PWR power control register
          PWR->CR |= PWR_CR_DBP;
        
          /** enable the backup regulator (used to maintain the backup SRAM content in
            * standby and Vbat modes).  NOTE : this bit is not reset when the device
            * wakes up from standby, system reset or power reset. You can check that
            * the backup regulator is ready on PWR->CSR.brr, see rm p144 */
          //enable backup power regulator this makes sram backup posible. bit is not reset by software!
          PWR->CSR |= PWR_CSR_BRE;
        
          for( i = 0; i < bytes; i++ ) {
            *(base_addr + offset + i) = *(data + i);
          }
          //Enable write protection backup sram when finished  
          PWR->CR &= ~PWR_CR_DBP;
          return 0;
        }
        
    int8_t BackupSramAsEEPROM::read_byte( uint8_t *data, uint16_t bytes, uint16_t offset ) {
        
          uint8_t* base_addr = (uint8_t *) BKPSRAM_BASE;
          uint16_t i;
          if( bytes + offset >= backup_size ) {
            /* ERROR : the last byte is outside the backup SRAM region */
            return -1;
          }
          //Enable the backup SRAM clock by setting BKPSRAMEN bit i
          RCC->AHB1ENR |= RCC_AHB1ENR_BKPSRAMEN; 
          
          for( i = 0; i < bytes; i++ ) {
            *(data + i) = *(base_addr + offset + i);
          }
          return 0;
        }
  
    uint8_t BackupSramAsEEPROM::read(uint16_t address) {

        uint8_t val = 0;
        read_byte(&val, 1, address);
      
        return val;
    }
    
    int8_t BackupSramAsEEPROM::write(uint16_t address, uint8_t val) {
        write_byte(&val, 1, address);   
        return 0;
    }
    
    int8_t BackupSramAsEEPROM::update(uint16_t address, uint8_t val) {
        write_byte(&val, 1, address);   
        return 0;
    }

BackupSramAsEEPROM EEPROM;

#endif

