#include "M4xApp.h"

//std
#include <stdexcept>
#include <iostream>

int main() {
    m4x::M4xApp app{};

    try {
        app.run();
    } catch (const std::exception& e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

	return EXIT_SUCCESS;
}
