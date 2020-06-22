//
// Created by Ashot on 5/23/2020.
//

#include "model.h"
#include <fstream>
void Model::load_inits(string path) {
    ifstream myfile (path,
            ios::in | ios::binary );

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
    myfile.close();

}
void Model::load(string path){
    auto myfile = std::fstream(
            path, std::ios::in | std::ios::binary);
    myfile.read((char *)&box.time, 8);

    int _n;
    myfile.read((char*)&_n, 4);
    assert(_n == N);

    for(auto& p: particles){
        myfile.read((char*)(p.r.data()), 3 * 8);

    }
    for(auto& p: particles) {
        myfile.read((char*) p.v.data(), 3 * 8);
    }
    myfile.close();
}

void Model::dump(string path, string name,
                 bool back_to_box, vector<int> particle_list){
    if (name == ""){
        char buff[200];
        snprintf(buff, sizeof(buff), "%0.3f - %d.bin",
                 time, collision_counter);
        name = buff;
    }
    auto myfile = std::fstream(
            path+"\\"+name,
            std::ios::out | std::ios::binary);
//    myfile.write((char*)box.sides.data(), 3*8);
    myfile.write((char*)&box.time, 8);


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
//    cout<<"AAAAAAA "<<_n<<endl;
    myfile.write((char*)&_n, 4);


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
        myfile.write(data, 3 * 8);
    }
    for(auto& p: *particles_iter) {
        myfile.write((char*) p.v.data(), 3 * 8);
    }
    myfile.close();
}

void Model::dump_inits(string path){
    char buff[200];
    snprintf(buff, sizeof(buff), "model_inits.bin");
    string name = buff;

    auto myfile = std::fstream(
            path+"\\"+name,
            std::ios::out | std::ios::binary);

    myfile.write((char*)box.sides.data(), 3*8);
    myfile.write((char*)&N, 4);

    for(auto& p: particles) {
        myfile.write((char*) &p.radius, 1 * 8);
    }
    for(auto& p: particles) {
        myfile.write((char*) &p.mass, 1 * 8);
    }
    myfile.close();
}