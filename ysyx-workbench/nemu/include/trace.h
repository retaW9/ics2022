#ifndef __TRACE_H__
#define __TRACE_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>


typedef struct
{
    uint64_t ValidLen;      
    uint64_t RingSize;      
    char* pHead;            
    char* pTail;            
    //char* pValid;         
    char* pValidTail;       
}iRingBuff;

#define INST_RING_BUF_SIZE 500

extern iRingBuff inst_ringbuf;

void initRingbuffer(iRingBuff* pRing ,uint64_t size);
void wirteRingbuffer(iRingBuff* pRing,char* buffer,uint64_t addLen);
//void readRingbuffer(iRingBuff* pRing,char* buffer,uint64_t len);
void releaseRingbuffer(iRingBuff *pRing);
void displayRingbuffer(iRingBuff *pRing);

#ifdef __cplusplus
}
#endif

#endif
