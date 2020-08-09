#include "default.h"
#include "main_simulation.h"

#include "main_simulation.h"

#include "particle.h"
#include "model.h"

/*
f1.91876 p1:Particle(79) r:{-42.9027 -58.3563  67.1153} v:{ -0.394935  -0.145312 -0.0945829}  R: 0.01 m: 1
  p2:Particle(17) r:{-0.627921  -93.3465   56.3916}     v:{0.0276567 -0.493832 -0.235447}  R: 0.01 m: 1

  */
bool test_collision(){
    Model model(Box(1,1,1));
    model._button=true;
    Particle p1(model.box, {-42.9027, -58.3563,  67.1153}, { -0.394935,  -0.145312, -0.0945829} , 0.01, 1);
    Particle p2(model.box, {-0.627921,  -93.3465 ,  56.3916},{0.0276567, -0.493832, -0.235447} , 0.01, 1);

    model.predict_collide_of(p1, p2);

    return 1;
}

bool test_collision2(){
    Model model(Box(2,2,2));
    model._button=true;
    model.load_inits(PROJECT_DIR+"\\python\\model_inits.bin");
    model.load(PROJECT_DIR+"\\ERROR-FILE1.bin");
//    cout<<model.particles[79]<<endl;
//    cout<<model.particles[17]<<endl;

//    cout<<model.box.sides.transpose()<<"  "<<model.box.time<<"  "<<model.time<<endl;

//    Particle p1(model.box, {-110.445, -77.3062,  80.6622},{0.454409, 0.856459,   1.4998} , 0.01, 1);
//    Particle p2(model.box, {-15.3374,  6.04934,  65.1134}, {-0.421084,  0.918819,  0.252611}  , 0.01, 1);

    model.predict_collide_of(model.particles[79], model.particles[17]);

    return 1;
}

int main(){
//    test_collision2();
//    time_interval_simulate();
    for(int i=0;i<1;i++){
        run_simulation(100);
    }
    return 0;
}