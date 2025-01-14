/*
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
#include "third_party/mbedtls/test/test.inc"
/*
 * *** THIS FILE WAS MACHINE GENERATED ***
 *
 * This file has been machine generated using the script:
 * generate_test_code.py and then mbedtls_test_suite.sh and then mbedtls_test_suite.sh
 *
 * Test file      : ./test_suite_version.c
 *
 * The following files were used to create this file.
 *
 *      Main code file      : suites/main_test.function
 *      Platform code file  : suites/host_test.function
 *      Helper file         : suites/helpers.function
 *      Test suite file     : suites/test_suite_version.function
 *      Test suite data     : suites/test_suite_version.data
 *
 */


#define TEST_SUITE_ACTIVE

#if defined(MBEDTLS_VERSION_C)
#include "third_party/mbedtls/version.h"
void test_check_compiletime_version( char * version_str )
{
    char build_str[100];
    char build_str_full[100];
    unsigned int build_int;

    memset( build_str, 0, 100 );
    memset( build_str_full, 0, 100 );

    mbedtls_snprintf( build_str, 100, "%d.%d.%d", MBEDTLS_VERSION_MAJOR,
        MBEDTLS_VERSION_MINOR, MBEDTLS_VERSION_PATCH );

    mbedtls_snprintf( build_str_full, 100, "mbed TLS %d.%d.%d", MBEDTLS_VERSION_MAJOR,
        MBEDTLS_VERSION_MINOR, MBEDTLS_VERSION_PATCH );

    build_int = MBEDTLS_VERSION_MAJOR << 24 |
            MBEDTLS_VERSION_MINOR << 16 |
            MBEDTLS_VERSION_PATCH << 8;

    TEST_ASSERT( build_int == MBEDTLS_VERSION_NUMBER );
    TEST_ASSERT( strcmp( build_str, MBEDTLS_VERSION_STRING ) == 0 );
    TEST_ASSERT( strcmp( build_str_full, MBEDTLS_VERSION_STRING_FULL ) == 0 );
    TEST_ASSERT( strcmp( version_str, MBEDTLS_VERSION_STRING ) == 0 );
exit:
    ;
}

void test_check_compiletime_version_wrapper( void ** params )
{

    test_check_compiletime_version( (char *) params[0] );
}
void test_check_runtime_version( char * version_str )
{
    char build_str[100];
    char get_str[100];
    char build_str_full[100];
    char get_str_full[100];
    unsigned int get_int;

    memset( build_str, 0, 100 );
    memset( get_str, 0, 100 );
    memset( build_str_full, 0, 100 );
    memset( get_str_full, 0, 100 );

    get_int = mbedtls_version_get_number();
    mbedtls_version_get_string( get_str );
    mbedtls_version_get_string_full( get_str_full );

    mbedtls_snprintf( build_str, 100, "%u.%u.%u",
        (get_int >> 24) & 0xFF,
        (get_int >> 16) & 0xFF,
        (get_int >> 8) & 0xFF );
    mbedtls_snprintf( build_str_full, 100, "mbed TLS %s", version_str );

    TEST_ASSERT( strcmp( build_str, version_str ) == 0 );
    TEST_ASSERT( strcmp( build_str_full, get_str_full ) == 0 );
    TEST_ASSERT( strcmp( version_str, get_str ) == 0 );
exit:
    ;
}

void test_check_runtime_version_wrapper( void ** params )
{

    test_check_runtime_version( (char *) params[0] );
}
#if defined(MBEDTLS_VERSION_FEATURES)
void test_check_feature( char *feature, int result )
{
    int check = mbedtls_version_check_feature( feature );
    TEST_ASSERT( check == result );
exit:
    ;
}

void test_check_feature_wrapper( void ** params )
{

    test_check_feature( (char *) params[0], *( (int *) params[1] ) );
}
#endif /* MBEDTLS_VERSION_FEATURES */
#endif /* MBEDTLS_VERSION_C */

/*----------------------------------------------------------------------------*/
/* Test dispatch code */


/**
 * \brief       Evaluates an expression/macro into its literal integer value.
 *              For optimizing space for embedded targets each expression/macro
 *              is identified by a unique identifier instead of string literals.
 *              Identifiers and evaluation code is generated by script:
 *              generate_test_code.py and then mbedtls_test_suite.sh and then mbedtls_test_suite.sh
 *
 * \param exp_id    Expression identifier.
 * \param out_value Pointer to int to hold the integer.
 *
 * \return       0 if exp_id is found. 1 otherwise.
 */
int get_expression( int32_t exp_id, int32_t * out_value )
{
    int ret = KEY_VALUE_MAPPING_FOUND;

    (void) exp_id;
    (void) out_value;

    switch( exp_id )
    {

#if defined(MBEDTLS_VERSION_C)

        case 0:
            {
                *out_value = -1;
            }
            break;
#endif

        default:
           {
                ret = KEY_VALUE_MAPPING_NOT_FOUND;
           }
           break;
    }
    return( ret );
}


/**
 * \brief       Checks if the dependency i.e. the compile flag is set.
 *              For optimizing space for embedded targets each dependency
 *              is identified by a unique identifier instead of string literals.
 *              Identifiers and check code is generated by script:
 *              generate_test_code.py and then mbedtls_test_suite.sh and then mbedtls_test_suite.sh
 *
 * \param dep_id    Dependency identifier.
 *
 * \return       DEPENDENCY_SUPPORTED if set else DEPENDENCY_NOT_SUPPORTED
 */
