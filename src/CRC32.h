/*
 *  CRC32.h
 *
 *  Created on: 25 окт. 2018 г.
 *      Author: ivan
 */

#ifndef CRC32_H_
#define CRC32_H_

#include <string>

namespace CRCVerifier
{
	class CRC32Calc
	{
	public:
		static const CRC32Calc& instance_pkzip();
		static const CRC32Calc& instance_reversed();

		CRC32Calc( uint32_t polynomial, uint32_t init=0xFFFFFFFF, bool refin=false, bool refout=false );
		~CRC32Calc() {}

		void begin_calc( uint32_t& crc32 ) const             { crc32 = init; }
		void end_calc( uint32_t& crc32 ) const               { crc32 = ref32(~crc32); }
		void add_byte( uint8_t byte, uint32_t& crc32 ) const { calc_crc32( ref8(byte), crc32 ); }
		void add_int32_be( uint32_t int_32, uint32_t& crc32 ) const;
		void add_int32_le( uint32_t int_32, uint32_t& crc32 ) const;
		void add_buffer( const void* buffer, size_t size, uint32_t& crc32 ) const;

		uint32_t calc_checksum( const void* buffer, size_t size ) const;

	private:
		CRC32Calc( const CRC32Calc& );
		CRC32Calc& operator =( const CRC32Calc& );

		void calc_crc32( const uint8_t byte, uint32_t& crc32 ) const { crc32 = (crc32 >> 8) ^ _crc32_table[byte ^ (crc32 & 0x000000FF)]; }

		inline uint32_t ref32(uint32_t v) const {
			if (!ref_out) return v;
			v = ((v>>1)&0x55555555) | ((v&0x55555555)<<1);
			v = ((v>>2)&0x33333333) | ((v&0x33333333)<<2);
			v = ((v>>4)&0x0F0F0F0F) | ((v&0x0F0F0F0F)<<4);
			v = ((v>>8)&0x00FF00FF) | ((v&0x00FF00FF)<<8);
			return (v>>16) | (v<<16);
		}

		inline uint8_t ref8(uint8_t v) const {
			if (!ref_in) return v;
			v = ((v>>1)&0x55) | ((v&0x55)<<1);
			v = ((v>>2)&0x33) | ((v&0x33)<<2);
			return ((v>>4)&0xF) | ((v&0xF)<<4);
		}

		uint32_t _crc32_table[256];
		uint32_t init;
		bool ref_in, ref_out;
	};

	class CRC32
	{
	public:
		CRC32( const CRC32Calc& calculator ) : _calculator( calculator ) { calculator.begin_calc( _crc32 ); }

		void     add_byte( uint8_t byte )                                { _calculator.add_byte( byte, _crc32 ); }
		void     add_int32_be( uint32_t int_32 )                         { _calculator.add_int32_be( int_32, _crc32 ); }
		void     add_int32_le( uint32_t int_32 )                         { _calculator.add_int32_le( int_32, _crc32 ); }
		void     add_buffer( const void* buffer, size_t size )           { _calculator.add_buffer( buffer, size, _crc32 ); }
		void     add_string( const std::string& str )                    { _calculator.add_buffer( str.c_str(), str.length(), _crc32 ); }
		uint32_t get_crc() const                                         { return ~_crc32; }

	private:
		const CRC32Calc& _calculator;
		uint32_t         _crc32;
	};

} // namespace CRCVerifier

#endif // CRC32_H_
