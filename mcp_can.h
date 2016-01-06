#ifndef _MCP2515_H_
#define _MCP2515_H_

#include <SPI.h>

#include "mcp_can_dfs.h"

enum CAN_SPEED {
    CAN_5KBPS,
    CAN_10KBPS,
    CAN_20KBPS,
    CAN_31K25BPS,
    CAN_33KBPS,
    CAN_40KBPS,
    CAN_50KBPS,
    CAN_80KBPS,
    CAN_83K3BPS,
    CAN_95KBPS,
    CAN_100KBPS,
    CAN_125KBPS,
    CAN_200KBPS,
    CAN_250KBPS,
    CAN_500KBPS,
    CAN_1000KBPS
};

class MCP_CAN
{
    public:
        enum ERROR {
            ERROR_OK        = 0,
            ERROR_FAIL      = 1,
            ERROR_ALLTXBUSY = 2,
            ERROR_FAILINIT  = 3,
            ERROR_FAILTX    = 4,
            ERROR_NOMSG     = 5
        };

        enum RXBn {
            RXB0 = 0,
            RXB1 = 1
        };

        enum /*class*/ MODE : uint8_t {
            MODE_NORMAL     = 0x00,
            MODE_SLEEP      = 0x20,
            MODE_LOOPBACK   = 0x40,
            MODE_LISTENONLY = 0x60,
            MODE_CONFIG     = 0x80,
            MODE_POWERUP    = 0xE0
        };

        enum /*class*/ CANINTF : uint8_t {
            CANINTF_RX0IF = 0x01,
            CANINTF_RX1IF = 0x02,
            CANINTF_TX0IF = 0x04,
            CANINTF_TX1IF = 0x08,
            CANINTF_TX2IF = 0x10,
            CANINTF_ERRIF = 0x20,
            CANINTF_WAKIF = 0x40,
            CANINTF_MERRF = 0x80
        };

    private:
        static const uint8_t MODE_MASK = 0xE0;

        static const uint8_t TXB_EXIDE_MASK = 0x08;
        static const uint8_t DLC_MASK       = 0x0F;
        static const uint8_t RTR_MASK       = 0x40;

        enum RXF {
            RXF0 = 0,
            RXF1 = 1,
            RXF2 = 2,
            RXF3 = 3,
            RXF4 = 4,
            RXF5 = 5
        };

        enum /*class*/ STAT : uint8_t {
            STAT_RX0IF = (1<<0),
            STAT_RX1IF = (1<<1)
        };

        static const uint8_t STAT_RXIF_MASK = STAT_RX0IF | STAT_RX1IF;

        enum /*class*/ TXBnCTRL : uint8_t {
            TXB_ABTF   = 0x40,
            TXB_MLOA   = 0x20,
            TXB_TXERR  = 0x10,
            TXB_TXREQ  = 0x08,
            TXB_TXIE   = 0x04,
            TXB_TXP    = 0x03
        };

        enum /*class*/ EFLG : uint8_t {
            EFLG_RX1OVR = (1<<7),
            EFLG_RX0OVR = (1<<6),
            EFLG_TXBO   = (1<<5),
            EFLG_TXEP   = (1<<4),
            EFLG_RXEP   = (1<<3),
            EFLG_TXWAR  = (1<<2),
            EFLG_RXWAR  = (1<<1),
            EFLG_EWARN  = (1<<0)
        };

        static const uint8_t EFLG_ERRORMASK = EFLG_RX1OVR
                                            | EFLG_RX0OVR
                                            | EFLG_TXBO
                                            | EFLG_TXEP
                                            | EFLG_RXEP;

        enum /*class*/ INSTRUCTION : uint8_t {
            INSTRUCTION_WRITE       = 0x02,
            INSTRUCTION_READ        = 0x03,
            INSTRUCTION_BITMOD      = 0x05,
            INSTRUCTION_LOAD_TX0    = 0x40,
            INSTRUCTION_LOAD_TX1    = 0x42,
            INSTRUCTION_LOAD_TX2    = 0x44,
            INSTRUCTION_RTS_TX0     = 0x81,
            INSTRUCTION_RTS_TX1     = 0x82,
            INSTRUCTION_RTS_TX2     = 0x84,
            INSTRUCTION_RTS_ALL     = 0x87,
            INSTRUCTION_READ_RX0    = 0x90,
            INSTRUCTION_READ_RX1    = 0x94,
            INSTRUCTION_READ_STATUS = 0xA0,
            INSTRUCTION_RX_STATUS   = 0xB0,
            INSTRUCTION_RESET       = 0xC0
        };

