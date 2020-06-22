/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                        *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#include "vsGeometryImplementationQt.h"

#include <QDebug>

vsGeometryImplementationQt::vsGeometryImplementationQt(vsVisualizerVTK *visualizerVTK,const SimTK::SimbodyMatterSubsystem& matter,const SimTK::State& state):SimTK::DecorativeGeometryImplementation(),m_state(state),m_matter(matter),m_visualizerVTK(visualizerVTK)
{
}

SimTK::Transform vsGeometryImplementationQt::calculateTransformForGeo(const SimTK::DecorativeGeometry &geo)
{
    const SimTK::MobilizedBody& mb = m_matter.getMobilizedBody(SimTK::MobilizedBodyIndex(geo.getBodyId()));
    const SimTK::Transform& x_GB = mb.getBodyTransform(m_state);
    const SimTK::Transform& x_BD = geo.getTransform();
    return  x_GB * x_BD;
}

void vsGeometryImplementationQt::calculateScaleFactors(double *out,const SimTK::DecorativeGeometry &geo)
{
    SimTK::Vec3 factors = geo.getScaleFactors();
    for (int i = 0; i < 3; ++i) {
        out[i] = factors[i] > 0? factors[i]:1;
    }
}


void vsGeometryImplementationQt::implementPointGeometry(const SimTK::DecorativePoint &geom)
{
    double scal_factores[3];
    calculateScaleFactors(scal_factores,geom);
    if(isUpdate){
        //get the component and update it
        m_visualizerVTK->updateDecorativeGeometry(m_renderedComponent,componentActorIndex,geom,
                                                  calculateTransformForGeo(geom),scal_factores);
    }else{
        vtkSmartPointer<vtkActor> pointActor =
                m_visualizerVTK->renderDecorativePoint(geom,calculateTransformForGeo(geom),scal_factores);
        m_visualizerVTK->addVtkActorToMap(m_renderedModel,pointActor);
        m_visualizerVTK->addVtkActorToComponentMap(m_renderedComponent,pointActor);
        //qDebug() << "loading qt geometry Point";
    }
}

void vsGeometryImplementationQt::implementLineGeometry(const SimTK::DecorativeLine &geom)
{
    double scal_factores[3];
    calculateScaleFactors(scal_factores,geom);
    if(isUpdate){
        //get the component and update it
        //m_visualizerVTK->updateDecorativeGeometry(m_renderedComponent,componentActorIndex,geom,
        //                                          calculateTransformForGeo(geom),scal_factores);
        vtkSmartPointer<vtkActor> lineActor =
                m_visualizerVTK->renderDecorativeLine(geom,calculateTransformForGeo(geom),scal_factores,
                                                      m_renderedComponent,componentActorIndex);
    }else{
    vtkSmartPointer<vtkActor> lineActor = m_visualizerVTK->renderDecorativeLine(geom,calculateTransformForGeo(geom),scal_factores);
    m_visualizerVTK->addVtkActorToMap(m_renderedModel,lineActor);
    m_visualizerVTK->addVtkActorToComponentMap(m_renderedComponent,lineActor);
    //qDebug() << "loading qt geometry Line";
    }
}

void vsGeometryImplementationQt::implementBrickGeometry(const SimTK::DecorativeBrick &geom)
{
    double scal_factores[3];
    calculateScaleFactors(scal_factores,geom);
    if(isUpdate){
        //get the component and update it
        m_visualizerVTK->updateDecorativeGeometry(m_renderedComponent,componentActorIndex,geom,
                                                  calculateTransformForGeo(geom),scal_factores);
    }else{
    vtkSmartPointer<vtkActor> brickActor = m_visualizerVTK->renderDecorativeBrick(geom,calculateTransformForGeo(geom),scal_factores);
    m_visualizerVTK->addVtkActorToMap(m_renderedModel,brickActor);
    m_visualizerVTK->addVtkActorToComponentMap(m_renderedComponent,brickActor);
    //qDebug() << "loading qt geometry Brick";
    }
}

void vsGeometryImplementationQt::implementCylinderGeometry(const SimTK::DecorativeCylinder &geom)
{
    double scal_factores[3];
    calculateScaleFactors(scal_factores,geom);
    if(isUpdate){
        //get the component and update it
        m_visualizerVTK->updateDecorativeGeometry(m_renderedComponent,componentActorIndex,geom,
                                                  calculateTransformForGeo(geom),scal_factores);
    }else{
    vtkSmartPointer<vtkActor> cylenderActor = m_visualizerVTK->renderDecorativeCylender(geom,calculateTransformForGeo(geom),scal_factores);
    m_visualizerVTK->addVtkActorToMap(m_renderedModel,cylenderActor);
    m_visualizerVTK->addVtkActorToComponentMap(m_renderedComponent,cylenderActor);
    //qDebug() << "loading qt geometry Cylinder";
    }
}

