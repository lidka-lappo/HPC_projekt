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

////////////////////////////////////////
	/*215At,216At,217At 85
	216Rn, 217Rn 218Rn 86
	217Fr,218Fr,219Fr 87
	217Ra, 218Ra, 219Ra 220Ra 225Ra 88
	221Pa, 222Pa, 223Pa 229Pa 91
	222U, 223U, 224U  225U 92
	50Co,51Co or 52Co 27
	229Th 90*/

	int ZS =30;//11;//8;
	int AS=69;//32;//20; 

////////////////////////////////////////

int check(int Z, int A)
{
    std::vector<TString> isotops;
	fstream myfile1;
	//myfile.open("isotopes.txt", ios::in);
	myfile1.open("../stable_longlivedisotopes.txt", ios::in);
	TString element;
	int is_stable =0;
	while(!myfile1.eof())
	{
		//TString isotop;
		TString tmp;
		myfile1>>tmp;
		
		if(!strcmp(tmp, names[Z]))
		{
			myfile1>>tmp;
			while((tmp.IsFloat()))
			{
				////cout <<tmp <<endl;
				if(atoi(tmp) == A )
					{ is_stable =1;}
				myfile1>>tmp;
			}
		}


	}
	myfile1.close();	
	return is_stable;
}

void printlist( vector<vector<Int_t>> &listZ, vector<vector<Int_t>> &listA, vector<vector<Double_t>> &listFreq)
{
	int s =listZ.size();
	for(int i =0; i<s; i++)
	{	
		cout<<i<<") ";
		int s1 =listZ.at(i).size();
		for(int j =0; j<s1; j++)
		{
			cout<<names[listZ.at(i).at(j)]<<listA.at(i).at(j)<<", ";
			//cout<<listA.at(i).at(j)<<", ";

		}
		cout<<endl;

	}

}



int to_stable(	int ZZ, int AA, Double_t f_product, Double_t f_direct, vector<vector<Int_t>> &listZ, vector<vector<Int_t>> &listA, vector<vector<Double_t>> &listFreq)
{
	if(!check(ZZ, AA))
	{
		std::vector<Int_t> tmplistZ;
		std::vector<Int_t> tmplistA;
		std::vector<Double_t> tmplistFreq;
	    	stringstream tmpname;
		tmpname <<"rooty/ZNproduct"<<names[ZZ]<<AA<<".root";
		string tmp_name = tmpname.str();
		const char *name =(char*) tmp_name.c_str();  
		TFile *file;
		file = TFile::Open(name);
		if (!file || file->IsZombie()) { delete file; 
			//cout <<"There is no such isotop "<<names[ZZ]<<AA<<endl;
	   		return 0; } //precaution
		else {
			//cout<<"This isotop "<<names[ZZ]<<AA<<" is producet from isotop: "<<endl;
			TH2D* hist;
			hist = (TH2D*)file->Get("N:Z")->Clone();;
		    	Int_t MaxBin;
	   		Int_t N,Z,x;
	   		Double_t f;
	   		MaxBin = hist->GetMaximumBin();
	   		hist->GetBinXYZ(MaxBin, Z, N, x);
	   		f = hist->GetBinContent(MaxBin);
	   		f = f/1000000.0*f_product;
	   		////cout<<f<<endl;
	   		
	   		////cout<<i<<") "<<names[Z]<<(N+Z)<<" with frequency "<<f<<endl;
	   		if(f>=f_direct)
	   		{
		   		tmplistZ.push_back(Z);
		   		tmplistA.push_back(Z+N);
		   		tmplistFreq.push_back(f);
			}
			hist->SetBinContent( Z,N,0);
	   		while(!check(Z,(Z+N)))
	   		{
		   		MaxBin = hist->GetMaximumBin();
		   		hist->GetBinXYZ(MaxBin, Z, N, x);
		   		f = hist->GetBinContent(MaxBin);
		   		if(f==0)
		   		{
		   			//cout<<"No production from stable isotop"<<endl;
		   			break;
		   		}
		   		f = f/1000000.0*f_product;
		   		//cout<<i<<") "<<names[Z]<<(N+Z)<<" with frequency "<<f<<endl;
				if(f>=f_direct)
	   			{
					tmplistZ.push_back(Z);
					tmplistA.push_back(Z+N);
					tmplistFreq.push_back(f);
	   			}
	   			else
	   			{

	   				break;
	   			}
	   			hist->SetBinContent( Z,N,0);
	   		}

	   		
		    delete hist;    
		    delete file;		    
	   		if(!tmplistZ.empty())
	   		{
	   			//cout<<"Dodajemy liste" <<endl;
		    		listZ.push_back(tmplistZ);
	   			listA.push_back(tmplistA);
	   			listFreq.push_back(tmplistFreq);
	   		}
	   		else
	   		{
	   			//cout<<"Nie dodajemy listy" <<endl;
	   			return 0;
	   		}
		    return 1;
		 }
	}
	else
	{
	//	cout<<"Isotop "<<names[ZZ]<<AA<<" is stable"<<endl;
		return -1;
	}


}

