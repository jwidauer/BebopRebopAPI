// Includes
#include "CBebopInterface.h"

#include "Utility.h"

// Namespaces
using namespace rebop;

CBebopInterface::CBebopInterface()
{

}

CBebopInterface::~CBebopInterface()
{
}


void CBebopInterface::Update()
{
	// TODO: Create a low frequency thread to manage reconnection behaviour and treat the entire interface as a subsystem
}

bool CBebopInterface::Takeoff()
{
	CCommandPacket packet( 128 );

	// Generate command
	eARCOMMANDS_GENERATOR_ERROR cmdError = ARCOMMANDS_Generator_GenerateARDrone3PilotingTakeOff(
			packet.m_pData,
			packet.m_bufferSize,
			&packet.m_dataSize );

	if( cmdError == ARCOMMANDS_GENERATOR_OK )
	{
		// Command should be acknowledged
		if( !m_networkInterface.SendData( packet, EOutboundBufferId::OUTBOUND_WITH_ACK, true ) )
		{
			LOG( ERROR ) << "Failed to send takeoff command.";
			return false;
		}
	}
	else
	{
		LOG( ERROR ) << "Failed to generate takeoff command. Err: " << cmdError;
		return false;
	}

	m_isFlying = true;

	return true;
}

bool CBebopInterface::Land()
{
	CCommandPacket packet( 128 );

	// Generate command
	eARCOMMANDS_GENERATOR_ERROR cmdError = ARCOMMANDS_Generator_GenerateARDrone3PilotingLanding(
			packet.m_pData,
			packet.m_bufferSize,
			&packet.m_dataSize );

	if( cmdError == ARCOMMANDS_GENERATOR_OK )
	{
		// Command should be acknowledged
		if( !m_networkInterface.SendData( packet, EOutboundBufferId::OUTBOUND_WITH_ACK, true ) )
		{
			LOG( ERROR ) << "Failed to send landing command.";
			return false;
		}
	}
	else
	{
		LOG( ERROR ) << "Failed to generate landing command. Err: " << cmdError;
		return false;
	}

	m_isFlying = false;

	return true;
}

bool CBebopInterface::Emergency()
{
	CCommandPacket packet( 128 );

	// Generate command
	eARCOMMANDS_GENERATOR_ERROR cmdError = ARCOMMANDS_Generator_GenerateARDrone3PilotingEmergency(
			packet.m_pData,
			packet.m_bufferSize,
			&packet.m_dataSize );

	if( cmdError == ARCOMMANDS_GENERATOR_OK )
	{
		// Command should be acknowledged
		if( !m_networkInterface.SendData( packet, EOutboundBufferId::OUTBOUND_WITH_ACK, true ) )
		{
			LOG( ERROR ) << "Failed to send emergency command.";
			return false;
		}
	}
	else
	{
		LOG( ERROR ) << "Failed to generate emergency command. Err: " << cmdError;
		return false;
	}

	m_isFlying = false;

	return true;
}

bool CBebopInterface::setAttitude( TPilotCommand attitudeIn )
{
  if( abs(attitudeIn.roll) > 100 || abs(attitudeIn.pitch) > 100 || abs(attitudeIn.yaw) > 100 || abs(attitudeIn.gaz) > 100 )
  {
    LOG( ERROR ) << "Attitude commands outside of range [-100,100].";
    return false;
  }

  m_attitude = attitudeIn;

  return true;
}

bool CBebopInterface::NavigateHome( ENavigateHome startOrStopIn )
{
	CCommandPacket packet( 128 );

	// Generate command
	eARCOMMANDS_GENERATOR_ERROR cmdError = ARCOMMANDS_Generator_GenerateARDrone3PilotingNavigateHome(
			packet.m_pData,
			packet.m_bufferSize,
			&packet.m_dataSize,
			(uint8_t)startOrStopIn );

	if( cmdError == ARCOMMANDS_GENERATOR_OK )
	{
		// Command should be acknowledged
		if( !m_networkInterface.SendData( packet, EOutboundBufferId::OUTBOUND_WITH_ACK, true ) )
		{
			LOG( ERROR ) << "Failed to send NavigateHome command.";
			return false;
		}
	}
	else
	{
		LOG( ERROR ) << "Failed to generate NavigateHome command. Err: " << cmdError;
		return false;
	}

	return true;
}

