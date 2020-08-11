/*                   VisSim www.Uni-Koblenz.de
 *
 * This example is simulation of a simple pendulum with drag force
 * BodyDragForce is built as a plugin
 *
 * By Ibraheem AL-Dhamari ibr.exg@gmail.com
 *
 * TODOs:
 * 1. create the model .osim file using cpp
 * 2. run the model using forwardtool
 *
*/
#include <OpenSim/OpenSim.h>
#include "OpenSim/Common/STOFileAdapter.h"
#include <time.h>
#include <BodyDragForce.h>

//#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "SimTKcommon/internal/Xml.h"

#define PI 3.14159265

using namespace OpenSim;
using namespace std;

using SimTK::Vec3;
using SimTK::Inertia;


SimTK::Xml::Element GlobalForceSetEl; //ForceSet
void findNode(SimTK::Xml::Element rootNd, std::string nodName) {
        SimTK::Xml::node_iterator pnI;
        SimTK::Xml::Element cEl;
        pnI = rootNd.element_begin();
        int j = 0;
        while (  pnI != rootNd.element_end() ) {
              cEl = pnI->getParentElement().getAllElements()[j];
              if (nodName == cEl.getNodeText()) {
                  GlobalForceSetEl = cEl;
                  //cout << cEl.getNodeText() << "  " << cEl.getRequiredAttribute("name").getValue() << endl;
                  break;
              }else
              findNode(cEl, nodName);
              pnI++; j++;
        }//while
}
void addForcePlugin(SimTK::Xml::Element forceNode, std::string modelPath) {
    try {
        SimTK::Xml::Document modelDoc(modelPath);
        SimTK::Xml::Element rootNd = modelDoc.getRootElement();
        cout << " model file is oppened: " <<modelPath<< endl;
        try {
            findNode(rootNd, "ForceSet");
            SimTK::Xml::Element ObjectsElem = GlobalForceSetEl.getAllElements()[0];
            //cout << ObjectsElem.getNodeText() << endl;
            ObjectsElem.appendNode(forceNode);
        }
        catch (Exception ee) {
            cout << " ForceSet Not found ......." << endl;
            SimTK::Xml::Element ForceSetElem("ForceSet");
            ForceSetElem.setAttributeValue("name", "ForceSet");
            SimTK::Xml::Element ObjectsElem("Objects");
            ForceSetElem.appendNode(ObjectsElem);
            ObjectsElem.appendNode(forceNode);
        }
        modelDoc.writeToFile(modelPath);
        cout << " Plug-in force is added ......." << endl;
    }
    catch(Exception e){
        cout << " ERROR: plug-in force is not added ......"<< endl;
    }

}

