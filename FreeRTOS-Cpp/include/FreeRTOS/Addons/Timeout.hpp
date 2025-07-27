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

#ifndef FREERTOS_ADDONS_TIMEOUT_HPP
#define FREERTOS_ADDONS_TIMEOUT_HPP

#include <FreeRTOS/Addons/Clock.hpp>

namespace FreeRTOS::Addons {

/**
 * @class Timeout Timeout.hpp <FreeRTOS/Timeout.hpp>
 *
 * @brief Timeout class based on FreeRTOS::Addons::Clock (<tt>std::chrono</tt>)
 * that facilitates timeout management. Timeout tracks time elapsed since
 * creation which helps handling timeout in sequences of blocking operations.
 *
 * Timeout is meant to be used together with extended FreeRTOS-Cpp APIs that
 * alter <tt>TickType_t</tt> with <tt>template<typename Rep, typename Period>
 * std::chrono::duration<Rep, Period></tt> or similar <tt>std::chrono</tt>
 * expressions.
 */
class Timeout {
 public:
  static Clock::duration max() {
    return Clock::duration::max();
  }

  template <typename Rep, typename Period>
  Timeout(const std::chrono::duration<Rep, Period> &duration = max())
      : duration{std::chrono::duration_cast<Clock::duration>(duration)} {
    this->initialTimePoint = Clock::now();
  }

  /**
   * @brief Returns the elapsed time since the Timeout was created.
   *
   * @retval Clock::duration The duration that has passed since the Timeout was
   * created.
   */
  Clock::duration elapsed() const {
    return Clock::now() - initialTimePoint;
  }

  /**
   * @brief Checks if the Timeout has expired.
   *
   * @retval true If the elapsed time is greater than or equal to the duration
   * specified at construction.
   * @retval false Otherwise.
   */
  bool is_expired() const {
    return this->elapsed() >= duration;
  }

  /**
   * @brief Returns the remaining time until the Timeout expires.
   *
   * @retval Clock::duration The remaining time until the Timeout expires.
   * @retval Clock::duration::zero() if the Timeout has already expired.
   */
  Clock::duration left() const {
    const auto elapsed = this->elapsed();
    if (elapsed >= duration) {
      return Clock::duration::zero();
    }
    return duration - elapsed;
  }

 private:
  Clock::duration duration;
  Clock::time_point initialTimePoint;
};

}  // namespace FreeRTOS::Addons

#endif  // FREERTOS_ADDONS_TIMEOUT_HPP