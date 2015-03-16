/*******************************************************************************
* File Name: SPI.c
* Version 1.20
*
* Description:
*  This file provides the source code to the API for the SCB Component.
*
* Note:
*
*******************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SPI_PVT.h"

#if(SPI_SCB_MODE_I2C_INC)
    #include "SPI_I2C_PVT.h"
#endif /* (SPI_SCB_MODE_I2C_INC) */

#if(SPI_SCB_MODE_EZI2C_INC)
    #include "SPI_EZI2C_PVT.h"
#endif /* (SPI_SCB_MODE_EZI2C_INC) */

#if(SPI_SCB_MODE_SPI_INC || SPI_SCB_MODE_UART_INC)
    #include "SPI_SPI_UART_PVT.h"
#endif /* (SPI_SCB_MODE_SPI_INC || SPI_SCB_MODE_UART_INC) */


/***************************************
*    Run Time Configuration Vars
***************************************/

/* Stores internal component configuration for unconfigured mode */
#if(SPI_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common config vars */
    uint8 SPI_scbMode = SPI_SCB_MODE_UNCONFIG;
    uint8 SPI_scbEnableWake;
    uint8 SPI_scbEnableIntr;

    /* I2C config vars */
    uint8 SPI_mode;
    uint8 SPI_acceptAddr;

    /* SPI/UART config vars */
    volatile uint8 * SPI_rxBuffer;
    uint8  SPI_rxDataBits;
    uint32 SPI_rxBufferSize;

    volatile uint8 * SPI_txBuffer;
    uint8  SPI_txDataBits;
    uint32 SPI_txBufferSize;

    /* EZI2C config vars */
    uint8 SPI_numberOfAddr;
    uint8 SPI_subAddrSize;
#endif /* (SPI_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Common SCB Vars
***************************************/

uint8 SPI_initVar = 0u;

#if !defined (CY_REMOVE_SPI_CUSTOM_INTR_HANDLER)
    cyisraddress SPI_customIntrHandler = NULL;
#endif /* !defined (CY_REMOVE_SPI_CUSTOM_INTR_HANDLER) */


/***************************************
*    Private Function Prototypes
***************************************/

static void SPI_ScbEnableIntr(void);
static void SPI_ScbModeStop(void);


/*******************************************************************************
* Function Name: SPI_Init
********************************************************************************
*
* Summary:
*  Initializes the SCB component to operate in one of the selected configurations:
*  I2C, SPI, UART or EZ I2C.
*  When the configuration is set to “Unconfigured SCB”, this function does
*  not do any initialization. Use mode-specific initialization APIs instead:
*  SCB_I2CInit, SCB_SpiInit, SCB_UartInit or SCB_EzI2CInit.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SPI_Init(void)
{
#if(SPI_SCB_MODE_UNCONFIG_CONST_CFG)
    if(SPI_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        SPI_initVar = 0u; /* Clear init var */
    }
    else
    {
        /* Initialization was done before call */
    }

#elif(SPI_SCB_MODE_I2C_CONST_CFG)
    SPI_I2CInit();

#elif(SPI_SCB_MODE_SPI_CONST_CFG)
    SPI_SpiInit();

#elif(SPI_SCB_MODE_UART_CONST_CFG)
    SPI_UartInit();

#elif(SPI_SCB_MODE_EZI2C_CONST_CFG)
    SPI_EzI2CInit();

#endif /* (SPI_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: SPI_Enable
********************************************************************************
*
* Summary:
*  Enables the SCB component operation.
*  The SCB configuration should be not changed when the component is enabled.
*  Any configuration changes should be made after disabling the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SPI_Enable(void)
{
#if(SPI_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Enable SCB block, only if it is already configured */
    if(!SPI_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        SPI_CTRL_REG |= SPI_CTRL_ENABLED;

        SPI_ScbEnableIntr();
    }
#else
    SPI_CTRL_REG |= SPI_CTRL_ENABLED;

    SPI_ScbEnableIntr();
#endif /* (SPI_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: SPI_Start
********************************************************************************
*
* Summary:
*  Invokes SCB_Init() and SCB_Enable().
*  After this function call, the component is enabled and ready for operation.
*  When configuration is set to “Unconfigured SCB”, the component must first be
*  initialized to operate in one of the following configurations: I2C, SPI, UART
*  or EZ I2C. Otherwise this function does not enable the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  SPI_initVar - used to check initial configuration, modified
*  on first function call.
*
*******************************************************************************/
void SPI_Start(void)
{
    if(0u == SPI_initVar)
    {
        SPI_Init();
        SPI_initVar = 1u; /* Component was initialized */
    }

    SPI_Enable();
}


/*******************************************************************************
* Function Name: SPI_Stop
********************************************************************************
*
* Summary:
*  Disables the SCB component and its interrupt.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SPI_Stop(void)
{
#if(SPI_SCB_IRQ_INTERNAL)
    SPI_DisableInt();
#endif /* (SPI_SCB_IRQ_INTERNAL) */

    SPI_CTRL_REG &= (uint32) ~SPI_CTRL_ENABLED;  /* Disable SCB block */

#if(SPI_SCB_IRQ_INTERNAL)
    SPI_ClearPendingInt();
#endif /* (SPI_SCB_IRQ_INTERNAL) */

    SPI_ScbModeStop(); /* Calls scbMode specific Stop function */
}


