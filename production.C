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

int check(int Z, int A)
{
    std::vector<TString> isotops;
	fstream myfile;
	//myfile.open("isotopes.txt", ios::in);
	myfile.open("../stable_longlivedisotopes.txt", ios::in);
	TString element;
	int is_stable =0;
	while(!myfile.eof())
	{
		//TString isotop;
		TString tmp;
		myfile>>tmp;
		
		if(!strcmp(tmp, names[Z]))
		{
			myfile>>tmp;
			while((tmp.IsFloat()))
			{
				//cout <<tmp <<endl;
				if(atoi(tmp) == A )
					{ is_stable =1;}
				myfile>>tmp;
			}
		}


	}
	myfile.close();	
	return is_stable;
}




void to_stable(	int ZZ, int AA, vector<Int_t> &listZ, vector<Int_t> &listA, vector<Double_t> &listFreq)
{

    stringstream tmpname;
	tmpname <<"ZNproduct"<<names[ZZ]<<AA<<".root";
	string tmp_name = tmpname.str();
	const char *name =(char*) tmp_name.c_str();  
	TFile *file;
	file = TFile::Open(name);
	if (!file || file->IsZombie()) { delete file; 
		cout <<"There is no such isotop "<<names[ZZ]<<AA<<endl;} //precaution
	else {
		cout<<"This isotop "<<names[ZZ]<<AA<<" is producet from isotop: "<<endl;
	    TH2D* hist;
	    hist = (TH2D*)file->Get("N:Z")->Clone();;
	    Int_t MaxBin;
   		Int_t N,Z,x;
   		Double_t f;
   		MaxBin = hist->GetMaximumBin();
   		hist->GetBinXYZ(MaxBin, Z, N, x);
   		f = hist->GetBinContent(MaxBin);

   		
   		//cout<<i<<") "<<names[Z]<<(N+Z)<<" with frequency "<<f<<endl;
   		listZ.push_back(Z);
   		listA.push_back(Z+N);
   		listFreq.push_back(f);
		hist->SetBinContent( Z,N,0);
   		while(!check(Z,(Z+N)))
   		{
	   		MaxBin = hist->GetMaximumBin();
	   		hist->GetBinXYZ(MaxBin, Z, N, x);
	   		f = hist->GetBinContent(MaxBin);
	   		if(f==0)
	   		{
	   			cout<<"No production from stable isotop"<<endl;
	   			break;
	   		}
	   		//cout<<i<<") "<<names[Z]<<(N+Z)<<" with frequency "<<f<<endl;
			listZ.push_back(Z);
			listA.push_back(Z+N);
			listFreq.push_back(f);
   			hist->SetBinContent( Z,N,0);
   		}

   		//cout<<names[Z]<<(N+Z)<<" is stable isotop" <<endl;
	    delete hist;    
	    delete file;
	    

	 }

}

void production()
{

    fstream myfile;
	myfile.open("way_of_production.txt", ios::out);
	for(int ZZ =1; ZZ<100; ZZ++)
	{
		for(int AA=lightisotop[ZZ]; AA<heavyisotop[ZZ]+1; AA++)
		{
			std::vector<Int_t> listZ;
    		std::vector<Int_t> listA;
    		std::vector<Double_t> listFreq;
    		int full=0;
    		
			to_stable(ZZ, AA, listZ, listA, listFreq);
			int n=listZ.size();
			if(n!=0)
			{myfile<<names[ZZ]<<AA<<"; ; ";}
			for(int s = 0 ; s < n ; s++){
				int Z = listZ.at(s);
				int A = listA.at(s);
				myfile<<names[Z]<<A<<"; ";
			}
			if(n!=0)
			{
				myfile<<endl;
				myfile<<names[ZZ]<<AA<<"; ; ";
			}
			for(int s = 0 ; s < n ; s++){
				double f = listFreq.at(s);
				myfile<<f<<"; ";
			}
			if(n!=0)
			{
				myfile<<endl;
			}
		}
	}
    myfile.close();
//to do later other paths and comparision
   /*b int tempZZ = listZ.at(0);
    int tempAA = listA.at(0);
    for(int i=0; i<3; i++)
    {
	    std::vector<Int_t> listZ1;
	    std::vector<Int_t> listA1;
	    std::vector<Double_t> listFreq1;
    	int j=0;
    	while(full==0 && j<n)
    	{
		   	full=to_stable(tempZZ, tempAA, listZ1, listA1, listFreq1);
		   	tempZZ = listZ.at(j);
	  		tempAA = listA.at(j);
	  		j++;
	  	}
    }
    cout<<listA.at(1);
*/
}