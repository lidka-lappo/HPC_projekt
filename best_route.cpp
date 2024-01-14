#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>


using namespace std;

string names[100] = {"empty",
           "H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", 
           "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", 
           "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn",
           "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", 
           "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", 
           "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", 
           "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", 
           "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", 
           "Tl", "Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra", "Ac", "Th", 
           "Pa", "U", "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es"};
int lightisotop[100] = {0, 1, 3, 4, 6, 7, 8, 10, 11, 14, 15, 17, 19, 21, 22, 25, 26, 29, 30, 33, 34, 38, 38, 42, 42, 44, 45, 50, 48, 53, 54, 59, 59, 63, 64, 69, 69, 73, 73, 76, 77, 81, 81, 85, 85, 89, 90, 92, 94, 96, 99, 103, 104, 108, 108, 112, 114, 117, 121, 121, 125, 128, 129, 130, 134, 135, 139, 140, 143, 144, 149, 150, 151, 155, 157, 159, 161, 164, 166, 170, 171, 176, 178, 184, 186, 191, 193, 197, 201, 205, 208, 211, 215, 219, 228, 223, 233, 233, 237, 240};
int heavyisotop[100] = {0, 7, 10, 12, 16, 21, 22, 24, 26, 31, 34, 37, 40, 43, 44, 46, 48, 51, 53, 56, 58, 61, 64, 67, 70, 73, 75, 77, 82, 82, 85, 87, 90, 92, 95, 101, 102, 106, 108, 111, 114, 117, 119, 122, 125, 128, 131, 132, 134, 137, 140, 142, 145, 147, 150, 152, 154, 157, 158, 161, 163, 165, 168, 169, 172, 174, 176, 178, 180, 181, 185, 188, 190, 194, 197, 199, 203, 205, 208, 210, 216, 217, 220, 224, 227, 229, 229, 233, 234, 236, 238, 239, 243, 252, 252, 252, 252, 253, 256, 257};


struct ProductionData {
    std::string isotope;
    std::vector<std::string> isotopes;
    std::vector<double> quantities;
};

struct route {
    std::vector<std::string> isotopes;
    std::vector<double> quantities;
};

bool isQuantity(const std::string& token) {
    // Using a try-catch block to convert the token to double
    try {
        // If it can be successfully converted to double, consider it a quantity
        std::stod(token);
        return true;
    } catch (const std::invalid_argument& e) {
        // If conversion fails, it's not a quantity
        return false;
    }
}

void open_data(vector<ProductionData> *productionData)
{
    std::ifstream file("way_of_production.txt");
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        ProductionData data;
        std::string token;

        // Read the first token (isotope)
        if (std::getline(iss, token, ';')) {

            data.isotope = token;
            
        }
        
        // Read the rest of the tokens (isotopes or quantities)
        while (std::getline(iss, token, ';')) 
        {
                    if (!token.empty()) 
                    {
                        if (isQuantity(token)) 
                        {
                                double tmp = stod(token);
                                data.quantities.push_back(tmp);
                                
                           
                        } 
                        else 
                        {
                            data.isotopes.push_back(token);
                        }
                    }
                
        }
        // Read the second line
        getline(file, line);
        istringstream iss1(line);
        // Read the first token (isotope)
        if (std::getline(iss1, token, ';')) {}
        while (std::getline(iss1, token, ';')) 
        {
                    if (!token.empty()) 
                    {
                        if (isQuantity(token)) 
                        {
                                double tmp = stod(token);
                                data.quantities.push_back(tmp);
                                
                           
                        } 
                        else 
                        {
                            data.isotopes.push_back(token);
                        }
                    }
                
        }
        
        productionData->push_back(data);

  
    }

}

/*ProductionData get_vektor(int Z, int A, vector<ProductionData> *productionData )
{
    ProductionData tmp;
      for (const auto& data : productionData) {
        string tmp_name = names[Z] + to_string(A);
        if(data.isotope.compare(tmp_name))
        {
            cout<<data.isotope<<endl;
            return data;
        }
      }
      return tmp;


}*/

ProductionData get_vektor(int Z, int A, std::vector<ProductionData>& productionData) {
    ProductionData tmp;
    tmp.isotope = "null";
    string tmp_name = names[Z] + to_string(A);
    for (const auto& data : productionData) {
        if(data.isotope == tmp_name) {
            //cout << data.isotope << endl;
            return data;
        }
    }
    return tmp;
}


int main() {

    //READING DATA FROM FILE
    vector<route> routes;
    std::vector<ProductionData> productionData;
    open_data(&productionData);
    int Z = 2;
    int A = 7;
    ProductionData current = get_vektor(Z, A, productionData);




    // Output the read data (just for demonstration)
   /*for (const auto& data : productionData) {
        std::cout << "Isotope: " << data.isotope << std::endl;
        // std::cout << "Isotopes: " << data.isotopes.size() << std::endl;
        //std::cout << "Quantities: " << data.quantities.size() << std::endl;
        for (size_t i = 0; i < data.isotopes.size(); ++i) {
               std::cout << "Isotope: " << data.isotopes[i] << " Quantity: " << data.quantities[i] << endl;
        }
        std::cout << std::endl;
    }*/

    return 0;
}
