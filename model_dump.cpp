//
// Created by Ashot on 5/23/2020.
//

#include "model.h"
#include <fstream>
#include <ctime>
#include <limits>

string get_time_string(){
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,sizeof(buffer),"%d-%m-%Y %H-%M-%S",timeinfo);
    std::string str(buffer);
    return str;
}

void Model::create_dumpfile(string name) {
    if (name == "") {
        name = "dump " + get_time_string();
    }
    string path = PROJECT_DIR + SEP + "dumps" + SEP + name + ".bin";
    model_file.open(path, ios::out | ios::binary);
    //////////////
    dump_inits();

}

void Model::load(string path, bool inits_only) {
    // getting file size
    ifstream myfile;
    myfile.open(path, ios::in | ios::binary );
    myfile.ignore( std::numeric_limits<std::streamsize>::max() );
    std::streamsize length = myfile.gcount();
    cout<<length<<endl;
    myfile.close();


    myfile.open(path, ios::in | ios::binary );


    myfile.read((char*)box.sides.data(), 3*8);
    myfile.read((char*)&N, 4);
    cout<<N<<endl;
    particles.clear();

    for(int i=0;i<N;++i){
        particles.push_back(Particle(box, 1, 1, i));
    }
    for(auto& p: particles) {
        myfile.read((char*) &p.radius, 1 * 8);
    }
    for(auto& p: particles) {
        myfile.read((char*) &p.mass, 1 * 8);
    }
    if (!inits_only){
        int offset = length - (8+4+8*N*3+8*N*3);
        myfile.seekg(offset);

        myfile.read((char *)&box.time, 8);

        int _n;
        myfile.read((char*)&_n, 4);
        cout<<"_n "<<_n<<endl;
        assert(_n == N);

        for(auto& p: particles){
            myfile.read((char*)(p.r.data()), 3 * 8);

        }
        for(auto& p: particles) {
            myfile.read((char*) p.v.data(), 3 * 8);
        }
    }
    myfile.close();

}

void Model::dump(bool back_to_box, vector<int> particle_list){
    assert(model_file.is_open());

    model_file.write((char*)&box.time, 8);


    vector<Particle> _particles;
    vector<Particle>* particles_iter = &_particles;
    if (particle_list.size()==0){
        particles_iter = &particles;
    }
    else{
        for (int i: particle_list){
            _particles.push_back(particles[i]);
        }
    }

    int _n = (*particles_iter).size();
    model_file.write((char*)&_n, 4);


    for(auto& p: *particles_iter){
        update_particle_position(p);
    }
    for(auto& p: *particles_iter){
        char* data;
        if (back_to_box){
//            auto r;
            data = (char*)p.back_to_box().data();
        }
        else{
            data = (char*)p.r.data();
        }
        model_file.write(data, 3 * 8);
    }
    for(auto& p: *particles_iter) {
        model_file.write((char*) p.v.data(), 3 * 8);
    }
}

void Model::dump_inits(){
    model_file.write((char*)box.sides.data(), 3*8);
    model_file.write((char*)&N, 4);

    for(auto& p: particles) {
        model_file.write((char*) &p.radius, 1 * 8);
    }
    for(auto& p: particles) {
        model_file.write((char*) &p.mass, 1 * 8);
    }
}