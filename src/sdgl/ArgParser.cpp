#include "ArgParser.h"

#include <sdgl/logging.h>

namespace sdgl {
    bool ArgParser::Arg::getValue(string *outVal) const
    {
        SDGL_ASSERT(outVal);

        if (!hasValue())
        {
            return false;
        }

        *outVal = m_value;
        return true;
    }

    bool ArgParser::Arg::getValue(int *outVal) const
    {
        SDGL_ASSERT(outVal);

        if (!hasValue())
        {
            return false;
        }

        try
        {
            *outVal = std::stoi(m_value);
            return true;
        }
        catch(const std::invalid_argument &_)
        {
            SDGL_ERROR("Invalid argument \"{}\" with value \"{}\" cannot be parsed as an int",
                m_name, m_value);
            return false;
        }
        catch(const std::out_of_range &_)
        {
            SDGL_ERROR("Out of range error: \"{}\" with value \"{}\" falls out of the range an int can contain",
                m_name, m_value);
            return false;
        }
        catch(...)
        {
            SDGL_ERROR("Unknown error while parsing arg \"{}\" with value \"{}\" as an int",
                m_name, m_value);
            return false;
        }
    }

    bool ArgParser::Arg::getValue(float *outVal) const
    {
        SDGL_ASSERT(outVal);

        if (!hasValue())
        {
            return false;
        }

        try
        {
            *outVal = std::stof(m_value);
            return true;
        }
        catch(const std::invalid_argument &_)
        {
            SDGL_ERROR("Invalid argument \"{}\" with value \"{}\" cannot be parsed as an int",
                m_name, m_value);
            return false;
        }
        catch(const std::out_of_range &_)
        {
            SDGL_ERROR("Out of range error: \"{}\" with value \"{}\" falls out of the range an int can contain",
                m_name, m_value);
            return false;
        }
        catch(...)
        {
            SDGL_ERROR("Unknown error while parsing arg \"{}\" with value \"{}\" as a float",
                m_name, m_value);
            return false;
        }
    }

    bool ArgParser::Arg::getName(string *outVal) const
    {
        SDGL_ASSERT(outVal);

        if (!hasName())
        {
            return false;
        }

        *outVal = m_name;
        return true;
    }

    ArgParser::ArgParser(int argc, char * const*argv) : m_nameless(), m_named()
    {
        setArgs(argc, argv);
    }

    ArgParser::ArgParser() : m_nameless(), m_named()
    {

    }

    bool ArgParser::setArgs(const int argc, char * const*argv)
    {
        SDGL_ASSERT(argc > 0);
        SDGL_ASSERT(argv);

        vector<string> nameless;
        map<string, Arg> named;

        // visit each arg
        for (int i = 0; i < argc; ++i)
        {
            const char *arg = argv[i];

            if (arg == nullptr || arg[0] == '\0')
            {
                SDGL_ERROR("Blank argument passed to ArgParser at index {}, arguments must have a length", i);
                return false;
            }

            // Check if nameless arg
            if (arg[0] != '-')
            {
                nameless.emplace_back(arg);
                continue;
            }

            // Find the arg name
            string argName;
            int valueOffsetIndex;     // position where the arg value or end of string begins

            if (arg[1] == 0)
            {
                SDGL_ERROR("ArgParser found malformed arg at index {}: a '-' by itself is invalid", i);
                return false;
            }

            if (std::isalpha(arg[1])) // single-letter argument
            {
                if (arg[2] == 0)
                {
                    argName = arg[1];
                    named.try_emplace(argName, Arg(argName, ""));
                    continue;
                }

                if (arg[2] != ':')
                {
                    SDGL_ERROR("ArgParser found malformed arg at index {}: a single character flag must be "
                        "followed by ':', but instead found '{}'", i, arg[2]);
                    return false;
                }

                argName = arg[1];
                valueOffsetIndex = 3;
            }
            else if (arg[1] == '-') // multi-letter argument
            {
                if (!std::isalpha(arg[2]))
                {
                    SDGL_ERROR("ArgParser found malformed arg at index {}: a named arg string must start with a "
                        "letter, but got '{}'", i, arg[2]);
                    return false;
                }

                int c = 2;
                while (arg[c] != '\0' && arg[c] != ':') // find end of arg name
                    ++c;
                valueOffsetIndex = c;

                argName.assign(arg + 2, c - 2);

                if (arg[c] == ':') // move past assignment colon
                    ++c;

                valueOffsetIndex = c;
            }
            else
            {
                SDGL_ERROR("ArgParser found malformed arg at index {}: arg flag name must be a \"-<char>:\" "
                    "or \"--<chars>:\", but got \"{}\"", i, arg);
                return false;
            }

            // Find the arg value
            string argValue = arg + valueOffsetIndex;

            // Done, add to named
            named.emplace(argName, Arg(argName, argValue));
        }


        m_nameless.swap(nameless);
        m_named.swap(named);
        return true;
    }

    bool ArgParser::getNamedArg(const string &name, Arg *outArg) const
    {
        SDGL_ASSERT(outArg);

        auto it = m_named.find(name);
        if (it == m_named.end())
        {
            return false;
        }

        *outArg = it->second;
        return true;
    }

    bool ArgParser::getNamelessArg(int index, string *outArg) const
    {
        SDGL_ASSERT(outArg);

        if (index < 0 || index >= m_nameless.size())
        {
            SDGL_ERROR("ArgParser unnamed argument index `{}` is out of range", index);
            return false;
        }

        *outArg = m_nameless[index];
        return true;
    }

    int ArgParser::getNamedArgCount() const
    {
        return (int)m_named.size();
    }

    int ArgParser::getNamelessArgCount() const
    {
        return (int)m_nameless.size();
    }

    bool ArgParser::getProgramName(string *outVal) const
    {
        SDGL_ASSERT(outVal);

        if (m_nameless.empty())
        {
            SDGL_ERROR("Argument list does not contain a program name");
            return false;
        }

        *outVal = m_nameless[0];
        return true;
    }

    void ArgParser::printArgs()
    {
        SDGL_LOG("ArgParser Args:");
        SDGL_LOG("UNNAMED:");
        for (auto &nameless : m_nameless)
        {
            SDGL_LOG("    \"{}\"", nameless);
        }

        SDGL_LOG("NAMED:");

        for (auto &[name, value] : m_named)
        {
            string outVal;
            if (value.getValue(&outVal))
                SDGL_LOG("    \"{}\": \"{}\"", name, outVal);
            else
                SDGL_LOG("    \"{}\"", name);
        }
    }
} // sdgl