/*******************************************************************************
* File Name: SPI_ss_s.h  
* Version 2.0
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_SPI_ss_s_H) /* Pins SPI_ss_s_H */
#define CY_PINS_SPI_ss_s_H

#include "cytypes.h"
#include "cyfitter.h"
#include "SPI_ss_s_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    SPI_ss_s_Write(uint8 value) ;
void    SPI_ss_s_SetDriveMode(uint8 mode) ;
uint8   SPI_ss_s_ReadDataReg(void) ;
uint8   SPI_ss_s_Read(void) ;
uint8   SPI_ss_s_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define SPI_ss_s_DRIVE_MODE_BITS        (3)
#define SPI_ss_s_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - SPI_ss_s_DRIVE_MODE_BITS))
#define SPI_ss_s_DRIVE_MODE_SHIFT       (0x00u)
#define SPI_ss_s_DRIVE_MODE_MASK        (0x07u << SPI_ss_s_DRIVE_MODE_SHIFT)

#define SPI_ss_s_DM_ALG_HIZ         (0x00u << SPI_ss_s_DRIVE_MODE_SHIFT)
#define SPI_ss_s_DM_DIG_HIZ         (0x01u << SPI_ss_s_DRIVE_MODE_SHIFT)
#define SPI_ss_s_DM_RES_UP          (0x02u << SPI_ss_s_DRIVE_MODE_SHIFT)
#define SPI_ss_s_DM_RES_DWN         (0x03u << SPI_ss_s_DRIVE_MODE_SHIFT)
#define SPI_ss_s_DM_OD_LO           (0x04u << SPI_ss_s_DRIVE_MODE_SHIFT)
#define SPI_ss_s_DM_OD_HI           (0x05u << SPI_ss_s_DRIVE_MODE_SHIFT)
#define SPI_ss_s_DM_STRONG          (0x06u << SPI_ss_s_DRIVE_MODE_SHIFT)
#define SPI_ss_s_DM_RES_UPDWN       (0x07u << SPI_ss_s_DRIVE_MODE_SHIFT)

/* Digital Port Constants */
#define SPI_ss_s_MASK               SPI_ss_s__MASK
#define SPI_ss_s_SHIFT              SPI_ss_s__SHIFT
#define SPI_ss_s_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define SPI_ss_s_PS                     (* (reg32 *) SPI_ss_s__PS)
/* Port Configuration */
#define SPI_ss_s_PC                     (* (reg32 *) SPI_ss_s__PC)
/* Data Register */
#define SPI_ss_s_DR                     (* (reg32 *) SPI_ss_s__DR)
/* Input Buffer Disable Override */
#define SPI_ss_s_INP_DIS                (* (reg32 *) SPI_ss_s__PC2)


#if defined(SPI_ss_s__INTSTAT)  /* Interrupt Registers */

    #define SPI_ss_s_INTSTAT                (* (reg32 *) SPI_ss_s__INTSTAT)

#endif /* Interrupt Registers */

#endif /* End Pins SPI_ss_s_H */


/* [] END OF FILE */
