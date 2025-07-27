/*
 * FreeRTOS-Cpp
 * Copyright (C) 2021 Jon Enz. All Rights Reserved.
 * Copyright (C) 2025 Lukas Neverauskis.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * https://github.com/jonenz/FreeRTOS-Cpp
 */

#ifndef FREERTOS_ADDONS_LOCKGUARD_HPP
#define FREERTOS_ADDONS_LOCKGUARD_HPP

#include <FreeRTOS/Mutex.hpp>

namespace FreeRTOS::Addons {

/**
 * @class LockGuard LockGuard.hpp <FreeRTOS/LockGuard.hpp>
 *
 * @brief A simple RAII-style lock guard for FreeRTOS mutexes.
 */
class LockGuard {
 public:
  LockGuard(const LockGuard &) = delete;
  LockGuard &operator=(const LockGuard &) = delete;
  LockGuard(LockGuard &&) = delete;
  LockGuard &operator=(LockGuard &&) = delete;

  explicit LockGuard(FreeRTOS::MutexBase &mutex) : mutex(mutex) {
    this->mutex.lock();
  }

  ~LockGuard() {
    this->mutex.unlock();
  }

 private:
  FreeRTOS::MutexBase &mutex;
};

}  // namespace FreeRTOS::Addons

#endif  // FREERTOS_ADDONS_LOCKGUARD_HPP