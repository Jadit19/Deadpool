#include <critical/message.h>

namespace Deadpool
{
    Message::Message(const std::string& sender, const std::string& data, const time_t sentTime) :
        _sender(sender),
        _data(data),
        _sentTime(sentTime)
    {
        jsonStringify();
    }

    Message::Message(const std::string json) :
        _json(json)
    {
        jsonParse();
    }

    std::string Message::getSender()
    {
        return _sender;
    }

    std::string Message::getData()
    {
        return _data;
    }

    time_t Message::getSentTime()
    {
        return _sentTime;
    }

    std::string Message::getJson()
    {
        return _json;
    }

    void Message::jsonStringify()
    {
        _json = "{\n\t\t\"sender\":\"";
        _json += _sender;
        _json += "\",\n\t\t\"str\":\"";
        _json += _data;
        _json += "\",\n\t\t\"time\":";
        _json += std::to_string(_sentTime);
        _json += "\n\t}";
    }

    void Message::jsonParse()
    {
        size_t pos1 = _json.find("\",\n\t\t\"str\":\"");
        size_t pos2 = _json.find("\",\n\t\t\"time\":");

        _sender = _json.substr(14, pos1-14);
        _data = _json.substr(pos1+12, pos2-pos1-13);
        _sentTime = std::stol(_json.substr(pos2+12, _json.size()-3));

        jsonStringify();
    }
}