#include "IOobject.H"
#include "PstreamReduceOps.H"
#include "catch2/catch_all.hpp"
#include "catch2/catch_test_macros.hpp"
#include "fvCFD.H"
#include "messageStream.H"
#include "myClass.H"
#include "volFieldsFwd.H"

#include <csetjmp>
#include <csignal>
#include <cstdlib>
#include <functional>

using namespace Foam;
extern Time* timePtr;
extern argList* argsPtr;

// ********************************************************************************************** //

jmp_buf tst_env; // POSIX code, don't really care what happens on Windows
void onSigAbrt(int signum) {
    signal (signum, SIG_DFL); longjmp (tst_env, 1);
}
void tryAndCatchAbortingCode(std::function<void(void)> func) {
    FatalError.throwExceptions();
    if (setjmp (tst_env) == 0) {
        signal(SIGABRT, &onSigAbrt); signal(SIGTERM, &onSigAbrt);
        func();
        signal(SIGABRT, SIG_DFL); signal(SIGTERM, SIG_DFL);
    }
    else {
        Pout<< "Either this code tried to abort or there was"
        " an attempt to terminate it (e.g. with a timeout) on " <<
            Pstream::myProcNo() << "..." << endl;
        bool abortedOrTerminated = true;
        REQUIRE(abortedOrTerminated == false);
    }
}

TEST_CASE("myClass objects are properly constructed", "[cavity][serial][parallel]")
{
    Time& runTime = *timePtr;
    argList& args = *argsPtr;

    // First class dependency
    fvMesh mesh
    (
        IOobject
        (
            "",
            runTime.constant(),
            runTime,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

    // Create the actual U field
    volVectorField U
    (
        IOobject
        (
            "U",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        mesh,
        dimensionedVector("initU", dimVelocity, vector(0,0,0))
    );


    SECTION("Velocity is found if mesh is read from disk correctly") {
        // Second class dependency
        dictionary dict;
        myClass obj(mesh, dict);

        REQUIRE(obj.velocityIsFound());
    }

    SECTION("Setting value matches what is supplied in the config. dictionary") {
        // Second class dependency
        dictionary dict;
        dict.set("setting", 10);
        myClass obj(mesh, dict);
        REQUIRE(obj.setting() == 10);
    }

    SECTION("An infinite loop is timed-out") {
        tryAndCatchAbortingCode(
            [&]() {
                while(true) {}
            }
        );
    }
}
