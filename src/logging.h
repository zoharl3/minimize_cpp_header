
#pragma once

#include <assert.h>

#include "logger.h"

// Verbosity
const int L_LOW = 0; // alway showing
const int L_MED = 10;
const int L_MEDPLUS = 15;
const int L_HIGH = 20;
const int L_FREQ = 30;
extern Logger Log;

template< typename... Args >
Logger &emphasize( std::string st, Args &&...args ) {
    return Log.emphasize( st.c_str(), std::forward<Args>(args)... );
}

template< typename... Args >
Logger &emphasize2( std::string st, Args &&...args ) {
    return Log.emphasize2( st.c_str(), std::forward<Args>( args )... );
}

template< typename... Args >
Logger &emphasize3( std::string st, Args &&...args ) {
    return Log.emphasize3( st.c_str(), std::forward<Args>( args )... );
}

template< typename... Args >
Logger &print( std::string st, Args &&...args ) {
    return Log.print( st.c_str(), std::forward<Args>( args )... );
}

template< typename... Args >
Logger &println( std::string st = "", Args &&...args ) {
    return Log.println( st.c_str(), std::forward<Args>( args )... );
}

template <typename... Args>
Logger &soft( std::string st, Args &&...args ) {
    return Log.soft( st.c_str(), std::forward<Args>( args )... );
}

template< typename... Args >
Logger &warn( std::string st, Args &&...args ) {
    return Log.warn( st.c_str(), std::forward<Args>( args )... );
}

template< typename... Args >
Logger &error( std::string st, Args &&...args ) {
    return Log.error( st.c_str(), std::forward<Args>( args )... );
}

template< typename... Args >
std::exception exception2( std::string st, Args&&... args ) {
    struct my_exception : public std::exception {
        std::string s;
        my_exception( std::string s_ ) : s( s_ ) {}
        ~my_exception() throw() {}
        const char *what() const throw() { return s.c_str(); }
    };

    char msg[ 500 ];
    sprintf( msg, st.c_str(), std::forward<Args>(args)... );

    return my_exception( msg );
}

#define LOG_MESSAGE_BLOCK(level, message) const Logger::Block &_Log_Block = Log.block(level, message);

// dumpv
template <class T>
T dumpv_helper( T x ) {
    return x;
}

#include <boost/preprocessor.hpp>

#pragma warning( disable : 4003 ) // not enough arguments for function-like macro invocation 'BOOST_PP_SEQ_DETAIL_IS_NOT_EMPTY'
#define _PPSTUFF_OUTVAR1( _var ) BOOST_PP_STRINGIZE(_var) " = " << dumpv_helper( _var ) << std::endl
#define _PPSTUFF_OUTVAR2( r, d, _var ) << _PPSTUFF_OUTVAR1( _var )
#define _PPSTUFF_OUTVAR_SEQ( vseq )               \
    _PPSTUFF_OUTVAR1( BOOST_PP_SEQ_HEAD( vseq ) ) \
    BOOST_PP_SEQ_FOR_EACH( _PPSTUFF_OUTVAR2, , BOOST_PP_SEQ_TAIL( vseq ) )
#define dumpv( ... ) Log << "[" << __FUNCTION__ << ", L" << __LINE__ << "] " << _PPSTUFF_OUTVAR_SEQ( BOOST_PP_VARIADIC_TO_SEQ( __VA_ARGS__ ) )

// utils and debugging
inline void stop( bool flag = 1, bool bAssert = 0 ) {
    if ( flag ) {
        if ( bAssert )
            assert( 0 );
        else
            throw exception2( "stop()" );
    }
}

inline void stopn( int n = 0 ) {
    static int ii = 0;
    if ( ii++ == n )
        throw exception2( "stopn()" );
}

inline void wait() {
    println( "\npress a key..." );
    getchar();
}