void vsGeometryImplementationQt::implementCircleGeometry(const SimTK::DecorativeCircle &geom)
{
    double scal_factores[3];
    calculateScaleFactors(scal_factores,geom);
    if(isUpdate){
        //get the component and update it
        m_visualizerVTK->updateDecorativeGeometry(m_renderedComponent,componentActorIndex,geom,
                                                  calculateTransformForGeo(geom),scal_factores);
    }else{
    vtkSmartPointer<vtkActor> circleActor = m_visualizerVTK->renderDecorativeCircle(geom,calculateTransformForGeo(geom),scal_factores);
    m_visualizerVTK->addVtkActorToMap(m_renderedModel,circleActor);
    m_visualizerVTK->addVtkActorToComponentMap(m_renderedComponent,circleActor);
    //qDebug() << "loading qt geometry Cercle";
    }
}

void vsGeometryImplementationQt::implementSphereGeometry(const SimTK::DecorativeSphere &geom)
{
    //qDebug() << "loading qt geometry Sphere";
    double scal_factores[3];
    calculateScaleFactors(scal_factores,geom);
    if(isUpdate){
        //get the component and update it
        m_visualizerVTK->updateDecorativeGeometry(m_renderedComponent,componentActorIndex,geom,
                                                  calculateTransformForGeo(geom),scal_factores);
    }else{
    vtkSmartPointer<vtkActor> sphereActor = m_visualizerVTK->renderDecorativeSphere(geom,calculateTransformForGeo(geom),scal_factores);
    m_visualizerVTK->addVtkActorToMap(m_renderedModel,sphereActor);
    m_visualizerVTK->addVtkActorToComponentMap(m_renderedComponent,sphereActor);
    }
}

void vsGeometryImplementationQt::implementEllipsoidGeometry(const SimTK::DecorativeEllipsoid &geom)
{
    double scal_factores[3];
    calculateScaleFactors(scal_factores,geom);
    if(isUpdate){
        //get the component and update it
        m_visualizerVTK->updateDecorativeGeometry(m_renderedComponent,componentActorIndex,geom,
                                                  calculateTransformForGeo(geom),scal_factores);
    }else{
    vtkSmartPointer<vtkActor> ellipsoidActor = m_visualizerVTK->renderDecorativeEllipsoid(geom,calculateTransformForGeo(geom),scal_factores);
    m_visualizerVTK->addVtkActorToMap(m_renderedModel,ellipsoidActor);
    m_visualizerVTK->addVtkActorToComponentMap(m_renderedComponent,ellipsoidActor);
    //qDebug() << "loading qt geometry Ellipsoid";
    }
}

void vsGeometryImplementationQt::implementFrameGeometry(const SimTK::DecorativeFrame &geom)
{
    double scal_factores[3];
    calculateScaleFactors(scal_factores,geom);
    if(isUpdate){
        //get the component and update it
        m_visualizerVTK->updateDecorativeGeometry(m_renderedComponent,componentActorIndex,geom,
                                                  calculateTransformForGeo(geom),scal_factores);
    }else{
    vtkSmartPointer<vtkProp> frameActor =
            m_visualizerVTK->renderDecorativeFrame(geom,calculateTransformForGeo(geom),scal_factores);
    m_visualizerVTK->addVtkActorToMap(m_renderedModel,frameActor);
    m_visualizerVTK->addVtkActorToComponentMap(m_renderedComponent,frameActor);
    //qDebug() << "loading qt geometry Frame";
    }
}

void vsGeometryImplementationQt::implementTextGeometry(const SimTK::DecorativeText &geom)
{
    //qDebug() << "loading qt geometry text";
    double scal_factores[3];
    calculateScaleFactors(scal_factores,geom);
    if(isUpdate){
        //get the component and update it
        m_visualizerVTK->updateDecorativeGeometry(m_renderedComponent,componentActorIndex,geom,
                                                  calculateTransformForGeo(geom),scal_factores);
    }else{
    vtkSmartPointer<vtkActor> textActor = m_visualizerVTK->renderDecorativeText(geom,calculateTransformForGeo(geom),scal_factores);
    m_visualizerVTK->addVtkActorToMap(m_renderedModel,textActor);
    m_visualizerVTK->addVtkActorToComponentMap(m_renderedComponent,textActor);
    }
}

