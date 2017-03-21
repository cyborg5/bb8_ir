/* IRLib_P90_BB8.h
 * Part of IRLib Library for Arduino receiving, decoding, and sending
 */
/*
  This unsupported protocol is for a Hasbro Star Wars Droid Toy
  https://www.amazon.com/Star-Wars-Force-Awakens-BB-8/dp/B00WHX8Z1O/
  
  It uses an 8-bit phase encoded system similar to RC6 protocol.
  The modulation frequency is 39 kHz however it seems to work at 38 as well.
  It begins with a header of 1500, 1000. The base value for the data 
  is 500. It starts with an initial 500 Mark followed by the eight bits of
  phase encoding data. A mark/space indicates "1" and a space/mark indicates
  a bit value of "0". The bits are defined as follows left to right.
  
  Bit 7: always "1"
  Bit 6: forward
  Bit 5: back
  Bit 4: left
  Bit 3: right
  Bit 2: always "0"
  Bit 1: odd parity
  Bit 0: always "1"
  
  IRP notation would be {39k,500,msb}<-1,1|1,-1>(3,-2,1,D:8,-20)

  Typically when you press a key on the remote, an idle signal 0x83 is sent
  followed by actual commands repeated however much you want. When the
  buttons are released the sequence terminates with approximately 4 copies 
  the idle signal. So for example if you press the "Forward" button the 
  sequence would be... 0x83, 0xc1, 0xc1,... ,0x83 ,0x83 ,0x83 ,0x83  
*/
#ifndef IRLIB_PROTOCOL_90_H
#define IRLIB_PROTOCOL_90_H
#define IR_SEND_PROTOCOL_90		case 90: IRsendBB8::send(data,data2); break;
#define IR_DECODE_PROTOCOL_90	if(IRdecodeBB8::decode()) return true;
#ifdef IRLIB_HAVE_COMBO
	#define PV_IR_DECODE_PROTOCOL_90 ,public virtual IRdecodeBB8
	#define PV_IR_SEND_PROTOCOL_90   ,public virtual IRsendBB8
#else
	#define PV_IR_DECODE_PROTOCOL_90  public virtual IRdecodeBB8
	#define PV_IR_SEND_PROTOCOL_90    public virtual IRsendBB8
#endif
#define BB8_HEAD_MARK 1500
#define BB8_HEAD_SPACE 1000
#define BB8_DATA 500
#define BB8_FW_BIT  0x40;
#define BB8_RT_BIT  0x08;
#define BB8_LT_BIT  0x10;
#define BB8_BK_BIT  0x20;
#define BB8_PT_BIT  0x02;
#define BB8_MASK    0x81;

#define BB8_FW    0xc1
#define BB8_FW_RT 0xcb
#define BB8_FW_LT 0xd3
#define BB8_IDLE  0x83
#define BB8_RIGHT 0x89
#define BB8_LEFT  0x91
#define BB8_BK_LT 0xb3
#define BB8_BACK  0xa1
#define BB8_BK_RT 0xab


#ifdef IRLIBSENDBASE_H

class IRsendBB8: public virtual IRsendBase {
  public:
    void send(uint8_t data) {
      enableIROut(39);
      mark(BB8_HEAD_MARK);
      space(BB8_HEAD_SPACE);
      mark(BB8_DATA);//Starts with initial Mark
      for (uint8_t i = 0; i < 8; i++) {
        if (data & 0x80) {
          mark(BB8_DATA); space(BB8_DATA);//"1" is a Mark/space
        } else {
          space(BB8_DATA); mark(BB8_DATA);//"0" is a space/Mark
        }
        data <<= 1;
      }
      space(10000);
    }
};
#endif  //IRLIBSENDBASE_H

#ifdef IRLIBDECODEBASE_H

class IRdecodeBB8: public virtual IRdecodeRC {
  public:
    bool forward, right, left, back, idle;
    virtual bool decode(void)  {
      IRLIB_ATTEMPT_MESSAGE(F("BB8ph"));
      resetDecoder();
      bufIndex_t Max=recvGlobal.decodeLength; 
      if ( (Max>16) || (Max<12) ) return RAW_COUNT_ERROR;
      if (! MATCH(recvGlobal.decodeBuffer [1],BB8_HEAD_MARK)) {
        return HEADER_MARK_ERROR(BB8_HEAD_MARK);
      }
      if (! MATCH(recvGlobal.decodeBuffer [2],BB8_HEAD_SPACE)) {
        return HEADER_SPACE_ERROR(BB8_HEAD_SPACE);
      }
      offset=3;//Skip gap and header
      data = 0;
      used = 0;
      uint8_t parity=1;
      //Skip initial Mark
      if (getRClevel(&used, BB8_DATA) != MARK) return DATA_MARK_ERROR(BB8_DATA);
      for (nBits = 0; offset < recvGlobal.decodeLength; nBits++) {
        RCLevel levelA, levelB; // Next two levels
        levelA = getRClevel(&used, BB8_DATA); 
        levelB = getRClevel(&used, BB8_DATA);
        if (levelA == MARK && levelB == SPACE) { 
          data = (data << 1) | 1;   // 1 bit
          parity ^= 1;
        } else if (levelA == SPACE && levelB == MARK) {
          data <<= 1; // zero bit
        } else {
          return DATA_MARK_ERROR(BB8_DATA); 
        } 
      }
      if(parity)IRLIB_REJECTION_MESSAGE(F("parity bit"));
      // Success
      forward = data & BB8_FW_BIT;
      right   = data & BB8_RT_BIT;
      left    = data & BB8_LT_BIT;
      back    = data & BB8_BK_BIT;
      idle    = (data == BB8_IDLE);
      bits = nBits;
      value = data;      
      protocolNum = 90;
      return true;
    }
};

#endif //IRLIBDECODEBASE_H

#define IRLIB_HAVE_COMBDelaminated hereO

#endif //IRLIB_PROTOCOL_90_H
