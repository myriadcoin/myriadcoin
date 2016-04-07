Myriad Core integration/staging tree
========================================

http://www.myriadcoin.org

Copyright (c) 2009-2015 Bitcoin Core Developers

Copyright (c) 2014-2016 Myriad Core Developers


What is Myriad?
-------------------

Myriad is an experimental new digital currency that enables instant payments to
anyone, anywhere in the world. Myriad uses peer-to-peer technology to operate
with no central authority: managing transactions and issuing money are carried
out collectively by the network. Myriad Core is the name of open source
software which enables the use of this currency.

For more information, as well as an immediately usable, binary version of
the Myriadcoin Core software, see http://www.myriadcoin.org/

License
-------

Myriad Core is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see http://opensource.org/licenses/MIT.

Building
--------

See files in the [doc](doc) directory for generic build instructions for Windows,
OSX and Unix. Basic requirements:

* GCC and binuntils
* Git
* Boost C++ libraries
* Berkeley DB - Should be version 4.8, newer versions are known to work but not guaranteed
* libssl (part of openssl)
* [miniupnpc](http://miniupnp.free.fr/) for optional UPNP support

Requirements to build the GUI:

* QT4 or QT5 development libraries
* protobuf development libraries
* libqrencode development libraries

A common question is how to build from Ubuntu, instructions follow:

1. sudo add-apt-repository ppa:bitcoin/bitcoin -y
2. sudo apt-get update
3. sudo apt-get install build-essential curl git libminiupnpc-dev libssl-dev m4 -y
4. sudo apt-get install libdb4.8-dev libdb4.8++-dev -y
5. sudo apt-get install libprotoc-dev libprotobuf-dev -y
6. sudo apt-get install libtool automake autoconf make pkg-config -y
7. sudo apt-get install libpng-dev -y
8. sudo apt-get install libqrencode-dev -y
9. sudo apt-get install libqt4-dev -y
10. sudo apt-get install libboost-all-dev -y
11. sudo apt-get install libcurl4-openssl-dev -y
12. git clone git://github.com/myriadteam/myriadcoin.git
13. cd myriadcoin
14. ./autogen.sh
15. ./configure
16. make


Development tips and tricks
---------------------------

**compiling for debugging**

Run configure with the --enable-debug option, then make. Or run configure with
CXXFLAGS="-g -ggdb -O0" or whatever debug flags you need.

**debug.log**

If the code is behaving strangely, take a look in the debug.log file in the data directory;
error and debugging message are written there.

The -debug=... command-line option controls debugging; running with just -debug will turn
on all categories (and give you a very large debug.log file).

The Qt code routes qDebug() output to debug.log under category "qt": run with -debug=qt
to see it.

**testnet and regtest modes**

Run with the -testnet option to run with "play myriad" on the test network, if you
are testing multi-machine code that needs to operate across the internet.

If you are testing something that can run on one machine, run with the -regtest option.
In regression test mode blocks can be created on-demand; see qa/rpc-tests/ for tests
that run in -regest mode.

**DEBUG_LOCKORDER**

Myriad Core is a multithreaded application, and deadlocks or other multithreading bugs
can be very difficult to track down. Compiling with -DDEBUG_LOCKORDER (configure
CXXFLAGS="-DDEBUG_LOCKORDER -g") inserts run-time checks to keep track of what locks
are held, and adds warning to the debug.log file if inconsistencies are detected.
