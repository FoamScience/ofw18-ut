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

    // Second class dependency
    dictionary dict;

    SECTION("Velocity is found if mesh is read from disk correctly") {
        myClass obj(mesh, dict);
        REQUIRE(obj.velocityIsFound());
    }

    SECTION("Setting value matches what is supplied in the config. dictionary") {
        myClass obj(mesh, dict);
        dict.set("setting", 10);
        REQUIRE(obj.setting() == 10);
    }
}
