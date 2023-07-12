
int windows_size = 60;
int buffer;
bool odd;

void start_print(TString s){
    if( s.Length() % 2 ) odd = true;
    else odd = false;

    if(odd) s = " "+s + " ... Start! ";
    else s = " "+s + " ... Start!! ";

    buffer = ( windows_size-s.Length() )/2;
    cout<<endl;
	cout<<std::string(windows_size, '*')<<endl;
	cout<<std::string(buffer, '*')<<s<<std::string(buffer, '*')<<endl;
}

void line_print(){
    cout<<std::string(windows_size, '*')<<endl;
}

int barlength;
int reserved = 15;
void print_progress_bar(double percentage, TString name) {
    barlength = windows_size - (reserved+5); 
    double progress = percentage * barlength;
    std::string progress_string = "[" + std::string(floor(progress), '#') + std::string(barlength - floor(progress), ' ') + "]";
    
    std::cout << std::setw(reserved) << std::left << name.Data() << " : " << progress_string << "\r";
}


void running_print(TString s){
    if( s.Length() % 2 ) odd = true;
    else odd = false;

    if(odd) s = " "+ s;
    else s = " "+s + " ";

    buffer = ( windows_size-s.Length() )/2;
	cout<<"*"<<s<<std::string(2*buffer-2, ' ')<<"*"<<endl;
}

void finish_print(TString s){
    if( s.Length() % 2 ) odd = true;
    else odd = false;

    if(odd) s = " "+s + " ... Done!! ";
    else s = " "+s + " ... Done!!! ";

    buffer = ( windows_size-s.Length() )/2;
	cout<<std::string(buffer, '*')<<s<<std::string(buffer, '*')<<endl;
	cout<<std::string(windows_size, '*')<<endl;
}