        enum /*class*/ REGISTER : uint8_t {
            MCP_RXF0SIDH = 0x00,
            MCP_RXF0SIDL = 0x01,
            MCP_RXF0EID8 = 0x02,
            MCP_RXF0EID0 = 0x03,
            MCP_RXF1SIDH = 0x04,
            MCP_RXF1SIDL = 0x05,
            MCP_RXF1EID8 = 0x06,
            MCP_RXF1EID0 = 0x07,
            MCP_RXF2SIDH = 0x08,
            MCP_RXF2SIDL = 0x09,
            MCP_RXF2EID8 = 0x0A,
            MCP_RXF2EID0 = 0x0B,
            MCP_CANSTAT  = 0x0E,
            MCP_CANCTRL  = 0x0F,
            MCP_RXF3SIDH = 0x10,
            MCP_RXF3SIDL = 0x11,
            MCP_RXF3EID8 = 0x12,
            MCP_RXF3EID0 = 0x13,
            MCP_RXF4SIDH = 0x14,
            MCP_RXF4SIDL = 0x15,
            MCP_RXF4EID8 = 0x16,
            MCP_RXF4EID0 = 0x17,
            MCP_RXF5SIDH = 0x18,
            MCP_RXF5SIDL = 0x19,
            MCP_RXF5EID8 = 0x1A,
            MCP_RXF5EID0 = 0x1B,
            MCP_TEC      = 0x1C,
            MCP_REC      = 0x1D,
            MCP_RXM0SIDH = 0x20,
            MCP_RXM0SIDL = 0x21,
            MCP_RXM0EID8 = 0x22,
            MCP_RXM0EID0 = 0x23,
            MCP_RXM1SIDH = 0x24,
            MCP_RXM1SIDL = 0x25,
            MCP_RXM1EID8 = 0x26,
            MCP_RXM1EID0 = 0x27,
            MCP_CNF3     = 0x28,
            MCP_CNF2     = 0x29,
            MCP_CNF1     = 0x2A,
            MCP_CANINTE  = 0x2B,
            MCP_CANINTF  = 0x2C,
            MCP_EFLG     = 0x2D,
            MCP_TXB0CTRL = 0x30,
            MCP_TXB0SIDH = 0x31,
            MCP_TXB0SIDL = 0x32,
            MCP_TXB0EID8 = 0x33,
            MCP_TXB0EID0 = 0x34,
            MCP_TXB0DLC  = 0x35,
            MCP_TXB0DATA = 0x36,
            MCP_TXB1CTRL = 0x40,
            MCP_TXB1SIDH = 0x41,
            MCP_TXB1SIDL = 0x42,
            MCP_TXB1EID8 = 0x43,
            MCP_TXB1EID0 = 0x44,
            MCP_TXB1DLC  = 0x45,
            MCP_TXB1DATA = 0x46,
            MCP_TXB2CTRL = 0x50,
            MCP_TXB2SIDH = 0x51,
            MCP_TXB2SIDL = 0x52,
            MCP_TXB2EID8 = 0x53,
            MCP_TXB2EID0 = 0x54,
            MCP_TXB2DLC  = 0x55,
            MCP_TXB2DATA = 0x56,
            MCP_RXB0CTRL = 0x60,
            MCP_RXB0SIDH = 0x61,
            MCP_RXB0SIDL = 0x62,
            MCP_RXB0EID8 = 0x63,
            MCP_RXB0EID0 = 0x64,
            MCP_RXB0DLC  = 0x65,
            MCP_RXB0DATA = 0x66,
            MCP_RXB1CTRL = 0x70,
            MCP_RXB1SIDH = 0x71,
            MCP_RXB1SIDL = 0x72,
            MCP_RXB1EID8 = 0x73,
            MCP_RXB1EID0 = 0x74,
            MCP_RXB1DLC  = 0x75,
            MCP_RXB1DATA = 0x76
        };

        static const int CAN_MAX_CHAR_IN_MESSAGE = 8;
        static const uint32_t SPI_CLOCK = 10000000; // 10MHz

        static const int N_TXBUFFERS = 3;
        static const int N_RXBUFFERS = 2;

        static const struct TXBn_REGS {
            REGISTER CTRL;
            REGISTER SIDH;
            REGISTER DATA;
        } TXB[N_TXBUFFERS];

        static const struct RXBn_REGS {
            REGISTER CTRL;
            REGISTER SIDH;
            REGISTER DATA;
        } RXB[N_RXBUFFERS];

        uint8_t SPICS;
        MODE m_mode;

    private:

        void mcp2515_reset(void);

        uint8_t readRegister(const REGISTER reg);

        void readRegisterS(const REGISTER reg, uint8_t values[], const uint8_t n);
        void setRegister(const REGISTER reg, const uint8_t value);

        void setRegisterS(const REGISTER reg, const uint8_t values[], const uint8_t n);

        void initCANBuffers(void);

        void modifyRegister(const REGISTER reg, const uint8_t mask, const uint8_t data);

        ERROR setCANCTRL_Mode(const MODE newmode);
        ERROR configRate(const CAN_SPEED canSpeed);
        ERROR mcp2515_init(const CAN_SPEED canSpeed);

        void write_id(const REGISTER reg, const bool ext, const uint32_t id);
    
        void prepareId(uint8_t *buffer, const bool ext, const uint32_t id);
    
        void startSPI();
        void endSPI();
    
    public:
        MCP_CAN(const uint8_t _CS, const MODE mode);
        ERROR begin(const CAN_SPEED speedset);
        ERROR initMask(const uint8_t num, const bool ext, const uint32_t ulData);
        ERROR initFilt(const RXF num, const bool ext, const uint32_t ulData);
        ERROR sendMessage(const uint32_t id, const bool ext, const bool rtr, const uint8_t len, const uint8_t *buf);
        ERROR readMessage(const RXBn rxbn, uint32_t *id, uint8_t *dlc, uint8_t buf[], bool *rtr, bool *ext);
        ERROR readMessage(uint32_t *id, uint8_t *dlc, uint8_t buf[], bool *rtr, bool *ext);
        bool checkReceive(void);
        bool checkError(void);
        uint8_t getInterrupts(void);
        uint8_t getInterruptMask(void);
        void clearInterrupts(void);
        void clearTXInterrupts(void);
        uint8_t getStatus(void);
};

#endif
