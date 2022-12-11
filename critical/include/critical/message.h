#pragma once

#include <string>

namespace Deadpool
{
    class Message
    {
        private:
            std::string _sender;
            std::string _data;
            time_t _sentTime;

            std::string _json;

            void jsonParse();
            void jsonStringify();

        public:
            Message(const std::string& sender, const std::string& data, const time_t sentTime);
            Message(const std::string json);

            std::string getSender();
            std::string getData();
            time_t getSentTime();

            std::string getJson();
    };
}