/*******************************************************************************
* Function Name: SPI_SetCustomInterruptHandler
********************************************************************************
*
* Summary:
*  Registers a function to be called by the internal interrupt handler.
*  First the function that is registered is called, then the internal interrupt
*  handler performs any operations such as software buffer management functions
*  before the interrupt returns.  It is the user's responsibility not to break the
*  software buffer operations. Only one custom handler is supported, which is
*  the function provided by the most recent call.
*  At initialization time no custom handler is registered.
*
* Parameters:
*  func: Pointer to the function to register.
*        The value NULL indicates to remove the current custom interrupt
*        handler.
*
* Return:
*  None
*
*******************************************************************************/
void SPI_SetCustomInterruptHandler(cyisraddress func)
{
#if !defined (CY_REMOVE_SPI_CUSTOM_INTR_HANDLER)
    SPI_customIntrHandler = func; /* Register interrupt handler */
#else
    if(NULL != func)
    {
        /* Suppress compiler warning */
    }
#endif /* !defined (CY_REMOVE_SPI_CUSTOM_INTR_HANDLER) */
}


/*******************************************************************************
* Function Name: SPI_ScbModeEnableIntr
********************************************************************************
*
* Summary:
*  Enables an interrupt for a specific mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
static void SPI_ScbEnableIntr(void)
{
#if(SPI_SCB_IRQ_INTERNAL)
    #if(SPI_SCB_MODE_UNCONFIG_CONST_CFG)
        /* Enable interrupt in the NVIC */
        if(0u != SPI_scbEnableIntr)
        {
            SPI_EnableInt();
        }
    #else
        SPI_EnableInt();

    #endif /* (SPI_SCB_MODE_UNCONFIG_CONST_CFG) */
#endif /* (SPI_SCB_IRQ_INTERNAL) */
}


/*******************************************************************************
* Function Name: SPI_ScbModeStop
********************************************************************************
*
* Summary:
*  Calls the Stop function for a specific operation mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
static void SPI_ScbModeStop(void)
{
#if(SPI_SCB_MODE_UNCONFIG_CONST_CFG)
    if(SPI_SCB_MODE_I2C_RUNTM_CFG)
    {
        SPI_I2CStop();
    }
    else if(SPI_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        SPI_EzI2CStop();
    }
    else
    {
        /* Do nohing for other modes */
    }
#elif(SPI_SCB_MODE_I2C_CONST_CFG)
    SPI_I2CStop();

#elif(SPI_SCB_MODE_EZI2C_CONST_CFG)
    SPI_EzI2CStop();

#endif /* (SPI_SCB_MODE_UNCONFIG_CONST_CFG) */
}


