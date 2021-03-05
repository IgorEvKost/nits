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

ui8 crc8(int size, const ui8 buf[])
{
    ui8 crc = 0;
    for ( int i = 0; i < size; ++i )
    {
        ui8 inbyte = buf[i];
        for ( ui8 j = 0; j < 8; ++j )
        {
            ui8 mix = (crc ^ inbyte) & 0x01;
            crc >>= 1;
            if ( mix ) crc ^= 0x8C;
            inbyte >>= 1;
        }
    }
    return crc;
}
