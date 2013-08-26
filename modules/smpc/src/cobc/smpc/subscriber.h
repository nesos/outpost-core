
#ifndef COBC_SMPC__SUBSCRIBER_H
#define COBC_SMPC__SUBSCRIBER_H

namespace cobc
{
	namespace smpc
	{
		/**
		 * Empty base class for every class that should receive data from
		 * a topic.
		 * 
		 * This class is needed to provide a common base class to cast to when
		 * using the member functions in the Subscriber class.
		 * 
		 * @see		Subscriber
		 * @see		Topic
		 * 
		 * @ingroup	smpc
		 * @author	Fabian Greif <fabian.greif@dlr.de>
		 */
		class Subscriber
		{
		};
	}
}

#endif // COBC_SMPC__SUBSCRIBER_H