#if(SPI_SCB_MODE_UNCONFIG_CONST_CFG)
    /*******************************************************************************
    * Function Name: SPI_SetPins
    ********************************************************************************
    *
    * Summary:
    *  Sets the pins settings accordingly to the selected operation mode.
    *  Only available in the Unconfigured operation mode. The mode specific
    *  initialization function calls it.
    *  Pins configuration is set by PSoC Creator when a specific mode of operation
    *  is selected in design time.
    *
    * Parameters:
    *  mode:      Mode of SCB operation.
    *  subMode:   Sub-mode of SCB operation. It is only required for SPI and UART
    *             modes.
    *  uartTxRx:  Direction for UART.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void SPI_SetPins(uint32 mode, uint32 subMode, uint32 uartTxRx)
    {
        uint32 hsiomSel [SPI_SCB_PINS_NUMBER];
        uint32 pinsDm   [SPI_SCB_PINS_NUMBER];
        uint32 pinsInBuf = 0u;

        uint32 i;

        /* Set default HSIOM to GPIO and Drive Mode to Analog Hi-Z */
        for(i = 0u; i < SPI_SCB_PINS_NUMBER; i++)
        {
            hsiomSel[i]  = SPI_HSIOM_DEF_SEL;
            pinsDm[i]    = SPI_PIN_DM_ALG_HIZ;
        }

        if((SPI_SCB_MODE_I2C   == mode) ||
           (SPI_SCB_MODE_EZI2C == mode))
        {
            hsiomSel[SPI_MOSI_SCL_RX_PIN_INDEX] = SPI_HSIOM_I2C_SEL;
            hsiomSel[SPI_MISO_SDA_TX_PIN_INDEX] = SPI_HSIOM_I2C_SEL;

            pinsDm[SPI_MOSI_SCL_RX_PIN_INDEX] = SPI_PIN_DM_OD_LO;
            pinsDm[SPI_MISO_SDA_TX_PIN_INDEX] = SPI_PIN_DM_OD_LO;
        }
    #if(!SPI_CY_SCBIP_V1_I2C_ONLY)
        else if(SPI_SCB_MODE_SPI == mode)
        {
            hsiomSel[SPI_MOSI_SCL_RX_PIN_INDEX] = SPI_HSIOM_SPI_SEL;
            hsiomSel[SPI_MISO_SDA_TX_PIN_INDEX] = SPI_HSIOM_SPI_SEL;
            hsiomSel[SPI_SCLK_PIN_INDEX]        = SPI_HSIOM_SPI_SEL;

            if(SPI_SPI_SLAVE == subMode)
            {
                /* Slave */
                pinsDm[SPI_MOSI_SCL_RX_PIN_INDEX] = SPI_PIN_DM_DIG_HIZ;
                pinsDm[SPI_MISO_SDA_TX_PIN_INDEX] = SPI_PIN_DM_STRONG;
                pinsDm[SPI_SCLK_PIN_INDEX]        = SPI_PIN_DM_DIG_HIZ;

            #if(SPI_SS0_PIN)
                /* Only SS0 is valid choice for Slave */
                hsiomSel[SPI_SS0_PIN_INDEX] = SPI_HSIOM_SPI_SEL;
                pinsDm  [SPI_SS0_PIN_INDEX] = SPI_PIN_DM_DIG_HIZ;
            #endif /* (SPI_SS1_PIN) */

            #if(SPI_MISO_SDA_TX_PIN)
                /* Disable input buffer */
                 pinsInBuf |= SPI_MISO_SDA_TX_PIN_MASK;
            #endif /* (SPI_MISO_SDA_TX_PIN_PIN) */
            }
            else /* (Master) */
            {
                pinsDm[SPI_MOSI_SCL_RX_PIN_INDEX] = SPI_PIN_DM_STRONG;
                pinsDm[SPI_MISO_SDA_TX_PIN_INDEX] = SPI_PIN_DM_DIG_HIZ;
                pinsDm[SPI_SCLK_PIN_INDEX]        = SPI_PIN_DM_STRONG;

            #if(SPI_SS0_PIN)
                hsiomSel [SPI_SS0_PIN_INDEX] = SPI_HSIOM_SPI_SEL;
                pinsDm   [SPI_SS0_PIN_INDEX] = SPI_PIN_DM_STRONG;
                pinsInBuf                                |= SPI_SS0_PIN_MASK;
            #endif /* (SPI_SS0_PIN) */

            #if(SPI_SS1_PIN)
                hsiomSel [SPI_SS1_PIN_INDEX] = SPI_HSIOM_SPI_SEL;
                pinsDm   [SPI_SS1_PIN_INDEX] = SPI_PIN_DM_STRONG;
                pinsInBuf                                |= SPI_SS1_PIN_MASK;
            #endif /* (SPI_SS1_PIN) */

            #if(SPI_SS2_PIN)
                hsiomSel [SPI_SS2_PIN_INDEX] = SPI_HSIOM_SPI_SEL;
                pinsDm   [SPI_SS2_PIN_INDEX] = SPI_PIN_DM_STRONG;
                pinsInBuf                                |= SPI_SS2_PIN_MASK;
            #endif /* (SPI_SS2_PIN) */

            #if(SPI_SS3_PIN)
                hsiomSel [SPI_SS3_PIN_INDEX] = SPI_HSIOM_SPI_SEL;
                pinsDm   [SPI_SS3_PIN_INDEX] = SPI_PIN_DM_STRONG;
                pinsInBuf                                |= SPI_SS3_PIN_MASK;
            #endif /* (SPI_SS2_PIN) */

                /* Disable input buffers */
            #if(SPI_MOSI_SCL_RX_PIN)
                pinsInBuf |= SPI_MOSI_SCL_RX_PIN_MASK;
            #endif /* (SPI_MOSI_SCL_RX_PIN) */

             #if(SPI_MOSI_SCL_RX_WAKE_PIN)
                pinsInBuf |= SPI_MOSI_SCL_RX_WAKE_PIN_MASK;
            #endif /* (SPI_MOSI_SCL_RX_WAKE_PIN) */

            #if(SPI_SCLK_PIN)
                pinsInBuf |= SPI_SCLK_PIN_MASK;
            #endif /* (SPI_SCLK_PIN) */
            }
        }
        else /* UART */
        {
            if(SPI_UART_MODE_SMARTCARD == subMode)
            {
                /* SmartCard */
                hsiomSel[SPI_MISO_SDA_TX_PIN_INDEX] = SPI_HSIOM_UART_SEL;
                pinsDm  [SPI_MISO_SDA_TX_PIN_INDEX] = SPI_PIN_DM_OD_LO;
            }
            else /* Standard or IrDA */
            {
                if(0u != (SPI_UART_RX & uartTxRx))
                {
                    hsiomSel[SPI_MOSI_SCL_RX_PIN_INDEX] = SPI_HSIOM_UART_SEL;
                    pinsDm  [SPI_MOSI_SCL_RX_PIN_INDEX] = SPI_PIN_DM_DIG_HIZ;
                }

                if(0u != (SPI_UART_TX & uartTxRx))
                {
                    hsiomSel[SPI_MISO_SDA_TX_PIN_INDEX] = SPI_HSIOM_UART_SEL;
                    pinsDm  [SPI_MISO_SDA_TX_PIN_INDEX] = SPI_PIN_DM_STRONG;

                #if(SPI_MISO_SDA_TX_PIN)
                     pinsInBuf |= SPI_MISO_SDA_TX_PIN_MASK;
                #endif /* (SPI_MISO_SDA_TX_PIN_PIN) */
                }
            }
        }
    #endif /* (!SPI_CY_SCBIP_V1_I2C_ONLY) */

    /* Configure pins: set HSIOM, DM and InputBufEnable */
    /* Note: the DR register settigns do not effect the pin output if HSIOM is other than GPIO */

    #if(SPI_MOSI_SCL_RX_PIN)
        SPI_SET_HSIOM_SEL(SPI_MOSI_SCL_RX_HSIOM_REG,
                                       SPI_MOSI_SCL_RX_HSIOM_MASK,
                                       SPI_MOSI_SCL_RX_HSIOM_POS,
                                       hsiomSel[SPI_MOSI_SCL_RX_PIN_INDEX]);

        SPI_spi_mosi_i2c_scl_uart_rx_SetDriveMode((uint8) pinsDm[SPI_MOSI_SCL_RX_PIN_INDEX]);

        SPI_SET_INP_DIS(SPI_spi_mosi_i2c_scl_uart_rx_INP_DIS,
                                     SPI_spi_mosi_i2c_scl_uart_rx_MASK,
                                     (0u != (pinsInBuf & SPI_MOSI_SCL_RX_PIN_MASK)));
    #endif /* (SPI_MOSI_SCL_RX_PIN) */

    #if(SPI_MOSI_SCL_RX_WAKE_PIN)
        SPI_SET_HSIOM_SEL(SPI_MOSI_SCL_RX_WAKE_HSIOM_REG,
                                       SPI_MOSI_SCL_RX_WAKE_HSIOM_MASK,
                                       SPI_MOSI_SCL_RX_WAKE_HSIOM_POS,
                                       hsiomSel[SPI_MOSI_SCL_RX_WAKE_PIN_INDEX]);

        SPI_spi_mosi_i2c_scl_uart_rx_wake_SetDriveMode((uint8)
                                                               pinsDm[SPI_MOSI_SCL_RX_WAKE_PIN_INDEX]);

        SPI_SET_INP_DIS(SPI_spi_mosi_i2c_scl_uart_rx_wake_INP_DIS,
                                     SPI_spi_mosi_i2c_scl_uart_rx_wake_MASK,
                                     (0u != (pinsInBuf & SPI_MOSI_SCL_RX_WAKE_PIN_MASK)));

         /* Set interrupt on falling edge */
        SPI_SET_INCFG_TYPE(SPI_MOSI_SCL_RX_WAKE_INTCFG_REG,
                                        SPI_MOSI_SCL_RX_WAKE_INTCFG_TYPE_MASK,
                                        SPI_MOSI_SCL_RX_WAKE_INTCFG_TYPE_POS,
                                        SPI_INTCFG_TYPE_FALLING_EDGE);
    #endif /* (SPI_MOSI_SCL_RX_WAKE_PIN) */

    #if(SPI_MISO_SDA_TX_PIN)
        SPI_SET_HSIOM_SEL(SPI_MISO_SDA_TX_HSIOM_REG,
                                       SPI_MISO_SDA_TX_HSIOM_MASK,
                                       SPI_MISO_SDA_TX_HSIOM_POS,
                                       hsiomSel[SPI_MISO_SDA_TX_PIN_INDEX]);

        SPI_spi_miso_i2c_sda_uart_tx_SetDriveMode((uint8) pinsDm[SPI_MISO_SDA_TX_PIN_INDEX]);

        SPI_SET_INP_DIS(SPI_spi_miso_i2c_sda_uart_tx_INP_DIS,
                                     SPI_spi_miso_i2c_sda_uart_tx_MASK,
                                    (0u != (pinsInBuf & SPI_MISO_SDA_TX_PIN_MASK)));
    #endif /* (SPI_MOSI_SCL_RX_PIN) */

    #if(SPI_SCLK_PIN)
        SPI_SET_HSIOM_SEL(SPI_SCLK_HSIOM_REG, SPI_SCLK_HSIOM_MASK,
                                       SPI_SCLK_HSIOM_POS, hsiomSel[SPI_SCLK_PIN_INDEX]);

        SPI_spi_sclk_SetDriveMode((uint8) pinsDm[SPI_SCLK_PIN_INDEX]);

        SPI_SET_INP_DIS(SPI_spi_sclk_INP_DIS,
                             SPI_spi_sclk_MASK,
                            (0u != (pinsInBuf & SPI_SCLK_PIN_MASK)));
    #endif /* (SPI_SCLK_PIN) */

    #if(SPI_SS0_PIN)
        SPI_SET_HSIOM_SEL(SPI_SS0_HSIOM_REG, SPI_SS0_HSIOM_MASK,
                                       SPI_SS0_HSIOM_POS, hsiomSel[SPI_SS0_PIN_INDEX]);

        SPI_spi_ss0_SetDriveMode((uint8) pinsDm[SPI_SS0_PIN_INDEX]);

        SPI_SET_INP_DIS(SPI_spi_ss0_INP_DIS,
                                     SPI_spi_ss0_MASK,
                                     (0u != (pinsInBuf & SPI_SS0_PIN_MASK)));
    #endif /* (SPI_SS1_PIN) */

    #if(SPI_SS1_PIN)
        SPI_SET_HSIOM_SEL(SPI_SS1_HSIOM_REG, SPI_SS1_HSIOM_MASK,
                                       SPI_SS1_HSIOM_POS, hsiomSel[SPI_SS1_PIN_INDEX]);

        SPI_spi_ss1_SetDriveMode((uint8) pinsDm[SPI_SS1_PIN_INDEX]);

        SPI_SET_INP_DIS(SPI_spi_ss1_INP_DIS,
                                     SPI_spi_ss1_MASK,
                                     (0u != (pinsInBuf & SPI_SS1_PIN_MASK)));
    #endif /* (SPI_SS1_PIN) */

    #if(SPI_SS2_PIN)
        SPI_SET_HSIOM_SEL(SPI_SS2_HSIOM_REG, SPI_SS2_HSIOM_MASK,
                                       SPI_SS2_HSIOM_POS, hsiomSel[SPI_SS2_PIN_INDEX]);

        SPI_spi_ss2_SetDriveMode((uint8) pinsDm[SPI_SS2_PIN_INDEX]);

        SPI_SET_INP_DIS(SPI_spi_ss2_INP_DIS,
                                     SPI_spi_ss2_MASK,
                                     (0u != (pinsInBuf & SPI_SS2_PIN_MASK)));
    #endif /* (SPI_SS2_PIN) */

    #if(SPI_SS3_PIN)
        SPI_SET_HSIOM_SEL(SPI_SS3_HSIOM_REG,  SPI_SS3_HSIOM_MASK,
                                       SPI_SS3_HSIOM_POS, hsiomSel[SPI_SS3_PIN_INDEX]);

        SPI_spi_ss3_SetDriveMode((uint8) pinsDm[SPI_SS3_PIN_INDEX]);

        SPI_SET_INP_DIS(SPI_spi_ss3_INP_DIS,
                                     SPI_spi_ss3_MASK,
                                     (0u != (pinsInBuf & SPI_SS3_PIN_MASK)));
    #endif /* (SPI_SS3_PIN) */
    }

#endif /* (SPI_SCB_MODE_UNCONFIG_CONST_CFG) */


/* [] END OF FILE */