bool CBebopInterface::SetAutoTakeoffMode( EAutoTakeoffMode modeIn )
{
	CCommandPacket packet( 128 );

	// Generate command
	eARCOMMANDS_GENERATOR_ERROR cmdError = ARCOMMANDS_Generator_GenerateARDrone3PilotingAutoTakeOffMode(
			packet.m_pData,
			packet.m_bufferSize,
			&packet.m_dataSize,
			(uint8_t)modeIn );

	if( cmdError == ARCOMMANDS_GENERATOR_OK )
	{
		// Command should be acknowledged
		if( !m_networkInterface.SendData( packet, EOutboundBufferId::OUTBOUND_WITH_ACK, true ) )
		{
			LOG( ERROR ) << "Failed to send AutoTakeoffMode command.";
			return false;
		}
	}
	else
	{
		LOG( ERROR ) << "Failed to generate AutoTakeoffMode command. Err: " << cmdError;
		return false;
	}

	return true;
}

bool CBebopInterface::Flip( EFlipDirection directionIn )
{
	CCommandPacket packet( 128 );

	// Generate command
	eARCOMMANDS_GENERATOR_ERROR cmdError = ARCOMMANDS_Generator_GenerateARDrone3AnimationsFlip(
			packet.m_pData,
			packet.m_bufferSize,
			&packet.m_dataSize,
			(eARCOMMANDS_ARDRONE3_ANIMATIONS_FLIP_DIRECTION)directionIn );

	if( cmdError == ARCOMMANDS_GENERATOR_OK )
	{
		// Command should be acknowledged
		if( !m_networkInterface.SendData( packet, EOutboundBufferId::OUTBOUND_WITH_ACK, true ) )
		{
			LOG( ERROR ) << "Failed to send Flip command.";
			return false;
		}
	}
	else
	{
		LOG( ERROR ) << "Failed to generate Flip command. Err: " << cmdError;
		return false;
	}

	return true;
}

bool CBebopInterface::SetMaxAltitude( float maxAltitudeMetersIn )
{
	CCommandPacket packet( 128 );

	// Generate command
	eARCOMMANDS_GENERATOR_ERROR cmdError = ARCOMMANDS_Generator_GenerateARDrone3PilotingSettingsMaxAltitude(
			packet.m_pData,
			packet.m_bufferSize,
			&packet.m_dataSize,
			maxAltitudeMetersIn );

	if( cmdError == ARCOMMANDS_GENERATOR_OK )
	{
		// Command should be acknowledged
		if( !m_networkInterface.SendData( packet, EOutboundBufferId::OUTBOUND_WITH_ACK, true ) )
		{
			LOG( ERROR ) << "Failed to send AutoTakeoffMode command.";
			return false;
		}
	}
	else
	{
		LOG( ERROR ) << "Failed to generate AutoTakeoffMode command. Err: " << cmdError;
		return false;
	}

	return true;
}

bool CBebopInterface::SetMaxTilt( float maxTiltDegreesIn )
{
	CCommandPacket packet( 128 );

	// Generate command
	eARCOMMANDS_GENERATOR_ERROR cmdError = ARCOMMANDS_Generator_GenerateARDrone3PilotingSettingsMaxTilt(
			packet.m_pData,
			packet.m_bufferSize,
			&packet.m_dataSize,
			maxTiltDegreesIn );

	if( cmdError == ARCOMMANDS_GENERATOR_OK )
	{
		// Command should be acknowledged
		if( !m_networkInterface.SendData( packet, EOutboundBufferId::OUTBOUND_WITH_ACK, true ) )
		{
			LOG( ERROR ) << "Failed to send SetMaxTilt command.";
			return false;
		}
	}
	else
	{
		LOG( ERROR ) << "Failed to generate SetMaxTilt command. Err: " << cmdError;
		return false;
	}

	return true;
}

