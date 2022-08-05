#include <iostream>
#include "Application.h"

int main() {

    Application app;
    app.initializeCore();
    app.initializeAudio();
    app.run();

    return 0;
}
