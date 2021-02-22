//
//

typedef unsigned char ui8;

unsigned int crc16(int cnt, unsigned char volatile buf[])
{
	int i,j;
	unsigned f;
	unsigned int mbCRC = 0xFFFF;

	for (i = 0; i<cnt; i++)
	{
		mbCRC = mbCRC ^ buf[i];
		for (j = 1; j <= 8; j++)
		{
			f = mbCRC & 0x0001;
			mbCRC = mbCRC >> 1;
			if (f) mbCRC = mbCRC ^ 0xA001;
		}
	}
	
	//Reverse byte order.
//	f= mbCRC >> 8;
//	mbCRC = (mbCRC << 8) | f;
	return(mbCRC);
}


ui8 crc8(int cnt, unsigned char volatile buf[])
{
    ui8 crc = 0xff;
    int i, j;
    for (i = 0; i < cnt; i++)
    {
        crc ^= buf[i];
        for (j = 0; j < 8; j++)
        {
            if ((crc & 0x80) != 0)
                crc = (ui8)((crc << 1) ^ 0x31);
            else
                crc <<= 1;
        }
    }
    return crc;
}

//int main()
//{
//uint8_t data[8] = {0xBE,0xEF,0,0,0,0,0,0};
//uint8_t crc;
//    crc = gencrc(data, 2);   /* returns 0x92 */
//    printf("%1x\n", crc);
//    crc = gencrc(data+2, 1); /* returns 0xac */
//    printf("%1x\n", crc);
//    return 0;
//}
