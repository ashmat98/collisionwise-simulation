//
// Created by Ashot on 5/23/2020.
//

#include "model.h"
#include <fstream>
void Model::dump(string path, string name){
    if (name == ""){
        char buff[200];
        snprintf(buff, sizeof(buff), "%0.3f - %d.bin",
                time, collision_counter);
        name = buff;
    }
    auto myfile = std::fstream(
            path+"\\"+name,
            std::ios::out | std::ios::binary);

    myfile.write((char*)box.sides.data(), 3*8);
    myfile.write((char*)&box.time, 8);
    myfile.write((char*)&N, 4);

    for(auto& p: particles){
        update_particle_position(p);
    }
    for(auto& p: particles){
        myfile.write((char*)p.r.data(), 3 * 8);
    }
    for(auto& p: particles) {
        myfile.write((char *) p.v.data(), 3 * 8);
    }

    myfile.close();
}