void vsGeometryImplementationQt::implementMeshGeometry(const SimTK::DecorativeMesh &geom)
{
    //loaded from MeshFile
    //qDebug() << " from qt geometry no file ";
}

void vsGeometryImplementationQt::implementMeshFileGeometry(const SimTK::DecorativeMeshFile& geom)
{
    //qDebug() << " from qt geometry "<< QString::fromStdString(geom.getMeshFile());
    double scal_factores[3];
    calculateScaleFactors(scal_factores,geom);
    if(isUpdate){
        //get the component and update it
        m_visualizerVTK->updateDecorativeGeometry(m_renderedComponent,componentActorIndex,geom,
                                                  calculateTransformForGeo(geom),scal_factores);
    }else{
    vtkSmartPointer<vtkActor> meshFileActor = m_visualizerVTK->renderDecorativeMeshFile(geom,calculateTransformForGeo(geom),scal_factores);
    m_visualizerVTK->addVtkActorToMap(m_renderedModel,meshFileActor);
    m_visualizerVTK->addVtkActorToComponentMap(m_renderedComponent,meshFileActor);
    }
}

void vsGeometryImplementationQt::implementTorusGeometry(const SimTK::DecorativeTorus &geom)
{
    double scal_factores[3];
    calculateScaleFactors(scal_factores,geom);
    if(isUpdate){
        //get the component and update it
        m_visualizerVTK->updateDecorativeGeometry(m_renderedComponent,componentActorIndex,geom,
                                                  calculateTransformForGeo(geom),scal_factores);
    }else{
    vtkSmartPointer<vtkActor> torusActor = m_visualizerVTK->renderDecorativeTorus(geom,calculateTransformForGeo(geom),scal_factores);
    m_visualizerVTK->addVtkActorToMap(m_renderedModel,torusActor);
    m_visualizerVTK->addVtkActorToComponentMap(m_renderedComponent,torusActor);
    //qDebug() << "loading qt geometry Torus";
    }
}

void vsGeometryImplementationQt::implementArrowGeometry(const SimTK::DecorativeArrow &geom)
{
    double scal_factores[3];
    calculateScaleFactors(scal_factores,geom);
    if(isUpdate){
        //get the component and update it
        m_visualizerVTK->updateDecorativeGeometry(m_renderedComponent,componentActorIndex,geom,
                                                  calculateTransformForGeo(geom),scal_factores);
    }else{
    vtkSmartPointer<vtkActor> arrowActor = m_visualizerVTK->renderDecorativeArrow(geom,calculateTransformForGeo(geom),scal_factores);
    m_visualizerVTK->addVtkActorToMap(m_renderedModel,arrowActor);
    m_visualizerVTK->addVtkActorToComponentMap(m_renderedComponent,arrowActor);
    //qDebug() << "loading qt geometry Arrow";
    }
}

void vsGeometryImplementationQt::implementConeGeometry(const SimTK::DecorativeCone &geom)
{
    double scal_factores[3];
    calculateScaleFactors(scal_factores,geom);
    if(isUpdate){
        //get the component and update it
        m_visualizerVTK->updateDecorativeGeometry(m_renderedComponent,componentActorIndex,geom,
                                                  calculateTransformForGeo(geom),scal_factores);
    }else{
    vtkSmartPointer<vtkActor> coneActor = m_visualizerVTK->renderDecorativeCone(geom,calculateTransformForGeo(geom),scal_factores);
    m_visualizerVTK->addVtkActorToMap(m_renderedModel,coneActor);
    m_visualizerVTK->addVtkActorToComponentMap(m_renderedComponent,coneActor);
    //qDebug() << "loading qt geometry Cone";
    }
}

OpenSim::Model *vsGeometryImplementationQt::renderedModel() const
{
    return m_renderedModel;
}

void vsGeometryImplementationQt::setRenderedModel(OpenSim::Model *renderedModel)
{
    m_renderedModel = renderedModel;
}

void vsGeometryImplementationQt::setRenderedComponent(OpenSim::Component *renderedComponent)
{
    m_renderedComponent = renderedComponent;
}

OpenSim::Component *vsGeometryImplementationQt::renderedComponent() const
{
    return m_renderedComponent;
}

int vsGeometryImplementationQt::getComponentActorIndex() const
{
    return componentActorIndex;
}

void vsGeometryImplementationQt::setComponentActorIndex(int value)
{
    componentActorIndex = value;
}

bool vsGeometryImplementationQt::getIsUpdate() const
{
    return isUpdate;
}

void vsGeometryImplementationQt::setIsUpdate(bool value)
{
    isUpdate = value;
}

