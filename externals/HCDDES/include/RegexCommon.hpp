#ifndef _ISIS_REGEXCOMMON_
#define _ISIS_REGEXCOMMON_

#include <string>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

struct RegexCommon {

	// Not a validity check for matrix literals (we can't do real bracket nesting), but eliminate almost anything that is not one (like variable identifiers)
	static boost::regex getMatrixLiteralRegex( void ) {
		static boost::regex matrixLiteralRegex( "(([\\[\\(]*)((-?\\d*(\\.\\d+)?(e-?\\d+)?))([ ,;\\[\\]\\(\\)](-?\\d*(\\.\\d+)?(e-?\\d+)?))*([\\]\\)]*))?", boost::regex_constants::perl );
		return matrixLiteralRegex;
	}

	static boost::regex getFloatingPointRegex( void ) {
		static boost::regex floatingPointRegex( "[+-]?\\s*(?:[0-9]+\\.[0-9]*|\\.?[0-9]+)(?:[Ee][+-][0-9]{1,3})?", boost::regex_constants::perl );
		return floatingPointRegex;
	}

	static std::string getIdentifierRegexString( void ) {
		static std::string identifierRegexString( "(?:[A-Za-z_][A-Za-z0-9_]*)" );
		return identifierRegexString;
	}

	static boost::regex getIdentifierRegex( void ) {
		static boost::regex identifierRegex( getIdentifierRegexString(), boost::regex_constants::perl );
		return identifierRegex;
	}

	static std::string getDottedIdentifierRegexString( void ) {
		static std::string dottedIdentifierRegexString = "(?:" + getIdentifierRegexString() + "(?:\\s*\\.\\s*" + getIdentifierRegexString() + ")*)";
		return dottedIdentifierRegexString;
	}

	static boost::regex getDottedIdentifierRegex( void ) {
		static boost::regex dottedIdentifierRegex( getDottedIdentifierRegexString(), boost::regex_constants::perl );
		return dottedIdentifierRegex;
	}

	static std::string getTransposePreCharsRegexString( void ) {
		static std::string transposePreCharsRegexString = "[]).A-Za-z0-9_]";
		return transposePreCharsRegexString;
	}

	static std::string getStringRegexString( void ) {
		static std::string stringRegexString = "(?:(?<!" + getTransposePreCharsRegexString() + ")'(?:[^']|'')*'(?!'))";
		return stringRegexString;
	}

	static std::string getCommentRegexString( void ) {
		static std::string commentRegexString = "(?:/\\*.*?\\*/|%.*?$|//.*?$)";
		return commentRegexString;
	}

	static boost::regex getCommentRegex( void ) {
		static boost::regex commentRegex( getCommentRegexString(), boost::regex_constants::perl );
		return commentRegex;
	}

	static std::string getNonNewlineWhitespaceRegexString( void ) {
		static std::string nonNewlineWhitespaceRegexString = "[ \\t\\r\\f]";
		return nonNewlineWhitespaceRegexString;
	}

	static std::string getSkipStringRegexString( void ) {
		static std::string skipStringRegexString = "(?:" + getTransposePreCharsRegexString() + "'|" + getStringRegexString() + ")";
		return skipStringRegexString;
	}

	static std::string getSkipStringCommentRegexString( void ) {
		static std::string skipStringCommentRegexString = "(?:" + getSkipStringRegexString() + "|" + getCommentRegexString() + ")";
		return skipStringCommentRegexString;
	}

	static std::string getFunctionSpecRegexString( void ) {
		static std::string functionSpecRegexString = 
		 "(?:(" + getIdentifierRegexString() + "|\\[\\s*" + getIdentifierRegexString() + "(?:\\s*,\\s*" + getIdentifierRegexString() + ")*\\s*\\])"
		 "\\s*=\\s*)?" +
		 getIdentifierRegexString() + "\\s*"
		 "(?:\\(\\s*(" + getIdentifierRegexString() + "(?:\\s*,\\s*" + getIdentifierRegexString() + ")*)?\\s*\\))?";

		return functionSpecRegexString;
	}