bool CBebopInterface::SetMaxVerticalSpeed( float maxVerticalSpeedMetersPerSecIn )
{
	CCommandPacket packet( 128 );

	// Generate command
	eARCOMMANDS_GENERATOR_ERROR cmdError = ARCOMMANDS_Generator_GenerateARDrone3SpeedSettingsMaxVerticalSpeed(
			packet.m_pData,
			packet.m_bufferSize,
			&packet.m_dataSize,
			maxVerticalSpeedMetersPerSecIn );

	if( cmdError == ARCOMMANDS_GENERATOR_OK )
	{
		// Command should be acknowledged
		if( !m_networkInterface.SendData( packet, EOutboundBufferId::OUTBOUND_WITH_ACK, true ) )
		{
			LOG( ERROR ) << "Failed to send SetMaxVerticalSpeed command.";
			return false;
		}
	}
	else
	{
		LOG( ERROR ) << "Failed to generate SetMaxVerticalSpeed command. Err: " << cmdError;
		return false;
	}

	return true;
}

bool CBebopInterface::SetMaxRotationSpeed( float maxRotationSpeedDegPerSecIn )
{
	CCommandPacket packet( 128 );

	// Generate command
	eARCOMMANDS_GENERATOR_ERROR cmdError = ARCOMMANDS_Generator_GenerateARDrone3SpeedSettingsMaxRotationSpeed(
			packet.m_pData,
			packet.m_bufferSize,
			&packet.m_dataSize,
			maxRotationSpeedDegPerSecIn );

	if( cmdError == ARCOMMANDS_GENERATOR_OK )
	{
		// Command should be acknowledged
		if( !m_networkInterface.SendData( packet, EOutboundBufferId::OUTBOUND_WITH_ACK, true ) )
		{
			LOG( ERROR ) << "Failed to send SetMaxRotationSpeed command.";
			return false;
		}
	}
	else
	{
		LOG( ERROR ) << "Failed to generate SetMaxRotationSpeed command. Err: " << cmdError;
		return false;
	}

	return true;
}

bool CBebopInterface::SetAbsoluteControlMode( EAbsoluteControlMode modeIn )
{
	CCommandPacket packet( 128 );

	// Generate command
	eARCOMMANDS_GENERATOR_ERROR cmdError = ARCOMMANDS_Generator_GenerateARDrone3PilotingSettingsAbsolutControl(
			packet.m_pData,
			packet.m_bufferSize,
			&packet.m_dataSize,
			(uint8_t)modeIn );

	if( cmdError == ARCOMMANDS_GENERATOR_OK )
	{
		// Command should be acknowledged
		if( !m_networkInterface.SendData( packet, EOutboundBufferId::OUTBOUND_WITH_ACK, true ) )
		{
			LOG( ERROR ) << "Failed to send SetAbsoluteControlMode command.";
			return false;
		}
	}
	else
	{
		LOG( ERROR ) << "Failed to generate SetAbsoluteControlMode command. Err: " << cmdError;
		return false;
	}

	return true;
}

bool CBebopInterface::SetHullProtectionPresence( EHullPresence presenceIn )
{
	CCommandPacket packet( 128 );

	// Generate command
	eARCOMMANDS_GENERATOR_ERROR cmdError = ARCOMMANDS_Generator_GenerateARDrone3SpeedSettingsHullProtection(
			packet.m_pData,
			packet.m_bufferSize,
			&packet.m_dataSize,
			(uint8_t)presenceIn );

	if( cmdError == ARCOMMANDS_GENERATOR_OK )
	{
		// Command should be acknowledged
		if( !m_networkInterface.SendData( packet, EOutboundBufferId::OUTBOUND_WITH_ACK, true ) )
		{
			LOG( ERROR ) << "Failed to send SetHullProtectionPresence command.";
			return false;
		}
	}
	else
	{
		LOG( ERROR ) << "Failed to generate SetHullProtectionPresence command. Err: " << cmdError;
		return false;
	}

	return true;
}