int routes_check(int j, Double_t f_direct, vector<vector<Int_t>> &is_effective, vector<vector<Int_t>> &listZ, vector<vector<Int_t>> &listA, vector<vector<Double_t>> &listFreq)
{
	int s =listZ.at(j).size();
	int number_of_routes =s;
	vector<Int_t> tmp_is_effective;
	int iter_del =0;
	for(int i =0; i<s; i++)
	{
//		cout<<"performing <<to stable>> on "<<names[listZ.at(j).at(i)]<< listA.at(j).at(i)<<endl;
		int stable = to_stable(listZ.at(j).at(i), listA.at(j).at(i), listFreq.at(j).at(i), f_direct, listZ, listA, listFreq);
					
		if(stable==1)
		{

			/*Double_t f=listFreq.at(j).at(i)*listFreq.at(i).at(0);	

			if(f<f_direct)
			{
				//tmp_is_effective.push_back(1);
				//number_of_routes++;
			}
			else 
			{
				//tmp_is_effective.push_back(0);
				//number_of_routes++;
			}*/
			tmp_is_effective.push_back(stable);
		}
		else if(stable ==0) //nieistnie
		{
			//cout<<"Usuwanie niepotrzebnosci "<<names[listZ.at(j).at(i)]<<listA.at(j).at(i)<<endl;
			//tmp_is_effective.push_back(0);
			listZ.at(j).erase(listZ.at(j).begin()+i, listZ.at(j).begin()+i+1);
			listA.at(j).erase(listA.at(j).begin()+i, listA.at(j).begin()+i+1);
			listFreq.at(j).erase(listFreq.at(j).begin()+i, listFreq.at(j).begin()+i+1);
			number_of_routes--;
			i--;
			s--;
		}
		else //stabilny
		{
			//tmp_is_effective.push_back(0);
			tmp_is_effective.push_back(stable);
			number_of_routes--;
		}

	} 

	is_effective.push_back(tmp_is_effective);
	return number_of_routes;
}

