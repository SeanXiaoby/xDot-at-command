#include "Command.h"
#include <algorithm>

const char Command::newline[] = "\r\n";

Command::Command() : _name(""), _text(""), _desc(""), _usage("NONE")
{
    _queryable = false;
}

Command::Command(const char* name, const char* text, const char* desc, const char* usage) :
    _name(name), _text(text), _desc(desc), _usage(usage)
{
    _queryable = false;
}

std::string Command::usage() const
{
    std::string usage(_text);
    usage.append(": ");
    usage.append(_usage);
    return usage;
}

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
        if (input.find(" ") != std::string::npos)
            bytes = mts::Text::split(input, " ");
        else if (input.find(":") != std::string::npos)
            bytes = mts::Text::split(input, ":");
        else if (input.find("-") != std::string::npos)
            bytes = mts::Text::split(input, "-");
        else if (input.find(".") != std::string::npos)
            bytes = mts::Text::split(input, ".");

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
        if (str.find_first_of(":-.") == std::string::npos) {
            // no delim found
            return false;
        }
        if (str.find(":") != std::string::npos && std::count(str.begin(), str.end(), ':') != numDelims) {
            return false;
        }
        if (str.find(".") != std::string::npos && std::count(str.begin(), str.end(), '.') != numDelims) {
            return false;
        }
        if (str.find("-") != std::string::npos && std::count(str.begin(), str.end(), '-') != numDelims) {
            return false;
        }

        return str.find_first_not_of("0123456789abcdefABCDEF:-.") == std::string::npos;
    }   

    return false;
}

