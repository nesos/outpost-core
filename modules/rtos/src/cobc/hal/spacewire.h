/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef COBC_HAL_SPACEWIRE_H
#define COBC_HAL_SPACEWIRE_H

#include <stdint.h>
#include <cstddef>

namespace cobc
{
	namespace hal
	{
		/**
		 * SpaceWire Interface
		 *
		 *
		 *
		 * @author	Fabian Greif <fabian.greif@dlr.de>
		 */
		class SpaceWire
		{
		public:
			enum Blocking
			{
				NON_BLOCKING = 0,
				BLOCKING = 1
			};

			enum EndMarker
			{
				PARTIAL = 0,	///< Leave packet open to add additional data later
				EOP = 1,		///< End of packet
				EEP = 2			///< Error end of packet
			};

			enum Result
			{
				SUCCESS,
				FAILURE,
				TIMEOUT
			};

			/**
			 * Send buffer.
			 */
			struct TransmitBuffer
			{
				TransmitBuffer(uint8_t * d) :
					data(d), length(), end(EOP)
				{
				}

				/**
				 * Points to preallocated memory section. Max. size is
				 * implementation specific.
				 */
				uint8_t * const data;
				std::size_t length;
				EndMarker end;
			};

			struct ReceiveBuffer
			{
				ReceiveBuffer() :
					data(0), length(0), end(EOP)
				{
				}

				const ReceiveBuffer&
				operator = (const ReceiveBuffer& other)
				{
					data = other.data;
					length = other.length;
					end = other.end;

					return *this;
				}

				const uint8_t * data;
				std::size_t length;
				EndMarker end;
			};

			virtual ~SpaceWire();

			/**
			 * Open a configured SpaceWire device.
			 *
			 * @return	`true` if the device could be opened,
			 * 			`false` otherwise.
			 */
			virtual bool
			open() = 0;

			virtual void
			close() = 0;

			virtual void
			up(Blocking blocking = NON_BLOCKING) = 0;

			virtual void
			down(Blocking blocking = NON_BLOCKING) = 0;

			virtual bool
			isUp() = 0;


			/**
			 * Request a send buffer.
			 *
			 * The SpaceWire link is blocked until the buffer is returned
			 * by sending the buffer via send()!
			 *
			 * \param[out]	buffer
			 * 		Pointer to a send buffer. 0 in a case of a failure.
			 * \param[in]	blocking
			 * 		Blocking mode.
			 */
			virtual Result
			requestBuffer(TransmitBuffer *& buffer, Blocking blocking = BLOCKING) = 0;

			/**
			 * Send a configured buffer.
			 *
			 * This releases the buffer.
			 * The buffer must contain a complete message.
			 *
			 * \param[out]	buffer
			 * 		Pointer to a send buffer. Must be the same pointer which
			 * 		was requested via requestBuffer() earlier.
			 */
			virtual Result
			send(TransmitBuffer * buffer) = 0;

			/**
			 * Receive data.
			 *
			 * \param[out]	buffer
			 * 		Pointer to a receive buffer. 0 in a case of a failure.
			 * \param[in]	blocking
			 * 		Blocking mode.
			 */
			virtual Result
			receive(ReceiveBuffer& buffer, Blocking blocking = BLOCKING) = 0;

			/**
			 * Release receive buffer.
			 *
			 * \param[in]	buffer
			 * 		Send buffer. Must be the same pointer which	was acquired
			 * 		by a receive() earlier.
			 */
			virtual void
			releaseBuffer(const ReceiveBuffer& buffer) = 0;

			/**
			 * Discard all messages currently waiting in the receive buffers.
			 */
			virtual void
			flushReceiveBuffer() = 0;
		};
	}
}

#endif // COBC_HAL_SPACEWIRE_H