	static boost::regex getFunctionSpecRegex( void ) {
		static boost::regex functionSpecRegex( getFunctionSpecRegexString(), boost::regex_constants::perl );
		return functionSpecRegex;
	}

	static boost::regex getFunctionHeaderRegex( void ) {

		static boost::regex functionHeaderRegex(
		 "^" + getNonNewlineWhitespaceRegexString() + "*function\\s+" + getFunctionSpecRegexString(), boost::regex_constants::perl
		);

		return functionHeaderRegex;
	}

	static std::string eliminateContinuations( std::string inputString ) {

		static boost::regex findContinuationRegex(
		 "^(?:" + getSkipStringCommentRegexString() + "|\\.(?!\\.\\.$)|[^'%.])*+",
		 boost::regex_constants::perl
		);

		const std::string &constInputString = inputString;
		std::string outputString;

		boost::match_results< std::string::const_iterator > results;

		while(  !inputString.empty() && boost::regex_search( inputString, results, findContinuationRegex )  ) {
			outputString += boost::trim_right_copy(  std::string( constInputString.begin(), results[1].first )  ) + " ";
			inputString = results.suffix();
			if ( !inputString.empty() ) inputString = boost::trim_left_copy(  inputString.substr( 3 )  );
		}

		outputString += inputString;
		return outputString;
	}

	static std::string eliminateComments( std::string inputString ) {

		static boost::regex findCommentRegex(
		 "^(?:" + getSkipStringRegexString() + "|/(?=[^/*])|[^%'/])*+(" + getCommentRegexString() + ")",
		 boost::regex_constants::perl
		);

		const std::string &constInputString = inputString;
		std::string outputString;

		boost::match_results< std::string::const_iterator > results;

		while(  boost::regex_search( inputString, results, findCommentRegex )  ) {
			outputString += boost::trim_right_copy(  std::string( constInputString.begin(), results[1].first )  );
			std::string comment = results[1];
			inputString = results.suffix();
			if (  comment[0] == '/' && comment[1] == '*' && !isspace( inputString[0] )  ) {
				outputString += " ";
			}
		}

		outputString += inputString;
		return outputString;
	}

	static std::string eliminateBlankLines( std::string inputString ) {

		static boost::regex findBlankLineRegex(
		 "^(?:" + getNonNewlineWhitespaceRegexString() + "*$\\r?\\n|" + getNonNewlineWhitespaceRegexString() + "+$\\r?)", boost::regex_constants::perl
		);

		const std::string &constInputString = inputString;
		std::string outputString;

		boost::match_results< std::string::const_iterator > results;

		while(  boost::regex_search( inputString, results, findBlankLineRegex )  ) {
			outputString += results.prefix();
			inputString = results.suffix();
		}

		outputString += inputString;
		return outputString;
	}

	static std::string eliminateWhitespace( std::string inputString ) {

		static boost::regex findWhitespaceRegex(
		 "(?:" + getSkipStringCommentRegexString() + "|" + getNonNewlineWhitespaceRegexString() + "(?!" + getNonNewlineWhitespaceRegexString() + ")" +
		 "|[^%'" + getNonNewlineWhitespaceRegexString().substr( 1, getNonNewlineWhitespaceRegexString().size() - 2 ) + "])*+" +
		 "(" + getNonNewlineWhitespaceRegexString() + "{2,})", boost::regex_constants::perl );

		const std::string &constInputString = inputString;
		std::string outputString;

		boost::match_results< std::string::const_iterator > results;

		while(  boost::regex_search( inputString, results, findWhitespaceRegex )  ) {
			outputString += std::string( constInputString.begin(), results[1].first );
			if ( outputString[ outputString.size() - 1 ] != '\n' ) outputString += " ";
			inputString = results.suffix();
		}

		outputString += inputString;
		return outputString;
	}

};

#endif // _ISIS_REGEXCOMMON_
