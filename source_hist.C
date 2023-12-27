//3d histogram NZE

typedef std::basic_string < char, std::char_traits < char >, std::allocator < char >> MojString;


TString names[100] = {"empty",
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

Int_t lightisotop[100] = {0, 1, 3, 4, 6, 7, 8, 10, 11, 14, 15, 17, 19, 21, 22, 25, 26, 29, 30, 33, 34, 38, 38, 42, 42, 44, 45, 50, 48, 53, 54, 59, 59, 63, 64, 69, 69, 73, 73, 76, 77, 81, 81, 85, 85, 89, 90, 92, 94, 96, 99, 103, 104, 108, 108, 112, 114, 117, 121, 121, 125, 128, 129, 130, 134, 135, 139, 140, 143, 144, 149, 150, 151, 155, 157, 159, 161, 164, 166, 170, 171, 176, 178, 184, 186, 191, 193, 197, 201, 205, 208, 211, 215, 219, 228, 223, 233, 233, 237, 240};
Int_t heavyisotop[100] = {0, 7, 10, 12, 16, 21, 22, 24, 26, 31, 34, 37, 40, 43, 44, 46, 48, 51, 53, 56, 58, 61, 64, 67, 70, 73, 75, 77, 82, 82, 85, 87, 90, 92, 95, 101, 102, 106, 108, 111, 114, 117, 119, 122, 125, 128, 131, 132, 134, 137, 140, 142, 145, 147, 150, 152, 154, 157, 158, 161, 163, 165, 168, 169, 172, 174, 176, 178, 180, 181, 185, 188, 190, 194, 197, 199, 203, 205, 208, 210, 216, 217, 220, 224, 227, 229, 229, 233, 234, 236, 238, 239, 243, 252, 252, 252, 252, 253, 256, 257};



void source_hist()
{

   for(int ZS=1; ZS<100; ZS++)
   {
    for(int AS=lightisotop[ZS]; AS<=heavyisotop[ZS]; AS++)
      {
      //int AS = 18;
      //int ZS = 10;
         stringstream tmpname;
         tmpname <<"../ALL_T0_ATM"<<AS<<names[ZS]<<".root";
         string tmp_name = tmpname.str();
         const char *name =(char*) tmp_name.c_str();  
         TFile *file;
         cout<<"Reading File : "<<name<<endl;
         cout<<endl;
         file = TFile::Open(name);

         if (!file || file->IsZombie()) { delete file; cout <<"There is no such isotop"<<endl; } //precaution
         else {
            
               TTree *t = (TTree*)file->Get("fNtuple");
               //creating histogram
               TH2* ZN = new TH2D("Z:N", "Z:N", 150, 0, 150, 150, 0, 150);
               Double_t ID, Z,A, N, E, Q, T;
               Char_t name[10];
               Char_t type[10];
               t->SetBranchAddress("ID",&ID);
               t->SetBranchAddress("Z",&Z);
               t->SetBranchAddress("A",&A);
               t->SetBranchAddress("N",&N);
               t->SetBranchAddress("E",&E);
               t->SetBranchAddress("Q",&Q);
               t->SetBranchAddress("T",&T);
               t->SetBranchAddress("name",&name);
               t->SetBranchAddress("type",&type);

               Int_t n = (Int_t)t->GetEntries();

               for (Int_t i=0; i<n; i++) 
               {
                  t->GetEntry(i);
                  if(A>0 && Z> 0 && N>0)
                  ZN->Fill((A-Z), Z);
               }

               stringstream tmpname1;
               tmpname1 <<"target"<<names[ZS]<<AS<<".root";

               string tmp_name1 = tmpname1.str();
               const char *name1 =(char*) tmp_name1.c_str();

               TFile *file2 = new TFile(name1, "RECREATE");
               file2->cd();
               ZN->Write();
               file2->Write();

               delete ZN;
               delete t;
               delete file2;
               delete file;
            }



      }
   }

}