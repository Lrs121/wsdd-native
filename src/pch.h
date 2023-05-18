// Copyright (c) 2022, Eugene Gershnik
// SPDX-License-Identifier: BSD-3-Clause

#ifndef HEADER_PCH_H_INCLUDED
#define HEADER_PCH_H_INCLUDED

#include <sys_config.h>

#include <argum/parser.h>
#include <argum/type-parsers.h>
#include <argum/validators.h>

#include <spdlog/fmt/fmt.h>

//must come before sys_string due to S macro collision
#include <asio.hpp>

#include <sys_string/sys_string.h>

#include <intrusive_shared_ptr/ref_counted.h>
#include <intrusive_shared_ptr/refcnt_ptr.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/pattern_formatter.h>

#include <outcome.hpp>

#include <toml++/toml.h>

#if __has_include(<uuid/uuid.h>)
    #include <uuid/uuid.h>
#elif __has_include(<uuid.h>)
    #include <uuid.h>
#else
    #error No uuid.h header available
#endif
#include <pwd.h>
#include <grp.h>
#include <sys/wait.h>
#include <sys/file.h>

#if HAVE_SYSTEMD
    #include <systemd/sd-daemon.h>
#endif

#include <memory>
#include <stdexcept>
#include <vector>
#include <array>
#include <set>
#include <deque>
#include <optional>
#include <variant>
#include <tuple>
#include <system_error>
#include <random>
#include <filesystem>
#include <regex>

#include <stdio.h>

#if WSDDN_PLATFORM_APPLE

#include <CoreFoundation/CoreFoundation.h>
#include <SystemConfiguration/SCNetworkConfiguration.h>
#include <OpenDirectory/OpenDirectory.h>

#include <os/log.h>

#include <intrusive_shared_ptr/apple_cf_ptr.h>

#endif

#define WSDLOG_TRACE(...)    do { if (spdlog::should_log(spdlog::level::trace))    spdlog::trace(__VA_ARGS__);    } while(false)
#define WSDLOG_DEBUG(...)    do { if (spdlog::should_log(spdlog::level::debug))    spdlog::debug(__VA_ARGS__);    } while(false)
#define WSDLOG_INFO(...)     do { if (spdlog::should_log(spdlog::level::info))     spdlog::info(__VA_ARGS__);     } while(false)
#define WSDLOG_WARN(...)     do { if (spdlog::should_log(spdlog::level::warn))     spdlog::warn(__VA_ARGS__);     } while(false)
#define WSDLOG_ERROR(...)    do { if (spdlog::should_log(spdlog::level::err))      spdlog::error(__VA_ARGS__);    } while(false)
#define WSDLOG_CRITICAL(...) do { if (spdlog::should_log(spdlog::level::critical)) spdlog::critical(__VA_ARGS__); } while(false)


using namespace sysstr;
using namespace isptr;

namespace ip = asio::ip;
namespace outcome = OUTCOME_V2_NAMESPACE;


template <> struct fmt::formatter<sys_string> : private fmt::formatter<const char *> {

    using super = fmt::formatter<const char *>;
    using super::parse;

    template <typename FormatContext>
    auto format(const sys_string & str, FormatContext & ctx) -> decltype(ctx.out()) {
        return super::format(str.c_str(), ctx);
    }
};


#endif
