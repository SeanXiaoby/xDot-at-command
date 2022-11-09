#include "Command.h"
#include <algorithm>

const char Command::newline[] = "\r\n";

#if MTS_CMD_TERM_VERBOSE
Command::Command() : _name(""), _text(""), _desc(""), _usage("NONE")
#else
Command::Command() : _text("")
#endif
{
    _queryable = false;
}

#if MTS_CMD_TERM_VERBOSE
Command::Command(const char* name, const char* text, const char* desc, const char* usage) :
    _name(name), _text(text), _desc(desc), _usage(usage)
#else
Command::Command(const char* text) :
    _text(text)
#endif
{
    _queryable = false;
}

bool Command::verify(const std::vector<std::string>& args) {
    if (args.size() == 1)
        return true;

#if MTS_CMD_TERM_VERBOSE
    CommandTerminal::setErrorMessage("Invalid arguments");
#endif
    return false;
}


#if MTS_CMD_TERM_VERBOSE
std::string Command::usage() const
{
    std::string usage(_text);
    usage.append(": ");
    usage.append(_usage);
    return usage;
}
#endif

bool Command::queryable() const
{
    return _queryable;
}

void Command::readByteArray(const std::string& input, std::vector<uint8_t>& out, size_t len)
{
    // if input length is greater than expected byte output
    // there must be a delimiter included
    if (input.length() > len * 2)
    {
        std::vector < std::string > bytes;


        char delims[] = " :-.";

        for (int i = 0; i < sizeof(delims); ++i) {
            if (input.find(delims[i]) != std::string::npos) {
                bytes = mts::Text::split(input, delims[i]);
                break;
            }
        }

        if (bytes.size() != len) {
            return;
        }

        int temp;
        // Read in the key components...
        for (size_t i = 0; i < len; i++)
        {
            sscanf(bytes[i].c_str(), "%02x", &temp);
            out.push_back(temp);
        }
    }
    else
    {
        // no delims
        int temp;

        // Read in the key components...
        for (size_t i = 0; i < len; i++)
        {
            if (i * 2 < input.size())
            {
                sscanf(input.substr(i * 2).c_str(), "%02x", &temp);
                out.push_back(temp);
            }
        }
    }
}

bool Command::isHexString(const std::string& str, size_t bytes) {
    int numDelims = bytes - 1;
    size_t minSize = bytes * 2;
    size_t maxSize = minSize + numDelims;

    if (str.size() == minSize) {
        return str.find_first_not_of("0123456789abcdefABCDEF") == std::string::npos;
    }
    else if (str.size() == maxSize) {
        char delims[] = ":-.";
        if (str.find_first_of(delims) == std::string::npos) {
            // no delim found
            return false;
        }

        for (int i = 0; i < sizeof(delims); ++i) {
            if (str.find(delims[i]) != std::string::npos && std::count(str.begin(), str.end(), delims[i]) != numDelims) {
                return false;
            }
        }

        return str.find_first_not_of("0123456789abcdefABCDEF:-.") == std::string::npos;
    }   

    return false;
}


int Command::strToDataRate(const std::string& str)
{
    std::string dr = mts::Text::toUpper(str);

    int datarate = -1;
    uint8_t i;

    int res = sscanf(dr.c_str(), "%d", &datarate);

    if (res == 0) {
        for (i = 0; i < 24; i++) {
            if (mDot::DataRateStr(i).find(dr) != std::string::npos) {
                datarate = i;
                break;
            }
        }
    }
    return datarate;
}


bool Command::printRecvData()
{
    bool recvd = false;
    std::vector<uint8_t> rx_data;
    if (CommandTerminal::Dot()->recv(rx_data) == mDot::MDOT_OK) {
        if (!rx_data.empty()) {
            recvd = true;
            std::string formatted_data = CommandTerminal::formatPacketData(rx_data, CommandTerminal::Dot()->getRxOutput());
            CommandTerminal::Serial()->write(formatted_data.c_str(), formatted_data.length());
            CommandTerminal::Serial()->write(Command::newline, sizeof(Command::newline));
        }
    }
    return recvd;
}