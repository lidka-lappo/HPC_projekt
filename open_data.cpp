#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>


using namespace std;

struct ProductionData {
    std::string isotope;
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

int main() {
    std::ifstream file("way_of_production.txt");
    std::string line;
    std::vector<ProductionData> productionData;

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
        
 productionData.push_back(data);

  
    }

    // Output the read data (just for demonstration)
   for (const auto& data : productionData) {
        std::cout << "Isotope: " << data.isotope << std::endl;
        // std::cout << "Isotopes: " << data.isotopes.size() << std::endl;
        //std::cout << "Quantities: " << data.quantities.size() << std::endl;
        for (size_t i = 0; i < data.isotopes.size(); ++i) {
               std::cout << "Isotope: " << data.isotopes[i] << " Quantity: " << data.quantities[i] << endl;
        }
        std::cout << std::endl;
    }

    return 0;
}
