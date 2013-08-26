
#ifndef COBC_SMPC__TOPIC_H
#define COBC_SMPC__TOPIC_H

#include <stdint.h>
#include <cobc/rtos/mutex.h>
#include <cobc/utils.h>

#include "list.h"

namespace cobc
{
	namespace smpc
	{
		// forward declaration
		class Subscription;

		/**
		 * Non-template base class for %Topic<>.
		 *
		 * @warning
		 * 		This class should only be used through cobc::smpc::Topic
		 * 		and never alone!
		 *
		 * @see		Topic
		 * @author	Fabian Greif <fabian.greif@dlr.de>
		 */
		class TopicBase : protected List<TopicBase>
		{
			// Needed to allow Subscription() to append itself to the
			// subscription list
			friend class Subscription;
			friend class List<TopicBase>;

		public:
			/**
			 * Constructor.
			 */
			TopicBase();

			/**
			 * Destroy the topic.
			 *
			 * @warning
			 * 		The destruction and creation of topic during the normal
			 * 		runtime is not thread-safe. If topics need to be
			 * 		destroyed outside the initialization of the application
			 * 		it is necessary to hold all other threads which
			 * 		might also create or destroy topics and/or subscriptions.
			 */
			~TopicBase();

			/**
			 * Publish new data.
			 *
			 * Forwards the pointer to all connected subscribers. This
			 * function is thread safe.
			 */
			void
			publish(void * message);

#ifdef UNITTEST
			void
			dumpConnectedSubscriptions();
#endif

		protected:
			/// List of all topics currently active.
			static TopicBase* listOfAllTopics;

			/// Used to protect the publish() method
			rtos::Mutex mutex;

			/// Pointer to the list of subscriptions
			Subscription* subscriptions;

		private:
			static void
			clearSubscriptions();
		};

		/**
		 * Type-safe %Topic.
		 *
		 * Type-safe data exchange between different components. Data
		 * can be published under a predefined topic. Any number of
		 * subscribers can connect to that topic to get notified if new
		 * data is published.
		 *
		 * There is also a not type-safe version of this %Topic<>%Subscriber
		 * protocol called cobc::smpc::TopicRaw. Use this version if you need
		 * to exchange data with a unknown and/or changing length.
		 *
		 * @ingroup	smpc
		 * @see		TopicRaw
		 * @see		Subscription
		 * @see		Subscriber
		 * @author	Fabian Greif <fabian.greif@dlr.de>
		 */
		template <typename T>
		class Topic : protected TopicBase
		{
			// Needed to allow up-casting in smpc::Subscription constructor
			friend class Subscription;

		public:
			/// Type of the data distributed by this topic.
			typedef T Type;
			typedef typename cobc::remove_const<T>::type NonConstType;

			/**
			 * Constructor.
			 */
			inline Topic() :
				TopicBase()
			{
			}

			/**
			 * Destroy the topic.
			 *
			 * @warning
			 * 		The destruction and creation of topics during the normal
			 * 		runtime is not thread-safe. If topics need to be
			 * 		destroyed outside the initialization of the application
			 * 		it is necessary to hold all other threads which
			 * 		might also create or destroy topics and/or subscriptions.
			 */
			~Topic()
			{
			}

			/**
			 * Publish new data.
			 *
			 * Forwards the pointer to all connected subscribers. This
			 * function is thread safe.
			 */
			inline void
			publish(T& message)
			{
				// This cast may remove any const qualifiers from the
				// type. This is no problem because they are enforced
				// when creating a subscription.
				// The qualifiers are only stripped for the internal
				// processing which is done invisible to the user.
				// See also cobc::com::Subscription::Subscription().
				NonConstType * ptr = const_cast< NonConstType * >(&message);
				TopicBase::publish(reinterpret_cast<void *>(ptr));
			}

#ifdef UNITTEST
			using TopicBase::dumpConnectedSubscriptions;
#endif
		};
	}
}

#endif // COBC_SMPC__TOPIC_H