int main(int argc, char* argv[]) {
        std::cout<<"=============================================="<<std::endl;
        std::cout<<"=         Simple Pendulum in OpenSim         ="<<std::endl;
        std::cout<<"=============================================="<<std::endl;

        // Pendulum,Force and Simulation Parameters
        // This helps to experiements with different parameters

                double enablePlugin = 1.0    ; // 1 = pendulum has drag force, 0 = no drag force
                double enableSim    = 1.0    ; // 1 = run the simulation , 0 = only build the model file
                double enableVis    = 1.0    ; // 1 = show visualizer , 0 = no visualisation
                if (not enableSim) enableVis=0;

                //assuming run inside the build folder
                std::string bodyDragForceLibPath="BodyDragForce/libBodyDragForce";

                double bodyDragForceCoefficient = 0.5;
                double bodyDragForceExponent    = 3.0;

                double gravity      = -9.81  ;
                double stepSize     = 0.001  ;
                double initialTime  = 0.0;
                double finalTime    = 5 ;
                double th1Deg       =90 ;               // degrees

                double      rodRd       = 0.1;
                double      rodHeight   = 1;
                double      rodMass     = 1;
                Vec3        rodCoM      = Vec3(0) ;
                Inertia     rodInertia  = Inertia(1);
                SimTK::Vec3 rodColor(1.0, 1.0, 0.0);
                double      sphRd        = 0.3;
                double      sphMass      = 1;
                Vec3        sphCoM       = Vec3(0) ;
                Inertia sphInertia       = Inertia(1);
                SimTK::Vec3 sphColor(1.0, 0.0, 0.0);
                Vec3        sphOrient   = Vec3(0);
        if  (argc >2){
            std::cout<<" using user arguments: "<<std::endl;
            for (int i=0; i<argc ;i++){
                std::cout<<argv[i]<<" " ;
            }
            std::cout<<" "<<std::endl ;

            finalTime    = strtod(argv[1], NULL);
            enableVis    = strtod(argv[2], NULL);
        }else{
          std::cout<<" using default arguments: "<<std::endl;
        }

     double   th1Rad         = th1Deg *((22.0/7.0)/180.0);   // radians
     Vec3     rodPos        = Vec3(0 ,  rodHeight   ,0) ; //it will be under the ground
     Vec3     rodOrient     = Vec3(0,0,th1Rad);

     double   sphX,sphY;
     Vec3     sphPos  ;

     sphX         =   rodHeight * sin( th1Rad);
     sphY         =   rodHeight * cos( th1Rad);
     sphPos        = Vec3(sphX, sphY , 0) ;

     try {

        Model pendulumDF;  // DF =Drag Force
        pendulumDF.setName("pendulumDF");
        pendulumDF.setGravity(SimTK::Vec3(0, gravity, 0));
        Ground& ground = pendulumDF.updGround();


         //                         body name, mass ,massCenter, inertia
        Body* rodBdy = new    Body("rod", rodMass,         rodCoM,     rodInertia );
        Cylinder rodGeom (rodRd , rodHeight);
        rodGeom.setColor(rodColor);    //set the color of the first block so they are different
        SimTK::Transform rodTrsfrm(SimTK::Vec3(0,0,0));
        auto* rodCenter = new PhysicalOffsetFrame("rodCenter", *rodBdy, rodTrsfrm);
        rodBdy->addComponent(rodCenter);     rodCenter->attachGeometry(rodGeom.clone());
        rodBdy->setName();
        pendulumDF.addBody(rodBdy);

        // Connect the bodies with pin joints. Assume each body is 1 m long.
        auto* groundTrodJnt = new PinJoint("groundTrodJnt", pendulumDF.getGround(), Vec3(0),  Vec3(0),  *rodBdy, rodPos, rodOrient);
        pendulumDF.addJoint(groundTrodJnt);

        Body* sphBdy = new    Body("sph", sphMass,     sphCoM,     sphInertia );
        Sphere sphGeom (sphRd);
        sphGeom.setColor(sphColor); //set the color of the first block so they are different
        SimTK::Transform sphTrsfrm(SimTK::Vec3( 0, 0, 0));
        auto* sphCenter = new PhysicalOffsetFrame("sphCenter", *sphBdy, sphTrsfrm);
        sphBdy->addComponent(sphCenter);        sphCenter->attachGeometry(sphGeom.clone());
        pendulumDF.addBody(sphBdy);

        auto*  rodTsphJnt= new  PinJoint ("rodTsphJnt", *rodBdy,Vec3(0), Vec3(0),     *sphBdy, Vec3(0), Vec3(0));
        rodTsphJnt = new  PinJoint ("rodTsphJnt", *rodBdy, Vec3(sphX-rodHeight,sphY,0), rodOrient,     *sphBdy, sphPos, sphOrient);

        pendulumDF.addJoint(rodTsphJnt);

        rodBdy->setName("myRod");

        //rodTsphJnt->getCoordinate().setDefaultLocked(true);

        Coordinate &jntCord = rodTsphJnt->updCoordinate();
        jntCord.setDefaultLocked(true);

        SimTK::Xml::Element nElem("BodyDragForce");  	     nElem.setAttributeValue("name", "dragForce");
        if (enablePlugin) {
            SimTK::Xml::Element bdySph("body_name");          bdySph.setValue("sph");                                      nElem.appendNode(bdySph);
            SimTK::Xml::Element coefficientNd("coefficient"); coefficientNd.setValue(to_string(bodyDragForceCoefficient)); nElem.appendNode(coefficientNd);
            SimTK::Xml::Element exponentNd("exponent");	      exponentNd.setValue(to_string(bodyDragForceExponent));       nElem.appendNode(exponentNd);
        }
        pendulumDF.finalizeConnections();

        if (enableVis == 1.0)   pendulumDF.setUseVisualizer(true);

        // Initialize the system and get the default state
        SimTK::State& si = pendulumDF.initSystem();

        rodTsphJnt->getCoordinate().setLocked(si, true);

        pendulumDF.print(pendulumDF.getName()+".osim");

        // add the plugin force section
        if (enablePlugin) addForcePlugin(nElem, pendulumDF.getName() + ".osim");

        if (enableVis == 1.0)   pendulumDF.setUseVisualizer(true);
        //=============================================================
        //                  SIMULATION
        //=============================================================
        if (enableSim==1.0){
            // Set to true to visualize the simulation, which can be useful for


            //rodTsphJnt->getCoordinate().setLocked(si, true);  // the sphere is fixed to rod

            if (enableVis == 1.0){
                std::cout << "Vsualization is enabled .... " << std::endl;
                SimTK::Visualizer& viz = pendulumDF.updVisualizer().updSimbodyVisualizer();
                viz.setBackgroundType(viz.SolidColor);
                viz.setBackgroundType(SimTK::Visualizer::GroundAndSky);
                viz.setShowFrameNumber(true);
                viz.setGroundHeight(-2);


            }else{
                std::cout << "Vsualization is disabled .... " << std::endl;
            }

            // Create the force reporter for obtaining the forces applied to the model
            // during a forward simulation
            ForceReporter* fReporter    = new ForceReporter(&pendulumDF);
            StatesReporter* sReporter   = new StatesReporter(&pendulumDF);
            BodyKinematics* bKinematics = new BodyKinematics(&pendulumDF, true);
            Array<std::string> bodiesToRecord ; //= {"all"};

            bKinematics->setModel(pendulumDF);
            bKinematics->setBodiesToRecord(bodiesToRecord);

            pendulumDF.addAnalysis(fReporter);
            pendulumDF.addAnalysis(sReporter);
            pendulumDF.addAnalysis(bKinematics);

            Manager manager(pendulumDF);
            manager.setIntegratorMethod(Manager::IntegratorMethod::RungeKuttaFeldberg);
            manager.setIntegratorAccuracy(1.0e-12);
            manager.setIntegratorMaximumStepSize(stepSize);

            // Print out details of the model
            pendulumDF.printDetailedInfo(si, std::cout);

            // Integrate from initial time to final time
            si.setTime(initialTime);
            std::cout << "\nIntegrating from " << initialTime << " to " << finalTime << std::endl;
            manager.initialize(si);
            manager.integrate(finalTime);


            int fR = fReporter->printResults(  pendulumDF.getName(),"",stepSize);
            int sR = sReporter->printResults(  pendulumDF.getName(),"",stepSize);
            int kR = bKinematics->printResults(pendulumDF.getName(),"",stepSize);

            //TODO: how to close simbody-visualizer window?
            // for now use: pkill simbody in the terminal
        } //endif simulation
    } catch (OpenSim::Exception ex)	{
        std::cout << ex.getMessage() << std::endl;
        return 1;
    }
    // declaring argument of time()
    time_t my_time = time(NULL);

    // ctime() used to give the present date time
    printf(" All OpenSim tasks are completed on: %s", ctime(&my_time));
}
