#ifndef STRING_FUNCS_INCLUDED
#define STRING_FUNCS_INCLUDED

#include <fstream>
#include <string>
#include <algorithm> 
#include <cctype>
#include <locale>
#include <vector>
#include <sstream>
#include <utility>

namespace StringFuncs
{
	using namespace std;

	void SearchAndReplace( string& source, const string& find, const string& replace )
	{
		for( size_t i = 0; ( i = source.find( find, i ) ) != string::npos; )
		{
			source.replace( i, find.length(), replace );
			i += replace.length();
		}
	}

	// trim from start (in place)
	static inline void ltrim( string &s )
	{
		s.erase( s.begin(), find_if( s.begin(), s.end(), []( int ch ) {
			return !isspace( ch );
		} ) );
	}

	// trim from end (in place)
	static inline void rtrim( string &s )
	{
		s.erase( find_if( s.rbegin(), s.rend(), []( int ch ) {
			return !isspace( ch );
		} ).base(), s.end() );
	}

	// trim from both ends (in place)
	static inline void Trim( string &s ) {
		ltrim( s );
		rtrim( s );
	}

	vector< string > Explode( const string& s, char delim )
	{
		vector< string > result;
		istringstream iss( s );

		for( string token; getline( iss, token, delim ); )
		{
			result.push_back( move( token ) );
		}

		return result;
	}
}

#endif