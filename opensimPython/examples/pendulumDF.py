#=====================================================
# Simple Pendulum Simulation Model
# Author: Ibraheem Al-Dhamari 
# last update: 16.7.2020
#=====================================================
import opensim as osim
import sys, math
import numpy as np

# Setup
print("==============================================")
print("=         Simple Pendulum in OpenSim         =")
print("==============================================")

#Pendulum,Force and Simulation Parameters
#This helps to experiements with different parameters

enablePlugin = 1.0    ; #1 = pendulum has drag force, 0 = no drag force
enableSim    = 1.0    ; #1 = run the simulation , 0 = only build the model file
enableVis    = 1.0    ; #1 = show visualizer , 0 = no visualisation
enableVis=0 if (not enableSim) else enableVis


#assuming run inside the build folder
bodyDragForceLibPath="BodyDragForce/libBodyDragForce";

bodyDragForceCoefficient = 0.5;
bodyDragForceExponent    = 3.0;

gravity      = -9.81  ;
stepSize     = 0.001  ;
initialTime  = 0.0;
finalTime    = 5 ;
th1Deg       = 90 ;               #degrees

rodRd        = 0.1;
rodHeight    = 1;
rodMass      = 1;
rodCoM       = osim.Vec3(0) ;
rodInertia   = osim.Inertia(1)  #Inertia(1);
rodColor     = osim.Vec3(1.0, 1.0, 0.0);
sphRd        = 0.3;
sphMass      = 1;
sphCoM       = osim.Vec3(0)
sphInertia   = osim.Inertia(1)
sphColor     = osim.Vec3(1.0, 0.0, 0.0);
sphOrient    = osim.Vec3(0)



th1Rad        = th1Deg *((22.0/7.0)/180.0);   #// radians
rodPos        = osim.Vec3(0 ,  rodHeight   ,0) ;       #//it will be under the ground
rodOrient     = osim.Vec3(0,0,th1Rad);


sphX         =   rodHeight * math.sin( th1Rad);
sphY         =   rodHeight * math.cos( th1Rad);
sphPos       = osim.Vec3(sphX, sphY , 0) ;
    
    

# Define global model where the arm lives.
pendulumDF = osim.Model()
pendulumDF.setName("pendulumDF");
pendulumDF.setGravity(osim.Vec3(0,gravity,0)) ;
ground = pendulumDF.updGround();

#                         body name, mass ,massCenter, inertia
rodBdy  = osim.Body("rod", rodMass,         rodCoM,     rodInertia );
rodGeom = osim.Cylinder(rodRd , rodHeight);
rodGeom.setColor(rodColor);    #//set the color of the first block so they are different
rodTrsfrm = osim.Transform (osim.Vec3(0));
rodCenter = osim.PhysicalOffsetFrame("rodCenter", rodBdy, rodTrsfrm);
rodBdy.addComponent(rodCenter);   
rodCenter.attachGeometry(rodGeom.clone());
pendulumDF.addBody(rodBdy);

# Connect the bodies with pin joints. Assume each body is 1 m long.
groundTrodJnt = osim.PinJoint("groundTrodJnt", pendulumDF.getGround(), osim.Vec3(0),  osim.Vec3(0),  rodBdy, rodPos, rodOrient);
pendulumDF.addJoint(groundTrodJnt);
    
sphBdy = osim.Body("sph", sphMass,     sphCoM,     sphInertia );
sphGeom = osim.Sphere(sphRd);
sphGeom.setColor(sphColor); #//set the color of the first block so they are different
sphTrsfrm = osim.Transform(osim.Vec3( 0));
sphCenter = osim.PhysicalOffsetFrame("sphCenter", sphBdy, sphTrsfrm);
sphBdy.addComponent(sphCenter);       
sphCenter.attachGeometry(sphGeom.clone());
pendulumDF.addBody(sphBdy);

#rodTsphJnt = osim.PinJoint ("rodTsphJnt", rodBdy,osim.Vec3(0), osim.Vec3(0),   osim.sphBdy, osim.Vec3(0), osim.Vec3(0));
rodTsphJnt =   osim.PinJoint    ("rodTsphJnt",rodBdy, osim.Vec3(sphX-rodHeight,sphY,0), rodOrient,     sphBdy, sphPos, sphOrient);

pendulumDF.addJoint(rodTsphJnt);


#rodTsphJnt->getCoordinate().setDefaultLocked(true);
jntCord = rodTsphJnt.updCoordinate()
jntCord.setDefaultLocked(True);


rodBdy.setName("myrod");

#vector<string> dragForce ;
#add BodyDragForce
pendulumDF.finalizeConnections();

if (enableVis == 1.0): 
    pendulumDF.setUseVisualizer(True);

# Initialize the system and get the default state
si = pendulumDF.initSystem();

rodTsphJnt.getCoordinate().setLocked(si, True);

pendulumDF.printToXML(pendulumDF.getName()+".osim");

if (enableSim==1.0):
    #Set to true to visualize the simulation, which can be useful for   
    if (enableVis == 1.0):
        print( "Vsualization is enabled .... ")
        viz = pendulumDF.updVisualizer().updSimbodyVisualizer();
        viz.setBackgroundType(viz.SolidColor);
        viz.setBackgroundType(osim.SimTKVisualizer.GroundAndSky);
        viz.setShowFrameNumber(True);
        viz.setGroundHeight(-2);
    else:
        print( "Vsualization is disabled .... ")

    # Create the force reporter for obtaining the forces applied to the model
    # during a forward simulation
    fReporter    = osim.ForceReporter(pendulumDF);
    sReporter    = osim.StatesReporter(pendulumDF);

    bKinematics  = osim.BodyKinematics(pendulumDF, True);
    bodiesToRecord = osim.ArrayStr()
    bKinematics.setModel(pendulumDF);
    bKinematics.setBodiesToRecord(bodiesToRecord);
 
    pendulumDF.addAnalysis(fReporter);
    pendulumDF.addAnalysis(sReporter);
    pendulumDF.addAnalysis(bKinematics);

    manager = osim.Manager(pendulumDF);
    manager.setIntegratorMethod(osim.Manager.IntegratorMethod_RungeKuttaFeldberg);
    manager.setIntegratorAccuracy(1.0e-12);
    manager.setIntegratorMaximumStepSize(stepSize);

#    BUG
   # print simulation information 
#     pendulumDF.printDetailedInfo(si  ,  sys.stdout);
    pendulumDF.printDetailedInfo(si );

    # Integrate from initial time to final time
    si.setTime(initialTime);
    print( "\nIntegrating from " , initialTime , " to " , finalTime)
    manager.initialize(si);
    manager.integrate(finalTime);

    fReporter.printResults(   pendulumDF.getName() , "" , stepSize);
    sReporter.printResults(   pendulumDF.getName() , "" , stepSize);
    bKinematics.printResults( pendulumDF.getName() , "" , stepSize);
print(" All OpenSim tasks are completed");