void all_routes()
{
	cout<<"tworze plik"<<endl;		
	stringstream tmpname1;
	tmpname1<<"All_chains_of_production.txt";
	string tmp_name1 = tmpname1.str();
	const char *name1 =(char*) tmp_name1.c_str(); 

	fstream myfile;
	myfile.open(name1, ios::out);
	
	for(int ZZ =1; ZZ<100; ZZ++)
	{for(int AA =lightisotop[ZZ]; AA<=heavyisotop[ZZ]; AA++)
 	{
	cout<<names[ZZ]<<AA<<endl;
	std::vector<vector<Int_t>> listZ;
	std::vector<vector<Int_t>> listA;
	std::vector<vector<Double_t>> listFreq;

	std::vector<Int_t> tmplistZ;
	std::vector<Int_t> tmplistA;
	std::vector<Double_t> tmplistFreq;


	Double_t f =1;
	//int ZZ =ZS; 
	//int AA=AS; 

	//setting first element;

	tmplistZ.push_back(ZZ);
	tmplistA.push_back(AA);
	tmplistFreq.push_back(1.0);

	listZ.push_back(tmplistZ);
	listA.push_back(tmplistA);
	listFreq.push_back(tmplistFreq);


	std::vector<vector<Int_t>> is_effective;
	Double_t f_direct=0;

	int number_of_routes = routes_check(0, f_direct, is_effective, listZ, listA, listFreq);
	if(number_of_routes ==0)
	{
	continue;
	}
	int s =listFreq.at(1).size()-1;
	f_direct=listFreq.at(1).at(s);


	
	int j=1;	
	while(j<listZ.size())
	{
		number_of_routes = routes_check(j, f_direct, is_effective, listZ, listA, listFreq);
		j++;

	}

	
	j=0;	
	while(j<listZ.size())
	{
		if(listZ.at(j).empty())
		{
			listZ.erase(listZ.begin()+j, listZ.begin()+j+1);
			listA.erase(listA.begin()+j, listA.begin()+j+1);
			listFreq.erase(listFreq.begin()+j, listFreq.begin()+j+1);	
		}
		else
		{
			j++;
		}
	}


	
	std::vector<Int_t> father;
	std::vector<Int_t> mother;
	
	int k =1;
	int i = 0;
	int tmp =1;
	father.push_back(0);
	mother.push_back(0);
//	cout<<" j: "<<0<<" i: "<<i<<" list_size "<<listZ.at(0).size()-1<<" k: "<<0<<" tmp: "<<tmp-1<<endl;
		
	for(int j =1; j<listZ.size(); j++)
	{
//		cout<<" j: "<<j<<" i: "<<i<<" list_size "<<listZ.at(i).size()-1<<" k: "<<k<<" tmp: "<<tmp-1<<endl;
		
		father.push_back(k);
		mother.push_back(tmp-1);
		if(tmp<(listZ.at(i).size()-1))
		{
			k++;
			tmp++;
		}
		else
		{
			
			i++;
			tmp=1;
		}



	}
	
	s =listZ.at(1).size()-1;
	//myfile<<" <- "<<names[listZ.at(1).at(s)]<<listA.at(1).at(s)<<" with frequency "<<f_direct<<endl;


	
	int tmpZ =0;
	int tmpA =0;

	//printlist( listZ, listA, listFreq);
	//cout<<endl;
	//cout<<endl;
	//printlist( is_effective, is_effective, listFreq);
	//cout<<endl;
	//cout<<endl;
	
	int best_route = 1;
	Double_t max_freq = f_direct;
	int n_vectors = listZ.size()-1;
	for(int lev =n_vectors; lev >0; lev --)
	{


		int pos = listZ.at(lev).size()-1;
		if(listFreq.at(lev).at(pos)>max_freq)
		{
			best_route=lev;
			max_freq=listFreq.at(lev).at(pos);
		}
		//myfile<<"with frequency "<<listFreq.at(lev).at(pos)<<endl;
		myfile<<names[listZ.at(lev).at(pos)]<<listA.at(lev).at(pos)<<"; ";
		
		
		int level=lev-father.at(lev);
		pos =mother.at(lev);
		while(level>0)
		{
			myfile<<names[listZ.at(level).at(pos)]<<listA.at(level).at(pos)<<"; ";
			
			//cout<<level<<":"<<pos<<" --> ";
			//is_effective.at(level).at(pos)=0;
			pos =mother.at(level);
			level=level-father.at(level);
			
		}
		myfile<<names[listZ.at(0).at(0)]<<listA.at(0).at(0)<<endl;

		

	}
	
	listZ.clear();
	listA.clear();
	listFreq.clear();

	tmplistZ.clear();
	tmplistA.clear();
	tmplistFreq.clear();

	father.clear();
	mother.clear();
	
	}//end of ZZ and AA loops
	}

	myfile.close();

}
