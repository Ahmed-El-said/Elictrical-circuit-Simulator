#include <iostream>
#include <string>
#include <memory.h>
#include <cmath>

using namespace std;

struct cmplx
{
    double real;
    double imag;
};

void conject (cmplx &elementVal)
{
    elementVal.imag = -1 * elementVal.imag;
}

cmplx addCmplx(cmplx addC1 , cmplx addC2)
{
    cmplx cmplxOut;
    cmplxOut.real = addC1.real + addC2.real;
    cmplxOut.imag = addC1.imag + addC2.imag;
    return cmplxOut;
}

cmplx subtractCmplx(cmplx subC1 , cmplx subC2)
{
    cmplx cmplxOut;
    cmplxOut.real = subC1.real - subC2.real;
    cmplxOut.imag = subC1.imag - subC2.imag;
    return cmplxOut;
}
cmplx multiCmplx(cmplx multiC1 , cmplx multiC2)
{
    cmplx cmplxOut;
    cmplxOut.real = (multiC1.real * multiC2.real) - (multiC1.imag * multiC2.imag) ;
    cmplxOut.imag = (multiC1.real * multiC2.imag) + (multiC1.imag * multiC2.real) ;
    return cmplxOut;
}

cmplx divisionCmplx(cmplx divC1 , cmplx divC2)
{
    cmplx cmplxOut;
    conject(divC2);
    cmplxOut = multiCmplx( divC1 , divC2 );
    double denominator = ( pow(divC2.real , 2) + pow(divC2.imag , 2) );
    cmplxOut.real = cmplxOut.real / denominator;
    cmplxOut.imag = cmplxOut.imag / denominator;
    return cmplxOut ;
}

double Magnitude(cmplx magC1)
{
    double magnitude = sqrt(pow( magC1.real , 2 ) + pow( magC1.imag , 2 ));
    return magnitude;
}


//Solves system of linear equations
cmplx* SolveSystem(cmplx** matrix, unsigned int size);

