/*
 * ftdi.c
 *
 *  Created on: 01.10.2014.
 *      Author: SrkoS
 */

#include "ftdi.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//#include "../debug.h"

FT_STATUS ftdi_open(uint32_t BaudRate, uint32_t ReadTimeout_ms,
		FT_HANDLE *ftHandle)
{
	FT_STATUS ftStatus;
	int ftdi_index = 0;

	if (ftHandle == (FT_HANDLE *) NULL)
		return FT_INVALID_HANDLE;

	if (*ftHandle)
	{
		FT_Close(*ftHandle);
		*ftHandle = 0;
	}

	do
	{
		ftStatus = FT_Open(ftdi_index, ftHandle);

		if (ftStatus == 2)
			return ftStatus; // no other FTDI device

		ftdi_index++;
	} while (ftStatus);

	// OK / open !

	ftStatus = ftdi_configure_hnd(*ftHandle, BaudRate, ReadTimeout_ms);
	if (ftStatus)
	{
		FT_Close(*ftHandle);
		*ftHandle = 0;
	}

	return ftStatus;
}

/**
 * configure opened handle
 * @return
 */
FT_STATUS ftdi_configure_hnd(FT_HANDLE ftHandle, uint32_t speed,
		uint32_t timeout_ms)
{
	FT_STATUS ftStatus;
	uint8_t lat_timer = DEFAULT_LATENCYTIMER;

	uint32_t dwDriverVer;
	ftStatus = FT_GetDriverVersion(ftHandle, (DWORD *) &dwDriverVer);
//	//dbg_prn(DBG_UART, "FTDI Driver version = 0x%08x | status= %d", dwDriverVer,
	//		(uint32_t) ftStatus);

	//dbg_prn(DBG_UART, "> ftdi_configure_hnd([%p], speed= %d, timeout= %d)\n", ftHandle			speed, timeout_ms);

	ftStatus = FT_OK;

//Turn off bit bang mode
	ftStatus |= FT_SetBitMode(ftHandle, 0x00, 0);
// Reset the device
	ftStatus |= FT_ResetDevice(ftHandle);
// Purge transmit and receive buffers
	ftStatus |= FT_Purge(ftHandle, FT_PURGE_RX | FT_PURGE_TX);
// Set the baud rate
	ftStatus |= FT_SetBaudRate(ftHandle, speed);
// 1 s timeouts on read / write
	ftStatus |= FT_SetTimeouts(ftHandle, timeout_ms, 1000);
// Set to communicate at 8N1
	ftStatus |= FT_SetDataCharacteristics(ftHandle, FT_BITS_8, FT_STOP_BITS_1,
			FT_PARITY_NONE); // 8N1
// Disable hardware / software flow control
	ftStatus |= FT_SetFlowControl(ftHandle, FT_FLOW_NONE, 0, 0);
// Set the latency of the receive buffer way down (2 ms) to facilitate speedy transmission
	ftStatus |= FT_SetLatencyTimer(ftHandle, lat_timer);

	if (ftStatus)
	{
//		ReaderCloseHnd();

		//dbg_prn(DBG_USB, "Error while configure FTDI port!");
	}

// debug
	ftStatus = FT_GetLatencyTimer(ftHandle, &lat_timer);
	//dbg_prn(DBG_USB, "LatencyTimer(%d)=> %d\n", DEFAULT_LATENCYTIMER,
//			(int) lat_timer);

//	update_ftdi_info();

	return ftStatus;
}
//---------------------------------------------------------------------------

// Get the number of devices currently connected
/*
int ftdi_getDevNum(void)
{
	DWORD numDevs;
	int ret_num;

	FT_STATUS ftStatus = FT_ListDevices(&numDevs, NULL, FT_LIST_NUMBER_ONLY);

	if (ftStatus)
	{
		ret_num = ftStatus;

		//dbg_prn(DBG_UART, "FTDI FT_ListDevices failed: %d", ret_num);

		ret_num = -ret_num;
	}
	else
	{
		// FT_ListDevices OK, number of devices connected is in numDevs
		ret_num = numDevs;

		//dbg_prn(DBG_UART, "Number Of connected FTDI Devices= %d\n", ret_num);
	}

	return ret_num;
}
//---------------------------------------------------------------------------
*/
void ftdi_get_set_vid_pid(void)
{
#ifndef _WIN32

	FT_STATUS status;
	DWORD dwVID = 0;
	DWORD dwPID = 0;

	status = FT_GetVIDPID(&dwVID, &dwPID);
	//dbg_prn(DBG_UART, "(PRE)\t\t FT_GetVIDPID= (%d): %04X : %04X\n", status,
			dwVID, dwPID);

	dwVID = 0x0403;
	dwPID = 0x6001;
	status = FT_SetVIDPID(dwVID, dwPID);
	//dbg_prn(DBG_UNIT, "(SET_TO)\t FT_SetVIDPID= (%d): %04X : %04X\n", status,
			dwVID, dwPID);

	status = FT_GetVIDPID(&dwVID, &dwPID);
	//dbg_prn(DBG_UNIT, "(POST)\t\t FT_GetVIDPID= (%d): %04X : %04X\n", status,
			dwVID, dwPID);

#endif // #ifndef _WIN32
}
//---------------------------------------------------------------------------

int ftdi_write(FT_HANDLE ftHandle, void *buffer, uint32_t size)
{
	FT_STATUS ft_status;
	uint32_t bytes_written;
	int retry;
	int r = 0;

	for (retry = 0; retry <= MAX_RETRY_NUMBER_WRITE; ++retry)
	{
		ft_status = FT_Write(ftHandle, buffer, size, (LPDWORD) &bytes_written);

		//dbg_prn(DBG_UART, "FtWR:size= %d | wrtn= %d | stat= %u | retry= %d\n", size,
//				bytes_written, (uint32_t) ft_status, retry);

		if (ft_status == FT_OK)
			break; // TODO: check if OK and no all bytes written

		// sleep(1);
	}

	if (ft_status)
		return ft_status;

	if (bytes_written != size)
	{
//		return -abs(bytes_written - size);
		r = size;
		r -= bytes_written;
		r = abs(r);
		r = -r;
	}

	return r;
}
//------------------------------------------------------------------------------

int ftdi_read(FT_HANDLE ftHandle, void *buffer, uint32_t size)
{
	FT_STATUS ft_status;
	uint32_t bytes_returned;
	int retry;
	int r = 0;

	memset(buffer, 0, size);

	for (retry = 0; retry <= MAX_RETRY_NUMBER_READ; ++retry)
	{
		ft_status = FT_Read(ftHandle, buffer, size, (LPDWORD) &bytes_returned);

		uint8_t *d = buffer;

		//dbg_prn(DBG_UART, "FtRD:size= %d (rded= %d) | rd[0]= 0x%02X | "
				//"stat= %u | retry= %d\n", size, bytes_returned, d[0],
				//(uint32_t) ft_status, retry);

		if (bytes_returned == 0)
		{
			// bp
			continue;
		}

		if (ft_status == FT_OK)
			break; // TODO: check if OK and no all bytes written

		// sleep(1);
	}

	if (ft_status)
		return ft_status;
/*
	if (bytes_returned != size)
	{
//		return -abs(bytes_returned - size);
		r = size;
		r -= bytes_returned;
		r = abs(r);
		r = -r;
	}
*/
	return bytes_returned;
}
//------------------------------------------------------------------------------
