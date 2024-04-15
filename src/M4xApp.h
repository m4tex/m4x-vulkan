/** @file */

//
// Created by m4tex on 4/15/24.
//

#ifndef M4XDEV_M4XAPP_H
#define M4XDEV_M4XAPP_H

namespace m4x {
    /**
     *
     */
    class M4xApp {
    public:
        void run();
    private:
        void createWindow();
        void initVulkan();
        void mainLoop();

    };
} // m4x

#endif //M4XDEV_M4XAPP_H
