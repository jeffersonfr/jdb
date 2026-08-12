#pragma once

#include <functional>
#include <iostream>

#include <boost/asio/io_context.hpp>
#include <boost/asio/strand.hpp>
#include <boost/thread/thread.hpp>

#include <crypt.h>

namespace jdb {
  struct Scope {
    Scope(std::size_t threads = 1)
      : mStrand{mIoContext.get_executor()}, 
      mWorkGuard{boost::asio::make_work_guard(mIoContext)} {
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
      std::allocator<void> allocator;

      mStrand.post(task, allocator);
    }

    void post_ordered(std::function<void()> task) {
      std::allocator<void> allocator;

      mStrand.post(task, allocator);
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
    boost::asio::strand<boost::asio::io_context::executor_type> mStrand;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> mWorkGuard;
    boost::thread_group mThreads;
  };
}

/*
namespace jdb {
  struct Scope {
    Scope(std::size_t threads = 1)
      : mStrand{mIoContext}, 
      mWorkGuard{boost::asio::make_work_guard(mIoContext)} {
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
      std::allocator<void> allocator;

      mStrand.post(task, allocator);
    }

    void post_ordered(std::function<void()> task) {
      boost::asio::post(mStrand, task);
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
    boost::asio::io_context::strand mStrand;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> mWorkGuard;
    boost::thread_group mThreads;
  };
}
*/

