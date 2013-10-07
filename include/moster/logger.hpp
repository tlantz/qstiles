#ifndef MOSTER_LOGGER_HPP_
#define MOSTER_LOGGER_HPP_

#include <moster/os.hpp>

#ifndef MOSTER_LOGGER_MAXNAME_LEN
    #define MOSTER_LOGGER_MAXNAME_LEN 32
#endif
#ifndef MOSTER_LOGGER_BUFFSIZE
    #define MOSTER_LOGGER_BUFFSIZE 128
#endif

namespace moster 
{
    class logger
    {

    public:

        enum log_level
        {
            Debug,
            Info,
            Warn,
            Error
        };

    private:

        log_level level_;
        irr::c8 name_[MOSTER_LOGGER_MAXNAME_LEN];
		irr::c8 buffer_[MOSTER_LOGGER_BUFFSIZE];

    public:

		explicit logger(irr::c8 * name, const log_level = Info);

        const log_level level() const;

        void level(const log_level);

		void log(const log_level, irr::c8 * format, ...);
    };

}

#endif