int dep_check( int dep_id )
{
    int ret = DEPENDENCY_NOT_SUPPORTED;

    (void) dep_id;

    switch( dep_id )
    {

#if defined(MBEDTLS_VERSION_C)

        case 0:
            {
#if defined(MBEDTLS_AES_C)
                ret = DEPENDENCY_SUPPORTED;
#else
                ret = DEPENDENCY_NOT_SUPPORTED;
#endif
            }
            break;
#endif

        default:
            break;
    }
    return( ret );
}


/**
 * \brief       Function pointer type for test function wrappers.
 *
 * A test function wrapper decodes the parameters and passes them to the
 * underlying test function. Both the wrapper and the underlying function
 * return void. Test wrappers assume that they are passed a suitable
 * parameter array and do not perform any error detection.
 *
 * \param param_array   The array of parameters. Each element is a `void *`
 *                      which the wrapper casts to the correct type and
 *                      dereferences. Each wrapper function hard-codes the
 *                      number and types of the parameters.
 */
typedef void (*TestWrapper_t)( void **param_array );


/**
 * \brief       Table of test function wrappers. Used by dispatch_test().
 *              This table is populated by script:
 *              generate_test_code.py and then mbedtls_test_suite.sh and then mbedtls_test_suite.sh
 *
 */
TestWrapper_t test_funcs[] =
{
/* Function Id: 0 */

#if defined(MBEDTLS_VERSION_C)
    test_check_compiletime_version_wrapper,
#else
    NULL,
#endif
/* Function Id: 1 */

#if defined(MBEDTLS_VERSION_C)
    test_check_runtime_version_wrapper,
#else
    NULL,
#endif
/* Function Id: 2 */

#if defined(MBEDTLS_VERSION_C) && defined(MBEDTLS_VERSION_FEATURES)
    test_check_feature_wrapper,
#else
    NULL,
#endif

};

/**
 * \brief        Execute the test function.
 *
 *               This is a wrapper function around the test function execution
 *               to allow the setjmp() call used to catch any calls to the
 *               parameter failure callback, to be used. Calls to setjmp()
 *               can invalidate the state of any local auto variables.
 *
 * \param fp     Function pointer to the test function.
 * \param params Parameters to pass to the #TestWrapper_t wrapper function.
 *
 */
void execute_function_ptr(TestWrapper_t fp, void **params)
{
#if defined(MBEDTLS_PSA_CRYPTO_EXTERNAL_RNG)
    mbedtls_test_enable_insecure_external_rng( );
#endif

#if defined(MBEDTLS_CHECK_PARAMS)
    mbedtls_test_param_failed_location_record_t location_record;

    if ( setjmp( mbedtls_test_param_failed_get_state_buf( ) ) == 0 )
    {
        fp( params );
    }
    else
    {
        /* Unexpected parameter validation error */
        mbedtls_test_param_failed_get_location_record( &location_record );
        mbedtls_test_fail( location_record.failure_condition,
                           location_record.line,
                           location_record.file );
    }

    mbedtls_test_param_failed_reset_state( );
#else
    fp( params );
#endif

#if defined(MBEDTLS_TEST_MUTEX_USAGE)
    mbedtls_test_mutex_usage_check( );
#endif /* MBEDTLS_TEST_MUTEX_USAGE */
}

/**
 * \brief        Dispatches test functions based on function index.
 *
 * \param func_idx    Test function index.
 * \param params      The array of parameters to pass to the test function.
 *                    It will be decoded by the #TestWrapper_t wrapper function.
 *
 * \return       DISPATCH_TEST_SUCCESS if found
 *               DISPATCH_TEST_FN_NOT_FOUND if not found
 *               DISPATCH_UNSUPPORTED_SUITE if not compile time enabled.
 */
int dispatch_test( size_t func_idx, void ** params )
{
    int ret = DISPATCH_TEST_SUCCESS;
    TestWrapper_t fp = NULL;

    if ( func_idx < (int)( sizeof( test_funcs ) / sizeof( TestWrapper_t ) ) )
    {
        fp = test_funcs[func_idx];
        if ( fp )
            execute_function_ptr(fp, params);
        else
            ret = DISPATCH_UNSUPPORTED_SUITE;
    }
    else
    {
        ret = DISPATCH_TEST_FN_NOT_FOUND;
    }

    return( ret );
}


/**
 * \brief       Checks if test function is supported in this build-time
 *              configuration.
 *
 * \param func_idx    Test function index.
 *
 * \return       DISPATCH_TEST_SUCCESS if found
 *               DISPATCH_TEST_FN_NOT_FOUND if not found
 *               DISPATCH_UNSUPPORTED_SUITE if not compile time enabled.
 */
int check_test( size_t func_idx )
{
    int ret = DISPATCH_TEST_SUCCESS;
    TestWrapper_t fp = NULL;

    if ( func_idx < (int)( sizeof(test_funcs)/sizeof( TestWrapper_t ) ) )
    {
        fp = test_funcs[func_idx];
        if ( fp == NULL )
            ret = DISPATCH_UNSUPPORTED_SUITE;
    }
    else
    {
        ret = DISPATCH_TEST_FN_NOT_FOUND;
    }

    return( ret );
}

int main( int argc, const char *argv[] )
{
    int ret;
    mbedtls_test_platform_setup();
    ret = execute_tests( argc, argv, "/zip/third_party/mbedtls/test/test_suite_version.datax" );
    mbedtls_test_platform_teardown();
    return( ret );
}
