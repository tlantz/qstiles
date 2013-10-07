#include <moster/logger.hpp>
#ifndef _MSC_VER
#include "config.h"
#endif
#include <cstring>
#include <cstdio>
#include <cstdarg>
#include <time.h>

using namespace irr;

namespace moster
{

    namespace logger_impl
    {
		c8 * timestamp(c8 * buffer)
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
            strftime(buffer, MOSTER_LOGGER_BUFFSIZE, "%d%b%y %H:%M:%S", mptr);
            return buffer;
        }

        c8 * level2str(const logger::log_level level)
        {
            switch (level)
            {
                case logger::Debug:
                    return "DEBUG";
                case logger::Info:
                    return "INFO";
                case logger::Warn:
                    return "WARN";
                case logger::Error:
                    return "ERROR";
                default:
                    return "BLEH";
            }
        }
    }

    logger::logger(c8 * name, const logger::log_level level) :
        level_(level)
    {
#ifdef _MSC_VER
		strcpy_s(name_, MOSTER_LOGGER_MAXNAME_LEN - 1, name);
#else
        strncpy(name_, name, MOSTER_LOGGER_MAXNAME_LEN - 1);
#endif
        name_[MOSTER_LOGGER_MAXNAME_LEN - 1] = '\0';
    }

    const logger::log_level logger::level() const { return level_; }

    void logger::level(const logger::log_level level) { level_ = level; }

	void logger::log(const logger::log_level level, c8 * format, ...)
    {
        printf("%s %-6s ", logger_impl::timestamp(buffer_),
            logger_impl::level2str(level));
        va_list arguments;
        va_start(arguments, format);
        vprintf(format, arguments);
        va_end(arguments);
        wprintf(L"\n");
    }
}

