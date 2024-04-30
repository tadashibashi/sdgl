#pragma once
#include <sdgl/sdglib.h>

namespace sdgl {

    /// Parses arguments from the command line
    ///
    /// Supports args in the following formats:
    /// -<single-char><:value>    (letter flag, value is optional)
    /// --<arg-name><:value> (named flag; names may contain dashes/underscores, but not spaces; value is optional)
    /// <value>              (no flag value)
    ///
    /// @note Multiple arguments of the same name are not supported, it will use the first occurrence only
    class ArgParser {
    public:
        class Arg
        {
        public:
            /// @param name argument name, or empty string if not a named argument
            /// @param value argument value, or empty string if valueless arg
            Arg(string name, string value) :
                m_name(std::move(name)), m_value(std::move(value)) { }

            /// Get argument value as a string
            /// @param outVal pointer to variable to receive value
            /// @returns whether retrieval succeeded; if `false` this argument doesn't have a value
            bool getValue(string *outVal) const;

            /// Get argument value as an integer
            /// @param outVal pointer to variable to receive parsed value
            /// @returns whether parse succeeded; if `false`, check sdgl::getError() for more info
            bool getValue(int *outVal) const;

            /// Get argument value as a floating point number
            /// @param outVal pointer to variable to receive parsed value
            /// @returns whether parse succeeded; if `false`, check sdgl::getError() for more info
            bool getValue(float *outVal) const;

            bool getName(string *outVal) const;

            /// Check whether argument has a name
            [[nodiscard]]
            bool hasName() const { return !m_name.empty(); }

            /// Check whether argument has a value
            [[nodiscard]]
            bool hasValue() const { return !m_value.empty(); }

        private:
            string m_name;
            string m_value;
        };

        /// Create arg parser with command line args
        /// @param argc number of args in the args array
        /// @param argv arg array
        ArgParser(int argc, char * const*argv);

        /// Create arg parser with no args, call `setArgs` to set up args
        ArgParser();

        /// Replace args with new ones
        /// @param argc number of args in the args array
        /// @param argv arg array
        /// @returns whether args were valid; if it catches any problems, the function fails and does not update the
        ///          parser's internal state
        bool setArgs(int argc, char * const *argv);

        /// Get an argument with a named flag
        /// @param name name of the flag e.g. "o", "output", etc.
        /// @param outArg receive the argument
        /// @returns whether the arg was received
        bool getNamedArg(const string &name, Arg *outArg) const;

        /// Get an arg without a flag name
        /// @param index nameless arg index (not the same as arg array index!);
        ///              find the max index via `getNamelessArgCount() - 1`
        bool getNamelessArg(int index, string *outArg) const;

        /// Get number of named arguments
        [[nodiscard]]
        int getNamedArgCount() const;

        /// Get the number of nameless arguments, (+1 of the max index to put into getNamelessArg)
        [[nodiscard]]
        int getNamelessArgCount() const;


        /// Get the first nameless argument
        /// @param outVal - string to receive the program name
        /// @returns whether value was received; if for some reason no first nameless arg exists it returns false
        [[nodiscard]]
        bool getProgramName(string *outVal) const;

        /// Print arguments to the console
        void printArgs();


    private:

        // Raw arguments
        vector<string> m_nameless;
        map<string, Arg> m_named;
    };

} // sdgl
