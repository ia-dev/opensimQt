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
    qDebug() << "loading qt geometry Point";
}

void vsGeometryImplementationQt::implementLineGeometry(const SimTK::DecorativeLine &geom)
{
    qDebug() << "loading qt geometry Line";
}

void vsGeometryImplementationQt::implementBrickGeometry(const SimTK::DecorativeBrick &geom)
{
    qDebug() << "loading qt geometry Brick";
}

void vsGeometryImplementationQt::implementCylinderGeometry(const SimTK::DecorativeCylinder &geom)
{
    qDebug() << "loading qt geometry Cylinder";
}

void vsGeometryImplementationQt::implementCircleGeometry(const SimTK::DecorativeCircle &geom)
{
    qDebug() << "loading qt geometry Cercle";
}

void vsGeometryImplementationQt::implementSphereGeometry(const SimTK::DecorativeSphere &geom)
{
    qDebug() << "loading qt geometry Sphere";
    double scal_factores[3];
    calculateScaleFactors(scal_factores,geom);
    m_visualizerVTK->renderDecorativeSphere(geom,calculateTransformForGeo(geom),scal_factores);
}

void vsGeometryImplementationQt::implementEllipsoidGeometry(const SimTK::DecorativeEllipsoid &geom)
{
    qDebug() << "loading qt geometry Ellipsoid";
}

void vsGeometryImplementationQt::implementFrameGeometry(const SimTK::DecorativeFrame &geom)
{
    qDebug() << "loading qt geometry Frame";
}

void vsGeometryImplementationQt::implementTextGeometry(const SimTK::DecorativeText &geom)
{
    qDebug() << "loading qt geometry text";
}

void vsGeometryImplementationQt::implementMeshGeometry(const SimTK::DecorativeMesh &geom)
{
    qDebug() << " from qt geometry no file ";
}

void vsGeometryImplementationQt::implementMeshFileGeometry(const SimTK::DecorativeMeshFile& geom)
{
    qDebug() << " from qt geometry "<< QString::fromStdString(geom.getMeshFile());
    double scal_factores[3];
    calculateScaleFactors(scal_factores,geom);
    m_visualizerVTK->renderDecorativeMeshFile(geom,calculateTransformForGeo(geom),scal_factores);
}

void vsGeometryImplementationQt::implementTorusGeometry(const SimTK::DecorativeTorus &geom)
{
    qDebug() << "loading qt geometry Torus";
}

void vsGeometryImplementationQt::implementArrowGeometry(const SimTK::DecorativeArrow &geom)
{
    qDebug() << "loading qt geometry Arrow";
}

void vsGeometryImplementationQt::implementConeGeometry(const SimTK::DecorativeCone &geom)
{
    qDebug() << "loading qt geometry Cone";
}

