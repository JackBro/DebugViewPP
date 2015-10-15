// (C) Copyright Gert-Jan de Vos and Jan Wilmans 2013.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

// Repository at: https://github.com/djeedjay/DebugViewPP/

#pragma once

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "PassiveLogSource.h"
#include "Process.h"

namespace fusion {
namespace debugviewpp {

class ILineBuffer;

class SocketReader : public PassiveLogSource
{
public:
	SocketReader(Timer& timer, ILineBuffer& lineBuffer, int port);
	virtual ~SocketReader();

	virtual void Abort();

private:
	void Loop();
	void StartReceive();
	void ReceiveUDPMessage(const boost::system::error_code& error, std::size_t bytes_transferred);

	int m_port;
	boost::asio::io_service m_ioservice;
	boost::asio::ip::udp::socket m_socket;
	boost::asio::ip::udp::endpoint m_remote_endpoint;

	boost::array<char, 2000> m_recvBuffer;
	boost::thread m_thread;
};

} // namespace debugviewpp 
} // namespace fusion
