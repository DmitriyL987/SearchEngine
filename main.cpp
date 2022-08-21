#include <thread>
#include <mutex>
#include <gtest/gtest.h>

#include "converterJSON.h"




int main() {

    ConverterJSON converterJSON;
    if(converterJSON.error) {
        return -1;
    }


    return 0;
}
