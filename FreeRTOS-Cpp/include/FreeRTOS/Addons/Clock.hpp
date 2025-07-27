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

#ifndef FREERTOS_ADDONS_CLOCK_HPP
#define FREERTOS_ADDONS_CLOCK_HPP

#include <FreeRTOS/Kernel.hpp>
#include <chrono>

namespace FreeRTOS::Addons {

/**
 * @class Clock Clock.hpp <FreeRTOS/Clock.hpp>
 *
 * @brief FreeRTOS scheduler tick count
 * (<tt>FreeRTOS::Kernel::getTickCount()<tt>) std::chrono clock wrapper.
 *
 * Clock is meant to be used together with <tt>FreeRTOS::Addons::Timeout</tt>
 * and extended FreeRTOS-Cpp APIs that alter <tt>TickType_t</tt> with
 * <tt>template<typename Rep, typename Period> std::chrono::duration<Rep,
 * Period></tt> or similar <tt>std::chrono</tt> expressions.
 *
 * @note The epoch is defined as the moment when the FreeRTOS scheduler is
 * started.
 *
 * @see<https://en.cppreference.com/w/cpp/named_req/Clock.html>.
 */
struct Clock {
  typedef TickType_t rep;
  typedef std::ratio<1, configTICK_RATE_HZ> period;
  typedef std::chrono::duration<rep, period> duration;
  typedef std::chrono::time_point<Clock> time_point;

  static_assert(duration::max().count() == portMAX_DELAY,
                "FreeRTOS::Addons::Clock::duration::max() must be equivalent "
                "to portMAX_DELAY");

  static const bool is_steady = true;

  /**
   * @brief Converts a time_point to TickType_t.
   *
   * @note Expressed in similar style as
   * <https://en.cppreference.com/w/cpp/chrono/system_clock/to_time_t.html>.
   *
   * @param t <tt>time_point</tt> value to convert.
   * @return TickType_t The tick count corresponding to the time_point.
   */
  static constexpr TickType_t toTickCount(const time_point& t) noexcept {
    using namespace std::chrono;
    return pdMS_TO_TICKS(
        duration_cast<milliseconds>(t.time_since_epoch()).count());
  }

  /**
   * @brief Converts a TickType_t to time_point.
   *
   * @note Expressed in similar style as
   * <https://en.cppreference.com/w/cpp/chrono/system_clock/from_time_t.html>.
   *
   * @param t <tt>TickType_t</tt> value to convert.
   * @return time_point The time_point corresponding to the TickType_t.
   */
  static constexpr time_point fromTickCount(TickType_t t) noexcept {
    using namespace std::chrono;
    return time_point{
        milliseconds(t * ((TickType_t)1000) / configTICK_RATE_HZ)};
  }

  static time_point now() noexcept {
    return fromTickCount(xPortIsInsideInterrupt()
                             ? Kernel::getTickCount()
                             : Kernel::getTickCountFromISR());
  }
};

}  // namespace FreeRTOS::Addons

#endif  // FREERTOS_ADDONS_CLOCK_HPP