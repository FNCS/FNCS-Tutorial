/* C++ STL */
#include <iostream>

/* FNCS headers */
#include <fncs.hpp>

using namespace std; /* C++ standard namespace */

/* some C++ compilers have a nullptr instance,
 * otherwise we fall back to the old default of NULL */
#ifndef nullptr 
#define nullptr NULL
#endif

/* the current time of our simulator */
static fncs::time time_current=0;
/* time when sim will stop */
static fncs::time time_stop=10;

/* our simulator function, called by the main() function later */
static void generic_simulator()
{
    fncs::time time_granted=0; /* the time step FNCS has allowed us to process */
    fncs::time time_desired=0; /* the time step we would like to go to next */

    /* initialize FNCS with default configuration file fncs.zpl */
    fncs::initialize();

    /* unless FNCS detects another simulator terminates early, the
     * this simulator will run from time step 0 to time step 9 */
    while (time_current < time_stop) {
        /* do useful work; check for incoming messages to the objects we
         * registered earlier */
        cout << "SimA: Working. Time is " << time_current << endl;
        fncs::publish("simObjA.attribute", "value");
        fncs::route("simObjA", "simObjB", "key", "value");

        /* Check for incoming messages. */
        vector<string> events = fncs::get_events();
        for (vector<string>::iterator key=events.begin();
                key!=events.end(); ++key) {
            cout << "SimA: received topic '"
                << *key
                << "' with value '"
                << fncs::get_value(*key)
                << "'"
                << endl;
        }

        /* Which time step do we wish to go to next? This does not
         * necessarily need to be a delta of 1; FNCS supports
         * arbitrary time deltas. */
        time_desired = time_current + 1;

        /* Synchronization by requesting the next time step. The next
         * time could very well be smaller than this simulator is able
         * to process i.e. the network simulator runs in nanoseconds
         * while this simulator runs in seconds. In such a case, the
         * time_granted will be the same as time_current. Another
         * possibility is that the time_granted is less than
         * time_desired due to another simulator requesting a smaller
         * time delta. */
        time_granted = fncs::time_request(time_desired);

        cout << "SimA: time_request"
            << " current=" << time_current
            << " desired=" << time_desired
            << " granted=" << time_granted << endl;

        time_current = time_granted;
    }
    cout << "SimA: DONE!" << endl;

    fncs::finalize();
}


/* a simple main that calls our simulator function;
 * it is sometimes useful to capture and report exceptions but this
 * might be overkill for your particular needs */
int main(int argc, char **argv)
{
    try {
        cout << "starting generic simulator" << endl;
        generic_simulator();
    } catch (const exception &e) {
        cout << e.what() << endl;
    }
    catch (const string &e) {
        cout << e << endl;
    }
    cout << "finished generic simulator" << endl;
    return 0;
}

