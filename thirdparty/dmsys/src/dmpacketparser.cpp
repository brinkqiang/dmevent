
#include "dmpacketparser.h"
#include <memory.h>

int32_t DMAPI HDMPacketParser::ParsePacket(const char* pBuf, int32_t nLen) {
    if (nLen < GetPacketHeaderSize()) {
        return 0;
    }

    SDMPacketHeader* pHeader = (SDMPacketHeader*)pBuf;

    int32_t nPacketLen = CheckPacketHeader(pHeader);

    if (nPacketLen < 0) {
        return nPacketLen;
    }

    if (nPacketLen <= nLen) {
        return nPacketLen;
    }

    return 0;
}

int32_t DMAPI HDMPacketParser::GetPacketHeaderSize()
{
    return sizeof(SDMPacketHeader);
}

int32_t DMAPI HDMPacketParser::GetMsgHeaderSize()
{
    return sizeof(SMsgHeader);
}

int32_t HDMPacketParser::BuildPacketHeader(void* pHeader,
        int32_t nDataLen) {
    SDMPacketHeader* poHeader = (SDMPacketHeader*)pHeader;
    poHeader->wMark = PACKET_MARK;
    poHeader->nDataLen = nDataLen;
    poHeader->wCheckSum = ((nDataLen ^ PACKET_CHECKSUM1) & PACKET_CHECKSUM2);

    poHeader->wMark = htons(poHeader->wMark);
    poHeader->nDataLen = htonl(poHeader->nDataLen);
    poHeader->wCheckSum = htons(poHeader->wCheckSum);

    return sizeof(*poHeader);
}

int32_t HDMPacketParser::CheckPacketHeader(void* pHeader) {
    SDMPacketHeader* poHeader = (SDMPacketHeader*)pHeader;
    poHeader->wMark = ntohs(poHeader->wMark);
    poHeader->nDataLen = ntohl(poHeader->nDataLen);
    poHeader->wCheckSum = htons(poHeader->wCheckSum);

    if (PACKET_MARK != poHeader->wMark) {
        return -1000;
    }

    uint16_t wCheckSum = ((poHeader->nDataLen ^ PACKET_CHECKSUM1) &
                           PACKET_CHECKSUM2);

    if (wCheckSum != poHeader->wCheckSum) {
        return -1001;
    }

    return sizeof(*poHeader) + poHeader->nDataLen;
}

int32_t DMAPI HNotParser::GetPacketHeaderSize()
{
    return 0;
}

int32_t DMAPI HNotParser::GetMsgHeaderSize()
{
    return 0;
}