bool CBebopInterface::SetOutdoorMode( EOutdoorMode modeIn )
{
	CCommandPacket packet( 128 );

	// Generate command
	eARCOMMANDS_GENERATOR_ERROR cmdError = ARCOMMANDS_Generator_GenerateARDrone3SpeedSettingsOutdoor(
			packet.m_pData,
			packet.m_bufferSize,
			&packet.m_dataSize,
			(uint8_t)modeIn );

	if( cmdError == ARCOMMANDS_GENERATOR_OK )
	{
		// Command should be acknowledged
		if( !m_networkInterface.SendData( packet, EOutboundBufferId::OUTBOUND_WITH_ACK, true ) )
		{
			LOG( ERROR ) << "Failed to send SetOutdoorMode command.";
			return false;
		}
	}
	else
	{
		LOG( ERROR ) << "Failed to generate SetOutdoorMode command. Err: " << cmdError;
		return false;
	}

	return true;
}

bool CBebopInterface::SetGpsHomeLocation( const TGpsHomeLocation& locationIn )
{
	CCommandPacket packet( 128 );

	// Generate command
	eARCOMMANDS_GENERATOR_ERROR cmdError = ARCOMMANDS_Generator_GenerateARDrone3GPSSettingsSetHome(
			packet.m_pData,
			packet.m_bufferSize,
			&packet.m_dataSize,
			locationIn.latitude,
			locationIn.longitude,
			locationIn.altitude );

	if( cmdError == ARCOMMANDS_GENERATOR_OK )
	{
		// Command should be acknowledged
		if( !m_networkInterface.SendData( packet, EOutboundBufferId::OUTBOUND_WITH_ACK, true ) )
		{
			LOG( ERROR ) << "Failed to send SetGpsHomeLocation command.";
			return false;
		}
	}
	else
	{
		LOG( ERROR ) << "Failed to generate SetGpsHomeLocation command. Err: " << cmdError;
		return false;
	}

	return true;
}

bool rebop::CBebopInterface::FlatTrim()
{
	CCommandPacket packet( 128 );

	// Generate command
	eARCOMMANDS_GENERATOR_ERROR cmdError = ARCOMMANDS_Generator_GenerateARDrone3PilotingFlatTrim(
			packet.m_pData,
			packet.m_bufferSize,
			&packet.m_dataSize );

	if( cmdError == ARCOMMANDS_GENERATOR_OK )
	{
		// Command should be acknowledged
		if( !m_networkInterface.SendData( packet, EOutboundBufferId::OUTBOUND_WITH_ACK, true ) )
		{
			LOG( ERROR ) << "Failed to send takeoff command.";
			return false;
		}
	}
	else
	{
		LOG( ERROR ) << "Failed to generate flat trim command. Err: " << cmdError;
		return false;
	}

	return true;
}

bool CBebopInterface::ResetGpsHome()
{
	CCommandPacket packet( 128 );

	// Generate command
	eARCOMMANDS_GENERATOR_ERROR cmdError = ARCOMMANDS_Generator_GenerateARDrone3GPSSettingsResetHome(
			packet.m_pData,
			packet.m_bufferSize,
			&packet.m_dataSize );

	if( cmdError == ARCOMMANDS_GENERATOR_OK )
	{
		// Command should be acknowledged
		if( !m_networkInterface.SendData( packet, EOutboundBufferId::OUTBOUND_WITH_ACK, true ) )
		{
			LOG( ERROR ) << "Failed to send ResetGpsHome command.";
			return false;
		}
	}
	else
	{
		LOG( ERROR ) << "Failed to generate ResetGpsHome command. Err: " << cmdError;
		return false;
	}

	return true;
}
