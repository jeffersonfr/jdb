#pragma once

#include <functional>
#include <optional>
#include <mutex>

template<typename T>
struct MutableState;

template<typename T>
struct State {
  State(MutableState<T> &state) : mState{state} {
  }

  virtual ~State() = default;

  virtual void observe(std::function<void(T const &)> callback) {
    mState.mCallback = callback;
  }

private:
  MutableState<T> &mState;
};

template<typename T>
struct VolatileState : public State<T> {
  friend class State<T>;

  VolatileState() : State<T>(*this) {
  }

  void observe(std::function<void(T const &)> callback) override {
    mCallback = callback;
  }

  void notify(T const &data) { mCallback(data); }

private:
  std::function<void(T const &)> mCallback;
};

template<typename T>
struct MutableState : public State<T> {
  friend class State<T>;

  MutableState() : State<T>(*this) {
  }

  void observe(std::function<void(T const &)> callback) override {
    mCallback = callback;
  }

  void notify(T const &data) {
    std::scoped_lock lock(mMutex);

    mData = data;

    mCallback(data);
  }

  std::optional<T> get() {
    return mData;
  }

private:
  std::function<void(T const &)> mCallback;
  std::optional<T> mData;
  std::mutex mMutex;
};
