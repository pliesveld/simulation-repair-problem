// Copyright(c) 2015-present, Gabi Melman & spdlog contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once

#include <spdlog/details/null_mutex.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/details/synchronous_factory.h>
#include <cassert>
#include <string>
#include <string_view>
#include <mutex>

#include "GDConsole.h"

namespace spdlog {
namespace sinks {

template<typename Mutex>
class gdconsole_sink : public base_sink<Mutex>
{
protected:
    void sink_it_(const details::log_msg &msg) override {
		GDConsole* gdConsole = GDConsole::get_singleton();
		if(gdConsole)
		{
			gdConsole->LogLine(msg.payload.data());
			gdConsole->LogLine("\n");
		}
    }
    void flush_() override {}
};

using gdconsole_sink_mt = gdconsole_sink<details::null_mutex>;
using gdconsole_sink_st = gdconsole_sink<details::null_mutex>;

} // namespace sinks

template<typename Factory = spdlog::synchronous_factory>
inline std::shared_ptr<logger> gdconsole_logger_mt(const std::string &logger_name)
{
    auto gdconsole_logger = Factory::template create<sinks::gdconsole_sink_mt>(logger_name);
    return gdconsole_logger;
}

template<typename Factory = spdlog::synchronous_factory>
inline std::shared_ptr<logger> gdconsole_logger_st(const std::string &logger_name)
{
    auto gdconsole_logger = Factory::template create<sinks::gdconsole_sink_st>(logger_name);
    return gdconsole_logger;
}

} // namespace spdlog
