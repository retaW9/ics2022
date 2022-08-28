#include <trace.h>

/*
 * @Description：Initialize the instruction ringbuffer
 * @Arguments：pRing : size : the size of the ringbuf
 * @Returns	：
 */
void initRingbuffer(iRingBuff *p, uint64_t size)
{
    p->pHead = (char *)malloc(size);
    p->pValidTail = p->pHead;
    p->pTail = p->pHead + size;
    p->ValidLen = 0;
    p->RingSize = size;
    // printf("1=%ld\n",p->RingSize);

    if (p->pHead != NULL)
        printf("request ring buffer memroy successful !\n");
    else
        printf("request memroy failed !\n");
}

/*
 * @Description :   wirte 'this.logbuf' to the ringbuf
 * @Arguments	:   pRing:     addLen:
 * @Returns     :
 */
void wirteRingbuffer(iRingBuff *p, char *buffer, uint64_t addLen)
{
    //printf("len1=%ld,size=%ld\n", addLen, p->RingSize);
    if (addLen <= p->RingSize)
    {
        /* copy the value to the pValidTail, if () divide into 2 part */
        if (p->pValidTail + addLen > p->pTail)
        {
            int len1 = p->pTail - p->pValidTail;
            int len2 = addLen - len1;
            memmove(p->pValidTail, buffer, len1);
            memmove(p->pHead, buffer + len1, len2);
            p->pValidTail = p->pHead + len2;
            p->ValidLen = len2;
        }
        else
        {
            memmove(p->pValidTail, buffer, addLen);
            p->pValidTail += addLen;
            p->ValidLen += addLen;
        }
    }
    else
        printf("buffer size is too samll!\n");
}
/*
 * @Description:
 * @Arguments:
 * @Returns:
 */
/*void readRingbuffer(iRingBuff *pRing, char *buffer, uint64_t len)
{
    if (0 < pRing.validLen)
    {
        if (len >= pRing.validLen)
            len = pRing.validLen;

        if (pRing.pValid + len > pRing.pTail) //
        {
            int len1 = pRing.pTail - pRing.pValid;
            int len2 = len - len1;
            memcpy(buffer, pRing.pValid, len1);
            memcpy(buffer + len1, pRing.pHead, len2);
            pRing.pValid = pRing.pHead + len2;
        }
        else
        {
            memcpy(buffer, pRing.pValid, len);
            pRing.pValid = pRing.pValid + len;
        }
        pRing.validLen -= len;
    }
}*/

/*
 * @Description:
 * @Arguments:
 * @Returns:
 */
void displayRingbuffer(iRingBuff *p)
{
    int n = 0;
    for (uint8_t i = 0; i < 10; i++)
    {
        if (p->pValidTail == p->pHead + n)
        {
            printf("-->\t%s\n", p->pHead + n);
        }

        printf("\t%s\n", p->pHead + n);
        n = n + 50;
    }
}

/*
 * @Description:free the instruction ringbuf
 * @Arguments:
 * @Returns:
 */
void releaseRingbuffer(iRingBuff *pRing)
{
    if (NULL != pRing->pHead && NULL != pRing->pHead)
    {
        free(pRing->pHead);
        memset((char *)pRing->pHead, 0, sizeof(iRingBuff));
    }
}