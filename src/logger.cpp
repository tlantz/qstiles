#include <moster/logger.hpp>
#ifndef _MSC_VER
#include "config.h"
#endif
#include <cstring>
#include <cstdio>
#include <cstdarg>
#include <time.h>

namespace moster
{

    namespace logger_impl
    {
		os::osstr timestamp(os::osstr buffer)
        {
            time_t t = time(NULL);
            struct tm * mptr;
#ifdef HAVE_LOCALTIME_R
            struct tm m;
            mptr = localtime_r(&t, &m);
#elif _MSC_VER
			struct tm m;
			localtime_s(&m, &t);
			mptr = &m;
#else
            mptr = localtime(&t);
#endif
            wcsftime(buffer, MOSTER_LOGGER_BUFFSIZE, L"%d%b%y %H:%M:%S", mptr);
            return buffer;
        }

        os::osstr level2str(const logger::log_level level)
        {
            switch (level)
            {
                case logger::Debug:
                    return L"DEBUG";
                case logger::Info:
                    return L"INFO";
                case logger::Warn:
                    return L"WARN";
                case logger::Error:
                    return L"ERROR";
                default:
                    return L"BLEH";
            }
        }
    }

    logger::logger(os::osstr name, const logger::log_level level) :
        level_(level)
    {
#ifdef _MSC_VER
		wcscpy_s(name_, MOSTER_LOGGER_MAXNAME_LEN - 1, name);
#else
        wstrncpy(name_, name, MOSTER_LOGGER_MAXNAME_LEN - 1);
#endif
        name_[MOSTER_LOGGER_MAXNAME_LEN - 1] = '\0';
    }

    const logger::log_level logger::level() const { return level_; }

    void logger::level(const logger::log_level level) { level_ = level; }

	void logger::log(const logger::log_level level, os::osstr format, ...)
    {
        wprintf(L"%s %-6s ", logger_impl::timestamp(buffer_),
            logger_impl::level2str(level));
        va_list arguments;
        va_start(arguments, format);
        vwprintf(format, arguments);
        va_end(arguments);
        wprintf(L"\n");
    }
}

