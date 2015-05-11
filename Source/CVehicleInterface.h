#pragma once

// Includes
#include "CNetworkInterface.h"
#include "Utility.h"
#include "BebopCommandsAndData.h"

namespace rebop
{

class CVehicleInterface
{
private:
  // private variables
  bool m_isConnected;

  ARSAL_Thread_t m_tPilotingThread;         // Thread for sending Piloting commands every 25 ms

  // Private methods
  bool StartPilotingThread();
  bool SendPilotCommand( const commands::bebop::navigation::TPilotCommand &poseIn );

  static void* PilotingThreadFunction( void* dataIn );

protected:
  // Attributes
  bool m_isFlying;
  CNetworkInterface m_networkInterface;
  commands::bebop::navigation::TPilotCommand m_attitude;

public:
	//Methods
	CVehicleInterface();
	virtual ~CVehicleInterface();

	virtual void Initialize();
	virtual void Update();
	virtual void Cleanup();
	virtual bool IsConnected();
};

}
