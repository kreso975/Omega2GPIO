#include <iostream>
#include <unistd.h>
#include <string>
#include "gpio.h"

const uint8_t LED_PIN = 3;

static void show_usage( std::string argv )
{
    std::cerr    << "Usage: <option> [<STRING> <STRING>]\n"
                 << "Options:\n"
                 << "\t-h, --help\t\tShow this help message\n"
                 << "\t-p, --pin\t\tset pin\n"
                 << "\t-d, --direction\t\tSet pin direction [0 = Output | 1 = Input]\n"
                 << "\t-v, --value\t\tSet value [LOW | HIGH]\n"
                 << "\t-c, --command\t\tdo Command [blink | something]\n"
                 << std::endl;
}

int main( int argc, char* argv[] )
{
    if ( argc > 1 )
    {
        Gpio::initialize();

        std::string arg = argv[1];
        if ( ( arg == "-h" ) || ( arg == "--help" ) )
            show_usage(argv[0]); // Show help

        else if ( ( arg == "-p" ) || ( arg == "--pin" ) )
        {
            // set the pin mode to output, so that we can digitalWrite() it
            Gpio::pinMode(LED_PIN, GPD_OUTPUT);
        }
        else if ( ( arg == "-c" ) || ( arg == "--command" ) )
        {
            if ( argv[2] == "blink" )
            {
                // set the pin mode to output, so that we can digitalWrite() it
                Gpio::pinMode(LED_PIN, GPD_OUTPUT);

                // by the default our led is set to be on
                bool ledStatus = true;
                int counter = 0;

                while ( counter <= 20 )
                {
                    // toggle the pin
                    Gpio::digitalWrite(LED_PIN, ledStatus);

                    // and wait 500 ms
                    usleep(500 * 1000);

                    ledStatus = !ledStatus;
                    counter++;
                }

                return 0;
            }
            else
            {
                show_usage(argv[0]); // Show help - no String
                return 1;
            }

        }
    }
    else
    {
        show_usage( argv[0] );
        return 0;
    }

    return 0;
}