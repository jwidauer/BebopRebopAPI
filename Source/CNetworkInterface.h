#pragma once

// Includes
#include <string>
#include <functional>

extern "C"
{
#include <stdio.h>
#include <malloc.h>
#include <libARSAL/ARSAL.h>
#include <libARNetwork/ARNetwork.h>
#include <libARNetworkAL/ARNetworkAL.h>
#include <libARCommands/ARCommands.h>
#include <libARDiscovery/ARDiscovery.h>
}

#include "CNetworkSettings.h"
#include "CCommandPacket.h"

namespace rebop
{

// Typedefs
typedef std::function<void()> TDisconnectionCallback;
typedef std::function<void()> TConnectionCallback;
typedef std::function<void(float, float, float, void*)> TAttitudeChangedCallback;
typedef std::function<void(float, float, float, void*)> TSpeedChangedCallback;
typedef std::function<eARNETWORK_MANAGER_CALLBACK_RETURN( int bufferIdIn, uint8_t *dataIn, void *customDataIn, eARNETWORK_MANAGER_CALLBACK_STATUS causeIn )> TCommandCallback;
typedef struct
{
	void *networkInterface;
	EInboundBufferId bufferID;
} Rx_Threads_Data_t;

class CNetworkInterface
{
protected:
	// Pointers
	ARNETWORKAL_Manager_t 		     *	m_pNetworkALManager;
	ARNETWORK_Manager_t 		       *	m_pNetworkManager;
	void                           *  m_pAttitudeChangedCallbackObject;
	void                           *  m_pSpeedChangedCallbackObject;

	TCommandCallback				          m_pDefaultCommandCallback;
	TConnectionCallback			         	m_pConnectionCallback;
	TDisconnectionCallback		       	m_pDisconnectionCallback;
	TAttitudeChangedCallback          m_pAttitudeChangedCallback;
	TSpeedChangedCallback             m_pSpeedChangedCallback;


	// Threads
	ARSAL_Thread_t 				          	m_tRxThread;
	ARSAL_Thread_t 			    	      	m_tTxThread;
	ARSAL_Thread_t				          	m_tMonitorThread;

	std::array<ARSAL_Thread_t, 2>	    m_tRxThreads;	// Threads to read data from buffer

	// Attributes
	CNetworkSettings 			          	m_networkSettings;
	std::array<Rx_Threads_Data_t, 2>  m_tRxThreads_data;

	bool 					 		m_killMonitor;
	bool 							m_isConnected;
	bool							m_isRunning;

	// Constants
	static const uint32_t 	          m_kMonitorRetryDelay 	= 5;
	static const int 				          m_kMaxBytesToRead 		= 128 * 1024;		// 128kb - arbitrary

public:
	// Methods
	CNetworkInterface();
	virtual ~CNetworkInterface();

	// Composite behavior functions
	bool Initialize();
	bool PerformNetworkDiscovery();
	bool InitializeNetworkManagers();
	bool StartNetworkThreads();
	bool isRunning();
	void StopNetwork();
	void Cleanup();

	// Wrapper functions
	bool Flush();
	bool SendData( const CCommandPacket &dataIn, EOutboundBufferId outboundBufferIdIn, bool doDataCopyIn );
	bool ReadData( CCommandPacket& dataOut, EInboundBufferId inboundBufferIdIn );
	bool TryReadData( CCommandPacket& dataOut, EInboundBufferId inboundBufferIdIn );
	bool ReadDataWithTimeout( CCommandPacket& dataOut, EInboundBufferId inboundBufferIdIn, uint32_t timeoutMsIn );
	bool FlushInboundBuffer( EInboundBufferId inboundBufferIdIn );
	bool FlushOutboundBuffer( EOutboundBufferId outboundBufferIdIn );
	bool SetMinimumTimeBetweenSends( uint32_t delayMsIn );
	int GetEstimatedLatency();
	int GetEstimatedMissPercentage( EOutboundBufferId outboundBufferIdIn );
	void DecodeData( CCommandPacket& dataOut);

	// Callback setting functions
	void setAttitudeChangedCallback( TAttitudeChangedCallback callbackFun, void* ptrIn );
	void setSpeedChangedCallback( TSpeedChangedCallback callbackFun, void* ptrIn);

	// Callback registration functions
	void RegisterFlightCommandCallbacks();   // register callbacks for flight commands
	void RegisterConnectionCallback( TConnectionCallback callbackIn );
	void UnregisterConnectionCallback();
	void RegisterDisconnectionCallback( TDisconnectionCallback callbackIn );
	void UnregisterDisconnectionCallback();

	// Callback functions
	static eARNETWORK_MANAGER_CALLBACK_RETURN DefaultCommandCallback( int bufferIdIn, uint8_t *dataIn, void *customDataIn, eARNETWORK_MANAGER_CALLBACK_STATUS causeIn );
	static void OnDisconnect( ARNETWORK_Manager_t *networkManagerIn, ARNETWORKAL_Manager_t *networkALManagerIn, void *customDataIn );
	static eARDISCOVERY_ERROR SendJsonCallback( uint8_t *txDataIn, uint32_t *txDataSizeIn, void *customDataIn );
	static eARDISCOVERY_ERROR ReceiveJsonCallback( uint8_t *dataRxIn, uint32_t dataRxSizeIn, char *ipIn, void *customDataIn );

	// Monitor/Reader thread function
	static void* MonitorThreadFunction( void* dataIn );
	static void* ReaderThreadFunction( void* dataIn );
};

}
