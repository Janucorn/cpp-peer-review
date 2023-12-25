struct ReadCountryParams {
    const string* country_name;
    const string* country_iso_code;
    const string* country_phone_code;
    const string* country_time_zone;
    const vector<Language>* languages;

    ReadCountryParams& SetName(const string& name) {
        this->country_name = &name;
        return *this;
    }

    ReadCountryParams& SetISO(const string& iso_code) {
        this->country_iso_code = &iso_code;
        return *this;
    }

    ReadCountryParams& SetPhone(const string& phone_code) {
        this->country_phone_code = &phone_code;
        return *this;
    }
    
    ReadCountryParams& SetTimeZone(const string& time_zone) {
        this->country_time_zone = &time_zone;
        return *this;
    }

    ReadCountryParams& SetLanguages(const vector<Language>& languages) {
        this->languages = &languages;
        return *this;
    }
};
struct ReadSubjsonParams {
    const Json* json;
    
    ReadSubjsonParams& SetJson(const Json& json) {
        this->json = &json;
        return *this;
    }
};
// Дана функция ParseCitySubjson, обрабатывающая JSON-объект со списком городов конкретной страны:
void ParseCitySubjson(vector<City>& cities, const ReadSubjsonParams& json_params, const ReadCountryParams& country_params) {
    for (const auto& city_json : json_params.json.AsList()) {
        const auto& city_obj = city_json.AsObject();
        cities.push_back({ city_obj["name"s].AsString(), city_obj["iso_code"s].AsString(),
                          _country_params.country_phone_code + city_obj["phone_code"s].AsString(), params.country_name, params.country_iso_code,
                          params.country_time_zone, params.languages });
    }
}

// ParseCitySubjson вызывается только из функции ParseCountryJson следующим образом:
void ParseCountryJson(vector<Country>& countries, vector<City>& cities, const Json& json) {
    for (const auto& country_json : json.AsList()) {
        const auto& country_obj = country_json.AsObject();
        countries.push_back({
            country_obj["name"s].AsString(),
            country_obj["iso_code"s].AsString(),
            country_obj["phone_code"s].AsString(),
            country_obj["time_zone"s].AsString(),
            });
        Country& country = countries.back();
        for (const auto& lang_obj : country_obj["languages"s].AsList()) {
            country.languages.push_back(FromString<Language>(lang_obj.AsString()));
        }
        ReadSubjsonParams subjson_params;
        subjson_params.SetJson(country_obj["cities"s]);

        ReadCountryParams country_params;
        country_params.SetName(country.name).
            SetISO(country.iso_code).
            SetPhone(country.phone_code).
            SetTimeZone(country.time_zone).
            SetLanguages(country.languages);

        ParseCitySubjson(cities, subjson_params, country_params);
    }
}