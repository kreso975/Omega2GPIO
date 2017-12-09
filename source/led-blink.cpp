#include <iostream>
#include <unistd.h>
#include <string>
#include <time.h>
#include "gpio.h"
#include <chrono>
#include <thread>
typedef std::chrono::high_resolution_clock Clock;


const uint8_t LED_PIN = 3;          // Test pin - LED light

const uint8_t TRIG_PIN = 6;         // Associate pin 9 to TRIG
const uint8_t ECHO_PIN = 1;         // Associate pin 8 to ECHO



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
        std::string arg3 = argv[3];

        if ( ( arg == "-h" ) || ( arg == "--help" ) )
            show_usage(argv[0]); // Show help

        else if ( ( arg == "-p" ) || ( arg == "--pin" ) )
        {
            // set the pin mode to output, so that we can digitalWrite() it
            Gpio::pinMode( (int) argv[2], GPD_OUTPUT );
        }
        else if ( ( arg == "-c" ) || ( arg == "--command" ) )
        {
            if ( argv[2] == "blink" )
            {
                // set the pin mode to output, so that we can digitalWrite() it
                Gpio::pinMode( LED_PIN, GPD_OUTPUT );

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
        else if ( ( arg == "-t" ) && ( arg3 == "-e" ) )
        {
            // HC-SR04
            // TODO use arg values for GPIO PINs
            Gpio::pinMode( LED_TRIG, GPD_OUTPUT );
            Gpio::pinMode( LED_ECHO, GPD_INPUT );

            while ( true )
            {
                Gpio::digitalWrite( LED_TRIG, false );
                nanosleep(2000);                                        // Delay of 2 microseconds

                Gpio::digitalWrite( LED_TRIG, true );
                nanosleep(10000);                                       // Delay of 10 microseconds
                Gpio::digitalWrite( LED_TRIG, false );

                while ( Gpio::digitalRead(LED_ECHO) == 0 )              // Check whether the ECHO is LOW
                    auto pulse_start = Clock::now();                    // Saves the last known time of LOW pulse

                while ( Gpio::digitalRead(LED_ECHO) == 1 )  // Check whether the ECHO is HIGH
                    auto pulse_end = Clock::now();                // Saves the last known time of HIGH pulse


                std::cout << "Delta pulse_end-pulse_start: "
                          << std::chrono::duration_cast<std::chrono::milliseconds>(pulse_end - pulse_start).count()
                          << " milliseconds\n" << std::endl;

                auto pulse_duration = pulse_end - pulse_start;   // Get pulse duration to a variable

                float distance = pulse_duration * 17150;          // Multiply pulse duration by 17150 to get distance
                distance = round(distance, 2);              // Round to two decimal points

                if ( ( distance > 2 ) && ( distance < 400 ) )   // Check whether the distance is within range
                    printf ( "Distance: %4.2f cm \n", distance - 0.5); // Print distance with 0.5 cm calibration
                else
                    printf ( "Out Of Range \n" );                       //display out of range

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