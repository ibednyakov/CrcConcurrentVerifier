/*
 *  CRC32.cpp
 *
 *  Created on: 25 окт. 2018 г.
 *      Author: ivan
 */

#include "CRC32.h"


namespace CRCVerifier
{

	/*static*/ const CRC32Calc& CRC32Calc::instance_pkzip()
	{
		// This is the official polynomial used by CRC32 in PKZip.
		// Often times the polynomial shown reversed as 0x04C11DB7.
		static const CRC32Calc crc32_calculator( 0xEDB88320 );
		return crc32_calculator;
	}


	/*static*/ const CRC32Calc& CRC32Calc::instance_reversed()
	{
		static const CRC32Calc crc32_calculator( 0x04C11DB7 );
		return crc32_calculator;
	}


	CRC32Calc::CRC32Calc(uint32_t polynomial, uint32_t iv, bool refin, bool refout)
		: init(iv)
		, ref_in(refin)
		, ref_out(refout)
	{
		for (int i = 0; i < 256; i++)
		{
			uint32_t crc = i;
			for (int j = 8; j > 0; j--)
			{
				if (crc & 1)
					crc = (crc >> 1) ^ polynomial;
				else
					crc >>= 1;
			}
			_crc32_table[i] = crc;
		}
	}


	void CRC32Calc::add_int32_be(uint32_t int_32, uint32_t& crc32) const
	{
		add_byte( int_32 >> 24, crc32 );
		add_byte( int_32 >> 16, crc32 );
		add_byte( int_32 >> 8,  crc32 );
		add_byte( int_32 >> 0,  crc32 );
	}


	void CRC32Calc::add_int32_le(uint32_t int_32, uint32_t& crc32) const
	{
		add_byte( int_32 >> 0,  crc32 );
		add_byte( int_32 >> 8,  crc32 );
		add_byte( int_32 >> 16, crc32 );
		add_byte( int_32 >> 24, crc32 );
	}


	void CRC32Calc::add_buffer(const void* mem, size_t size, uint32_t& crc32) const
	{
		const uint8_t* buffer = reinterpret_cast<const uint8_t*>( mem );
		const uint8_t* buffer_end = buffer + size;
		for( ; buffer != buffer_end; ++buffer )
			add_byte( *buffer, crc32 );
	}


	uint32_t CRC32Calc::calc_checksum(const void* buffer, size_t size) const
	{
		uint32_t crc32;
		begin_calc( crc32 );
		add_buffer( buffer, size, crc32 );
		end_calc( crc32 );
		return crc32;
	}

} // namespace CRCVerifier
