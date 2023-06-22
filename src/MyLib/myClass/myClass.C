#include "myClass.H"
#include "volFields.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(myClass, 0);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::myClass::myClass
(
    const fvMesh& mesh,
    const dictionary& dict
)
:
    mesh_(mesh),
    dict_(dict),
    velocityIsFound_(mesh.foundObject<volVectorField>("U")),
    setting_(dict.lookupOrDefault<label>("setting", 0))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::myClass::~myClass()
{}

// ************************************************************************* //
