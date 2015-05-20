// Includes
#include "CVehicleInterface.h"

#include "Utility.h"

// Set up easylogger
INITIALIZE_EASYLOGGINGPP

// Namespaces
using namespace rebop;

CVehicleInterface::CVehicleInterface()
{
	// Init attributes
  m_tPilotingThread = nullptr;

	m_isConnected     = false;
	m_isFlying        = false;
}

CVehicleInterface::~CVehicleInterface()
{

}

void CVehicleInterface::Initialize()
{
	// Initialize the network interface
	m_isConnected = m_networkInterface.Initialize();

	//Send "CommonCommonAllStates" command to avoid drifting
	CCommandPacket packet( 128 );

	// Generate command
	eARCOMMANDS_GENERATOR_ERROR cmdError = ARCOMMANDS_Generator_GenerateCommonCommonAllStates(
			packet.m_pData,
			packet.m_bufferSize,
			&packet.m_dataSize );

	if( cmdError == ARCOMMANDS_GENERATOR_OK )
	{
		// Command should be acknowledged
		if( !m_networkInterface.SendData( packet, EOutboundBufferId::OUTBOUND_WITH_ACK, true ) )
		{
			LOG( ERROR ) << "Failed to send anti-drift command.";
		}
	}
	else
	{
		LOG( ERROR ) << "Failed to generate anti-drift command. Err: " << cmdError;
	}

	LOG( INFO ) << "Sent anti-drift command.";

	StartPilotingThread();
}

bool CVehicleInterface::StartPilotingThread()
{
  LOG( INFO ) << "Starting piloting thread.";

  if( ARSAL_Thread_Create( &( m_tPilotingThread ), PilotingThreadFunction, this ) != 0 )
  {
    LOG( ERROR ) << "Creation of piloting thread failed.";
    return false;
  }

  LOG( INFO ) << "Piloting thread started.";

  return true;
}

bool CVehicleInterface::SendPilotCommand( const commands::bebop::navigation::TPilotCommand &commandIn )
{
  CCommandPacket packet( 128 );

  // Generate command
  eARCOMMANDS_GENERATOR_ERROR cmdError = ARCOMMANDS_Generator_GenerateARDrone3PilotingPCMD(
      packet.m_pData,
      packet.m_bufferSize,
      &packet.m_dataSize,
      commandIn.flag,
      commandIn.roll,
      commandIn.pitch,
      commandIn.yaw,
      commandIn.gaz,
      commandIn.psi );

  if( cmdError == ARCOMMANDS_GENERATOR_OK )
  {
    // Command should not be acknowledged
    if( !m_networkInterface.SendData( packet, EOutboundBufferId::OUTBOUND, true ) )
    {
      LOG( ERROR ) << "Failed to send Pilot Command.";
      return false;
    }
  }
  else
  {
    LOG( ERROR ) << "Failed to generate Pilot Command. Err: " << cmdError;
    return false;
  }

  return true;
}

void CVehicleInterface::Update()
{

}

void CVehicleInterface::Cleanup()
{
	// Shutdown the network
	m_networkInterface.Cleanup();

	if( m_tPilotingThread != nullptr )
	{
	  ARSAL_Thread_Join( m_tPilotingThread, nullptr );
	  ARSAL_Thread_Destroy( &( m_tPilotingThread ) );
	  m_tPilotingThread = nullptr;
  }
}

bool CVehicleInterface::IsConnected()
{
	return m_isConnected;
}

void CVehicleInterface::setAttitudeChangedCallback( TAttitudeChangedCallback callbackFun, void* ptrIn )
{
  m_networkInterface.setAttitudeChangedCallback( callbackFun, ptrIn );
}

void CVehicleInterface::setSpeedChangedCallback( TSpeedChangedCallback callbackFun, void* ptrIn )
{
  m_networkInterface.setSpeedChangedCallback( callbackFun, ptrIn );
}

void* CVehicleInterface::PilotingThreadFunction( void* dataIn )
{
  if( dataIn == nullptr)
  {
    LOG( ERROR ) << "Invalid pointer given to reader thread. Reader thread not active.";
    return nullptr;
  }

  CVehicleInterface *m_vehicleInterface = ( CVehicleInterface* ) dataIn;

  while( m_vehicleInterface->m_networkInterface.isRunning() )
  {
    if( m_vehicleInterface->m_isFlying )
    {
      m_vehicleInterface->SendPilotCommand( m_vehicleInterface->m_attitude );
      usleep(25000);
    }
  }

  return nullptr;
}
