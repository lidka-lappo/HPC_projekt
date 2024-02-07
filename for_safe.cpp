#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
#include <omp.h>
#include "timing.h"


using namespace std;
//H na A zamienione
//string names[20] = {"empty", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P"};
string names[100] = {"empty",
           "A", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", 
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

struct Index {
    int i;
    int j;
};

struct ProductionData {
    std::string isotope;
    std::vector<std::string> isotopes;
    std::vector<double> quantities;
};

struct route {
    std::vector<std::string> isotopes;
    std::string parent_isotope;
    std::vector<long double> frequency;
    long double final_frequency;
    bool finish= false;
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
    b_t(); // start timing
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
    double read_t = e_t(); // stop timing
    printf("# READ TIME: %f sec\n", read_t);
}


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

ProductionData get_vektor(string name, std::vector<ProductionData>& productionData) {
    ProductionData tmp;
    tmp.isotope = "null";
    string tmp_name = name;
    for (const auto& data : productionData) {
        if(data.isotope == tmp_name) {
            //cout << data.isotope << endl;
            return data;
        }
    }
    return tmp;
}


Index find_the_parent(string parent_name, vector<route> &MainQueue_second)
{
    Index index;
    index.i = 0;
    index.j = 0;
    //cout<<"szukam"<<parent_name<<endl;
    for(int i = 0; i< MainQueue_second.size(); i++)
    {
        for(int j = 0; j< MainQueue_second[i].isotopes.size(); j++)
        {
            if(MainQueue_second[i].isotopes[j] == parent_name)
            {
                index.i =i;
                index.j =j;
            }
        }
    }
    return index;
}



int main() {

    //READING DATA FROM FILE
    vector<route> routes;
    route MainQueue;
    vector<route> MainQueue_second;
    std::vector<ProductionData> productionData;
    open_data(&productionData);
    int Z = 21; //1 //19  //21
    int A = 48; //1 //38 //48

    route tmp_route;
    tmp_route.isotopes.push_back(names[Z] + to_string(A));
    tmp_route.frequency.push_back(1);
    tmp_route.finish = true;

    MainQueue.isotopes.push_back(names[Z] + to_string(A));
    MainQueue.frequency.push_back(1);
    
    MainQueue_second.push_back(tmp_route);

    int i = 0;
    string tmp_current_name;            
    long double  tmp_current_freq; 



 //////////////////////////////////////////////////////////////               
//  MAKING WEKTORS
//////////////////////////////////////////////////////////////
   b_t(); // start timing
    ProductionData current = get_vektor(Z, A, productionData);
    long double best_frequency = current.quantities[current.quantities.size()-1]/(1000000.0); 
    do
    {
        //cout<<current.isotope<<endl;
        if(current.isotope!="null")
        {
            //cout<<"Calculating: "<<MainQueue.isotopes.size()<<endl;
            route full_route;
                        
            for(int j =0; j<current.isotopes.size()-1; j++)
            {
                //cout<<current.isotopes[j];
                if(best_frequency<=current.quantities[j]/(1000000.0))
                {
                    MainQueue.isotopes.push_back(current.isotopes[j]);
                    MainQueue.frequency.push_back(current.quantities[j]/(1000000.0));
                    full_route.isotopes.push_back(current.isotopes[j]);
                    full_route.frequency.push_back(current.quantities[j]/(1000000.0));
                    full_route.parent_isotope = current.isotope;
                    //std::cout << "Isotope: " << current.isotopes[j] << " Quantity: " << current.quantities[j] << endl;
                }
            }
            if(!full_route.isotopes.empty())
            {
                MainQueue_second.push_back(full_route);
            }
            if(best_frequency<=current.quantities[current.isotopes.size()-1]/(1000000.0))
            {
                route tmp_route;
                tmp_route.isotopes.push_back(current.isotopes[current.isotopes.size()-1]);
                tmp_route.frequency.push_back(current.quantities[current.isotopes.size()-1]/(1000000.0));
                tmp_route.finish = false;
                tmp_route.parent_isotope = current.isotope;
                routes.push_back(tmp_route);     
            }          
        }
       
        tmp_current_name = MainQueue.isotopes[MainQueue.isotopes.size()-1];
        tmp_current_freq = MainQueue.frequency[MainQueue.isotopes.size()-1];
        current = get_vektor(tmp_current_name, productionData);

        MainQueue.isotopes.pop_back();
        MainQueue.frequency.pop_back();
        i++;
        
    }
    while(!MainQueue.isotopes.empty());

    double t_wektors = e_t(); // stop timing
    printf("# MAKING WEKTORS TIME: %f sec\n", t_wektors);

 /////////////////////////////////////////////////////////////////   
// GETTING ROUTES
//////////////////////////////////////////////////////////////////

    b_t();

    for(int i = 0;  i <routes.size(); i++)
    { 
        
        route current;
        current=routes[i];
        while(!current.finish)
        {
            Index parent_index = find_the_parent(current.parent_isotope, MainQueue_second);
            //cout<< parent_index.i<<parent_index.j<<endl;
            routes[i].isotopes.push_back(MainQueue_second[parent_index.i].isotopes[parent_index.j]);
            routes[i].frequency.push_back(MainQueue_second[parent_index.i].frequency[parent_index.j]);
            route tmp_current;
            tmp_current.isotopes.push_back(MainQueue_second[parent_index.i].isotopes[parent_index.j]);
            tmp_current.finish = MainQueue_second[parent_index.i].finish;
            tmp_current.parent_isotope = MainQueue_second[parent_index.i].parent_isotope;
            current = tmp_current;

        }
        routes[i].finish=true;

    }

    double t_routes = e_t(); // stop timing
    printf("# MAKING ROUTES TIME: %f sec\n", t_routes);
  
    for (const auto& route : routes) {
        std::cout << "Route Isotopes and Quantities: " << std::endl;
        for (size_t i = 0; i < route.isotopes.size(); ++i) {
            std::cout<<route.isotopes[i] << ", ";
           // std::cout << "Isotope: " << route.isotopes[i] << " Frequency: " << route.frequency[i]<< std::endl;
        }
        std::cout << "Finish? "<< route.finish << std::endl;
        std::cout << std::endl;

    }



    return 0;
}
