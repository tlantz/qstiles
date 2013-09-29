#include <moster/logger.hpp>
#include <cstring>
#include <cstdio>
#include <cstdarg>
#include <time.h>

namespace moster
{

    namespace logger_impl
    {
        const char * timestamp(char * buffer)
        {
            time_t t = time(NULL);
            struct tm * mptr;
#ifdef HAVE_LOCALTIME_R
            struct tm m;
            mptr = localtime_r(&t, &m);
#else
            mptr = localtime(&t);
#endif
            strftime(buffer, MOSTER_LOGGER_BUFFSIZE, "%d%b%y %H:%M:%S", mptr);
            return buffer;
        }

        const char * level2str(const logger::log_level level)
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

    logger::logger(const char * name, const logger::log_level level) :
        level_(level)
    { 
        strncpy(name_, name, MOSTER_LOGGER_MAXNAME_LEN - 1);
        name_[MOSTER_LOGGER_MAXNAME_LEN - 1] = '\0';
    }

    const logger::log_level logger::level() const { return level_; }

    void logger::level(const logger::log_level level) { level_ = level; }

    void logger::log(const logger::log_level level, const char * format, ...)
    {
        printf("%s %-6s ", logger_impl::timestamp(buffer_),
            logger_impl::level2str(level));
        va_list arguments;
        va_start(arguments, format);
        vprintf(format, arguments);
        va_end(arguments);
        printf("\n");
    }
}

