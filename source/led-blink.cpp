#include <iostream>
#include <unistd.h>
#include <string>
#include <math.h>
#include <ctime>
#include "gpio.h"
#include <chrono>
#include <thread>
using namespace std::chrono;
typedef std::chrono::high_resolution_clock Clock;


const uint8_t LED_PIN = 6;          // Test pin - LED light

const uint8_t TRIG_PIN = 3;         // Associate pin 3 to TRIG
const uint8_t ECHO_PIN = 2;         // Associate pin 2 to ECHO



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

void nsleep(long us)
{
    struct timespec wait;

    wait.tv_sec = us / (1000 * 1000);
    wait.tv_nsec = (us % (1000 * 1000)) * 1000;
    nanosleep(&wait, NULL);
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
            //Gpio::pinMode( (int) arg2, GPD_OUTPUT );
        }
        else if ( ( arg == "-c" ) || ( arg == "--command" ) )
        {
            //if ( arg2 == "blink" )
            //{
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
            //}
            //else
            //{
            //    show_usage(argv[0]); // Show help - no String
            //    return 1;
            //}

        }
        else if ( ( arg == "-t" ) && ( arg3 == "-e" ) )
        {
            // HC-SR04

            // TODO use arg values for GPIO PINs
            Gpio::pinMode( TRIG_PIN, GPD_OUTPUT );
            Gpio::pinMode( ECHO_PIN, GPD_INPUT );

            while ( true )
            {
                Gpio::digitalWrite( TRIG_PIN, false );
                std::this_thread::sleep_for(seconds(1));        // Delay of 2 seconds

                Gpio::digitalWrite( TRIG_PIN, true );
                nsleep(10000);                                  // I used this because it's more precise

                Gpio::digitalWrite( TRIG_PIN, false );


                // TODO, add value greater then 400cm as out of range here
                while ( !Gpio::digitalRead(ECHO_PIN)  ) {}      // Check whether the ECHO is LOW
                Clock::time_point pulseStart = Clock::now();    // Mark pulseStart

                while ( Gpio::digitalRead(ECHO_PIN) )  {}       // Check whether the ECHO is HIGH
                Clock::time_point pulseEnd = Clock::now();      // Mark pulseEnd

               //duration<double> pulseDuration = duration_cast<duration<double>>(pulseEnd - pulseStart);
                auto dur = pulseEnd - pulseStart;
                auto distance = duration_cast<duration<float>>(dur * 1000000 / 29.1 / 2 ).count();
                //float distance<duration<float>>(dur * 1000000 / 29.1 / 2 ).count();


                distance = roundf( distance * 100 ) / 100;        // Round to two decimal points

                std::cout << "Delta pulse_end-pulse_start: "
                          << duration_cast<microseconds>(pulseEnd - pulseStart).count()
                          << " microseconds \n"
                          << duration_cast<microseconds>((pulseEnd - pulseStart) / 29.1 / 2 ).count()
                          << " cm\n\n"
                          << duration_cast<duration<float>>(dur).count()
                          << " float\n"
                          << duration_cast<duration<float>>(dur * 1000000 / 29.1 / 2 ).count()
                          << " cm\n"
                          << distance
                          << " cm rounded\n\n" << std::endl;

                //auto pulse_duration = pulseEnd - pulseStart;   // Get pulse duration to a variable


                //float distance =  dur * 17150;          // Multiply pulse duration by 17150 to get distance
                //distance = roundf( distance * 100 ) / 100;        // Round to two decimal points


                //if ( ( distance > 2 ) && ( distance < 400 ) )   // Check whether the distance is within range
                    //printf ( "Distance: %4.2f cm \n", distance - 0.5); // Print distance with 0.5 cm calibration
                //else
                    //printf ( "Out Of Range \n" );                       //display out of range


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