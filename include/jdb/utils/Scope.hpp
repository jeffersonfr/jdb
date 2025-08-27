#pragma once

#include <functional>

#include <boost/asio/io_service.hpp>
#include <boost/asio/strand.hpp>
#include <boost/thread/thread.hpp>

#include <iostream>

namespace jdb {
  struct Scope {
    Scope(std::size_t threads = 1)
      : mStrand{mIoContext}, mWorkGuard{boost::asio::make_work_guard(mIoContext)} {
      for (std::size_t i = 0; i < threads; ++i) {
        mThreads.create_thread([&]() {
          mIoContext.run();
        });
      }
    }

    ~Scope() {
      stop();
    }

    void post(std::function<void()> task) {
      mIoContext.post(task);
    }

    void post_ordered(std::function<void()> task) {
      mStrand.post(task);
    }

    void stop() {
      if (mWorkGuard.owns_work()) {
        mWorkGuard.reset();
        mIoContext.stop();
        mThreads.interrupt_all();
        mThreads.join_all();
      }
    }

  private:
    boost::asio::io_context mIoContext;
    boost::asio::io_service::strand mStrand;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> mWorkGuard;
    boost::thread_group mThreads;
  };
}
