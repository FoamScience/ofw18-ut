/**
 * A simple class used for the OFW18 Unit/Integration testing training
**/

#ifndef myClass_H
#define myClass_H

#include "fvMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                Class myClass Declaration
\*---------------------------------------------------------------------------*/

class myClass
{
protected:
    //- Protected members
    const fvMesh& mesh_;
    const dictionary& dict_;
    bool velocityIsFound_;
    label setting_;

public:

    //- Runtime type information
    TypeName("myClass");

    // Constructors

        //- Construct from components
        myClass(const fvMesh&, const dictionary&);


    //- Destructor
    virtual ~myClass();


    // Member Functions
        bool velocityIsFound() const {
            return velocityIsFound_;
        }
        label setting() const {
            return setting_;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
