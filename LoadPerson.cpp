struct ReadDBParams {
    string_view db_name;
    int db_connection_timeout;
    bool db_allow_exceptions;
    DBLogLevel* db_log_level;

    ReadDBParams& SetName(string_view db_name) {
        this->db_name = move(db_name);
        return *this;
    }

    ReadDBParams& SetConnectionTimeout(int db_connection_timeout) {
        this->db_connection_timeout = db_connection_timeout;
        return *this;
    }

    ReadDBParams& SetAllowExceptions(bool db_allow_exceptions) {
        this->db_allow_exceptions = db_allow_exceptions;
        return *this;
    }

    ReadDBParams& SetLogLevel(DBLogLevel& db_log_level) {
        this->db_log_level = &db_log_level;
        return *this;
    }
};

struct ReadPersonParams {
    int min_age;
    int max_age;
    string_view name_filter;

    ReadPersonParams& SetMinAge(int age) {
        this->min_age = age;
        return *this;
    }

    ReadPersonParams& SetMaxAge(int age) {
        this->max_age = age;
        return *this;
    }

    ReadPersonParams& SetNameFilter(string_view name_filter) {
        this->name_filter = move(name_filter);
    }
};

vector<Person> LoadPersons(ReadDBParams& db_params, ReadPersonParams& person_params) {
    DBConnector connector(db_params.db_allow_exceptions, *db_params.db_log_level);
    DBHandler db;
    if (db_params.db_name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(db_params.db_name, db_params.db_connection_timeout);
    } else {
        db = connector.Connect(db_params.db_name, db_params.db_connection_timeout);
    }
    if (!db_params.db_allow_exceptions && !db.IsOK()) {
        return {};
    }

    ostringstream query_str;
    query_str << "from Persons "s
        << "select Name, Age "s
        << "where Age between "s << person_params.min_age << " and "s << person_params.max_age << " "s
        << "and Name like '%"s << db.Quote(person_params.name_filter) << "%'"s;
    DBQuery query(query_str.str());

    vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({ move(name), age });
    }
    return persons;
}