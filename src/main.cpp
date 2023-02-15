/**
 * 
 *      @author Sean Hobeck
 *       @date 02-14-2023
 * 
 **/
#include "sdk/sdk.hpp"


/// @brief Note this is very similar to __attribute__((section (.ctors)))
__attribute__((constructor))
static void attach() 
{
    std::cout << "We attached" << std::endl;
};

/// @brief Note this is very similar to __attribute__((section (.dtors)))
__attribute__((destructor))
static void detach() 
{

};