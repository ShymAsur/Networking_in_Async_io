#pragma once
#include "net_common.h"
//implementing a thread safe queue

namespace olc {
	namespace net {
		//lock queue
		template <typename T>
		class tsqueue {
		public:
			tsqueue() = default;
			tsqueue(const tsqueue<T>&) = delete;
			const T& front() {
				std::scoped_lock(muxQueue);
				return deqQueue.front();
			}
			const T& back() {
				std::scoped_lock(muxQueue);
				return deqQueue.back();
			}
			void push_back(const T& item) {
				std::scoped_lock(muxQueue);
				deqQueue.emplace_back(item);
			}
			void push_front(const T& item) {
				std::scoped_lock(muxQueue);
				deqQueue.emplace_front(item);
			}
			bool empty() {
				std::scoped_lock(muxQueue);
				return deqQueue.empty();
			}
			size_t count() {
				std::scoped_lock(muxQueue);
				return deqQueue.size();
			}
			void clear() {
				std::scoped_lock(muxQueue);
				deqQueue.clear();
			}
			T pop_front() {
				std::scoped_lock lock(muxQueue);
				auto t = std::move(deqQueue.front());
				deqQueue.pop_front();
				return t;
			}
			T pop_back() {
				std::scoped_lock lock(muxQueue);
				auto t = std::move(deqQueue.back());
				deqQueue.pop_back();
				return t;
			}
		protected:
			std::mutex muxQueue; //to prevent multiple threads from accessing the same queue.
			std::deque<T> deqQueue;
		};
	}
}