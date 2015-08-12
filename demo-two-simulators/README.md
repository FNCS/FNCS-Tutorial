# FNCS2-Tutorial Two Simulators

Step-by-step guide for building and running FNCS2 with two small
power simulator applications.

## Table of Contents
- [Hardware Requirements](#hardware-requirements)
- [Software and Dependencies](#software-and-dependencies)
  - [ZeroMQ Installation](#zeromq-installation)
  - [CZMQ Installation](#czmq-installation)
  - [FNCS2 Installation](#fncs-installation)
- [Important Environment Variables](#important-environment-variables)
- [Model Description](#model-description)
- [Running the Co-Simulation](#running-the-co-simulation)

## Hardware Requirements
[back to contents](#table-of-contents)

This software was developed and tested exclusively using the Linux
operating system.  Our developers have used RedHat EL5 as well as Ubuntu
12.04.5 LTS (precise), both of which were 64-bit systems. This software
has not been built or tested on Windows. Although all of the software
packages and their dependencies are known to have Windows installers, we
have not yet developed any way to patch those official distributions
with FNCS2 enhancements.

## Software and Dependencies
[back to contents](#table-of-contents)

This portion of the tutorial will walk you through installing all
prerequisite software. The following software will be covered,
indicating the primary software installed and the list of dependencies:

You will need to install git in order to clone (checkout) all of our
FNCS2 and related software packages.

- FNCS2
  - ZeroMQ (3.2.x)
  - CZMQ (3.0.x)

It will be assumed that you will be installing all software into
$HOME/FNCS2-install. Doing so will greatly simplify the steps of this
tutorial since we can set $LD_LIBRARY_PATH and $PATH accordingly, as
well as any other needed installation paths while building many of the
involved software packages. In fact, now would be a good time to set a
shortcut environment variable, like so:

```
export FNCS2_INSTALL="$HOME/FNCS2-install"
```

NOTE: You could, in theory, change this to point to wherever you wish to
install FNCS2 and all related software packages.

It is also assumed that you are using a Bourne shell; all of the
step-by-step instructions (like the one above) that appear in this
tutorial  will assume a Bourne shell. If you are using a C shell, we
hope you can adapt the steps as needed, mostly in how environment
variables are set.

### ZeroMQ Installation
[back to contents](#table-of-contents)

http://zeromq.org/

The ZeroMQ library is one library that our FNCS2 library depends on.
We have extensively tested our software using version 3.2.x, however
later versions may also work but have not yet been tested.

Get the ZeroMQ software and install it using the following steps:

```
# we are doing everything from your $HOME directory
cd $HOME

# download zeromq
wget http://download.zeromq.org/zeromq-3.2.4.tar.gz
# if you do not have wget, use
# curl -O http://download.zeromq.org/zeromq-3.2.4.tar.gz

# unpack zeromq, change to its directory
tar -xzf zeromq-3.2.4.tar.gz
cd zeromq-3.2.4

# configure, make, and make install 
./configure --prefix=$FNCS2_INSTALL
make
make install
```

Congratulations, you have now installed ZeroMQ. 

### CZMQ Installation
[back to contents](#table-of-contents)

http://czmq.zeromq.org/

Installing CZMQ is like any other software using configure and make.
The main challenge is specifying the installation location (--prefix)
for CZMQ as well as the location where ZeroMQ was installed.  If you
installed ZeroMQ as written above, the following will work for you.

```
# we are doing everything from your $HOME directory
cd $HOME

# download czmq
wget http://download.zeromq.org/czmq-3.0.0-rc1.tar.gz
# if you do not have wget, use
# curl -O http://download.zeromq.org/czmq-3.0.0-rc1.tar.gz

# unpack czmq, change to its directory
tar -xzf czmq-3.0.0-rc1.tar.gz
cd czmq-3.0.0

# configure, make, and make install
./configure --prefix=$HOME/FNCS2_install --with-libzmq=$HOME/FNCS2_install
make
make install
```

Congratulations, you have now installed CZMQ. 

### FNCS2 Installation
[back to contents](#table-of-contents)

https://github.com/GridOPTICS/FNCS2

The FNCS2 software will build and install the FNCS2 library, the various
FNCS2 header files, as well as the broker application. The FNCS2 broker
represents the central server that all other simulator clients will
connect to in order to synchronize in time and exchange messages with
other simulators. The FNCS2 library and header represent the needed API
for communicating with the broker using the sync and messaging function
calls.

Get the FNCS2 software and install it using the following steps:

```
# we are doing everything from your $HOME directory
cd $HOME

# download FNCS2
git clone https://github.com/GridOPTICS/FNCS2

# change to FNCS2 directory
cd FNCS2

# configure, make, and make install 
./configure --prefix=$FNCS2_INSTALL --with-zmq=$FNCS2_INSTALL
make
make install
```

Congratulations, you have now installed FNCS2.

## Important Environment Variables
[back to contents](#table-of-contents)

Now that all of the FNCS2 and related software is installed, now would be
a great time to set some important environment variables. If you have
been following the steps exactly, then you can copy-and-paste the
following into a handy shell script that you can source before running
the co-simulation. If you are reading this file from the github sources,
you will find the file [here](env.sh).

Here is what the file recently looked like, but please refer to the
original file as linked above.

```Bash
#!/bin/sh

export FNCS2_INSTALL="$HOME/FNCS2-install"

# update LD_LIBRARY_PATH for all but MCR
if test "x$LD_LIBRARY_PATH" = x
then
    export LD_LIBRARY_PATH="$FNCS2_INSTALL/lib"
else
    export LD_LIBRARY_PATH="$FNCS2_INSTALL/lib:$LD_LIBRARY_PATH"
fi

# update PATH for all but MCR
if test "x$PATH" = x
then
    export PATH="$FNCS2_INSTALL/bin"
else
    export PATH="$FNCS2_INSTALL/bin:$PATH"
fi
```

## Model Description
[back to contents](#table-of-contents)

In this current tutorial directory you will find two source files,
[simA.cpp](simA.cpp) and [simB.cpp](simB.cpp), as well as a
[Makefile](Makefile) for compiling.

We have set up the co-sim to use two generic simulators, namely "A" and
"B". "A" publishes certain object state at each time step for any
interested subscriber to listen to. "A" also mimics two endpoints
communicating by sending a message "from" a named endpoint in simulator
A to a named endpoint in simulator B. This behavior is useful when
integrating a communication network simulator in a later demo.

### A Complete Breakdown of our First Simulator
Let's look at [simA.cpp](simA.cpp) in detail.  We've attempted to insert
comments step by step to make the file self-documenting as a reference.
You should start by reading the file line by line. We will highlight
important code for you in the next section of this tutorial. Here is
what the file recently looked like, but please refer to the original
file as linked above.

```c++
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
```

#### Boilerplate Setup and Teardown
You'll notice there is quite a bit of "boilerplate" code -- code
necessary for simply setting up the simulator and its FNCS connection.
All simulators need the following:
- A way to tell current simulator time
- fncs::initialize()
- A loop to process time steps with fncs::time_request(desired_time)
- fncs::finalize()

Those are the necessities -- you don't even have to publish values --
your simulator could simply keep in synch with the other simulators but
otherwise hang out on its own and never communicate (but that's not
terribly useful.)

#### Configuration ZPL File
Before receiving a published value, you must configure a simulator to
subscribe to the topics of interest. This is done using a ZPL-formatted
file. Details about the ZPL format can be found at the ZeroMQ website
(http://rfc.zeromq.org/spec:4).

Each simulator should have a corresponding "fncs.zpl" file.  By default,
the fncs.zpl is assumed to be in the current working directory where the
simulator is launched.  Otherwise, you can specify the name and/or
location of the file using the environment variable FNCS_CONFIG_FILE.

The following code block is an example of the fncs.zpl file.  The inline
comments in the code block indicate which fields are required.  In
short, the only required fields are the name, time_delta, and broker
address.  The subscriptions are actually optional (think of a weather
simulator that only reports the temperature but does not receive data).
However, if you specify a subscription, each subscription has some
required values and some optional values.

```
name = sim1                 # required; across the co-simulation, all names must be unique
time_delta = 1s             # required; format is <number><unit>; smallest time step supported by the simulator
broker = tcp://localhost:5570   # required; broker location
values                      # optional; list of exact-string-matching topic subscriptions
    foo                     # required; lookup key
        topic = some_topic  # required; format is any reasonable string (not a regex)
        default = 10        # optional; default value
        type = int          # optional; currently unused; data type
        list = false        # optional; defaults to "false"; whether incoming values queue up (true) or overwrite the last value (false)
    bar                     # see "foo" above
        topic = some_topic  # see "foo" above
        default = 0.1       # see "foo" above; here we used a floating point default
        type = double       # see "foo" above
        list = true         # see "foo" above; this is the only difference between "foo" and "bar" here
matches                     # optional; list of regular-expression matching topic subscriptions
    baz                     # required; lookup key
        topic = sim1/.*     # required; topic (a regex)
```

The [simA.zpl](simA.zpl) config file is used by simA.  One important bit
to glean from this file is the location where the "broker" will be
running. In our case it's running locally, but you could start the
broker on another server so long as you tell the simulators where it is
located. Another important parameter is the "time_delta" which tells the
broker which unit of time the simulator uses. This becomes important as
you add simulators with different units of time. Lastly, across all
simulator instances within your co-simulation, they must all be assigned
a unique name.

## Running the Co-Simulation
[back to contents](#table-of-contents)

We need to create a working directory for our co-simulation. Each
simulator software package will generate output files, as usual, to the
current working directory. In addition, each example simulator has
diagnostic output to standard output (the terminal). The simulators are
designed to locate files from the working directory, for example, as
inputs. Please use this current tutorial directory as a working
directory for the co-simulation run.  

In this directory you will find only a few files since our simulators
are rather simple.  You will find the necessary ZPL files that each
simulator needs to pass information to the FNCS library during
initialization.  There is also a handy script file for starting the
co-simulation [run.sh](run.sh).  All runtime files will be described in
detail next.

Assuming you have set up your environment correctly, perhaps having
sourced the env.sh script, you should be able to run "make" without
issue. This will compile the two source files. To run this simple
example, the file [run.sh](run.sh) has been provided which will redirect
stdout from each simulator to its own file and will also launch the
"fncs_broker" application which is required to run the co-simulation.

If you execute the [run.sh](run.sh) file, the output from each simulator
should appear in its own file appropriately named. The output is rather
uninspiring, but you should at least notice that messages weren't
received out of order. The [run.sh](run.sh) file isn't anything special,
but it does at least execute the "fncs_broker" application with the
correct number of simulators expected (2). The total number of
simulators connecting to the broker is its only parameter.

## FAQ/Troubleshooting
Q: What happens when you specify 3 to the fncs_broker instead of 2?

A: The co-sim will hang, as it is expecting another simulator to connect.

Q: What happens when you specify 3 to the fncs_broker and simply execute
another instance of either simA or simB to be the third
simulator?

A: An error will occur. All simulators must be uniquely named across the
entire co-simulation.
