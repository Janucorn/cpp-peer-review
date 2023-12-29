
enum class DateTimeError {
    YEAR_IS_TOO_SMALL,
    YEAR_IS_TOO_BIG,
    MONTH_IS_TOO_SMALL,
    MONTH_IS_TOO_BIG,
    DAY_IS_TOO_SMALL,
    DAY_IS_TOO_BIG,
    HOUR_IS_TOO_SMALL,
    HOUR_IS_TOO_BIG,
    MINUTE_IS_TOO_SMALL,
    MINUTE_IS_TOO_BIG,
    SECOND_IS_TOO_SMALL,
    SECOND_IS_TOO_BIG
};

class CheckDateTimeValidate {
public:

    optional<DateTimeError> CheckDateTimeValidity(const DateTime& dt) {
        dt_ = dt;
        if (dt.year < 1) {
            return (DateTimeError::YEAR_IS_TOO_SMALL);
        }
        if (dt.year > 9999) {
            return (DateTimeError::YEAR_IS_TOO_BIG);
        }

        if (dt.month < 1) {
            return (DateTimeError::MONTH_IS_TOO_SMALL);
        }
        if (dt.month > 12) {
            return (DateTimeError::MONTH_IS_TOO_BIG);
        }

        const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
        const array month_lengths = { 31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

        if (dt.day < 1) {
            return (DateTimeError::DAY_IS_TOO_SMALL);
        }
        if (dt.day > month_lengths[dt.month - 1]) {
            return (DateTimeError::DAY_IS_TOO_BIG);
        }

        if (dt.hour < 0) {
            return (DateTimeError::HOUR_IS_TOO_SMALL);
        }
        if (dt.hour > 23) {
            return (DateTimeError::HOUR_IS_TOO_BIG);
        }

        if (dt.minute < 0) {
            return (DateTimeError::MINUTE_IS_TOO_SMALL);
        }
        if (dt.minute > 59) {
            return (DateTimeError::MINUTE_IS_TOO_BIG);
        }

        if (dt.second < 0) {
            return (DateTimeError::SECOND_IS_TOO_SMALL);
        }
        if (dt.second > 59) {
            return (DateTimeError::SECOND_IS_TOO_BIG);
        } 
        return {};
    }

    optional<string_view> GetErrorMessage() {
        if (dt_ != nullopt) {
            if (CheckDateTimeValidity(*dt_) != nullopt) {
                auto error = *(CheckDateTimeValidity(*dt_));

                return error_messages_.at(error);
            }
            
        }  
        return {};
    }
    
private:
    std::optional<DateTime> dt_;
    const std::map<DateTimeError, std::string_view> error_messages_ = {
        {DateTimeError::YEAR_IS_TOO_SMALL, "year is too small"sv},
        {DateTimeError::YEAR_IS_TOO_BIG, "year is too big"sv},
        {DateTimeError::MONTH_IS_TOO_SMALL, "month is too small"sv},
        {DateTimeError::MONTH_IS_TOO_BIG, "month is too big"sv},
        {DateTimeError::DAY_IS_TOO_SMALL, "day is too small"sv},
        {DateTimeError::DAY_IS_TOO_BIG, "day is too big"sv},
        {DateTimeError::HOUR_IS_TOO_SMALL, "hour is too small"sv},
        {DateTimeError::HOUR_IS_TOO_BIG, "hour is too big"sv},
        {DateTimeError::MINUTE_IS_TOO_SMALL, "minute is too small"sv},
        {DateTimeError::MINUTE_IS_TOO_BIG, "minute is too big"sv},
        {DateTimeError::SECOND_IS_TOO_SMALL, "second is too small"sv},
        {DateTimeError::SECOND_IS_TOO_BIG, "second is too big"sv}
    };
};