int main()
{
	//Constants
	const cmplx POS_REAL_ONE = { ( 1.0, 0.0 ) };
	const cmplx NEG_REAL_ONE = { (-1.0, 0.0 ) };

	//Nodes and elements count
	unsigned int nodes;
	unsigned int elements;
	unsigned int vs = 0;
	unsigned int ind_VS;
    unsigned int dep_VS;
	unsigned int tot_VS;

	//Element temporary variables
	string elementType;
	unsigned int elementPosNode;
	unsigned int elementNegNode;
	cmplx elementVal;

	//dependent sources variables
	unsigned int depPosNode;
	unsigned int depNegNode;

	//Ask user for nodes, elements and voltage sources count
	cout << "Enter the number of nodes: ";
	cin >> nodes;
	cout << "Enter the number of elements: ";
	cin >> elements;
	cout << "Enter the number of independent voltage sources: ";
	cin >> ind_VS;
	cout << "Enter the number of dependent sources: ";
	cin >> dep_VS;

	//summing the total number of voltage sources
	tot_VS = ind_VS + dep_VS;

	//Create matrix (Two dimensional array)
	cmplx** matrix = new cmplx*[nodes + tot_VS];
	for (unsigned int i = 0; i < (nodes + tot_VS); i++)
	{
		matrix[i] = new cmplx[nodes + tot_VS + 1];
		memset(matrix[i], 0, (nodes + tot_VS + 1) * sizeof(cmplx));
	}

	//Ask user to enter elements
	cout << "Enter elements data:" << endl;

	//Loop over elements count
	for (unsigned int i = elements; i > 0; i--)
	{
		//Get element data from input
	    cin >> elementType;
		cin >> elementPosNode >> elementNegNode;
		cin >> elementVal.real >> elementVal.imag;
		elementPosNode--;
		elementNegNode--;

		//Get dependent source data
		if(elementType[0]== 'd' || elementType[0]== 'D')
        {
			cin >> elementPosNode >> elementNegNode;
			cin >> depPosNode >> depNegNode;
			depPosNode--;
			depNegNode--;
        }

		//Inject element into matrix based on its type
		switch (elementType[0])
		{
			case 'R':
			case 'r':
			case 'C':
            case 'c':
            case 'L':
            case 'l':
				elementVal = divisionCmplx(POS_REAL_ONE, elementVal);
				matrix[elementPosNode][elementPosNode] = addCmplx(matrix[elementPosNode][elementPosNode], elementVal);
				matrix[elementPosNode][elementNegNode] = subtractCmplx(matrix[elementPosNode][elementPosNode], elementVal);
				matrix[elementNegNode][elementPosNode] = subtractCmplx(matrix[elementPosNode][elementPosNode], elementVal);
				matrix[elementNegNode][elementNegNode] = addCmplx(matrix[elementPosNode][elementPosNode], elementVal);
				break;

            case 'I':
			case 'i':
				matrix[elementPosNode][nodes + tot_VS] = addCmplx(matrix[elementPosNode][nodes], elementVal);
				matrix[elementNegNode][nodes + tot_VS] = subtractCmplx(matrix[elementPosNode][nodes], elementVal);
				break;

			case 'V':
            case 'v':
                matrix[nodes + vs][elementPosNode] = POS_REAL_ONE;
                matrix[nodes + vs][elementNegNode] = NEG_REAL_ONE;
                matrix[nodes + vs][nodes + tot_VS] = elementVal;
                matrix[elementPosNode][nodes + vs] = NEG_REAL_ONE;
                matrix[elementNegNode][nodes + vs] = POS_REAL_ONE;
                vs++;
                break;

            case 'd':
            case 'D':
				if (elementType[1] == 'v' || elementType[1] == 'V')
				{
					matrix[nodes + vs][elementPosNode] = POS_REAL_ONE;
					matrix[nodes + vs][elementNegNode] = NEG_REAL_ONE;
					matrix[nodes + vs][depPosNode] = multiCmplx(NEG_REAL_ONE, elementVal);
					matrix[nodes + vs][depNegNode] = elementVal;
					matrix[elementPosNode][nodes + vs] = NEG_REAL_ONE;
					matrix[elementNegNode][nodes + vs] = POS_REAL_ONE;
					vs++;
				}
				else if (elementType[1] == 'i' || elementType[1] == 'I')
				{
                    matrix[elementPosNode][depPosNode]=subtractCmplx(matrix[elementPosNode][depPosNode],elementVal);
                    matrix[elementPosNode][depNegNode]=addCmplx(matrix[elementPosNode][depNegNode],elementVal);
                    matrix[elementNegNode][depPosNode]=addCmplx(matrix[elementPosNode][depPosNode],elementVal);
                    matrix[elementNegNode][depNegNode]=subtractCmplx(matrix[elementPosNode][depNegNode],elementVal);

				}
                break;
		}
	  }


	//Set last row (equation) [V4 = 0]
	for (unsigned int j = 0; j < nodes + tot_VS + 1; j++)
		matrix[nodes - 1][j] = addCmplx(POS_REAL_ONE,NEG_REAL_ONE);
	matrix[nodes - 1][nodes - 1] = POS_REAL_ONE;

	//Get solutions
	cmplx* solutions = SolveSystem(matrix, nodes + tot_VS);
    unsigned int c = 0;//counter to determine which voltage current is printed

	//Print solutions
	for (unsigned int i = 0; i < nodes; i++)
		cout << "V(" << i + 1 << ") = " << solutions[i].real<<" " <<solutions[i].imag<<"j"<< endl;      //here
	for(unsigned int i=nodes; i < (nodes + tot_VS);i++)
    {
        cout << "Iv(" << c + 1 << ") = "  << solutions[i].real <<" " <<solutions[i].imag<<"j"<< endl;    //here
        c++;
    }


	//Delete solutions
	delete[] solutions;

	//Delete matrix
	for (unsigned int i = 0; i < nodes + tot_VS; i++)
		delete[] matrix[i];
	delete[] matrix;
	return 0;
}

cmplx* SolveSystem(cmplx** matrix, unsigned int size)
{
	cmplx tempFactor;
	unsigned int i, j, k;

	for (i = 0; i < size; i++)
	{
		for (j = i + 1; j < size; j++)
		{
			if (Magnitude(matrix[i][i]) < Magnitude(matrix[j][i]))   // here
			{
				for (k = 0; k <= size; k++)
				{
					tempFactor = matrix[i][k];
					matrix[i][k] = matrix[j][k];
					matrix[j][k] = tempFactor;
				}
			}
		}
	}

	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			if (j != i)
			{
				tempFactor = divisionCmplx( matrix[j][i] , matrix[i][i] );
				for (k = i; k < size + 1; k++)
					matrix[j][k] = subtractCmplx( matrix[j][k] , multiCmplx( tempFactor , matrix[i][k] ) );
			}
		}
	}

	cmplx* solutions = new cmplx[size];
	for (i = 0; i < size; i++)
		solutions[i] = divisionCmplx( matrix[i][size] , matrix[i][i] );
	return solutions;
}
